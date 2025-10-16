#include "MenuTask.h"

extern QueueHandle_t g_xQueueMenu;//队列
//extern SemaphoreHandle_t g_xSemMenu; 

extern TaskHandle_t xShowMenuTaskHandle;
extern TaskHandle_t xShowTimeTaskHandle;
extern TaskHandle_t xShowWoodenFishTaskHandle;
extern TaskHandle_t xShowFlashLightTaskHandle;
extern TaskHandle_t xShowSettingTaskHandle;
extern TaskHandle_t xShowClockTaskHandle;
extern TaskHandle_t xShowCalendarTaskHandle;
extern TaskHandle_t xShowDHT11TaskHandle;

//const char str[5][10] = {"手电筒", "日历", "闹钟", "温湿度", "设置"};//;
//const char *str[] = {"手电筒", "日历", "闹钟", "温湿度", "设置"};
//const char *str[] = {"手电筒", "日历", "闹钟", "温湿度", "设置"};
const char *str[] = {"手电筒", "日历", "闹钟", "温湿度", "设置"};

/* app's name */
//str1 fly1 = {"fly1", NULL};
//str1 dino1 = {"hum", NULL};
//str1 test1 = {"torch", NULL};
//str1 block1 = {"clock", NULL};
//str1 setting1 = {"setting", NULL};

/* some data */
//uint8_t dock_pos = 2;
//uint8_t dock_status = 10;
//uint8_t dock[5] = {45, 55, 65, 75, 85};
//uint8_t dock_y = 58, dock_r = 3;  
//int str_flag = 2;
//int8_t R_move_pos[5] = {-1, 9, 49, 89, 129};
//BaseType_t select = 3;

//int queue_flag = 0;
//uint32_t end_flag = 1;
//uint32_t seclect_end = 0;//showsetting
//LVGL相关定义
int order[3] = {1, 2, 3}; 
#define MENU_ANIM_TIME 			300
#define CONTAINER_CENTER_X 	0        	//容器中心坐标
#define NUM_IMG 						5					//图片总数
#define VISIBLE 						3					//屏幕可现实图片数
#define ANIM_SPEED 					500				//动画速度
static lv_obj_t * menu_cont;					//菜单容器
static lv_obj_t *dot_cont;						//底部圆圈容器
static lv_obj_t *menu_label;					//顶部标签
static lv_obj_t *imgs[NUM_IMG];   		//所有图片对象
static lv_obj_t *dots[NUM_IMG];				//圆圈对象
static int order[VISIBLE];        		//当前屏幕显示的3张图片索引
static int zoom_large 		= 512; 			//中间图片 2x; 初始尺寸为256
static int zoom_small 		= 180; 			//两边图片 
// 位置偏移量（相对于容器中心）
const int pos_offset[VISIBLE+2] = 
{
	-184,                               //完全在左边屏幕外
	-94,                                //左边显示
	0,                                  //中间显示
	94,                                 //右边显示
	184                                 //完全在右边屏幕外
};
// 更新底部小圆圈状态
static void update_dots(void)
{
	for (int i = 0; i < NUM_IMG; i++)
	{
		if (i == order[1]) 						// 当前中间图片索引高亮
			lv_obj_set_style_bg_color(dots[i], lv_color_hex(0xC7C3F4), 0);
		else
			lv_obj_set_style_bg_color(dots[i], lv_color_white(), 0);
	}
}
// 动画完成回调函数
static void anim_invalidate(lv_anim_t * a)
{
	lv_obj_invalidate(menu_cont);  
}

static void anim_ready_cb_r(lv_anim_t * a)
{
    lv_obj_t * img = (lv_obj_t *)a->var; 
    lv_obj_set_x(img, pos_offset[4]); // 最左边图片放到最右边隐藏位置
	lv_obj_invalidate(menu_cont);  
}

static void anim_ready_cb_l(lv_anim_t * a)
{
    lv_obj_t * img = (lv_obj_t *)a->var; 
    lv_obj_set_x(img, pos_offset[0]); // 最右边图片放到最右边隐藏位置
	lv_obj_invalidate(menu_cont);  
}

//右移动画
void menu_switch_right()
{
	lv_anim_t a_x, a_zoom;
	int next_idx = (order[VISIBLE-1] + 1) % NUM_IMG;								// 新进入右侧图片索引
	int idx[VISIBLE+1] = {order[0], order[1], order[2], next_idx};	// 当前三张 + 新右侧
	//1.初始化
	
	// 起始位置
	int start_pos[VISIBLE+1] = 
	{
		pos_offset[1], // 左
		pos_offset[2], // 中
		pos_offset[3], // 右
		pos_offset[4]  // 新右边
	};
	// 目标位置
	int end_pos[VISIBLE+1] = 
	{
		pos_offset[0], // 左边移出
		pos_offset[1], // 左
		pos_offset[2], // 中
		pos_offset[3]  // 新右边
	};
	//起始缩放
	int start_zoom[VISIBLE+1] = 
	{
		zoom_small, // 左
		zoom_large, // 中
		zoom_small, // 右
		zoom_small  // 新右边
	};
	//目标缩放
	int end_zoom[VISIBLE+1] = 
	{
		zoom_small, // 左边移出
		zoom_small, // 左
		zoom_large, // 中
		zoom_small  // 新右边
	};
	
	//2.动画
	for(int i=0; i<VISIBLE+1; i++)
	{
		lv_obj_t *img = imgs[idx[i]];
	//平移
		lv_anim_init(&a_x);
		lv_anim_set_var(&a_x, img);
		lv_anim_set_exec_cb(&a_x, (lv_anim_exec_xcb_t)lv_obj_set_x);
		lv_anim_set_values(&a_x, start_pos[i], end_pos[i]);
		lv_anim_set_time(&a_x, ANIM_SPEED);
		if(i==0) lv_anim_set_ready_cb(&a_x, anim_ready_cb_r); // 左边移出隐藏【回调】
		else	lv_anim_set_ready_cb(&a_x, anim_invalidate);
		lv_anim_start(&a_x);
	//缩放
		lv_anim_init(&a_zoom);
		lv_anim_set_var(&a_zoom, img);
		lv_anim_set_exec_cb(&a_zoom, (lv_anim_exec_xcb_t)lv_img_set_zoom);
		lv_anim_set_values(&a_zoom, start_zoom[i], end_zoom[i]);
		lv_anim_set_time(&a_zoom, ANIM_SPEED);
		lv_anim_start(&a_zoom);
	}
	
	//3.更新order
	order[0] = order[1];
	order[1] = order[2];
	order[2] = next_idx;
	lv_label_set_text(menu_label, str[order[1]]);
lv_obj_align(menu_label, LV_ALIGN_TOP_MID, 0, 10); // 保持位置
}

//左移动画
void menu_switch_left()
{
    lv_anim_t a_x, a_zoom;
    int next_idx = (order[0] - 1 + NUM_IMG) % NUM_IMG; 							// 新进入左侧图片索引
    int idx[VISIBLE+1] = {next_idx, order[0], order[1], order[2]};	// 新左 + 当前三张
    // 1. 起始位置
    int start_pos[VISIBLE+1] = 
    {
			pos_offset[0], // 新左边在左出界
			pos_offset[1], // 左
			pos_offset[2], // 中
			pos_offset[3]  // 右
    };
    // 目标位置
    int end_pos[VISIBLE+1] = 
    {
			pos_offset[1], // 新左变为左
			pos_offset[2], // 左 -> 中
			pos_offset[3], // 中 -> 右
			pos_offset[4]  // 右移出右边屏幕
    };

    // 起始缩放
    int start_zoom[VISIBLE+1] = 
    {
			zoom_small, // 新左
			zoom_small, // 左
			zoom_large, // 中
			zoom_small  // 右
    };
    // 目标缩放
    int end_zoom[VISIBLE+1] = 
    {
			zoom_small, // 新左
			zoom_large, // 左 -> 中
			zoom_small, // 中 -> 右
			zoom_small  // 右移出
    };
    // 2. 动画
    for(int i=0; i<VISIBLE+1; i++)
    {
			lv_obj_t *img = imgs[idx[i]];

			// 平移动画
			lv_anim_init(&a_x);
			lv_anim_set_var(&a_x, img);
			lv_anim_set_exec_cb(&a_x, (lv_anim_exec_xcb_t)lv_obj_set_x);
			lv_anim_set_values(&a_x, start_pos[i], end_pos[i]);
			lv_anim_set_time(&a_x, ANIM_SPEED);
			if(i == VISIBLE) lv_anim_set_ready_cb(&a_x, anim_ready_cb_l); // 最右边移出隐藏至最左
			else	lv_anim_set_ready_cb(&a_x, anim_invalidate);
			lv_anim_start(&a_x);
			// 缩放动画
			lv_anim_init(&a_zoom);
			lv_anim_set_var(&a_zoom, img);
			lv_anim_set_exec_cb(&a_zoom, (lv_anim_exec_xcb_t)lv_img_set_zoom);
			lv_anim_set_values(&a_zoom, start_zoom[i], end_zoom[i]);
			lv_anim_set_time(&a_zoom, ANIM_SPEED);
			lv_anim_start(&a_zoom);
    }
    // 3. 更新 order
    order[2] = order[1];
    order[1] = order[0];
    order[0] = next_idx;
		lv_label_set_text(menu_label, str[order[1]]);
lv_obj_align(menu_label, LV_ALIGN_TOP_MID, 0, 10);
}


void MenuTask(void *params)
{	
	int spacing = 20;  // 圆点间距
	Key_data	key_data;
	//xSemaphoreTake(g_xQueueMenu, portMAX_DELAY);
	//初始化队列
	g_xQueueMenu = xQueueCreate(4, 4);
	
	//主菜单初始化
	//图片声明
	LV_IMG_DECLARE(icon_flashlight_64);
	LV_IMG_DECLARE(icon_calendar_64);
	LV_IMG_DECLARE(icon_settings_64);
	LV_IMG_DECLARE(icon_clock_64);
	LV_IMG_DECLARE(icon_thermometer_64);
	//菜单容器
	const lv_img_dsc_t *icon_list[NUM_IMG] = {&icon_flashlight_64, &icon_calendar_64, &icon_clock_64, &icon_thermometer_64, &icon_settings_64};
	menu_cont = lv_obj_create(lv_scr_act());								     //当前活动屏幕创建容器
	lv_obj_set_size(menu_cont, 240, 240);     							     // 容器大小
	lv_obj_center(menu_cont);                								     // 居中
	lv_obj_clear_flag(menu_cont, LV_OBJ_FLAG_SCROLLABLE); 	     // 禁止滚动
	lv_obj_set_style_bg_color(menu_cont, lv_color_white(), 0);   // 设置背景为白色
	lv_obj_set_style_bg_opa(menu_cont, LV_OPA_COVER, 0);         // 确保背景不透明
	// 去掉边框
	lv_obj_set_style_border_width(menu_cont, 0, 0);          		 // 去容器边框 边框宽度 0
	//初始化图片位置
	for(int i=0;i<NUM_IMG;i++)
	{
    imgs[i] = lv_img_create(menu_cont);
    lv_img_set_src(imgs[i], icon_list[i]);
    lv_img_set_zoom(imgs[i], zoom_small);
    //屏幕外图片
    if(i == 0) lv_obj_align(imgs[i], LV_ALIGN_CENTER, pos_offset[0], 0);
    else if(i == NUM_IMG-1) lv_obj_align(imgs[i], LV_ALIGN_CENTER, pos_offset[4], 0);
    else lv_obj_align(imgs[i], LV_ALIGN_CENTER, pos_offset[i], 0); // 屏幕显示的3张
	}
	lv_img_set_zoom(imgs[2], zoom_large);	// 设置中间图片缩放
	// 初始化 order 显示 2,3,4
	order[0] = 1;  // 左边 2
	order[1] = 2;  // 中间 3
	order[2] = 3;  // 右边 4
	//底部小圆圈
	dot_cont = lv_obj_create(menu_cont);											//圆圈容器->父容器menu_cont
	lv_obj_set_size(dot_cont, 240, 30);												//dot_cont容器大小
	lv_obj_align(dot_cont, LV_ALIGN_BOTTOM_MID, 0, 5);				//相对父容器位置
	lv_obj_clear_flag(dot_cont, LV_OBJ_FLAG_SCROLLABLE);			//清除 LV_OBJ_FLAG_SCROLLABLE 标志，使容器不可滚动。
	lv_obj_set_style_bg_opa(dot_cont, LV_OPA_TRANSP, 0);			//容器背景透明
	lv_obj_set_style_border_width(dot_cont, 0, 0);     				//去容器边框 边框宽度 0
	// 创建三个小圆点
	for (int i = 0; i < NUM_IMG; i++) 
	{
		dots[i] = lv_obj_create(dot_cont);											//对象
		lv_obj_set_size(dots[i], 8, 8);													//对象大小
		lv_obj_set_style_radius(dots[i], LV_RADIUS_CIRCLE, 0); 	//对象为圆点;圆角半径为 LV_RADIUS_CIRCLE，完全圆形
		lv_obj_set_style_bg_opa(dots[i], LV_OPA_COVER, 0);			//设置圆点背景不透明
		lv_obj_set_style_bg_color(dots[i], lv_color_white(), 0);//圆点颜色
		lv_obj_clear_flag(dots[i], LV_OBJ_FLAG_SCROLLABLE);			//圆点不滚动
		// 水平排列：让中间点在屏幕对称轴上
		int offset_x = (i - (NUM_IMG - 1) / 2) * spacing;				//计算水平偏移，(NUM_IMG - 1) / 2表示中心圆点的索引
		lv_obj_align(dots[i], LV_ALIGN_CENTER, offset_x, 0);		//圆点对齐dot_cont容器的偏移
	}
	lv_obj_set_style_bg_color(dots[order[1]], lv_color_hex(0xC7C3F4), 0);// 当前选中项（order[1]）的圆点高亮
	// 顶部标签
	menu_label = lv_label_create(menu_cont);
	LV_FONT_DECLARE(menu_label_font);																		//定义字体
	lv_obj_set_style_text_color(menu_label, lv_color_black(), 0); 			//文字颜色
	lv_obj_set_style_text_font(menu_label, &menu_label_font, 0); 				//字体						对应宏LV_FONT_MONTSERRAT_14要开
	lv_label_set_text(menu_label, str[order[1]]);    										//初始显示中间图片对应的名称
	lv_obj_align(menu_label, LV_ALIGN_TOP_MID, 0, 10); 									//上方偏移 10px

	
	while(1)
	{
		//等待队列
		xQueueReceive(g_xQueueMenu, &key_data, portMAX_DELAY);
		/* handle data */
		if(key_data.rdata == 1)
		{	
			menu_switch_right();
			update_dots();
			key_data.rdata = 0;
			key_data.ldata = 0;	
		}
		else if(key_data.ldata == 1)
		{

			menu_switch_left();
			update_dots();
			key_data.rdata = 0;
			key_data.ldata = 0;
		}
		/* ststus machine : task scheduling  */
		else if(key_data.exdata == 1)
		{
//			switch(dock_pos)
//			{
//				case 0: vTaskResume(xShowCalendarTaskHandle);vTaskSuspend(NULL);key_data.exdata = 0;break;
//				case 1: vTaskResume(xShowFlashLightTaskHandle);vTaskSuspend(NULL);key_data.exdata = 0;break;
//				case 2: vTaskResume(xShowDHT11TaskHandle);vTaskSuspend(NULL);key_data.exdata = 0;break;
//				case 3: vTaskResume(xShowClockTaskHandle);vTaskSuspend(NULL);key_data.exdata = 0;break;
//				case 4: vTaskResume(xShowSettingTaskHandle);vTaskSuspend(NULL);key_data.exdata = 0;break;
//			}
		}
		else if(key_data.updata == 1)
		{
//			vTaskResume(xShowTimeTaskHandle);
//			vTaskSuspend(NULL);
			key_data.updata = 0;
		}
	}
}

