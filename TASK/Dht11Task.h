#ifndef __DHT11TASK_H
#define __DHT11TASK_H
#include "FreeRTOS_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "key.h"

#include "dht11.h"
#include "Dht11Screen.h"

void ClockTask( void * pvParameters );

#endif
