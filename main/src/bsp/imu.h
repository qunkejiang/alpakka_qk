#ifndef IMU_C_H
#define IMU_C_H

#include <driver/ledc.h>
#include "driver/spi_master.h"


typedef struct  
{
	union 
	{
		float q[4];
		struct 
		{
			float q0;
			float q1;
			float q2;
			float q3;
		};
		struct 
		{
			float w;
			float x;
			float y;
			float z;
		};
	};
} quaternion_t;
typedef union
{
    struct
    {
        float x;
        float y;
        float z;
    };
    float axis[3];
} Axis3f;

typedef struct  
{
	quaternion_t quaternion;
	float rMat[3];
}pose_t;


typedef struct
{
    pose_t pose;
    Axis3f acc;
    Axis3f gyro;
    float temp;
} Imu_Data_t;

struct imu_config_t
{
    ledc_timer_t  timer_num;
    ledc_channel_t channel;
    // ledc_channel_t channel1;
    spi_host_device_t host;
    int mosi_io_num;
    int miso_io_num;
    int sclk_io_num;
    int spics_io_num;
    int sync_io_num;
    // int spics1_io_num;
    // int sync1_io_num;
};

class Imu {
private:
    Imu_Data_t raw_data;
    spi_device_handle_t icm42688_spi;
    // spi_device_handle_t icm42688_spi1;
    
    void parse_icm42688_data(const uint8_t *buffer, Imu_Data_t *Data);
    esp_err_t icm42688_read_register(spi_device_handle_t *spi,uint8_t reg, uint8_t *data, uint8_t len);
    void icm42688_write_register(spi_device_handle_t *spi,uint8_t reg, uint8_t data);
    void icm42688_initialize(spi_device_handle_t *spi);
public:
    uint8_t calibration_step;
    Imu_Data_t data;
    void calibration(float *G_off);
    esp_err_t update(float *G_off);
    Imu(imu_config_t *config);
};

#endif
