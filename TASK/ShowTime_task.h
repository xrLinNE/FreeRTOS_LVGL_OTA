#ifndef __SHOWTIME_TASK_H
#define __SHOWTIME_TASK_H

//ͷ�ļ�
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "queue.h"
#include "semphr.h"
#include "key.h"
//����
void ShowTimeTask(void *params);
void TimerCallBackFun(TimerHandle_t xTimer);

#endif
