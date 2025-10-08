#ifndef __ICM_42688_P_H
#define __ICM_42688_P_H


/**USER BANK 0 REGISTER MAP �C DESCRIPTIONS**/
#define DEVICE_CONFIG 0X11		//Serial IF: R/W	 Reset value: 0x00
#define     DEVICE_RESET 0X01 //DEVICE_CONFIG
// #define DRIVE_CONFIG  0X13		//Serial IF: R/W   Reset value: 0x05
// #define     I2C_SLEW_RAT (0x05<<3)
// #define     I3C_SLEW_RAT (0x05)
#define INT_CONFIG    0X14		//Serial IF: R/W	 Reset value: 0x00
#define     INT1_PUSH_PULL (0X01<<1)  

#define FIFO_CONFIG 	0X16		//Serial IF: R/W	 Reset value: 0x00
#define     Stream_to_FIFO 0X40

// #define TEMP_DATA1	  0X1D		//Serial IF: SYNCR Reset value: 0x80
// #define TEMP_DATA0 		0X1E		//Serial IF: SYNCR Reset value: 0x00

// #define ACCEL_DATA_X1 0X1F		//Serial IF: SYNCR Reset value: 0x80
// #define ACCEL_DATA_X0 0X20		//Serial IF: SYNCR Reset value: 0x00
// #define ACCEL_DATA_Y1 0X21		//Serial IF: SYNCR Reset value: 0x80
// #define ACCEL_DATA_Y0 0X22		//Serial IF: SYNCR Reset value: 0x00
// #define ACCEL_DATA_Z1 0X23		//Serial IF: SYNCR Reset value: 0x80
// #define ACCEL_DATA_Z0 0X24		//Serial IF: SYNCR Reset value: 0x00

// #define GYRO_DATA_X1  0X25		//Serial IF: SYNCR Reset value: 0x80
// #define GYRO_DATA_X0  0X26		//Serial IF: SYNCR Reset value: 0x00
// #define GYRO_DATA_Y1  0X27		//Serial IF: SYNCR Reset value: 0x80
// #define GYRO_DATA_Y0  0X28		//Serial IF: SYNCR Reset value: 0x00
// #define GYRO_DATA_Z1  0X29		//Serial IF: SYNCR Reset value: 0x80
// #define GYRO_DATA_Z0  0X2A		//Serial IF: SYNCR Reset value: 0x00

// #define TMST_FSYNCH   0X2B		//Serial IF: SYNCR Reset value: 0x00
// #define TMST_FSYNCL   0X2C    //Serial IF: SYNCR Reset value: 0x00
// #define INT_STATUS    0X2D    //Serial IF: R/C   Reset value: 0x10
#define FIFO_COUNTH   0X2E    //Serial IF: R     Reset value: 0x00
// #define FIFO_COUNTL   0X2F    //Serial IF: R     Reset value: 0x00
#define FIFO_DATA     0X30    //Serial IF: R     Reset value: 0xFF
// #define APEX_DATA0    0X31    //Serial IF: SYNCR Reset value: 0x00
// #define APEX_DATA1    0X32    //Serial IF: SYNCR Reset value: 0x00
// #define APEX_DATA2    0X33    //Serial IF: R     Reset value: 0x00
// #define APEX_DATA3    0X34    //Serial IF: R     Reset value: 0x04
// #define APEX_DATA4    0X35    //Serial IF: R     Reset value: 0x00
// #define APEX_DATA5    0X36    //Serial IF: R     Reset value: 0x00
// #define INT_STATUS2   0X37    //Serial IF: R/C   Reset value: 0x00
// #define INT_STATUS3   0X38    //Serial IF: R/C   Reset value: 0x00

// #define SIGNAL_PATH_RESET   0X4B    //Serial IF: W/C   Reset value: 0x00

// #define INTF_CONFIG0  0X4C    //Serial IF: R/W   Reset value: 0x30
#define INTF_CONFIG1  0X4D    //Serial IF: R/W   Reset value: 0x91
#define     RTC_MODE  (1<<2) //UI_DRDY_INT1_EN
#define     CLKSEL  (1<<0) //INT1_DRDY_EN
#define PWR_MGMT0     0X4E    //Serial IF: R/W   Reset value: 0x00
#define     ACCEL_Low_Noise 0X03
#define     GYRO_Low_Noise  (0X03<<2)  

#define GYRO_CONFIG0  0X4F    //Serial IF: R/W   Reset value: 0x06
#define     GFS_2000DPS   (0x00<<5) // default
#define     GFS_1000DPS   (0x01<<5)
#define     GFS_500DPS    (0x02<<5)
#define     GFS_250DPS    (0x03<<5)
#define     GFS_125DPS    (0x04<<5)
#define     GFS_62_5DPS   (0x05<<5)
#define     GFS_31_25DPS  (0x06<<5)
#define     GFS_15_125DPS (0x07<<5)
#define     GODR_8000Hz  0x03
#define     GODR_4000Hz  0x04
#define     GODR_2000Hz  0x05
#define     GODR_1000Hz  0x06 // default
#define     GODR_200Hz   0x07
#define     GODR_100Hz   0x08
#define     GODR_50Hz    0x09
#define     GODR_25Hz    0x0A
#define     GODR_12_5Hz  0x0B
#define     GODR_500Hz   0x0F
#define ACCEL_CONFIG0 0X50    //Serial IF: R/W   Reset value: 0x06
#define     AFS_2G  (0x03<<5)
#define     AFS_4G  (0x02<<5)
#define     AFS_8G  (0x01<<5)
#define     AFS_16G (0x00<<5)  // default
#define     AODR_8000Hz   0x03
#define     AODR_4000Hz   0x04
#define     AODR_2000Hz   0x05
#define     AODR_1000Hz   0x06 // default
#define     AODR_200Hz    0x07
#define     AODR_100Hz    0x08
#define     AODR_50Hz     0x09
#define     AODR_25Hz     0x0A
#define     AODR_12_5Hz   0x0B
#define     AODR_6_25Hz   0x0C
#define     AODR_3_125Hz  0x0D
#define     AODR_1_5625Hz 0x0E
#define     AODR_500Hz    0x0F

//#define GYRO_CONFIG1  0X51    //Serial IF: R/W   Reset value: 0x16

#define GYRO_ACCEL_CONFIG0   0X52    //Serial IF: R/W   Reset value: 0x11
#define     ACCEL_UI_FILT_BW4    (1<<4)
#define     ACCEL_UI_FILT_BW10    (4<<4)
#define     ACCEL_UI_FILT_BW40    (7<<4)
#define     ACCEL_UI_FILT_Low_Latency    (14<<4)
#define     GYRO_UI_FILT_BW4     1
#define     GYRO_UI_FILT_BW10     4
#define     GYRO_UI_FILT_BW40     7
#define     GYRO_UI_FILT_Low_Latency     14

//#define ACCEL_CONFIG1 0X53    //Serial IF: R/W   Reset value: 0x0D
// #define TMST_CONFIG   0X54    //Serial IF: R/W   Reset value: 0x23
// #define APEX_CONFIG0  0X56    //Serial IF: R/W   Reset value: 0x82
// #define SMD_CONFIG    0X57    //Serial IF: R/W   Reset value: 0x00
#define FIFO_CONFIG1  0X5F    //Serial IF: R/W   Reset value: 0x00
#define     FIFO_WM_GT_TH           (1<<5)
#define     FIFO_HIRES_EN           (1<<4)
#define     FIFO_TMST_FSYNC_EN      (1<<3)
#define     FIFO_TEMP_EN            (1<<2)
#define     FIFO_GYRO_EN            (1<<1)
#define     FIFO_ACCEL_EN           (1<<0)

#define FIFO_CONFIG2  0X60    //Serial IF: R/W   Reset value: 0x00
#define FIFO_CONFIG3  0X61    //Serial IF: R/W   Reset value: 0x00
// #define FSYNC_CONFIG  0X62    //Serial IF: R/W   Reset value: 0x10
// #define INT_CONFIG0   0X63    //Serial IF: R/W   Reset value: 0x00
#define INT_CONFIG1   0X64    //Serial IF: R/W   Reset value: 0x10
#define     INT_ASYNC_RESET   (1<<4)
#define INT_SOURCE0   0X65    //Serial IF: R/W   Reset value: 0x10
#define     UI_DRDY_INT1_EN   (1<<3)
#define     FIFO_THS_INT1_EN   (1<<2)
 
// #define INT_SOURCE1   0X66    //Serial IF: R/W   Reset value: 0x00
// #define INT_SOURCE3   0X68    //Serial IF: R/W   Reset value: 0x00
// #define INT_SOURCE4   0X69    //Serial IF: R/W   Reset value: 0x00

// #define FIFO_LOST_PKT0   0X6C    //Serial IF: R   Reset value: 0x00

// #define FIFO_LOST_PKT1   0X6D    //Serial IF: R   Reset value: 0x00
// #define SELF_TEST_CONFIG   0X70    //Serial IF: R/W   Reset value: 0x00
#define WHO_AM_I   0X75   //Serial IF: R   Reset value: 0x47
#define REG_BANK_SEL   0X76    //Serial IF: R/W   Reset value: 0x00


// /**USER BANK 1 REGISTER MAP �C DESCRIPTIONS**/
// #define SENSOR_CONFIG0   0X03    //Serial IF: R/W   Reset value: 0x80
// #define GYRO_CONFIG_STATIC2   0X0B    //Serial IF: R/W   Reset value: 0xA0
// #define GYRO_CONFIG_STATIC3   0X0C    //Serial IF: R/W   Reset value: 0x0D
// #define GYRO_CONFIG_STATIC4   0X0D    //Serial IF: R/W   Reset value: 0xAA
// #define GYRO_CONFIG_STATIC5   0X0E    //Serial IF: R/W   Reset value: 0x80
// #define GYRO_CONFIG_STATIC6   0X0F    //Serial IF: R/W   Reset value: 0xXX (Factory trimmed on an individual device basis)
// #define GYRO_CONFIG_STATIC7   0X10    //Serial IF: R/W   Reset value: 0xXX (Factory trimmed on an individual device basis)
// #define GYRO_CONFIG_STATIC8   0X11    //Serial IF: R/W   Reset value: 0xXX (Factory trimmed on an individual device basis)
// #define GYRO_CONFIG_STATIC9   0X12    //Serial IF: R/W   Reset value: 0xXX (Factory trimmed on an individual device basis)
// #define GYRO_CONFIG_STATIC10  0X13    //Serial IF: R/W   Reset value: 0x11
// #define XG_ST_DATA   0X5F    //Serial IF: R/W   Reset value: 0xXX (The value in this register indicates the self-test output generated during manufacturing tests)
// #define YG_ST_DATA   0X60    //Serial IF: R/W   Reset value: 0xXX (The value in this register indicates the self-test output generated during manufacturing tests)
// #define ZG_ST_DATA   0X61    //Serial IF: R/W   Reset value: 0xXX (The value in this register indicates the self-test output generated during manufacturing tests)
// #define TMSTVAL0   0X62    //Serial IF: R   Reset value: 0x00
// #define TMSTVAL1   0X63    //Serial IF: R   Reset value: 0x00
// #define TMSTVAL2   0X64    //Serial IF: R   Reset value: 0x00

// #define INTF_CONFIG4   0X7A    //Serial IF: R/W   Reset value: 0x83
#define INTF_CONFIG5   0X7B    //Serial IF: R/W   Reset value: 0x00
#define     CLKIN 4
// #define INTF_CONFIG6   0X7C    //Serial IF: R/W   Reset value: 0x5F

// /**USER BANK 2 REGISTER MAP �C DESCRIPTIONS**/
// #define ACCEL_CONFIG_STATIC2   0X03    //Serial IF: R/W   Reset value: 0x30
// #define ACCEL_CONFIG_STATIC3   0X04    //Serial IF: R/W   Reset value: 0x40
// #define ACCEL_CONFIG_STATIC4   0X05    //Serial IF: R/W   Reset value: 0x62
// #define XA_ST_DATA   0X3B    //Serial IF: R/W   Reset value: 0xXX (The value in this register indicates the self-test output generated during manufacturing tests)
// #define YA_ST_DATA   0X3C    //Serial IF: R/W   Reset value: 0xXX (The value in this register indicates the self-test output generated during manufacturing tests)
// #define ZA_ST_DATA   0X3D    //Serial IF: R/W   Reset value: 0xXX (The value in this register indicates the self-test output generated during manufacturing tests)

// /**USER BANK 4 REGISTER MAP �C DESCRIPTIONS**/
// #define APEX_CONFIG1   0X40    //Serial IF: R/W   Reset value: 0xA2
// #define APEX_CONFIG2   0X41    //Serial IF: R/W   Reset value: 0x85
// #define APEX_CONFIG3   0X42    //Serial IF: R/W   Reset value: 0x51
// #define APEX_CONFIG4   0X43    //Serial IF: R/W   Reset value: 0xA4
// #define APEX_CONFIG5   0X44    //Serial IF: R/W   Reset value: 0x8C
// #define APEX_CONFIG6   0X45    //Serial IF: R/W   Reset value: 0x5C
// #define APEX_CONFIG7   0X46    //Serial IF: R/W   Reset value: 0x45
// #define APEX_CONFIG8   0X47    //Serial IF: R/W   Reset value: 0x5B
// #define APEX_CONFIG9   0X48    //Serial IF: R/W   Reset value: 0x00
// #define ACCEL_WOM_X_THR   0X4A    //Serial IF: R/W   Reset value: 0x00
// #define ACCEL_WOM_Y_THR   0X4B    //Serial IF: R/W   Reset value: 0x00
// #define ACCEL_WOM_Z_THR   0X4C    //Serial IF: R/W   Reset value: 0x00
// #define INT_SOURCE6   0X4D    //Serial IF: R/W   Reset value: 0x00
// #define INT_SOURCE7   0X4E    //Serial IF: R/W   Reset value: 0x00
// #define INT_SOURCE8   0X4F    //Serial IF: R/W   Reset value: 0x00
// #define INT_SOURCE9   0X50    //Serial IF: R/W   Reset value: 0x00
// #define INT_SOURCE10  0X51    //Serial IF: R/W   Reset value: 0x00
// #define OFFSET_USER0  0X77    //Serial IF: R/W   Reset value: 0x00
// #define OFFSET_USER1  0X78    //Serial IF: R/W   Reset value: 0x00
// #define OFFSET_USER2  0X79    //Serial IF: R/W   Reset value: 0x00
// #define OFFSET_USER3  0X7A    //Serial IF: R/W   Reset value: 0x00
// #define OFFSET_USER4  0X7B    //Serial IF: R/W   Reset value: 0x00
// #define OFFSET_USER5  0X7C    //Serial IF: R/W   Reset value: 0x00
// #define OFFSET_USER6  0X7D    //Serial IF: R/W   Reset value: 0x00
// #define OFFSET_USER7  0X7E    //Serial IF: R/W   Reset value: 0x00
// #define OFFSET_USER8  0X7F    //Serial IF: R/W   Reset value: 0x00


#define DEG_PER_LSB (float)( 250.0 / 65536.0)
#define G_PER_LSB (float)(2 / 65536.0)

#endif 
