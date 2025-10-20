#ifndef __MENUSCREEN_H__
#define __MENUSCREEN_H__
//头文件
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "semphr.h"
#include "queue.h"
#include "key.h"
//LVGL
#include "lvgl.h"                
#include "lv_port_disp.h"        // 支持LVGL显示
//宏
#define CONTAINER_CENTER_X 	0        	//容器中心坐标
#define NUM_IMG 						5					//图片总数
#define VISIBLE 						3					//屏幕可现实图片数
#define ANIM_SPEED 					400				//动画速度
//函数
void update_dots(void);
void menu_switch_left(void);
void menu_switch_right(void);
lv_obj_t *create_menu_screen(void);
void delete_menu_screen(void);

#endif
