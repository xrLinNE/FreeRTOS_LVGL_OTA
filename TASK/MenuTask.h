#ifndef __MENUTASK_H__
#define __MENUTASK_H__
//头文件
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "semphr.h"
#include "queue.h"
#include "key.h"
//LVGL头文件
#include "lvgl.h"                // 它为整个LVGL提供了更完整的头文件引用
#include "lv_port_disp.h"        // LVGL的显示支持
//函数
void MenuTask(void *params);

#endif
