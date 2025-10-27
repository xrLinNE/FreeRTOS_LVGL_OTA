#include "Dht11Task.h"
u8 hum_integer, hum_decimal, temp_integer, temp_decimal;
//队列
extern 	QueueHandle_t g_xQueueMenu;
//任务句柄
extern 	TaskHandle_t MenuTask_handler;			//菜单任务
//屏幕
extern 	lv_obj_t *scr_menu;									//菜单任务屏幕
//外部变量
TimerHandle_t Dht11_Timer_handler = NULL;
u8 dataStatus = 0;
void Dht11TimerCallBackFun( TimerHandle_t pxTimer )
{  
	//在定时器中周期读取数据
	if( (DHT11_Read_Data(&hum_integer, &hum_decimal, &temp_integer, &temp_decimal)) != 0 )	dataStatus = 0;
	else	dataStatus = 1;
}
void Dht11Task( void * pvParameters )
{
	struct Key_data	key_data;
	Dht11_Timer_handler = xTimerCreate("timer3", 1000, pdTRUE, (void *)3, Dht11TimerCallBackFun);//周期定时器
	while(DHT11_Init())								//DHT11初始化，DHT11初始化要过一段时间才能保证数据正确
	{
		printf("DHT11 Error \r\n");
		delay_ms(1000);
	}
	while(1)
	{	
		if(xTimerIsTimerActive(Dht11_Timer_handler) != pdTRUE)	xTimerStart(Dht11_Timer_handler, 0);
		if(dataStatus == 1)
		{
			set_dht11_data(hum_integer, hum_decimal, temp_integer, temp_decimal);
			dataStatus = 0;
		}
		/* 读按键中断队列 */
		xQueueReceive(g_xQueueMenu, &key_data, 0);
		if(key_data.exdata == 1)
		{
			if(scr_menu == NULL) scr_menu = create_menu_screen();
			xTimerStop(Dht11_Timer_handler, 0);			//关闭定时器
			lv_scr_load(scr_menu);									//显示菜单	
			delete_dht11_screen();									//删除自己，节省内存
			vTaskResume(MenuTask_handler);
			vTaskSuspend(NULL);
			key_data.exdata = 0;
		}
//		UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
//		printf("Dht11Task Remaining stack: %lu Byte \r\n", uxHighWaterMark);
	}
}

