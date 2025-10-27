#include "MenuTask.h"
extern	int order[VISIBLE];        						//当前屏幕显示的3张图片索引
//队列
extern QueueHandle_t g_xQueueMenu;
//其他任务句柄
extern TaskHandle_t 	TimeTask_handler;				//时间任务
extern TaskHandle_t 	SettingsTask_handler;		//设置任务
extern TaskHandle_t 	ClockTask_handler;			//闹钟任务
extern TaskHandle_t 	CalendarTask_handler;		//日历任务
extern TaskHandle_t 	FlashlightTask_handler;	//手电筒任务
extern TaskHandle_t 	Dht11Task_handler;			//温湿度任务
//屏幕
extern lv_obj_t *scr_menu;		//菜单任务屏幕
extern lv_obj_t *scr_time;		//时间任务屏幕
extern lv_obj_t *scr_calendar;//日历任务屏幕
extern lv_obj_t *scr_clock;		//闹钟任务屏幕
extern lv_obj_t *scr_dht11;	//闹钟任务屏幕
void MenuTask(void *params)
{	
	Key_data	key_data;
	//xSemaphoreTake(g_xQueueMenu, portMAX_DELAY);
	//初始化队列
	while(1)
	{
		//等待队列
		xQueueReceive(g_xQueueMenu, &key_data, portMAX_DELAY);
		/* handle data */
		if(key_data.rdata == 1)
		{	
			menu_switch_right();
			update_dots();
			key_data.rdata = 0;
		}
		else if(key_data.ldata == 1)
		{

			menu_switch_left();
			update_dots();
			key_data.ldata = 0;
		}
		/* ststus machine : task scheduling  */
		else if(key_data.updata == 1)
		{
			switch(order[2])
			{
				case 1://手电筒
					break;
				case 2: //日历
					if(scr_calendar == NULL) scr_calendar = create_calendar_screen();
					lv_scr_load(scr_calendar);  //显示
					//lv_scr_load_anim(scr_calendar, LV_SCR_LOAD_ANIM_FADE_IN, 400, 10, false);//带动画切换，后删除自己节省内存
					vTaskResume(CalendarTask_handler);
					vTaskSuspend(NULL);
					key_data.updata = 0;
					break;
				case 3://闹钟
					if(scr_clock == NULL) scr_clock = create_clock_screen();
					lv_scr_load(scr_clock);  //显示
					vTaskResume(ClockTask_handler);
					vTaskSuspend(NULL);
					key_data.updata = 0;
					break;
				case 4://温湿度
					if(scr_dht11 == NULL) scr_dht11 = create_dht11_screen();
					lv_scr_load(scr_dht11);  //显示
					vTaskResume(Dht11Task_handler);
					vTaskSuspend(NULL);
					key_data.updata = 0;
					break;
				case 5://设置 
					break;
				default:
					break;
			}
		}
		else if(key_data.exdata == 1)
		{
			if(scr_time == NULL) scr_time = create_time_screen();
			lv_scr_load(scr_time);  //显示
			//菜单屏幕不删除，需要保留上一次的位置
			vTaskResume(TimeTask_handler);
			vTaskSuspend(NULL);
			key_data.exdata = 0;
		}
//		UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
//		printf("MenuTask Remaining stack: %lu Byte \r\n", uxHighWaterMark);
	}
}

