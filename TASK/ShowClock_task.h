#ifndef __SHOWCLOCK_TASK_H
#define __SHOWCLOCK_TASK_H

//ͷ�ļ�
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "queue.h"
#include "Data.h"
#include "stdio.h"
#include "math.h"
//����
void ShowClockTimeTask(void *params);
void ClockTimerCallBackFun(TimerHandle_t xTimer);

#endif
