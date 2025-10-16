#include "FreeRTOS_task.h"

//任务优先级
#define START_TASK_PRIO									1		
#define LVGL_TEST_TASK_PRIO							2		
#define LED_TASK_PRIO										3	
#define MENU_TASK_PRIO									3	
//任务堆栈
#define START_TASK_STACK_SIZE						128				//这里的单位是字，也就是128字，即128x4=512B 512字节
#define LVGL_TEST_TASK_STACK_SIZE				128	
#define LED_TASK_STACK_SIZE							128	
#define MENU_TASK_STACK_SIZE						256	
//任务句柄
TaskHandle_t 	start_task_handler;
TaskHandle_t 	lvgl_test_handler;
TaskHandle_t 	led_task_handler;

TaskHandle_t 	MenuTask_handler;
TimerHandle_t g_Timer_handler;			//时间显示任务句柄
TimerHandle_t g_Clock_Timer_handler;//闹钟任务句柄
//变量
//lv_ui 				guider_ui;//gui_guider.h 中使用到了 extern lv_ui guider_ui;
//队列
QueueHandle_t g_xQueueMenu;						//传递按键数据	
//任务函数
void start_task( void * pvParameters );
void lvgl_test_task( void * pvParameters );
void led_task( void * pvParameters );
extern void MenuTask(void *params);

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
	
//	g_Timer_handler = xTimerCreate("timer1", 1000, pdTRUE, NULL, (TimerCallbackFunction_t)TimerCallBackFun);//周期定时器
//	g_Clock_Timer_handler = xTimerCreate("timer2", 100, pdTRUE, NULL, (TimerCallbackFunction_t)ClockTimerCallBackFun);//周期定时器

//LVGL绘制任务
	xTaskCreate(
								 (TaskFunction_t				) 	lvgl_test_task,							
								 (char *        				)		"lvgl_test_task",						
								 (configSTACK_DEPTH_TYPE) 	LVGL_TEST_TASK_STACK_SIZE,	
								 (void *								)		NULL,										
								 (UBaseType_t						) 	LVGL_TEST_TASK_PRIO,				
								 (TaskHandle_t *				)		&lvgl_test_handler			
						 );	
//菜单任务								 
	xTaskCreate(
								 (TaskFunction_t				) 	MenuTask,							
								 (char *        				)		"MenuTask",						
								 (configSTACK_DEPTH_TYPE) 	MENU_TASK_STACK_SIZE,	
								 (void *								)		NULL,										
								 (UBaseType_t						) 	MENU_TASK_PRIO,				
								 (TaskHandle_t *				)		&MenuTask_handler			
						 );	
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
static void anim_x_cb(void * var, int32_t v)
{
    lv_obj_set_x(var, v);
}

static void anim_size_cb(void * var, int32_t v)
{
    lv_obj_set_size(var, v, v);
}
static void set_angle(void * img, int32_t v)
{
    lv_img_set_angle(img, v);
}

static void set_zoom(void * img, int32_t v)
{
    lv_img_set_zoom(img, v);
	
}
void lv_example(void)
{
    lv_obj_t * obj = lv_obj_create(lv_scr_act());
    lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_RED), 0);
    lv_obj_set_style_radius(obj, LV_RADIUS_CIRCLE, 0);

    lv_obj_align(obj, LV_ALIGN_LEFT_MID, 10, 0);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_values(&a, 10, 50);
    lv_anim_set_time(&a, 1000);
    lv_anim_set_playback_delay(&a, 100);
    lv_anim_set_playback_time(&a, 300);
    lv_anim_set_repeat_delay(&a, 500);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_in_out);

    lv_anim_set_exec_cb(&a, anim_size_cb);
    lv_anim_start(&a);
    lv_anim_set_exec_cb(&a, anim_x_cb);
    lv_anim_set_values(&a, 10, 240);
    lv_anim_start(&a);

}
void lvgl_test_task( void * pvParameters )
{
	//setup_ui(&guider_ui);//初始化ui

	//lv_example();
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
		LED0_TOGGLE();
		vTaskDelay(1000);
	}
}

