#ifndef __SHOWTIME_TASK_H
#define __SHOWTIME_TASK_H

//头文件
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "queue.h"
#include "semphr.h"
#include "u8g2.h"
#include "Data.h"
//函数
void ShowTimeTask(void *params);
void TimerCallBackFun(TimerHandle_t xTimer);

#endif
