#include "FreeRTOS_task.h"
//任务优先级
#define START_TASK_PRIO									1		
#define LVGL_HEART_TASK_PRIO						4	
#define LED_TASK_PRIO										2	
#define MENU_TASK_PRIO									2
#define TIME_TASK_PRIO									3					//在时间显示任务中挂起其他所有任务，需要保证时间任务最先运行
#define CALENDAR_TASK_PRIO							2
#define CLOCK_TASK_PRIO									2
#define DHT11_TASK_PRIO									2
//任务堆栈
//#define configTOTAL_HEAP_SIZE                           ((size_t)(10 * 1024))   /* FreeRTOS堆中可用的RAM总量, 单位: Byte, 无默认需定义 */
#define START_TASK_STACK_SIZE						64				//这里的单位是字，也就是128字，即128x4=512B 512字节
#define LVGL_HEART_TASK_STACK_SIZE			512	
#define LED_TASK_STACK_SIZE							128	
#define MENU_TASK_STACK_SIZE						256	
#define TIME_TASK_STACK_SIZE						512	
#define CALENDAR_TASK_STACK_SIZE				256	
#define CLOCK_TASK_STACK_SIZE						350	
#define DHT11_TASK_STACK_SIZE						400	
//任务句柄
TaskHandle_t 	start_task_handler;		//开始任务
TaskHandle_t 	lvgl_heart_handler;		//lvgl心跳任务
TaskHandle_t 	led_task_handler;			//led任务
TaskHandle_t 	MenuTask_handler;			//菜单任务
TaskHandle_t 	TimeTask_handler;			//时间任务
TaskHandle_t 	CalendarTask_handler;	//日历任务
TaskHandle_t 	ClockTask_handler;		//闹钟任务
TaskHandle_t 	Dht11Task_handler;		//温湿度任务
TimerHandle_t g_Timer_handler;			//时间定时器句柄

//变量
//lv_ui 				guider_ui;//gui_guider.h 中使用到了 extern lv_ui guider_ui;
//任务函数
void start_task( void * pvParameters );
void lvgl_heart_task( void * pvParameters );
void led_task( void * pvParameters );
//外部任务/回调函数
extern void MenuTask(void *params);
extern void TimeTask(void *params);
extern void CalendarTask(void *params);
extern void ClockTask(void *params);
extern void Dht11Task(void *params);
extern void TimerCallBackFun( TimerHandle_t pxTimer );

//钩子函数
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    printf("BAD!!!: task %s \r\n", pcTaskName);
}

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
	//定时器1  显示时间用
	g_Timer_handler = xTimerCreate("timer1", 1000, pdTRUE, (void *)1, TimerCallBackFun);						//周期定时器
////LVGL心跳任务
	xTaskCreate(
								 (TaskFunction_t				) 	lvgl_heart_task,							
								 (char *        				)		"lvgl_heart_task",						
								 (configSTACK_DEPTH_TYPE) 	LVGL_HEART_TASK_STACK_SIZE,	
								 (void *								)		NULL,										
								 (UBaseType_t						) 	LVGL_HEART_TASK_PRIO,				
								 (TaskHandle_t *				)		&lvgl_heart_handler			
						 );	
//时间任务								 
	if(xTaskCreate(
								 (TaskFunction_t				) 	TimeTask,							
								 (char *        				)		"TimeTask",						
								 (configSTACK_DEPTH_TYPE) 	TIME_TASK_STACK_SIZE,	
								 (void *								)		NULL,										
								 (UBaseType_t						) 	TIME_TASK_PRIO,				
								 (TaskHandle_t *				)		&TimeTask_handler			
						 ) != pdPASS)	printf("Create Time task failed!\r\n");
//菜单任务								 
	if(xTaskCreate(
								 (TaskFunction_t				) 	MenuTask,							
								 (char *        				)		"MenuTask",						
								 (configSTACK_DEPTH_TYPE) 	MENU_TASK_STACK_SIZE,	
								 (void *								)		NULL,										
								 (UBaseType_t						) 	MENU_TASK_PRIO,				
								 (TaskHandle_t *				)		&MenuTask_handler			
						 ) != pdPASS)	printf("Create Menu task failed!\r\n");
//日历任务								 
	if(xTaskCreate(
								 (TaskFunction_t				) 	CalendarTask,							
								 (char *        				)		"CalendarTask",						
								 (configSTACK_DEPTH_TYPE) 	CALENDAR_TASK_STACK_SIZE,	
								 (void *								)		NULL,										
								 (UBaseType_t						) 	CALENDAR_TASK_PRIO,				
								 (TaskHandle_t *				)		&CalendarTask_handler			
						 ) != pdPASS)	printf("Create calendar task failed!\r\n");
//闹钟任务								 
	if(xTaskCreate(
								 (TaskFunction_t				) 	ClockTask,							
								 (char *        				)		"ClockTask",						
								 (configSTACK_DEPTH_TYPE) 	CLOCK_TASK_STACK_SIZE,	
								 (void *								)		NULL,										
								 (UBaseType_t						) 	CLOCK_TASK_PRIO,				
								 (TaskHandle_t *				)		&ClockTask_handler			
						 ) != pdPASS)	printf("Create Clock task failed!\r\n");
//温湿度任务
	if(xTaskCreate(
								 (TaskFunction_t				) 	Dht11Task,							
								 (char *        				)		"Dht11Task",						
								 (configSTACK_DEPTH_TYPE) 	DHT11_TASK_STACK_SIZE,	
								 (void *								)		NULL,										
								 (UBaseType_t						) 	DHT11_TASK_PRIO,				
								 (TaskHandle_t *				)		&Dht11Task_handler			
						 ) != pdPASS)	printf("Create Dht11 task failed!\r\n");
//LED任务表示系统在运行
	xTaskCreate(
								 (TaskFunction_t				) 	led_task,							
								 (char *        				)		"led_task",						
								 (configSTACK_DEPTH_TYPE) 	LED_TASK_STACK_SIZE,	
								 (void *								)		NULL,										
								 (UBaseType_t						) 	LED_TASK_PRIO,				
								 (TaskHandle_t *				)		&led_task_handler			
						 );
	vTaskDelete(NULL);									//创建完三个任务后，删除自己
	taskEXIT_CRITICAL();                /* 退出临界区 */
}

void lvgl_heart_task( void * pvParameters )
{
	while(1)
	{
		lv_timer_handler();
		vTaskDelay(5);
	}
}
void led_task( void * pvParameters )
{
	while(1)
	{
//		if(U1_CB.URxDataOUT != U1_CB.URxDataIN)//缓冲区有数据
//		{
//			printf("hello !\r\n");
//			printf("Get %d Byte : \r\n", (U1_CB.URxDataOUT->end - U1_CB.URxDataOUT->start + 1));
//			for(int i = 0; i < (U1_CB.URxDataOUT->end - U1_CB.URxDataOUT->start + 1); i++)  printf("%c",U1_CB.URxDataOUT->start[i]);
//			printf("\r\n");
//			U1_CB.URxDataOUT++;																									
//			if(U1_CB.URxDataOUT == U1_CB.URxDataEND)
//			{
//				U1_CB.URxDataOUT = &U1_CB.URxDataPtr[0];															 	
//			}			
//		}
//		UBaseType_t task_count = uxTaskGetNumberOfTasks();
//    TaskStatus_t *task_status = pvPortMalloc(task_count * sizeof(TaskStatus_t));
//    if(task_status == NULL) return;

//    task_count = uxTaskGetSystemState(task_status, task_count, NULL);

//    for(UBaseType_t i = 0; i < task_count; i++)
//    {
//        printf("任务名: %s, 状态: %u, 优先级: %u, 堆栈剩余: %u\r\n",
//               task_status[i].pcTaskName,
//               task_status[i].eCurrentState,
//               (unsigned int)task_status[i].uxCurrentPriority,
////               task_status[i].usStackHighWaterMark);
////    }

//    vPortFree(task_status);
		LED0_TOGGLE();
		vTaskDelay(1000);
	}
}

