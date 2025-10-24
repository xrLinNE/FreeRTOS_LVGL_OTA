#ifndef __MENUTASK_H__
#define __MENUTASK_H__
//头文件
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "semphr.h"
#include "queue.h"
#include "key.h"
//LVGL
#include "lvgl.h"                
#include "lv_port_disp.h"        // 支持LVGL显示
//屏幕
#include "MenuScreen.h"
#include "TimeScreen.h"
#include "CalendarScreen.h"
#include "ClockScreen.h"
#include "Dht11Screen.h"
//函数
void MenuTask(void *params);

#endif
