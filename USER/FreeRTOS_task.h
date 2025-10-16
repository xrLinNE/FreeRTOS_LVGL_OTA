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
#include "lvgl.h"                // 它为整个LVGL提供了更完整的头文件引用
#include "lv_port_disp.h"        // LVGL的显示支持
//GUI Guider
#include "gui_guider.h"        	 // 使用GUI Guider绘制的图像
//任务头文件
#include "MenuTask.h"
//函数
void FreeRTOS_task(void);

#endif
