#include "cpu.h"

#include "Board.h"
#include "logging.h"

char* CPU::taskRuntimeBuffer = NULL;
void CPU::printTaskAndMemoryInfo(void *pvParameters)
{
    size_t cap_free_size,cap_total_size,cap_malloc,cap_min_free_size;
    while (1)
    { 
        vTaskList(taskRuntimeBuffer); 
        logging::info("---------------------------------------------\r\n");
        logging::info("task_name   task_status priority    stack task_id\r\n");
        logging::info("%s", taskRuntimeBuffer);
        logging::info("---------------------------------------------\r\n");
        
        vTaskGetRunTimeStats(taskRuntimeBuffer);
        logging::info("task_name       run_cnt         usage_rate\r\n");
        logging::info("%s", taskRuntimeBuffer);
        logging::info("---------------------------------------------\r\n\n");
        cap_free_size = heap_caps_get_free_size(MALLOC_CAP_INTERNAL);
        cap_total_size = heap_caps_get_total_size(MALLOC_CAP_INTERNAL);
        cap_malloc   =  cap_total_size-cap_free_size;
        cap_min_free_size = heap_caps_get_minimum_free_size(MALLOC_CAP_INTERNAL);
        logging::info("INTERNAL RAM left %dB/%dB %dB,min:%dB\n",cap_free_size,cap_total_size,cap_malloc,cap_min_free_size);//
        cap_free_size = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
        cap_total_size = heap_caps_get_total_size(MALLOC_CAP_SPIRAM);
        cap_malloc   =  cap_total_size-cap_free_size;
        cap_min_free_size = heap_caps_get_minimum_free_size(MALLOC_CAP_SPIRAM);
        logging::info("SPI      RAM left %dB/%dB %dB,min:%dB\n",cap_free_size,cap_total_size,cap_malloc,cap_min_free_size);//
        
        vTaskDelay(pdMS_TO_TICKS(10000));  
        //nvs_get_stats   
    }
}
void CPU::toggleTaskMonitor(bool enable)
{
    if(enable)
    {
        if(taskMonitorHandle == NULL)
            xTaskCreate(printTaskAndMemoryInfo, "task_monitor", 4096, NULL, 1, &taskMonitorHandle);
    }else
    {
        vTaskDelete(taskMonitorHandle);
        taskMonitorHandle = NULL;
    }
}

CPU::CPU()
{
    logging::info("Init_CPU_task\n");
    taskRuntimeBuffer = (char *)heap_caps_malloc(1024, MALLOC_CAP_SPIRAM);
    if(Board::get_nvm_data()->log_mask.cpu )
    {
        toggleTaskMonitor(1);
    }
}