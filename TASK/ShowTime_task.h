#ifndef __SHOWTIME_TASK_H
#define __SHOWTIME_TASK_H

//ͷ�ļ�
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "queue.h"
#include "semphr.h"
#include "u8g2.h"
#include "Data.h"
//����
void ShowTimeTask(void *params);
void TimerCallBackFun(TimerHandle_t xTimer);

#endif
