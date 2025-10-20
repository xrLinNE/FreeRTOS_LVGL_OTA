#ifndef __SHOWTIME_TASK_H
#define __SHOWTIME_TASK_H
//头文件
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "event_groups.h"
#include "led.h"
#include "key.h"
#include "delay.h"
//LVGL
#include "lvgl.h"                
#include "lv_port_disp.h"        
//GUI Guider
#include "gui_guider.h"        	 // 使用GUI GUIDER软件绘制的画面
#include "lv_demo_benchmark.h"		//测试用
//任务
#include "MenuTask.h"
#include "TimeTask.h"
#include "CalendarTask.h"
//函数
void FreeRTOS_task(void);

#endif
