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


void Task1()
{
	int start ; //the start time
	int end ; //the end time
	int toDelay;
	start=OSTimeGet();

	while(1)
	{
		while(OSTCBCur->compTime>0) //C ticks
		{
		// do nothing
		}
		end=OSTimeGet() ; // end time
		toDelay=(OSTCBCur->period)-(end-start) ;
		start=start+(OSTCBCur->period) ; // next start time
		OS_ENTER_CRITICAL();
		OSTCBCur->compTime = 1;
		OSTCBCur->deadline = OSTCBCur->deadline + OSTCBCur->period;
		OS_EXIT_CRITICAL();
		OSTimeDly (toDelay); // delay and wait (P-C) times
	}
}

void Task2()
{
	int start ; //the start time
	int end ; //the end time
	int toDelay;
	start=OSTimeGet();

	while(1)
	{
		while(OSTCBCur->compTime>0) //C ticks
		{
		// do nothing
		}
		end=OSTimeGet() ; // end time
		toDelay=(OSTCBCur->period)-(end-start) ;
		start=start+(OSTCBCur->period) ; // next start time
		OS_ENTER_CRITICAL();
		OSTCBCur->compTime = 3;
		OSTCBCur->deadline = OSTCBCur->deadline + OSTCBCur->period;
		OS_EXIT_CRITICAL();
		OSTimeDly (toDelay); // delay and wait (P-C) times
	}
}

void Task3()
{
	int start ; //the start time
	int end ; //the end time
	int toDelay;
	start=OSTimeGet();

	while(1)
	{
		while(OSTCBCur->compTime>0) //C ticks
		{
		// do nothing
		}
		end=OSTimeGet() ; // end time
		toDelay=(OSTCBCur->period)-(end-start) ;
		start=start+(OSTCBCur->period) ; // next start time
		OS_ENTER_CRITICAL();
		OSTCBCur->compTime = 4;
		OSTCBCur->deadline = OSTCBCur->deadline + OSTCBCur->period;
		//if(start > OSTCBCur->deadline) printf("time:%d task3 exceed deadline\n", OSTime);
		OS_EXIT_CRITICAL();

		OSTimeDly (toDelay); // delay and wait (P-C) times
	}
}

void print(){
	int idx=0;
	while(1){

		if (idx < buffer_idx) printf ("%c", buffer[idx ++]);

//		int i;
//		for(i =idx;i<idx+26;i++)
//			if(i<10000)
//				printf("%c",buffer[i]);
//		idx = idx +26;

	}
}
/* The main function creates two task and starts multi-tasking */
int main(void)
{
  OSTimeSet(0);
  OSTaskCreate(Task1, (void *)0, &task_stk[0][TASK_STACKSIZE-1],1,1,3);
  OSTaskCreate(Task2, (void *)0, &task_stk[1][TASK_STACKSIZE-1],2,3,6);
  OSTaskCreate(Task3, (void *)0, &task_stk[2][TASK_STACKSIZE-1],3,4,9);
  OSTaskCreate(print, (void *)0, &task_stk[3][TASK_STACKSIZE-1],4,0,0);
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
