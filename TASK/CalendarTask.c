#include "CalendarTask.h"
//队列
extern QueueHandle_t g_xQueueMenu;
//任务句柄
extern TaskHandle_t MenuTask_handler;			//菜单任务
//屏幕
extern lv_obj_t *scr_menu;		//菜单任务屏幕
//对象
extern lv_obj_t *calendar;   	//日历对象 
void CalendarTask( void * pvParameters )
{
	struct Key_data	key_data;
	
	while(1)
	{	

		/* 读按键中断队列 */
		xQueueReceive(g_xQueueMenu, &key_data, portMAX_DELAY);
		//右键：下个月
		if(key_data.rdata == 1) 
		{
			uint16_t year = lv_calendar_get_showed_date(calendar)->year;
			uint8_t month = lv_calendar_get_showed_date(calendar)->month;
			month++;
			if(month > 12) 
			{
					month = 1;
					year++;
			}
			lv_calendar_set_showed_date(calendar, year, month);
			key_data.rdata = 0;
		}
		//左键：上个月
		if(key_data.ldata == 1)
		{
			uint16_t year = lv_calendar_get_showed_date(calendar)->year;
			uint8_t month = lv_calendar_get_showed_date(calendar)->month;
			if(month == 1) 
			{
					month = 12;
					year--;
			}
			else month--;
			
			lv_calendar_set_showed_date(calendar, year, month);
			key_data.ldata = 0;
		}
		if(key_data.updata == 1)
		{
			if(scr_menu == NULL) scr_menu = create_menu_screen();
//			lv_scr_load_anim(scr_menu, LV_SCR_LOAD_ANIM_FADE_IN, 400, 10, true);//带动画切换，后删除自己节省内存
			lv_scr_load(scr_menu);//显示菜单	
			delete_time_screen();//删除自己，节省内存
			vTaskResume(MenuTask_handler);
			vTaskSuspend(NULL);
			key_data.updata = 0;
		}
	}
}


