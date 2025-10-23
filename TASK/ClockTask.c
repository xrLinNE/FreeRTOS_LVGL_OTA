#include "ClockTask.h"
//队列
extern 	QueueHandle_t g_xQueueMenu;
//任务句柄
extern 	TaskHandle_t MenuTask_handler;			//菜单任务
//屏幕
extern 	lv_obj_t *scr_menu;									//菜单任务屏幕
//对象
extern 	lv_obj_t *calendar;   							//日历对象 
//外部变量
extern	TimerHandle_t g_Clock_Timer_handler;//闹钟定时器句柄
extern	int8_t seclect_flag;
extern 	uint8_t g_clock_num[6];
uint16_t 	clock_second = 0;
uint32_t 	total_seconds_set = 0;
uint8_t 	clock_flag = 0;
//clock定时时间流逝显示
uint8_t csec_unit, csec_decade, cmin_unit, cmin_decade, chour_unit, chour_decade;

void ClockTask( void * pvParameters )
{
	struct Key_data	key_data;

	while(1)
	{	
		/* 读按键中断队列 */
		if(clock_flag == 0)
		{
			xQueueReceive(g_xQueueMenu, &key_data, portMAX_DELAY);
		}

		if(key_data.ldata == 1) 								//为了保证画面与按键方向一直，所以这里是反过来的
		{
			seclect_flag = (seclect_flag + 1) % 7;
			update_select_border(seclect_flag);
			key_data.ldata = 0;
		}

		if(key_data.rdata == 1)									//为了保证画面与按键方向一直，所以这里是反过来的
		{
			seclect_flag = (seclect_flag + 6) % 7;
			update_select_border(seclect_flag);
			key_data.rdata = 0;
		}
		if(key_data.updata == 1)
		{
			if(seclect_flag == 6)
			{
				/*启动定时器*/
				if(g_Clock_Timer_handler != NULL)
				{
					total_seconds_set = g_clock_num[5]*36000 + g_clock_num[4]*3600 + g_clock_num[3]*600 + g_clock_num[2]*60 + g_clock_num[1]*10 + g_clock_num[0];
					xTimerStart(g_Clock_Timer_handler, 0);
					clock_flag = 1;
					key_data.updata = 0;
				}
			}
			else
			{
				g_clock_num[seclect_flag]++;
				if(g_clock_num[seclect_flag]>9)	g_clock_num[seclect_flag]=0;	
				update_clock_digits(seclect_flag);		//更新SET值
				total_seconds_set = g_clock_num[5]*36000 + g_clock_num[4]*3600 + g_clock_num[3]*600 + g_clock_num[2]*60 + g_clock_num[1]*10 + g_clock_num[0];
				key_data.updata = 0;
			}
		}
		if(key_data.exdata == 1)
		{
			if(scr_menu == NULL) scr_menu = create_menu_screen();
			lv_scr_load(scr_menu);									//显示菜单	
			delete_clock_screen();									//删除自己，节省内存
			vTaskResume(MenuTask_handler);
			vTaskSuspend(NULL);
			key_data.exdata = 0;
		}
		//判断定时结束
		if((clock_flag == 1) && (clock_second == total_seconds_set))
		{
			//定时到，开灯
			LED1(1);
			vTaskDelay(1000);
			LED1(0);
			clock_flag = 0;
			//关闭定时器
			xTimerStop(g_Clock_Timer_handler, 0);
			csec_unit 		= 0;
			csec_decade 	= 0;
			cmin_unit 		= 0;
			cmin_decade	 	= 0;
			chour_unit 		= 0;
			chour_decade	= 0;
			clock_second 	= 0;
		}
	}
}
//闹钟定时器回调
void ClockTimerCallBackFun(TimerHandle_t xTimer)
{  
	clock_second++;		
	//显示
	csec_unit++;		
	if(csec_unit>9)		{csec_unit 		= 0; csec_decade++;	}
	if(csec_decade>5)	{csec_decade 	= 0; cmin_unit++;		}
	if(cmin_unit>9)		{cmin_unit 		= 0; cmin_decade++;	}
	if(cmin_decade>5)	{cmin_decade 	= 0; chour_unit++;	}
	if(chour_unit>5)	{chour_unit 	= 0; chour_decade++;}
	
	if( clock_second <= total_seconds_set )
		ret_set_time(chour_decade, chour_unit, cmin_decade, cmin_unit, csec_decade, csec_unit);
	else
		ret_set_time(0, 0, 0, 0, 0, 0);
}

