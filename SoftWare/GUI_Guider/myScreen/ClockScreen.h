#ifndef __CLOCKCREEN_H__
#define __CLOCKCREEN_H__
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
lv_obj_t *create_clock_screen(void);
void delete_clock_screen(void);
void update_select_border(uint8_t select_flag);
void update_clock_digits(int8_t seclect_flag);
void ret_set_time(uint8_t hour_decade, uint8_t hour_unit, uint8_t min_decade,  uint8_t min_unit, uint8_t sec_decade, uint8_t sec_unit);

#endif
