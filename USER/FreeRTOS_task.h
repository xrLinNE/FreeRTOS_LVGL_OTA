#ifndef __SHOWTIME_TASK_H
#define __SHOWTIME_TASK_H
//ͷ�ļ�
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "event_groups.h"
#include "led.h"
#include "key.h"
#include "delay.h"
//LVGL
#include "lvgl.h"                // ��Ϊ����LVGL�ṩ�˸�������ͷ�ļ�����
#include "lv_port_disp.h"        // LVGL����ʾ֧��
//GUI Guider
#include "gui_guider.h"        	 // ʹ��GUI Guider���Ƶ�ͼ��
//����ͷ�ļ�
#include "MenuTask.h"
//����
void FreeRTOS_task(void);

#endif
