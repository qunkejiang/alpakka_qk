#include "imu.h"
#include <esp_log.h>
#include <cstring>

#define IMU_SYNC_FREQ_HZ 32000

inline int32_t combine_20bit_signed(uint8_t msb, uint8_t lsb, uint8_t ext) {
    return ((msb << 24) | (lsb << 16) | (ext << 12)) >> 12;
}

esp_err_t Imu::icm42688_read_register(spi_device_handle_t *spi,uint8_t reg, uint8_t *data, uint8_t len)
{
    spi_transaction_t t={0};
    t.addr = reg|0x80;
    t.rxlength = 8 * len; // Receive length is the number of bytes to read
    t.rx_buffer = data;
    return spi_device_polling_transmit(*spi, &t);
}
void Imu::icm42688_write_register(spi_device_handle_t *spi,uint8_t reg, uint8_t data)
{
    spi_transaction_t t={0};
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
void Imu::calibration(float *G_off,int32_t *step)
{
    static int32_t count = 0;
    static float temporary[3] = {0.0f,0.0f,0.0f};
    switch (*step)
    {
    case 1:
        *step = 2;
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
            *step = 3;
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
        if(fifo_count>0)
        {
            if(icm42688_read_register(&icm42688_spi,FIFO_DATA, buffer, (fifo_count>80)?80:fifo_count) == ESP_OK)
            {;
                uint16_t i;
                int32_t acc[3] = {
                    combine_20bit_signed(buffer[1], buffer[2], buffer[17]>>4),//
                    combine_20bit_signed(buffer[3], buffer[4], buffer[18]>>4),//
                    combine_20bit_signed(buffer[5], buffer[6], buffer[19]>>4)//
                    };
                int32_t g[3]={
                    combine_20bit_signed(buffer[7], buffer[8], buffer[17]&0xf),//
                    combine_20bit_signed(buffer[9], buffer[10], buffer[18]&0xf),//
                    combine_20bit_signed(buffer[11], buffer[12], buffer[19]&0xf)//
                };
                int16_t temp = buffer[13]<<8|buffer[14];

                data.temp = raw_data.temp=(temp/132.48f) + 25.f ;
                for(i = 0; i < 3; i++)
                {
                    data.acc.axis[i] = raw_data.acc.axis[i] = acc[i] * G_PER_LSB ; 
                    raw_data.gyro.axis[i] = g[i]* (DEG_PER_LSB); //*DEG2RAD
                    data.gyro.axis[i] = raw_data.gyro.axis[i] - G_off[i];
                }

                return ESP_OK;
            }
            ESP_LOGE("ICM42688", "Failed to read FIFO data");
        }
    }
    else
    {
        ESP_LOGE("ICM42688", "Failed to read FIFO count");
    }
    return ESP_FAIL;
}

Imu::Imu(imu_config_t *config)
{
  // SPI
    spi_bus_config_t buscfg = {0};
    buscfg.mosi_io_num = config->mosi_io_num;
    buscfg.miso_io_num = config->miso_io_num;
    buscfg.sclk_io_num = config->sclk_io_num;
    buscfg.quadwp_io_num = -1;
    buscfg.quadhd_io_num = -1;
    buscfg.max_transfer_sz = 64;

    ESP_ERROR_CHECK(spi_bus_initialize(config->host, &buscfg, SPI_DMA_CH_AUTO));


    spi_device_interface_config_t spi_cfg = {0};
    spi_cfg.address_bits = 8; // address bits
    spi_cfg.mode = 3;
    spi_cfg.cs_ena_pretrans = 1; // CS pre-transmission delay in SPI bit-cycles
    spi_cfg.clock_speed_hz = 10000000; // S2/S3 can work with 26 Mhz, but esp32 seems only work up to 20 Mhz
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