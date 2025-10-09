#ifndef __SHOWCLOCK_TASK_H
#define __SHOWCLOCK_TASK_H

//头文件
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "queue.h"
#include "Data.h"
#include "stdio.h"
#include "math.h"
//函数
void ShowClockTimeTask(void *params);
void ClockTimerCallBackFun(TimerHandle_t xTimer);

#endif
