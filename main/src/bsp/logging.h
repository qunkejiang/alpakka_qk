// SPDX-License-Identifier: GPL-2.0-only
// Copyright (C) 2022, Input Labs Oy.

#pragma once
#include <stdarg.h>

typedef enum _LogLevel {
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_DEBUG,
} LogLevel;

namespace logging {
    // 核心输出函数
    void write(const char *msg, va_list args);
    
    // 各级别日志函数
    void info(const char *msg, ...);
    void warn(const char *msg, ...);
    void error(const char *msg, ...);
    void debug(const char *msg, ...);
    void debug_uart(const char *msg, ...);
    
}
