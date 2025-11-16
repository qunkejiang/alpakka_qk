// SPDX-License-Identifier: GPL-2.0-only
// Copyright (C) 2022, Input Labs Oy.

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include "webusb.h"
#include "board.h"

namespace logging {

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