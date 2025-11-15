// SPDX-License-Identifier: GPL-2.0-only
// Copyright (C) 2022, Input Labs Oy.

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include "logging.h"
#include "webusb.h"
#include "board.h"

//LogLevel logging::logging_level = LOG_INFO;
// LogMask logging_mask = LOG_BASIC;

// // Control if the logging logic should assume the execution is withint the
// // main loop or not.
// bool logging_onloop = false;

namespace logging {
// void logging_set_level(LogLevel level) {
//     logging_level = level;
//     config_set_log_level(logging_level);
// }

// LogLevel logging_get_level() {
//     return logging_level;
// }

// void logging_set_mask(LogMask mask) {
//     logging_mask = mask;
//     config_set_log_mask(logging_mask);
// }

// bool logging_has_mask(LogMask mask) {
//     return logging_mask & mask;
// }

// LogMask logging_get_mask() {
//     return logging_mask;
// }

// void logging_set_onloop(bool value) {
//     logging_onloop = value;
// }

// bool logging_get_onloop() {
//     return logging_onloop;
// }

// void logging_load_from_config() {
//     Config *config = config_read();
//     logging_level = config->log_level;
//     logging_mask = config->log_mask;
// }

// void logging_init() {
//     if (logging_level < LOG_DEBUG) return;
//     for(uint8_t i=0; i<80; i++) printf("_");
//     printf("\n");
// }

void write(const char *msg, va_list args) {
    char formatted[512] = {0,};
    uint16_t len = vsnprintf(formatted, 512, msg, args);
    printf(formatted);  // Serial.
    webusb_write(formatted,len);  // WebUSB.
}

void info(const char *msg, ...) {
    va_list va;
    va_start(va, 0);
    write(msg, va);
    va_end(va);
}

void warn(const char *msg, ...) {
    va_list va;
    va_start(va, 0);
    char warn[256] = {0,};
    sprintf(warn, "WARNING: %s", msg);
    write(warn, va);
    va_end(va);
}

void error(const char *msg, ...) {
    va_list va;
    va_start(va, 0);
    char error[256] = {0,};
    sprintf(error, "ERROR: %s", msg);
    write(error, va);
    va_end(va);
}

void debug(const char *msg, ...) {
    if (Board::get_nvm_data()->log_level < LOG_DEBUG) return;
    va_list va;
    va_start(va, 0);
    write(msg, va);
    va_end(va);
}



void debug_uart(const char *msg, ...) {
    if (Board::get_nvm_data()->log_level < LOG_DEBUG) return;
    va_list va;
    va_start(va, 0);
    char formatted[256] = {0,};
    vsnprintf(formatted, 256, msg, va);
    printf(formatted);  // Serial.
    va_end(va);
}

}