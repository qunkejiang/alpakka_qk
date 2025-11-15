
#ifndef MATRIX_KEYBOARD_H
#define MATRIX_KEYBOARD_H
#include <stdint.h>
#include <stdlib.h>


#include "driver/dedic_gpio.h"
#include "driver/gptimer.h"
#include "board_config.h"
typedef struct {
    uint8_t *key_edge;
    uint8_t *key_value;
} key_matrix_t;


typedef struct {
    const int *row_gpios;  /*!< Array, contains GPIO numbers used by row line */
    const int *col_gpios;  /*!< Array, contains GPIO numbers used by column line */
    uint32_t nr_row_gpios; /*!< row_gpios array size */
    uint32_t nr_col_gpios; /*!< col_gpios array size */
    uint32_t debounce_us;  /*!< Debounce time */
} matrix_kbd_config_t;

typedef struct {
    dedic_gpio_bundle_handle_t row_bundle;
    dedic_gpio_bundle_handle_t col_bundle;
    uint8_t nr_row_gpios;
    uint8_t nr_col_gpios;
}matrix_kbd_t;

class matrix_kbd{
private:
    void InitializeKeyboardTimer(uint16_t count);
    static bool ScanKeyboardColumns(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_data);
    matrix_kbd_t *mkbd = NULL;

public:
    uint8_t key_value[MATRIX_KEYBOARD_COLS_NUM]={0,0,0,0};
    matrix_kbd(matrix_kbd_config_t *config);
};

#endif 