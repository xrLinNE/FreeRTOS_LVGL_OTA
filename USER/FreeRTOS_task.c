#include "FreeRTOS_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "event_groups.h"
#include "led.h"
#include "key.h"
#include "delay.h"
//动态创建

#define START_TASK_PRIO					1		
#define TASK1_PRIO							2		

#define START_TASK_STACK_SIZE		128				//这里的单位是字，也就是128字，即128x4=512B 512字节
#define TASK1_STACK_SIZE				128	

TaskHandle_t start_task_handler;
TaskHandle_t task1_handler;

void start_task( void * pvParameters );
void task1( void * pvParameters );

void FreeRTOS_task(void)
{
	xTaskCreate(
								 (TaskFunction_t				) 	start_task,							/* 任务   			*/
								 (char *        				)		"start_task",						/* 任务名 			*/
								 (configSTACK_DEPTH_TYPE) 	START_TASK_STACK_SIZE,	/* 任务堆栈 		*/
								 (void *								)		NULL,										/* 任务入口参数 */
								 (UBaseType_t						) 	START_TASK_PRIO,				/* 任务优先级 	*/
								 (TaskHandle_t *				)		&start_task_handler			/* 任务句柄	    */
						 );
	vTaskStartScheduler();//开启任务调度
}
//Start
void start_task( void * pvParameters )
{
	taskENTER_CRITICAL();               /* 进入临界区 */
	xTaskCreate(
								 (TaskFunction_t				) 	task1,							
								 (char *        				)		"task1",						
								 (configSTACK_DEPTH_TYPE) 	TASK1_STACK_SIZE,	
								 (void *								)		NULL,										
								 (UBaseType_t						) 	TASK1_PRIO,				
								 (TaskHandle_t *				)		&task1_handler			
						 );								 
	vTaskDelete(NULL);									//创建完三个任务后，删除自己
	taskEXIT_CRITICAL();                /* 退出临界区 */
}

void task1( void * pvParameters )
{
	u8 key_num = 0, t = 0;
	uint8_t *buff = NULL;
	while(1)
	{
		key_num = KEY_Scan(0);
		switch(key_num)
		{
			case KEY1_PRES:
				buff = pvPortMalloc(100);
				if(buff != NULL)	printf("Memory application successful! \r\n");
				else							printf("Memory application failed! \r\n");
				break;
			case KEY2_PRES:
				if(buff != NULL)
				{
					printf("Release memory! \r\n");
					vPortFree(buff);
				}
				break;
		}
		if(t++ > 50)
		{
			t = 0;
			printf("The remaining free memory size is: %d \r\n", xPortGetFreeHeapSize());
		}
		vTaskDelay(10);
	}
}


