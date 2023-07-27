#include "pico/stdlib.h"

#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

#include "blink_agent.h"


//Standard Task priority
#define TASK_PRIORITY		( tskIDLE_PRIORITY + 1UL )

//LED PAD to use
#define LED1_PAD			25
#define LED2_PAD			3
#define LED3_PAD			4
#define LED4_PAD			5

#define CYCLE_TICKS			1000


void run_time_stats(){
    TaskStatus_t *pxTaskStatusArray;
    volatile UBaseType_t uxArraySize, x;
    unsigned long ulTotalRunTime;

    // Get number of tasks
    uxArraySize = uxTaskGetNumberOfTasks();
    printf("Number of tasks %d\n", uxArraySize);

    // Allocate a TaskStatus structure for each task
    pxTaskStatusArray = (TaskStatus_t *) pvPortMalloc(uxArraySize * sizeof(TaskStatus_t));

    if(pxTaskStatusArray != NULL) {
        // Generate raw status information about each task.
        uxArraySize = uxTaskGetSystemState(pxTaskStatusArray, uxArraySize, &ulTotalRunTime);

        // Print stats
        for(x=0; x< uxArraySize; x++){
            printf("Task: %d \t cPri:%d \t bPri:%d \t hw:%d \t%s\n",
                   pxTaskStatusArray[ x ].xTaskNumber ,
                   pxTaskStatusArray[ x ].uxCurrentPriority ,
                   pxTaskStatusArray[ x ].uxBasePriority ,
                   pxTaskStatusArray[ x ].usStackHighWaterMark ,
                   pxTaskStatusArray[ x ].pcTaskName
            );
        }

        vPortFree(pxTaskStatusArray);
    } else {
        printf("Failed to allocate space for tasks\n");
    }

    // Get heap allocation information
    HeapStats_t heapStats;
    vPortGetHeapStats(&heapStats);
    printf("HEAP avl: %d, blocks %d, alloc: %d, free: %d\n",
           heapStats.xAvailableHeapSpaceInBytes,
           heapStats.xNumberOfFreeBlocks,
           heapStats.xNumberOfSuccessfulAllocations,
           heapStats.xNumberOfSuccessfulFrees
    );

}

/***
 * Main task to blink external LED
 * @param params - unused
 */
void mainTask(void *params){
    BlinkAgent worker1(LED1_PAD, CYCLE_TICKS/2);
    BlinkAgent worker2(LED2_PAD, CYCLE_TICKS/4);
    BlinkAgent worker3(LED3_PAD, CYCLE_TICKS/3);
    BlinkAgent worker4(LED4_PAD, CYCLE_TICKS/6);

    printf("Main task started \n");
    worker1.start("Worker 1", TASK_PRIORITY);
    worker2.start("Worker 2", TASK_PRIORITY);
    worker3.start("Worker 3", TASK_PRIORITY);
    worker4.start("Worker 4", TASK_PRIORITY);

    while (true){
        run_time_stats();
        vTaskDelay(3000);
    }
}

/***
 * Launch the tasks and scheduler
 */
void vLaunch(void){
    // Start blink task
    TaskHandle_t task;
    xTaskCreate(mainTask, "MainThread", 500, NULL, TASK_PRIORITY, &task);

    // Start tasks and timer running
    vTaskStartScheduler();
}

/***
 * Main
 * @return
 */
int main(void){
    stdio_init_all();
    sleep_ms(2000);
    printf("GO!\n");
    // Start tasks and scheduler
    const char *rtos_name = "FreeRTOS";
    printf("Starting %s on core 0:\n", rtos_name);
    vLaunch();

    return 0;
}