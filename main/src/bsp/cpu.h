#ifndef CPU_C_H
#define CPU_C_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

class CPU {
private:
    TaskHandle_t taskMonitorHandle  = NULL;
public:
    static char* taskRuntimeBuffer;
    static void printTaskAndMemoryInfo(void *pvParameters);
    void toggleTaskMonitor(bool enable); 
    CPU();
};

#endif
