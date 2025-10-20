#include "ClockTask.h"
//动态创建

/*
* start_task: 创建三个任务
* task1: LED0 500ms闪烁
* task2: LED1 500ms闪烁
* task3: KEY0 按下删除task1
*/
#define START_TASK_PRIO					1		
#define TASK1_PRIO							2		
#define TASK2_PRIO							3
#define TASK3_PRIO							4

#define START_TASK_STACK_SIZE		128				//这里的单位是字，也就是128字，即128x4=512B 512字节
#define TASK1_STACK_SIZE				128	
#define TASK2_STACK_SIZE				128	
#define TASK3_STACK_SIZE				128	

TaskHandle_t start_task_handler;
TaskHandle_t task1_handler;
TaskHandle_t task2_handler;
TaskHandle_t task3_handler;

void start_task( void * pvParameters );
void task1( void * pvParameters );
void task2( void * pvParameters );
void task3( void * pvParameters );

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
	xTaskCreate(
								 (TaskFunction_t				) 	task2,							
								 (char *        				)		"task2",						
								 (configSTACK_DEPTH_TYPE) 	TASK2_STACK_SIZE,	
								 (void *								)		NULL,										
								 (UBaseType_t						) 	TASK2_PRIO,			
								 (TaskHandle_t *				)		&task2_handler			
						 );
	xTaskCreate(
								 (TaskFunction_t				) 	task3,							
								 (char *        				)		"task3",						
								 (configSTACK_DEPTH_TYPE) 	TASK3_STACK_SIZE,	
								 (void *								)		NULL,										
								 (UBaseType_t						) 	TASK3_PRIO,				
								 (TaskHandle_t *				)		&task3_handler			
						 );
								 
	vTaskDelete(NULL);//创建完三个任务后，删除自己
	taskEXIT_CRITICAL();                /* 退出临界区 */
}
//LED0
void task1( void * pvParameters )
{
	uint32_t task1_num = 0;
	while(1)
	{
		printf("task1 running, for %d times \r\n ", ++task1_num);
		LED0_TOGGLE();
		vTaskDelay(500);
	}
}
//LED1
void task2( void * pvParameters )
{
	uint32_t task2_num = 0;
	while(1)
	{
		printf("task2 running, for %d times.\r\n ", ++task2_num);
		LED1_TOGGLE();
		vTaskDelay(500);
	}
}
//KEY0
void task3( void * pvParameters )
{
    uint8_t key = 0;
    while(1)
    {
			//printf("task3 running...\r\n ");
			key = KEY_Scan(0);
			if(key == KEY0_PRES)
			{
				vTaskSuspend(task1_handler);//挂起task1
				printf("在task3中挂起task1.\r\n ");
			}
//			else if (key == KEY1_PRES)
//			{
//				vTaskResume(task1_handler);//恢复task1
//				printf("在task3中恢复task1.\r\n");
//			}
			vTaskDelay(10);
    }
}

