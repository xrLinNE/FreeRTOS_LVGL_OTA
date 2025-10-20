#ifndef __TIMETASK_H
#define __TIMETASK_H
//头文件
#include "FreeRTOS_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "event_groups.h"
#include "led.h"
#include "key.h"
#include "delay.h"
//LVGL
#include "lvgl.h"                
#include "lv_port_disp.h"        // 支持LVGL显示
//屏幕
#include "TimeScreen.h"
//函数
void TimeTask( void * pvParameters );

#endif
