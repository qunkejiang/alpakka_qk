#include "cpu.h"

#include "Board.h"
#include "logging.h"
#include "string.h"

TaskStatus_t *start_array = NULL, *end_array = NULL;
UBaseType_t start_array_size = 0, end_array_size = 0;
configRUN_TIME_COUNTER_TYPE start_run_time = 0, end_run_time = 0;
char* CPU::taskRuntimeBuffer = NULL;

void CPU::printTaskAndMemoryInfo(void *pvParameters)
{
    uint32_t total_elapsed_time = 0; // 提前声明这个变量
    size_t cap_free_size,cap_total_size,cap_malloc;
    while (1)
    { 

        cap_free_size = heap_caps_get_free_size(MALLOC_CAP_INTERNAL);
        cap_total_size = heap_caps_get_total_size(MALLOC_CAP_INTERNAL);
        cap_malloc   =  cap_total_size-cap_free_size;
        logging::info("INTERNAL RAM left %dB/%dB %dB\n",cap_free_size,cap_total_size,cap_malloc);//
        cap_free_size = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
        cap_total_size = heap_caps_get_total_size(MALLOC_CAP_SPIRAM);
        cap_malloc   =  cap_total_size-cap_free_size;
        logging::info("SPI      RAM left %dB/%dB %dB\n",cap_free_size,cap_total_size,cap_malloc);//
        
        start_array_size = uxTaskGetSystemState(start_array, start_array_size, &start_run_time);
        vTaskDelay(pdMS_TO_TICKS(10000)); 
        vTaskList(taskRuntimeBuffer); 
        logging::info("---------------------------------------------\r\n");
        logging::info("task_name task_status priority  stack  task_id\r\n");
        logging::info("%s", taskRuntimeBuffer);
        logging::info("---------------------------------------------\r\n");
        
        // vTaskGetRunTimeStats(taskRuntimeBuffer);
        // logging::info("task_name       run_cnt         usage_rate\r\n");
        // logging::info("%s", taskRuntimeBuffer);
        // logging::info("---------------------------------------------\r\n\n");


        //Get post delay task states
        end_array_size = uxTaskGetSystemState(end_array, end_array_size, &end_run_time);

        //Calculate total_elapsed_time in units of run time stats clock period.
        total_elapsed_time = (end_run_time - start_run_time);
        

        logging::info("| Task           |  Run Time  | Percentage\n");
        //Match each task in start_array to those in the end_array
        for (int i = 0; i < start_array_size; i++) {
            int k = -1;
            for (int j = 0; j < end_array_size; j++) {
                if (start_array[i].xHandle == end_array[j].xHandle) {
                    k = j;
                    //Mark that task have been matched by overwriting their handles
                    start_array[i].xHandle = NULL;
                    end_array[j].xHandle = NULL;
                    break;
                }
            }
            //Check if matching task found
            if (k >= 0) {
                uint32_t task_elapsed_time = end_array[k].ulRunTimeCounter - start_array[i].ulRunTimeCounter;
                uint32_t percentage_time = (task_elapsed_time * 100UL) / (total_elapsed_time);
                logging::info("| %14s | %10d | %d%%\n", start_array[i].pcTaskName, task_elapsed_time, percentage_time);
            }
        }

        //Print unmatched tasks
        for (int i = 0; i < start_array_size; i++) {
            if (start_array[i].xHandle != NULL) {
                logging::info("| %s | Deleted\n", start_array[i].pcTaskName);
            }
        }
        for (int i = 0; i < end_array_size; i++) {
            if (end_array[i].xHandle != NULL) {
                logging::info("| %s | Created\n", end_array[i].pcTaskName);
            }
        }
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

    //Allocate array to store current task states
    start_array_size = uxTaskGetNumberOfTasks() + 10;
    start_array = (TaskStatus_t *)heap_caps_malloc(sizeof(TaskStatus_t) * start_array_size, MALLOC_CAP_SPIRAM);
    if (start_array == NULL) {
    }


    //Allocate array to store tasks states post delay
    end_array_size = start_array_size;
    end_array = (TaskStatus_t *)heap_caps_malloc(sizeof(TaskStatus_t) * end_array_size, MALLOC_CAP_SPIRAM);
    if (end_array == NULL) {
    }

    if(Board::get_nvm_data()->log_mask.cpu )
    {
        toggleTaskMonitor(1);
    }
}