#include "TimeTask.h"

//时间定时器
extern TimerHandle_t g_Timer_handler;	
//队列
extern QueueHandle_t g_xQueueMenu;
//其他任务句柄
extern TaskHandle_t 	MenuTask_handler;				//菜单任务
extern TaskHandle_t 	SettingsTask_handler;		//设置任务
extern TaskHandle_t 	ClockTask_handler;			//闹钟任务
extern TaskHandle_t 	CalendarTask_handler;		//日历任务
extern TaskHandle_t 	FlashlightTask_handler;	//手电筒任务
extern TaskHandle_t 	Dht11Task_handler;			//温湿度任务
//屏幕
extern lv_obj_t *scr_menu;//菜单任务屏幕
extern lv_obj_t *scr_time;//时间任务屏幕
//时间参数
uint8_t sec_unit, sec_decade, min_unit, min_decade, hour_unit, hour_decade;

void TimeTask( void * pvParameters )
{

	//挂起其他所有任务
	vTaskSuspend(MenuTask_handler);
	vTaskSuspend(CalendarTask_handler);
	vTaskSuspend(ClockTask_handler);
	vTaskSuspend(Dht11Task_handler);
	//	vTaskSuspend(SettingsTask_handler);
	if(pdPASS == xTimerStart(g_Timer_handler, 0)) printf("hello \r\n");//启动定时器
	Key_data	key_data;
	//屏幕初始化
	scr_time = create_time_screen();
	lv_scr_load(scr_time);

	while(1)
	{
		set_time(hour_decade, hour_unit, min_decade,  min_unit, sec_decade, sec_unit);
		vTaskDelay(250);
		//如果读队列(不阻塞)有数据
		if (xQueueReceive(g_xQueueMenu, &key_data, 0) == pdPASS) 
		{
			//退出按钮
			if(key_data.exdata == 1)
			{	
				if(scr_menu == NULL) scr_menu = create_menu_screen();
//				lv_scr_load_anim(scr_menu, LV_SCR_LOAD_ANIM_FADE_IN, 400, 0, true);//带动画切换，后删除自己节省内存
				lv_scr_load(scr_menu);//显示菜单	
				delete_time_screen();//删除自己，节省内存
				vTaskResume(MenuTask_handler);
				vTaskSuspend(NULL);//挂起自己
				key_data.exdata = 0;
			}
		}
//		UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
//		printf("TimeTask Remaining stack: %lu Byte \r\n", uxHighWaterMark);
	}
}
//时间定时器回调
void TimerCallBackFun( TimerHandle_t pxTimer )
{  
	sec_unit++;		
	if(sec_unit>9){sec_unit = 0; sec_decade++;}
	if(sec_decade>5){sec_decade = 0; min_unit++;}
	if(min_unit>9){min_unit = 0; min_decade++;}
	if(min_decade>5){min_decade = 0; hour_unit++;}
	if(hour_unit>5){hour_unit = 0; hour_decade++;}
}

