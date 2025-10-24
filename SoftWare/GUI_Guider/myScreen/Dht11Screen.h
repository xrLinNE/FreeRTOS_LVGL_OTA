#ifndef __DHT11CREEN_H__
#define __DHT11CREEN_H__
//头文件
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "semphr.h"
#include "queue.h"
#include "key.h"
#include "lcd.h"
#include "lcd_init.h"
//LVGL
#include "lvgl.h"                
#include "lv_port_disp.h"        // 支持LVGL显示
//函数
lv_obj_t *create_dht11_screen(void);
void delete_dht11_screen(void);
void set_dht11_data(int hum_integer, int hum_decimal, int temp_integer, int temp_decimal);
#endif
