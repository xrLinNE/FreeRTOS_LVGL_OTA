#ifndef __MENUTASK_H__
#define __MENUTASK_H__
//ͷ�ļ�
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "semphr.h"
#include "queue.h"
#include "key.h"
//LVGLͷ�ļ�
#include "lvgl.h"                // ��Ϊ����LVGL�ṩ�˸�������ͷ�ļ�����
#include "lv_port_disp.h"        // LVGL����ʾ֧��
//����
void MenuTask(void *params);

#endif
