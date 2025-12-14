/*
 * FreeRTOS V202107.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/******************************************************************************
 * NOTE 1:  This project provides two demo applications.  A simple blinky
 * style project, and a more comprehensive test and demo application.  The
 * mainCREATE_SIMPLE_BLINKY_DEMO_ONLY setting in main.c is used to select
 * between the two.  See the notes on using mainCREATE_SIMPLE_BLINKY_DEMO_ONLY
 * in main.c.  This file implements the simply blinky style version.
 *
 * NOTE 2:  This file only contains the source code that is specific to the
 * basic demo.  Generic functions, such FreeRTOS hook functions, and functions
 * required to configure the hardware are defined in main.c.
 ******************************************************************************
 *
 * main_blinky() creates one queue, and two tasks.  It then starts the
 * scheduler.
 *
 * The Queue Send Task:
 * The queue send task is implemented by the prvQueueSendTask() function in
 * this file.  prvQueueSendTask() sits in a loop that causes it to repeatedly
 * block for 200 milliseconds, before sending the value 100 to the queue that
 * was created within main_blinky().  Once the value is sent, the task loops
 * back around to block for another 200 milliseconds...and so on.
 *
 * The Queue Receive Task:
 * The queue receive task is implemented by the prvQueueReceiveTask() function
 * in this file.  prvQueueReceiveTask() sits in a loop where it repeatedly
 * blocks on attempts to read data from the queue that was created within
 * main_blinky().  When data is received, the task checks the value of the
 * data, and if the value equals the expected 100, toggles an LED.  The 'block
 * time' parameter passed to the queue receive function specifies that the
 * task should be held in the Blocked state indefinitely to wait for data to
 * be available on the queue.  The queue receive task will only leave the
 * Blocked state when the queue send task writes to the queue.  As the queue
 * send task writes to the queue every 200 milliseconds, the queue receive
 * task leaves the Blocked state every 200 milliseconds, and therefore toggles
 * the LED every 200 milliseconds.
 */
//#define configENABLE_BACKWARD_COMPATIBILITY    1
#include <stdio.h>
/* Kernel includes. */
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
/* Standard demo includes. */
#include "partest.h"

/* Priorities at which the tasks are created. */
#define mainQUEUE_RECEIVE_TASK_PRIORITY     ( tskIDLE_PRIORITY + 2 )
#define mainQUEUE_SEND_TASK_PRIORITY        ( tskIDLE_PRIORITY + 1 )

/* The rate at which data is sent to the queue.  The 200ms value is converted
to ticks using the portTICK_PERIOD_MS constant. */
#define mainQUEUE_SEND_FREQUENCY_MS         ( pdMS_TO_TICKS( 200 ) )

/* The number of items the queue can hold.  This is 1 as the receive task
will remove items as they are added, meaning the send task should always find
the queue empty. */
#define mainQUEUE_LENGTH                    ( 1 )

/* The LED toggled by the Rx task. */
#define mainTASK_LED                        ( 0 )

/*-----------------------------------------------------------*/

/*
 * Called by main when mainCREATE_SIMPLE_BLINKY_DEMO_ONLY is set to 1 in
 * main.c.
 */
void main_blinky( void );
void main_lab( void );
/*
 * The tasks as described in the comments at the top of this file.
 */
static void prvQueueReceiveTask( void *pvParameters );
static void prvQueueSendTask( void *pvParameters );
static void Task1();
static void Task2();
static void Task2_1();
static void Task2_2();
static void Task2_3();
static void print();
static void my_idle();
/*-----------------------------------------------------------*/

/* The queue used by both tasks. */
static QueueHandle_t xQueue = NULL;
typedef struct tskTaskControlBlock       /* The old naming convention is used to prevent breaking kernel aware debuggers. */
{
    volatile StackType_t * pxTopOfStack; /*< Points to the location of the last item placed on the tasks stack.  THIS MUST BE THE FIRST MEMBER OF THE TCB STRUCT. */

    #if ( portUSING_MPU_WRAPPERS == 1 )
        xMPU_SETTINGS xMPUSettings; /*< The MPU settings are defined as part of the port layer.  THIS MUST BE THE SECOND MEMBER OF THE TCB STRUCT. */
    #endif

    ListItem_t xStateListItem;                  /*< The list that the state list item of a task is reference from denotes the state of that task (Ready, Blocked, Suspended ). */
    ListItem_t xEventListItem;                  /*< Used to reference a task from an event list. */
    UBaseType_t uxPriority;                     /*< The priority of the task.  0 is the lowest priority. */
    StackType_t * pxStack;                      /*< Points to the start of the stack. */
    int uxCompTime;
    int uxPeriod;
    int uxDeadline;
    char pcTaskName[ configMAX_TASK_NAME_LEN ]; /*< Descriptive name given to the task when created.  Facilitates debugging only. */ /*lint !e971 Unqualified char types are allowed for strings and single characters only. */

    #if ( ( portSTACK_GROWTH > 0 ) || ( configRECORD_STACK_HIGH_ADDRESS == 1 ) )
        StackType_t * pxEndOfStack; /*< Points to the highest valid address for the stack. */
    #endif

    #if ( portCRITICAL_NESTING_IN_TCB == 1 )
        UBaseType_t uxCriticalNesting; /*< Holds the critical section nesting depth for ports that do not maintain their own count in the port layer. */
    #endif

    #if ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxTCBNumber;  /*< Stores a number that increments each time a TCB is created.  It allows debuggers to determine when a task has been deleted and then recreated. */
        UBaseType_t uxTaskNumber; /*< Stores a number specifically for use by third party trace code. */
    #endif

    #if ( configUSE_MUTEXES == 1 )
        UBaseType_t uxBasePriority; /*< The priority last assigned to the task - used by the priority inheritance mechanism. */
        UBaseType_t uxMutexesHeld;
    #endif

    #if ( configUSE_APPLICATION_TASK_TAG == 1 )
        TaskHookFunction_t pxTaskTag;
    #endif

    #if ( configNUM_THREAD_LOCAL_STORAGE_POINTERS > 0 )
        void * pvThreadLocalStoragePointers[ configNUM_THREAD_LOCAL_STORAGE_POINTERS ];
    #endif

    #if ( configGENERATE_RUN_TIME_STATS == 1 )
        uint32_t ulRunTimeCounter; /*< Stores the amount of time the task has spent in the Running state. */
    #endif

    #if ( configUSE_NEWLIB_REENTRANT == 1 )
        /* Allocate a Newlib reent structure that is specific to this task.
         * Note Newlib support has been included by popular demand, but is not
         * used by the FreeRTOS maintainers themselves.  FreeRTOS is not
         * responsible for resulting newlib operation.  User must be familiar with
         * newlib and must provide system-wide implementations of the necessary
         * stubs. Be warned that (at the time of writing) the current newlib design
         * implements a system-wide malloc() that must be provided with locks.
         *
         * See the third party link http://www.nadler.com/embedded/newlibAndFreeRTOS.html
         * for additional information. */
        struct  _reent xNewLib_reent;
    #endif

    #if ( configUSE_TASK_NOTIFICATIONS == 1 )
        volatile uint32_t ulNotifiedValue[ configTASK_NOTIFICATION_ARRAY_ENTRIES ];
        volatile uint8_t ucNotifyState[ configTASK_NOTIFICATION_ARRAY_ENTRIES ];
    #endif

    /* See the comments in FreeRTOS.h with the definition of
     * tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE. */
    #if ( tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE != 0 ) /*lint !e731 !e9029 Macro has been consolidated for readability reasons. */
        uint8_t ucStaticallyAllocated;                     /*< Set to pdTRUE if the task is a statically allocated to ensure no attempt is made to free the memory. */
    #endif

    #if ( INCLUDE_xTaskAbortDelay == 1 )
        uint8_t ucDelayAborted;
    #endif

    #if ( configUSE_POSIX_ERRNO == 1 )
        int iTaskErrno;
    #endif
} tskTCB;
/*-----------------------------------------------------------*/

void main_blinky( void )
{
    /* Create the queue. */
    xQueue = xQueueCreate( mainQUEUE_LENGTH, sizeof( uint32_t ) );

    if( xQueue != NULL )
    {
        /* Start the two tasks as described in the comments at the top of this
        file. */
        xTaskCreate( prvQueueReceiveTask,               /* The function that implements the task. */
                    "Rx",                               /* The text name assigned to the task - for debug only as it is not used by the kernel. */
                    configMINIMAL_STACK_SIZE,           /* The size of the stack to allocate to the task. */
                    NULL,                               /* The parameter passed to the task - not used in this case. */
                    mainQUEUE_RECEIVE_TASK_PRIORITY,    /* The priority assigned to the task. */
                    NULL );                             /* The task handle is not required, so NULL is passed. */

        xTaskCreate( prvQueueSendTask, "TX", configMINIMAL_STACK_SIZE, NULL, mainQUEUE_SEND_TASK_PRIORITY, NULL );

        /* Start the tasks and timer running. */
        vTaskStartScheduler();
    }

    /* If all is well, the scheduler will now be running, and the following
    line will never be reached.  If the following line does execute, then
    there was insufficient FreeRTOS heap memory available for the Idle and/or
    timer tasks to be created.  See the memory management section on the
    FreeRTOS web site for more details on the FreeRTOS heap
    http://www.freertos.org/a00111.html. */
    for( ;; );
}

typedef struct Parameters{
    int period;
    int comptime;
};

void main_lab( void )
{

    buf_end = 0;
    /* Create the queue. */
    xQueue = xQueueCreate( mainQUEUE_LENGTH, sizeof( uint32_t ) );
    struct Parameters one;
    one.period=3;
    one.comptime=1;

    struct Parameters two;
    two.period=5;
    two.comptime=3;

    struct Parameters two_one;
    two_one.period=4;
    two_one.comptime=1;
    struct Parameters two_two;
    two_two.period=5;
    two_two.comptime=2;
    struct Parameters two_three;
    two_three.period=10;
    two_three.comptime=2;

    struct Parameters pri;
    pri.period=0;
    pri.comptime=0;

    struct Parameters idl;
    idl.period=500;
    idl.comptime=0;

    if( xQueue != NULL )
    {


//        xTaskCreate( Task1,               /* The function that implements the task. */
//                    "T1",                               /* The text name assigned to the task - for debug only as it is not used by the kernel. */
//                    configMINIMAL_STACK_SIZE,           /* The size of the stack to allocate to the task. */
//                    (void *)&one,                               /* The parameter passed to the task  */
//                    1,    /* The priority assigned to the task. */
//                    NULL );                             /* The task handle is not required, so NULL is passed. */
//        xTaskCreate( Task2,               /* The function that implements the task. */
//                           "T2",                               /* The text name assigned to the task - for debug only as it is not used by the kernel. */
//                           configMINIMAL_STACK_SIZE,           /* The size of the stack to allocate to the task. */
//                           (void *)&two,                               /* The parameter passed to the task  */
//                           2,    /* The priority assigned to the task. */
//                           NULL );


        xTaskCreate( Task2_1,               /* The function that implements the task. */
                            "T1",                               /* The text name assigned to the task - for debug only as it is not used by the kernel. */
                            configMINIMAL_STACK_SIZE,           /* The size of the stack to allocate to the task. */
                            (void *)&two_one,                               /* The parameter passed to the task  */
                            1,    /* The priority assigned to the task. */
                            NULL );                             /* The task handle is not required, so NULL is passed. */

        xTaskCreate( Task2_2,               /* The function that implements the task. */
                            "T2",                               /* The text name assigned to the task - for debug only as it is not used by the kernel. */
                            configMINIMAL_STACK_SIZE,           /* The size of the stack to allocate to the task. */
                            (void *)&two_two,                               /* The parameter passed to the task  */
                            2,    /* The priority assigned to the task. */
                            NULL );
                           /* The task handle is not required, so NULL is passed. */

        xTaskCreate( Task2_3,               /* The function that implements the task. */
                            "T3",                               /* The text name assigned to the task - for debug only as it is not used by the kernel. */
                            configMINIMAL_STACK_SIZE,           /* The size of the stack to allocate to the task. */
                            (void *)&two_three,                               /* The parameter passed to the task  */
                            3,    /* The priority assigned to the task. */
                            NULL );
           xTaskCreate( print,               /* The function that implements the task. */
                                    "print",                               /* The text name assigned to the task - for debug only as it is not used by the kernel. */
                                    configMINIMAL_STACK_SIZE,           /* The size of the stack to allocate to the task. */
                                    (void *)&pri,                               /* The parameter passed to the task  */
                                    4,    /* The priority assigned to the task. */
                                    NULL );
           xTaskCreate( my_idle,               /* The function that implements the task. */
                                    "Idle",                               /* The text name assigned to the task - for debug only as it is not used by the kernel. */
                                    configMINIMAL_STACK_SIZE,           /* The size of the stack to allocate to the task. */
                                    (void *)&idl,                               /* The parameter passed to the task  */
                                    4,    /* The priority assigned to the task. */
                                    NULL );

        /* Start the tasks and timer running. */
        vTaskStartScheduler();
    }

    /* If all is well, the scheduler will now be running, and the following
    line will never be reached.  If the following line does execute, then
    there was insufficient FreeRTOS heap memory available for the Idle and/or
    timer tasks to be created.  See the memory management section on the
    FreeRTOS web site for more details on the FreeRTOS heap
    http://www.freertos.org/a00111.html. */
    for( ;; );
}

const int END_TIME = 50;

static void Task1(void * pvParameters)
{
    long start ; //the start time
    long end ; //the end time
    long toDelay;
    start=0;
    TickType_t xStartTime;
    xStartTime = 0; // 取得目前系統 tick
    TaskHandle_t pxCurrentTCB = xTaskGetCurrentTaskHandle();
    while(1)
    {
        while(pxCurrentTCB->uxCompTime>0) {}
        end=xTaskGetTickCount() ; // end time
        if (end > END_TIME) break;

        toDelay=(pxCurrentTCB->uxPeriod)-(end-start) ;
        start=start+(pxCurrentTCB->uxPeriod) ; // next start time
        portDISABLE_INTERRUPTS();
        pxCurrentTCB->uxCompTime = 1;
        pxCurrentTCB->uxDeadline = pxCurrentTCB->uxDeadline + pxCurrentTCB->uxPeriod;
        portENABLE_INTERRUPTS();

        //vTaskDelay (toDelay); // delay and wait (P-C) times
        vTaskDelayUntil(&xStartTime, pxCurrentTCB->uxPeriod);
    }
    vTaskDelete(NULL);

}

static void Task2(void * pvParameters)
{
    long start ; //the start time
    long end ; //the end time
    long toDelay;
    start=0;
    TickType_t xStartTime;
    xStartTime = 0; // 取得目前系統 tick
    TaskHandle_t pxCurrentTCB = xTaskGetCurrentTaskHandle();
    while(1)
    {
        while(pxCurrentTCB->uxCompTime>0) {}
        end=xTaskGetTickCount() ; // end time
        if (end > END_TIME) break;

        toDelay=(pxCurrentTCB->uxPeriod)-(end-start) ;
        start=start+(pxCurrentTCB->uxPeriod) ; // next start time
        portDISABLE_INTERRUPTS();
        pxCurrentTCB->uxCompTime = 3;
        pxCurrentTCB->uxDeadline = pxCurrentTCB->uxDeadline + pxCurrentTCB->uxPeriod;
        portENABLE_INTERRUPTS();

        //vTaskDelay (toDelay); // delay and wait (P-C) times
        vTaskDelayUntil(&xStartTime, pxCurrentTCB->uxPeriod);
    }
    vTaskDelete(NULL);
}

static void Task2_1(void * pvParameters)
{
    long start ; //the start time
    long end ; //the end time
    int toDelay;
    start=0;
    TickType_t xStartTime;
    xStartTime = 0; // 取得目前系統 tick
    TaskHandle_t pxCurrentTCB = xTaskGetCurrentTaskHandle();
    while(1)
    {
        while(pxCurrentTCB->uxCompTime>0) //C ticks
        {
        // do nothing
        }
        end=xTaskGetTickCount() ; // end time
        if (end > END_TIME) break;

        toDelay=(pxCurrentTCB->uxPeriod)-(end-start) ;
        start=start+(pxCurrentTCB->uxPeriod) ; // next start time
        portDISABLE_INTERRUPTS();
        pxCurrentTCB->uxCompTime = 1;
        pxCurrentTCB->uxDeadline = pxCurrentTCB->uxDeadline + pxCurrentTCB->uxPeriod;
        portENABLE_INTERRUPTS();

        //vTaskDelay (toDelay); // delay and wait (P-C) times
        vTaskDelayUntil(&xStartTime, pxCurrentTCB->uxPeriod);
    }
    vTaskDelete(NULL);
}

static void Task2_2(void * pvParameters)
{
    long start ; //the start time
    long end ; //the end time
    long toDelay;
    start=0;
    TickType_t xStartTime;
    xStartTime = 0; // 取得目前系統 tick
    TaskHandle_t pxCurrentTCB = xTaskGetCurrentTaskHandle();
    while(1)
    {
        while(pxCurrentTCB->uxCompTime>0) //C ticks
        {
        // do nothing
        }
        end=xTaskGetTickCount() ; // end time
        if (end > END_TIME) break;

        toDelay=(pxCurrentTCB->uxPeriod)-(end-start) ;
        start=start+(pxCurrentTCB->uxPeriod) ; // next start time
        portDISABLE_INTERRUPTS();
        pxCurrentTCB->uxCompTime = 2;
        pxCurrentTCB->uxDeadline = pxCurrentTCB->uxDeadline + pxCurrentTCB->uxPeriod;
        portENABLE_INTERRUPTS();

        //vTaskDelay (toDelay); // delay and wait (P-C) times
        vTaskDelayUntil(&xStartTime, pxCurrentTCB->uxPeriod);
    }
    vTaskDelete(NULL);
}

static void Task2_3(void * pvParameters)
{
    long start ; //the start time
    long end ; //the end time
    long toDelay;
    start=0;
    TickType_t xStartTime;
    xStartTime = 0; // 取得目前系統 tick
    TaskHandle_t pxCurrentTCB = xTaskGetCurrentTaskHandle();
    while(1)
    {
        while(pxCurrentTCB->uxCompTime>0) //C ticks
        {
        // do nothing
        }
        end=xTaskGetTickCount() ; // end time
        if (end > END_TIME) break;

        toDelay=(pxCurrentTCB->uxPeriod)-(end-start) ;
        start=start+(pxCurrentTCB->uxPeriod) ; // next start time
        portDISABLE_INTERRUPTS();
        pxCurrentTCB->uxCompTime = 2;
        pxCurrentTCB->uxDeadline = pxCurrentTCB->uxDeadline + pxCurrentTCB->uxPeriod;
        portENABLE_INTERRUPTS();

        //vTaskDelay (toDelay); // delay and wait (P-C) times
        vTaskDelayUntil(&xStartTime, pxCurrentTCB->uxPeriod);

    }
    vTaskDelete(NULL);
}

static void print(void * pvParameters){
    TickType_t xStartTime;
    xStartTime = xTaskGetTickCount(); // 取得目前系統 tick

    vTaskDelayUntil(&xStartTime, END_TIME); // 等待直到 xStartTime + END_TIME ticks
    int idx=0;
    TaskHandle_t pxCurrentTCB = xTaskGetCurrentTaskHandle();
    while(1){
        portDISABLE_INTERRUPTS();
        while (idx < buf_end) {
//            printf ("%c, %d, %d\n", buf[idx ++], idx, buf_end);
            printf ("%c", buf[idx ++]);
        }
//        pxCurrentTCB->uxDeadline = pxCurrentTCB->uxDeadline + 15;
        portENABLE_INTERRUPTS();
    }

}

static void my_idle(void * pvParameters){
    while(1){
        vTaskDelay(1);
    }
}
/*-----------------------------------------------------------*/

static void prvQueueSendTask( void *pvParameters )
{
TickType_t xNextWakeTime;
const unsigned long ulValueToSend = 100UL;

    /* Remove compiler warning about unused parameter. */
    ( void ) pvParameters;

    /* Initialise xNextWakeTime - this only needs to be done once. */
    xNextWakeTime = xTaskGetTickCount();

    for( ;; )
    {
        /* Place this task in the blocked state until it is time to run again. */
        vTaskDelayUntil( &xNextWakeTime, mainQUEUE_SEND_FREQUENCY_MS );

        /* Send to the queue - causing the queue receive task to unblock and
        toggle the LED.  0 is used as the block time so the sending operation
        will not block - it shouldn't need to block as the queue should always
        be empty at this point in the code. */
        xQueueSend( xQueue, &ulValueToSend, 0U );
    }
}
/*-----------------------------------------------------------*/

static void prvQueueReceiveTask( void *pvParameters )
{
unsigned long ulReceivedValue;
const unsigned long ulExpectedValue = 100UL;

    /* Remove compiler warning about unused parameter. */
    ( void ) pvParameters;

    for( ;; )
    {
        /* Wait until something arrives in the queue - this task will block
        indefinitely provided INCLUDE_vTaskSuspend is set to 1 in
        FreeRTOSConfig.h. */
        xQueueReceive( xQueue, &ulReceivedValue, portMAX_DELAY );

        /*  To get here something must have been received from the queue, but
        is it the expected value?  If it is, toggle the LED. */
        if( ulReceivedValue == ulExpectedValue )
        {
            vParTestToggleLED( mainTASK_LED );
            ulReceivedValue = 0U;
        }
    }
}
/*-----------------------------------------------------------*/

