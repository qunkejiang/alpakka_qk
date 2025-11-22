#include "imu.h"
#include "ICM42688.h"
#include "common.h"
#include <string.h>
#include "logging.h"
#include <esp_timer.h>
#include "esp_log.h"


#define IMU_SYNC_FREQ_HZ 32000


void imuUpdate(Axis3f acc, Axis3f gyro,pose_t *data, float dt)	//数据融合 互补滤波
{
	static float exInt = 0.0f;
	static float eyInt = 0.0f;
	static float ezInt = 0.0f;		
	
	float Kp;		
	float Ki;		
	
	float *q=(float *)&data->quaternion.q;
    float *rMat=data->rMat;
	
	float normalise;
	float ex, ey, ez;
	float halfT = 0.5f * dt;
	
	gyro.x = radians(gyro.x);	// 度转弧度 
	gyro.y = radians(gyro.y);
	gyro.z = radians(gyro.z);
	
	normalise = invSqrt(acc.x * acc.x + acc.y * acc.y + acc.z * acc.z);
	if(normalise != 0.0f) 
	{
		acc.x *= normalise;
		acc.y *= normalise;
		acc.z *= normalise;

		ex = (acc.y * rMat[2] - acc.z * rMat[1]);//float x_ca = y_aa * z_ac - z_aa * y_ac;
		ey = (acc.z * rMat[0] - acc.x * rMat[2]);//float y_ca = z_aa * x_ac - x_aa * z_ac;
		ez = (acc.x * rMat[1] - acc.y * rMat[0]);//float z_ca = x_aa * y_ac - y_aa * x_ac;
		
		float accConfidence=(1.0f-abs(1.0f/normalise-1.0f));
		if(accConfidence>1.0f)
			accConfidence=1.0f;
		else if(accConfidence<0.0f)
			accConfidence=0.0f;
		Kp=0.4f*accConfidence;
		Ki=0.01f* dt*accConfidence;

		exInt += Ki * ex ;  
		eyInt += Ki * ey ;
		ezInt += Ki * ez ;
		
		gyro.x += Kp * ex + exInt;
		gyro.y += Kp * ey + eyInt;
		gyro.z += Kp * ez + ezInt; 
	} 
    
	float qLast[4]={q[0],q[1],q[2],q[3]};
	q[0] += (-qLast[1] * gyro.x - qLast[2] * gyro.y - qLast[3] * gyro.z) * halfT;
	q[1] += ( qLast[0] * gyro.x + qLast[2] * gyro.z - qLast[3] * gyro.y) * halfT;
	q[2] += ( qLast[0] * gyro.y - qLast[1] * gyro.z + qLast[3] * gyro.x) * halfT;
	q[3] += ( qLast[0] * gyro.z + qLast[1] * gyro.y - qLast[2] * gyro.x) * halfT;
	
	normalise = invSqrt(q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
	q[0] *= normalise;
	q[1] *= normalise;
	q[2] *= normalise;
	q[3] *= normalise;
    
    rMat[0] = 2.0f * (q[1] * q[3] - q[0] * q[2]);
    rMat[1] = 2.0f * (q[2] * q[3] + q[0] * q[1]);
    rMat[2] = 1.0f - (q[1] * q[1] - q[2] * q[2])*2.0f;
	
}

inline int32_t combine_20bit_signed(uint8_t msb, uint8_t lsb, uint8_t ext) {
    return ((msb << 24L) | (lsb << 16L) | (ext << 12L)) >> 12L;
}

esp_err_t Imu::icm42688_read_register(spi_device_handle_t *spi,uint8_t reg, uint8_t *data, uint8_t len)
{
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.addr = reg|0x80;
    t.rxlength = 8 * len; // Receive length is the number of bytes to read
    t.rx_buffer = data;
    return spi_device_polling_transmit(*spi, &t);
}
void Imu::icm42688_write_register(spi_device_handle_t *spi,uint8_t reg, uint8_t data)
{
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.addr = reg;
    t.length = 8; // Length is in bits
    t.tx_buffer = &data;
    spi_device_transmit(*spi, &t);
}
void Imu::icm42688_initialize(spi_device_handle_t *spi)
{
    icm42688_write_register(spi,REG_BANK_SEL, 0); 
    icm42688_write_register(spi,DEVICE_CONFIG, DEVICE_RESET); //软复位传感器
    vTaskDelay(20 / portTICK_PERIOD_MS);
    
    icm42688_write_register(spi,REG_BANK_SEL, 1); 
    icm42688_write_register(spi,INTF_CONFIG5, CLKIN);//PIN9设置为CLK_IN
    icm42688_write_register(spi,REG_BANK_SEL, 0);
    icm42688_write_register(spi,INTF_CONFIG1,RTC_MODE|CLKSEL);//

    icm42688_write_register(spi,INT_CONFIG, INT1_PUSH_PULL);//设置INT1为推挽输出
    icm42688_write_register(spi,FIFO_CONFIG, Stream_to_FIFO);//设置FIFO为流模式
    icm42688_write_register(spi,PWR_MGMT0, ACCEL_Low_Noise|GYRO_Low_Noise);//低噪声模式
    icm42688_write_register(spi,GYRO_CONFIG0, GFS_125DPS|GODR_1000Hz );//调整采样率和ODR
    icm42688_write_register(spi,ACCEL_CONFIG0, AFS_8G|AODR_1000Hz );//调整采样率和ODR
    //icm_42688_write_byet(spi,GYRO_ACCEL_CONFIG0, ACCEL_UI_FILT_Low_Latency|GYRO_UI_FILT_Low_Latency);//调整陀螺仪和加速度计的低通滤波器带宽

    // icm_42688_write_byet(GYRO_CONFIG1, 0x16);//调整带宽和滤波次数
    // icm_42688_write_byet(ACCEL_CONFIG1, 0x0D);//调整带宽和滤波次数
    // icm_42688_write_byet(SELF_TEST_CONFIG, 0x0D);//调整带宽和滤波次数

    icm42688_write_register(spi,FIFO_CONFIG1, FIFO_HIRES_EN|FIFO_TMST_FSYNC_EN); //启用高分辨率和时间戳
    
    // icm_42688_write_byet(spi,FIFO_CONFIG2, 20);//watermark_L
    // icm_42688_write_byet(spi,FIFO_CONFIG3, 0);//watermark_H
    //icm42688_write_register(spi,INT_SOURCE0, FIFO_THS_INT1_EN);
    icm42688_write_register(spi,INT_SOURCE0, UI_DRDY_INT1_EN);

}
void Imu::calibration(float *G_off)
{
    static int32_t count = 0;
    static float temporary[3] = {0.0f,0.0f,0.0f};
    switch (calibration_step)
    {
    case 1:
        calibration_step = 2;
        count = 0;
        temporary[0] = 0.0f;
        temporary[1] = 0.0f;        
        temporary[2] = 0.0f;
        break;
    case 2:
        count++;
        if(count>=5000)
        {
            G_off[0] = temporary[0]/5000.0f;
            G_off[1] = temporary[1]/5000.0f;
            G_off[2] = temporary[2]/5000.0f;
            calibration_step = 3;
            logging::info("imu offset x : %f\n", G_off[0]);
            logging::info("imu offset y : %f\n", G_off[1]);
            logging::info("imu offset z : %f\n", G_off[2]);
            //Board::GetInstance().nvm_->save_nvm_data();
        }else
        {
            temporary[0] += raw_data.gyro.axis[0];
            temporary[1] += raw_data.gyro.axis[1];
            temporary[2] += raw_data.gyro.axis[2];
        }
        break;
    default:
        break;
    }
}


esp_err_t Imu::update(float *G_off)
{
    uint8_t buffer[80];
    if(icm42688_read_register(&icm42688_spi,FIFO_COUNTH, buffer, 2) == ESP_OK)
    {
        uint16_t fifo_count = (buffer[0]<<8)|buffer[1];
        if(fifo_count>=20)
        {
            if(icm42688_read_register(&icm42688_spi,FIFO_DATA, buffer, (fifo_count>80)?80:fifo_count) == ESP_OK)
            {
                uint16_t i;
                int32_t acc[3] = {
                    combine_20bit_signed(buffer[1], buffer[2], buffer[17]>>4),//x
                    combine_20bit_signed(buffer[3], buffer[4], buffer[18]>>4),//y
                    combine_20bit_signed(buffer[5], buffer[6], buffer[19]>>4)//z
                    };
                int32_t g[3]={
                    combine_20bit_signed(buffer[7], buffer[8], buffer[17]&0xf),//x
                    combine_20bit_signed(buffer[9], buffer[10], buffer[18]&0xf),//y
                    combine_20bit_signed(buffer[11], buffer[12], buffer[19]&0xf)//z
                };
                int16_t temp = buffer[13]<<8|buffer[14];

                data.temp = raw_data.temp=(temp/132.48f) + 25.f ;
                for(i = 0; i < 3; i++)
                {
                    data.acc.axis[i] = raw_data.acc.axis[i] = acc[i] * G_PER_LSB ; 
                    raw_data.gyro.axis[i] = g[i] * (DEG_PER_LSB);
                    data.gyro.axis[i] = raw_data.gyro.axis[i] - G_off[i];
                }
                //logging::debug_uart("%f, %f, %f,%f\n",data.gyro.axis[0], data.gyro.axis[1], data.gyro.axis[2], data.temp);
                
                imuUpdate(raw_data.acc, raw_data.gyro, &data.pose, 0.001f);
                calibration(G_off);
                return ESP_OK;
            }
            logging::error("ICM42688", "Failed to read FIFO data\n");
        }
    }
    else
    {
        logging::error("ICM42688", "Failed to read FIFO count\n");
    }
    return ESP_FAIL;
}

Imu::Imu(imu_config_t *config)
{
  // SPI
    spi_bus_config_t buscfg;
    memset(&buscfg, 0, sizeof(buscfg));
    buscfg.mosi_io_num = config->mosi_io_num;
    buscfg.miso_io_num = config->miso_io_num;
    buscfg.sclk_io_num = config->sclk_io_num;
    buscfg.quadwp_io_num = -1;
    buscfg.quadhd_io_num = -1;
    buscfg.max_transfer_sz = 80;

    ESP_ERROR_CHECK(spi_bus_initialize(config->host, &buscfg, SPI_DMA_CH_AUTO));


    spi_device_interface_config_t spi_cfg;
    memset(&spi_cfg, 0, sizeof(spi_cfg));
    spi_cfg.address_bits = 8;
    spi_cfg.mode = 3;
    spi_cfg.cs_ena_pretrans = 1; // CS pre-transmission delay in SPI bit-cycles
    spi_cfg.clock_speed_hz = 10000000; //
    spi_cfg.spics_io_num = config->spics_io_num; // manual control CS
    spi_cfg.flags = SPI_DEVICE_HALFDUPLEX; // 启用半双工模式
    spi_cfg.queue_size = 3;
    ESP_ERROR_CHECK(spi_bus_add_device(config->host, &spi_cfg, &icm42688_spi));
    // spi_cfg.spics_io_num = config->spics1_io_num;
    // ESP_ERROR_CHECK(spi_bus_add_device(config->host, &spi_cfg, &icm42688_spi1));
    icm42688_initialize(&icm42688_spi);
    

  
    ledc_timer_config_t icm42688_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_8_BIT,
        .timer_num = config->timer_num,
        .freq_hz = IMU_SYNC_FREQ_HZ, 
        .clk_cfg = LEDC_AUTO_CLK,
        .deconfigure = false
    };
    ESP_ERROR_CHECK(ledc_timer_config(&icm42688_timer));

    // Setup LEDC peripheral for PWM backlight control
    ledc_channel_config_t icm42688_channel = {
        .gpio_num = config->sync_io_num,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = config->channel,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = config->timer_num,
        .duty = 128,
        .hpoint = 0,
        .sleep_mode = LEDC_SLEEP_MODE_NO_ALIVE_NO_PD,
        .flags = {
            .output_invert = 0,
        }
    };
    ESP_ERROR_CHECK(ledc_channel_config(&icm42688_channel));
}