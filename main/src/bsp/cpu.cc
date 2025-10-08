#include "cpu.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "esp_log.h"

#define TAG  "CPU"

void CPU::PrintTaskAndMemoryInfo(void *pvParameters)
{
    char* InfoBuffer = (char *)heap_caps_malloc(1024, MALLOC_CAP_SPIRAM);
    size_t cap_free_size,cap_total_size,cap_malloc,cap_min_free_size;
    while (1)
    {
        vTaskList(InfoBuffer); 
        printf("---------------------------------------------\r\n");
        printf("task_name   task_status priority    stack task_id\r\n");
        printf("%s", InfoBuffer);
        printf("---------------------------------------------\r\n");
        
        vTaskGetRunTimeStats(InfoBuffer);
        printf("task_name       run_cnt         usage_rate\r\n");
        printf("%s", InfoBuffer);
        printf("---------------------------------------------\r\n\n");
        cap_free_size = heap_caps_get_free_size(MALLOC_CAP_INTERNAL);
        cap_total_size = heap_caps_get_total_size(MALLOC_CAP_INTERNAL);
        cap_malloc   =  cap_total_size-cap_free_size;
        cap_min_free_size = heap_caps_get_minimum_free_size(MALLOC_CAP_INTERNAL);
        ESP_LOGW(TAG, "INTERNAL RAM left %dB/%dB %dB,min:%dB",cap_free_size,cap_total_size,cap_malloc,cap_min_free_size);//
        cap_free_size = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
        cap_total_size = heap_caps_get_total_size(MALLOC_CAP_SPIRAM);
        cap_malloc   =  cap_total_size-cap_free_size;
        cap_min_free_size = heap_caps_get_minimum_free_size(MALLOC_CAP_SPIRAM);
        ESP_LOGW(TAG, "SPI      RAM left %dB/%dB %dB,min:%dB",cap_free_size,cap_total_size,cap_malloc,cap_min_free_size);//
        vTaskDelay(pdMS_TO_TICKS(10000));     
    }
}


// void Init_CPU_task() {
// }
CPU::CPU()
{
    ESP_LOGI(TAG, "Init_CPU_task");
    xTaskCreate(PrintTaskAndMemoryInfo, "tp", 2560, NULL, 5, NULL);
    // Initialization code for Touch can be added here if needed
}