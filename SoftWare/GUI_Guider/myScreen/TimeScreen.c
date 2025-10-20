#include "TimeScreen.h"
lv_obj_t *scr_time = NULL;//时间任务屏幕
static lv_obj_t *hour_label = NULL;
static lv_obj_t *min_label = NULL;
static lv_obj_t *sec_label = NULL;
//时间更新
void set_time(uint8_t hour_decade, uint8_t hour_unit, uint8_t min_decade,  uint8_t min_unit, uint8_t sec_decade, uint8_t sec_unit)
{
    char buf[3];
    if(hour_label) 
		{
			sprintf(buf, "%d%d", hour_decade, hour_unit);
			lv_label_set_text(hour_label, buf);
		}
    if(min_label) 
		{
			sprintf(buf, "%d%d", min_decade, min_unit);
			lv_label_set_text(min_label, buf);
    }
    if(sec_label) 
		{
			sprintf(buf, "%d%d", sec_decade, sec_unit);
			lv_label_set_text(sec_label, buf);
    }
    lv_timer_handler(); // 立即刷新
}
//时间任务屏幕
lv_obj_t *create_time_screen(void)
{
	
	LV_IMG_DECLARE(xn); 																				//图片
	//创建时间屏幕
	lv_obj_t *scr = lv_obj_create(NULL);
	lv_obj_t *time_cont = lv_obj_create(scr);  									// 父容器
	lv_obj_set_size(time_cont, 240, 240);     							    // 容器大小
	lv_obj_center(time_cont);                								    // 居中
	lv_obj_clear_flag(time_cont, LV_OBJ_FLAG_SCROLLABLE); 	    // 禁止滚动
	//lv_obj_set_style_bg_color(time_cont, lv_color_white(), 0);  // 设置背景为白色
	lv_obj_set_style_bg_color(time_cont, lv_color_hex(0xCFEDE7), 0);
	lv_obj_set_style_bg_opa(time_cont, LV_OPA_COVER, 0);        // 确保背景不透明
	lv_obj_set_style_border_width(time_cont, 0, 0);          		// 去容器边框 边框宽度 0
	//图片
	lv_obj_t *img = lv_img_create(time_cont);   								// 父容器为 time_cont
	lv_img_set_src(img, &xn);         													// 设置图片源
	lv_obj_set_pos(img,  -15, 60); 
//	lv_obj_align(img, LV_ALIGN_BOTTOM_LEFT, 0, 0); 							// 左下角对齐
	// 顶部提示标签
	lv_obj_t *top_label = lv_label_create(time_cont);              // 父容器为 time_cont
	lv_label_set_text(top_label, "Have a good day");             // 设置文字
	lv_obj_set_style_text_color(top_label, lv_color_hex(0xFFA500), 0); // 
	lv_obj_set_style_text_font(top_label, &lv_font_montserrat_28, 0); // 可选，设置字体大小
	lv_obj_align(top_label, LV_ALIGN_TOP_MID, 0, 10); // x偏移0，y偏移10像素，距离顶部10像素
	//时间标签
	hour_label = lv_label_create(time_cont);
	min_label  = lv_label_create(time_cont);
	sec_label  = lv_label_create(time_cont);
	//冒号
	lv_obj_t *colon1 = lv_label_create(time_cont);
	lv_obj_t *colon2 = lv_label_create(time_cont);
	//排列
	lv_obj_set_style_text_font(hour_label, &lv_font_montserrat_28, 0);
	lv_obj_set_style_text_font(min_label, &lv_font_montserrat_28, 0); 
	lv_obj_set_style_text_font(sec_label, &lv_font_montserrat_28, 0); 
	lv_obj_set_style_text_font(colon1, &lv_font_montserrat_28, 0);  
	lv_obj_set_style_text_font(colon2, &lv_font_montserrat_28, 0); 
	lv_obj_set_style_text_color(hour_label, lv_color_black(), 0);
	lv_obj_set_style_text_color(min_label, lv_color_black(), 0);
	lv_obj_set_style_text_color(sec_label, lv_color_black(), 0);
	lv_obj_set_style_text_color(colon1, lv_color_black(), 0);
	lv_obj_set_style_text_color(colon2, lv_color_black(), 0);
	lv_label_set_text(hour_label, "00");
	lv_label_set_text(min_label,  "00");
	lv_label_set_text(sec_label,  "00");
	lv_label_set_text(colon1, ":");
	lv_label_set_text(colon2, ":");
	lv_obj_set_pos(hour_label, 95, 	100);//设置位置 
	lv_obj_set_pos(colon1,     135, 97);
	lv_obj_set_pos(min_label,  145, 100); 
	lv_obj_set_pos(colon2,     185, 97);
	lv_obj_set_pos(sec_label,  195, 100); 
	
	return scr;
}
void delete_time_screen(void) 
{
	if (scr_time) 
	{
		lv_obj_del(scr_time);
		scr_time = NULL;
	}
}

