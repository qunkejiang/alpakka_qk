#ifndef UI_HANDLE_H
#define UI_HANDLE_H

#include <stdint.h>
#include <stdbool.h>
#include "board.h"


typedef struct 
{
    key_matrix_t key_matrix;
    float adc_value[4];
    touch_data_t touch_data;
    int rotary_count = 0;
    Imu_Data_t Data;
    axp2101_data_t axp_data;
}sensor_data_t;

void ui_periodic_handle(sensor_data_t *sensor_data, Board* board);

#endif // UI_HANDLE_H
