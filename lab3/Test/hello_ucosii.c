/*************************************************************************
* Copyright (c) 2004 Altera Corporation, San Jose, California, USA.      *
* All rights reserved. All use of this software and documentation is     *
* subject to the License Agreement located at the end of this file below.*
**************************************************************************
* Description:                                                           *
* The following is a simple hello world program running MicroC/OS-II.The * 
* purpose of the design is to be a very simple application that just     *
* demonstrates MicroC/OS-II running on NIOS II.The design doesn't account*
* for issues such as checking system call return codes. etc.             *
*                                                                        *
* Requirements:                                                          *
*   -Supported Example Hardware Platforms                                *
*     Standard                                                           *
*     Full Featured                                                      *
*     Low Cost                                                           *
*   -Supported Development Boards                                        *
*     Nios II Development Board, Stratix II Edition                      *
*     Nios Development Board, Stratix Professional Edition               *
*     Nios Development Board, Stratix Edition                            *
*     Nios Development Board, Cyclone Edition                            *
*   -System Library Settings                                             *
*     RTOS Type - MicroC/OS-II                                           *
*     Periodic System Timer                                              *
*   -Know Issues                                                         *
*     If this design is run on the ISS, terminal output will take several*
*     minutes per iteration.                                             *
**************************************************************************/


#include <stdio.h>
#include "includes.h"

/* Definition of Task Stacks */
#define   TASK_STACKSIZE       512
OS_STK    task_stk[64][TASK_STACKSIZE];

/* Definition of Task Priorities */

#define TASK1_PERIOD      3
#define TASK1_C           1
#define TASK2_PERIOD      6
#define TASK2_C           3

OS_CPU_SR cpu_sr;

OS_EVENT *R1;
OS_EVENT *R2;

void simulate_cpu(INT16U ticks)
{
    INT32U start_time = OSTimeGet();
    while ((OSTimeGet() - start_time) < ticks) {
        // busy waiting
    }
}

void Task1()
{
	int start ; //the start time
	int end ; //the end time
	int toDelay;
	INT8U err;
	OSTimeDly(8);
	start=8;

//	while(1)
//	{
//		while(OSTCBCur->compTime>0) //C ticks
//		{
//		// do nothing
//		}
//		end=OSTimeGet() ; // end time
//		toDelay=(OSTCBCur->period)-(end-start) ;
//		start=start+(OSTCBCur->period) ; // next start time
//		OS_ENTER_CRITICAL();
//		OSTCBCur->compTime = 2;
//		OSTCBCur->deadline = OSTCBCur->deadline + OSTCBCur->period;
//		OS_EXIT_CRITICAL();
//		OSTimeDly (toDelay); // delay and wait (P-C) times
//	}
	while(OSTCBCur->compTime>0) //C ticks
	{
	// do nothing
	}
	INT8U before_prio = OSTCBCur->OSTCBPrio;
	OSMutexPend(R1, 0, &err);
	INT8U after_prio = OSTCBCur->OSTCBPrio;
	OS_ENTER_CRITICAL();
	sprintf(buffer+buffer_idx,"\n%10d T1 lock R1 (P=%2d to P=%2d)\n", OSTime,before_prio,after_prio);
	buffer_idx += 38;
	OS_EXIT_CRITICAL();

	simulate_cpu(2);

	before_prio = OSTCBCur->OSTCBPrio;
	OSMutexPend(R2, 0, &err);
	after_prio = OSTCBCur->OSTCBPrio;
	OS_ENTER_CRITICAL();
	sprintf(buffer+buffer_idx,"\n%10d T1 lock R2 (P=%2d to P=%2d)\n", OSTime,before_prio,after_prio);
	buffer_idx += 38;
	OS_EXIT_CRITICAL();

	simulate_cpu(2);

	before_prio = OSTCBCur->OSTCBPrio;
	OSMutexPost(R2);
	after_prio = OSTCBCur->OSTCBPrio;
	OS_ENTER_CRITICAL();
	sprintf(buffer+buffer_idx,"%10d T1 unlock R2 (P=%2d to P=%2d)\n", OSTime,before_prio,after_prio);
	buffer_idx += 40;
	OS_EXIT_CRITICAL();

	before_prio = OSTCBCur->OSTCBPrio;
	OSMutexPost(R1);
	after_prio = OSTCBCur->OSTCBPrio;
	OS_ENTER_CRITICAL();
	sprintf(buffer+buffer_idx,"%10d T1 unlock R1 (P=%2d to P=%2d)\n", OSTime,before_prio,after_prio);
	buffer_idx += 40;
	OS_EXIT_CRITICAL();
	OSTimeDly(100);
}

void Task2()
{
	int start ; //the start time
	int end ; //the end time
	int toDelay;
	INT8U err;
	OSTimeDly(4);
	start=4;

//	while(1)
//	{
//		while(OSTCBCur->compTime>0) //C ticks
//		{
//		// do nothing
//		}
//		end=OSTimeGet() ; // end time
//		toDelay=(OSTCBCur->period)-(end-start) ;
//		start=start+(OSTCBCur->period) ; // next start time
//		OS_ENTER_CRITICAL();
//		OSTCBCur->compTime = 2;
//		OSTCBCur->deadline = OSTCBCur->deadline + OSTCBCur->period;
//		OS_EXIT_CRITICAL();
//		OSTimeDly (toDelay); // delay and wait (P-C) times
//	}
	while(OSTCBCur->compTime>0) //C ticks
	{
	// do nothing
	}

	INT8U before_prio = OSTCBCur->OSTCBPrio;
	OSMutexPend(R2, 0, &err);
	INT8U after_prio = OSTCBCur->OSTCBPrio;
	OS_ENTER_CRITICAL();
	sprintf(buffer+buffer_idx,"\n%10d T2 lock R2 (P=%2d to P=%2d)\n", OSTime,before_prio,after_prio);
	buffer_idx += 38;
	OS_EXIT_CRITICAL();

	simulate_cpu(4);

	before_prio = OSTCBCur->OSTCBPrio;
	OSMutexPost(R2);
	after_prio = OSTCBCur->OSTCBPrio;
	OS_ENTER_CRITICAL();
	sprintf(buffer+buffer_idx,"%10d T2 unlock R2 (P=%2d to P=%2d)\n", OSTime,before_prio,after_prio);
	buffer_idx += 40;
	OS_EXIT_CRITICAL();
	OSTimeDly(100);
}

void Task3()
{
	int start ; //the start time
	int end ; //the end time
	int toDelay;
	INT8U err;
	start=0;

//	while(1)
//	{
//		while(OSTCBCur->compTime>0) //C ticks
//		{
//		// do nothing
//		}
//		end=OSTimeGet() ; // end time
//		toDelay=(OSTCBCur->period)-(end-start) ;
//		start=start+(OSTCBCur->period) ; // next start time
//		OS_ENTER_CRITICAL();
//		OSTCBCur->compTime = 2;
//		OSTCBCur->deadline = OSTCBCur->deadline + OSTCBCur->period;
//		OS_EXIT_CRITICAL();
//		OSTimeDly (toDelay); // delay and wait (P-C) times
//	}
	while(OSTCBCur->compTime>0) //C ticks
	{
	// do nothing
	}

	INT8U before_prio = OSTCBCur->OSTCBPrio;
	OSMutexPend(R1, 0, &err);
	INT8U after_prio = OSTCBCur->OSTCBPrio;
	OS_ENTER_CRITICAL();
	sprintf(buffer+buffer_idx,"\n%10d T1 lock R1 (P=%2d to P=%2d)\n", OSTime,before_prio,after_prio);
	buffer_idx += 38;
	OS_EXIT_CRITICAL();

	simulate_cpu(7);

	before_prio = OSTCBCur->OSTCBPrio;
	OSMutexPost(R1);
	after_prio = OSTCBCur->OSTCBPrio;
	OS_ENTER_CRITICAL();
	sprintf(buffer+buffer_idx,"%10d T1 unlock R1 (P=%2d to P=%2d)\n", OSTime,before_prio,after_prio);
	buffer_idx += 40;
	OS_EXIT_CRITICAL();
	OSTimeDly(100);
}

void Task1_2()
{
	int start ; //the start time
	int end ; //the end time
	int toDelay;
	INT8U err;
	OSTimeDly(5);
	start=5;

//	while(1)
//	{
//		while(OSTCBCur->compTime>0) //C ticks
//		{
//		// do nothing
//		}
//		end=OSTimeGet() ; // end time
//		toDelay=(OSTCBCur->period)-(end-start) ;
//		start=start+(OSTCBCur->period) ; // next start time
//		OS_ENTER_CRITICAL();
//		OSTCBCur->compTime = 2;
//		OSTCBCur->deadline = OSTCBCur->deadline + OSTCBCur->period;
//		OS_EXIT_CRITICAL();
//		OSTimeDly (toDelay); // delay and wait (P-C) times
//	}
	while(OSTCBCur->compTime>0) //C ticks
	{
	// do nothing
	}
	INT8U before_prio = OSTCBCur->OSTCBPrio;
	OSMutexPend(R2, 0, &err);
	INT8U after_prio = OSTCBCur->OSTCBPrio;
	OS_ENTER_CRITICAL();
	sprintf(buffer+buffer_idx,"\n%10d T1 lock R2 (P=%2d to P=%2d)\n", OSTime,before_prio,after_prio);
	buffer_idx += 38;
	OS_EXIT_CRITICAL();

	simulate_cpu(3);

	before_prio = OSTCBCur->OSTCBPrio;
	OSMutexPend(R1, 0, &err);
	after_prio = OSTCBCur->OSTCBPrio;
	OS_ENTER_CRITICAL();
	sprintf(buffer+buffer_idx,"\n%10d T1 lock R1 (P=%2d to P=%2d)\n", OSTime,before_prio,after_prio);
	buffer_idx += 38;
	OS_EXIT_CRITICAL();

	simulate_cpu(3);


	before_prio = OSTCBCur->OSTCBPrio;
	OSMutexPost(R1);
	after_prio = OSTCBCur->OSTCBPrio;
	OS_ENTER_CRITICAL();
	sprintf(buffer+buffer_idx,"%10d T1 unlock R1 (P=%2d to P=%2d)\n", OSTime,before_prio,after_prio);
	buffer_idx += 40;
	OS_EXIT_CRITICAL();

	simulate_cpu(3);

	before_prio = OSTCBCur->OSTCBPrio;
	OSMutexPost(R2);
	after_prio = OSTCBCur->OSTCBPrio;
	OS_ENTER_CRITICAL();
	sprintf(buffer+buffer_idx,"%10d T1 unlock R2 (P=%2d to P=%2d)\n", OSTime,before_prio,after_prio);
	buffer_idx += 40;
	OS_EXIT_CRITICAL();
	OSTimeDly(100);
}

void Task2_2()
{
	int start ; //the start time
	int end ; //the end time
	int toDelay;
	INT8U err;
	start=0;

//	while(1)
//	{
//		while(OSTCBCur->compTime>0) //C ticks
//		{
//		// do nothing
//		}
//		end=OSTimeGet() ; // end time
//		toDelay=(OSTCBCur->period)-(end-start) ;
//		start=start+(OSTCBCur->period) ; // next start time
//		OS_ENTER_CRITICAL();
//		OSTCBCur->compTime = 2;
//		OSTCBCur->deadline = OSTCBCur->deadline + OSTCBCur->period;
//		OS_EXIT_CRITICAL();
//		OSTimeDly (toDelay); // delay and wait (P-C) times
//	}
	while(OSTCBCur->compTime>0) //C ticks
	{
	// do nothing
	}
	INT8U before_prio = OSTCBCur->OSTCBPrio;
	OSMutexPend(R1, 0, &err);
	INT8U after_prio = OSTCBCur->OSTCBPrio;
	OS_ENTER_CRITICAL();
	sprintf(buffer+buffer_idx,"\n%10d T2 lock R1 (P=%2d to P=%2d)\n", OSTime,before_prio,after_prio);
	buffer_idx += 38;
	OS_EXIT_CRITICAL();

	simulate_cpu(6);

	before_prio = OSTCBCur->OSTCBPrio;
	OSMutexPend(R2, 0, &err);
	after_prio = OSTCBCur->OSTCBPrio;
	OS_ENTER_CRITICAL();
	sprintf(buffer+buffer_idx,"\n%10d T2 lock R2 (P=%2d to P=%2d)\n", OSTime,before_prio,after_prio);
	buffer_idx += 38;
	OS_EXIT_CRITICAL();

	simulate_cpu(2);

	before_prio = OSTCBCur->OSTCBPrio;
	OSMutexPost(R2);
	after_prio = OSTCBCur->OSTCBPrio;
	OS_ENTER_CRITICAL();
	sprintf(buffer+buffer_idx,"%10d T2 unlock R2 (P=%2d to P=%2d)\n", OSTime,before_prio,after_prio);
	buffer_idx += 40;
	OS_EXIT_CRITICAL();

	simulate_cpu(2);

	before_prio = OSTCBCur->OSTCBPrio;
	OSMutexPost(R1);
	after_prio = OSTCBCur->OSTCBPrio;
	OS_ENTER_CRITICAL();
	sprintf(buffer+buffer_idx,"%10d T2 unlock R1 (P=%2d to P=%2d)\n", OSTime,before_prio,after_prio);
	buffer_idx += 40;
	OS_EXIT_CRITICAL();
	OSTimeDly(100);
}

void print(){
	int idx=0;
	while(1){
		if (idx < buffer_idx) printf ("%c", buffer[idx ++]);
	}
}
/* The main function creates two task and starts multi-tasking */
int main(void)
{
  INT8U  err;
  OSTimeSet(0);
  R1 = OSMutexCreate(1, &err);
  R2 = OSMutexCreate(2, &err);
  OSTaskCreate(Task1, (void *)0, &task_stk[0][TASK_STACKSIZE-1],3,2,30);
  OSTaskCreate(Task2, (void *)0, &task_stk[1][TASK_STACKSIZE-1],4,2,30);
  OSTaskCreate(Task3, (void *)0, &task_stk[2][TASK_STACKSIZE-1],5,2,30);
//  OSTaskCreate(Task1_2, (void *)0, &task_stk[0][TASK_STACKSIZE-1],3,2,30);
//  OSTaskCreate(Task2_2, (void *)0, &task_stk[1][TASK_STACKSIZE-1],4,2,30);
  OSTaskCreate(print, (void *)0, &task_stk[3][TASK_STACKSIZE-1],12,0,20);
  OSStart();
  return 0;
}

/******************************************************************************
*                                                                             *
* License Agreement                                                           *
*                                                                             *
* Copyright (c) 2004 Altera Corporation, San Jose, California, USA.           *
* All rights reserved.                                                        *
*                                                                             *
* Permission is hereby granted, free of charge, to any person obtaining a     *
* copy of this software and associated documentation files (the "Software"),  *
* to deal in the Software without restriction, including without limitation   *
* the rights to use, copy, modify, merge, publish, distribute, sublicense,    *
* and/or sell copies of the Software, and to permit persons to whom the       *
* Software is furnished to do so, subject to the following conditions:        *
*                                                                             *
* The above copyright notice and this permission notice shall be included in  *
* all copies or substantial portions of the Software.                         *
*                                                                             *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING     *
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER         *
* DEALINGS IN THE SOFTWARE.                                                   *
*                                                                             *
* This agreement shall be governed in all respects by the laws of the State   *
* of California and by the laws of the United States of America.              *
* Altera does not recommend, suggest or require that this reference design    *
* file be used in conjunction or combination with any other product.          *
******************************************************************************/
