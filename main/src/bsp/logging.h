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

// typedef enum _LogMask {
//     LOG_BASIC = 0,
//     LOG_USB = 1,
//     LOG_CPU_SENS = 2,
//     LOG_WIRELESS = 4,
// } LogMask;

// void logging_set_onloop(bool value);
// bool logging_get_onloop();

// void logging_set_level(LogLevel level);
// LogLevel logging_get_level();

// void logging_set_mask(LogMask mask);
// bool logging_has_mask(LogMask mask);
// LogMask logging_get_mask();

// void logging_init();
// void logging_load_from_config();

namespace logging {
    // 核心输出函数
    void write(const char *msg, va_list args);
    
    // 各级别日志函数
    void info(const char *msg, ...);
    void warn(const char *msg, ...);
    void error(const char *msg, ...);
    void debug(const char *msg, ...);
    void debug_uart(const char *msg, ...);
    
    // // 可选：设置日志级别
    // void setLevel(int level);
    
    // // 可选：初始化和清理函数
    // void initialize();
    // void shutdown();
}
