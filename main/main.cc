/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "application.h"



extern "C"  void app_main(void)
{
    // Initialize the default event loop
    Application::GetInstance().Start();
}
