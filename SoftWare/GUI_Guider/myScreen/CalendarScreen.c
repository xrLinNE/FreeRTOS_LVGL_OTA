#include "CalendarScreen.h"
lv_obj_t *scr_calendar = NULL;//日历任务屏幕
lv_obj_t *calendar;   				//日历对象 
//日历任务屏幕
lv_obj_t *create_calendar_screen(void)
{
	//创建日历屏幕
	lv_obj_t *scr = lv_obj_create(NULL);
	lv_obj_t *calendar_cont = lv_obj_create(scr);  									// 父容器
	lv_obj_set_size(calendar_cont, 240, 240);     							    // 容器大小
	lv_obj_center(calendar_cont);                								    // 居中
	lv_obj_clear_flag(calendar_cont, LV_OBJ_FLAG_SCROLLABLE); 	    // 禁止滚动
	lv_obj_set_style_bg_color(calendar_cont, lv_color_white(), 0);  // 设置背景为白色
	lv_obj_set_style_bg_opa(calendar_cont, LV_OPA_COVER, 0);        // 确保背景不透明
	lv_obj_set_style_border_width(calendar_cont, 0, 0);          		// 去容器边框 边框宽度 0
	
	calendar = lv_calendar_create(calendar_cont);
	lv_obj_set_size(calendar, 240, 240);
	lv_obj_align(calendar, LV_ALIGN_CENTER, 0, 0);
	/*Highlight a few days*/
	static lv_calendar_date_t highlighted_days[1];       /*Only its pointer will be saved so should be static*/
	highlighted_days[0].year = 2025;
	highlighted_days[0].month = 10;
	highlighted_days[0].day = 1;
	
	lv_calendar_set_highlighted_dates(calendar, highlighted_days, 1);
  lv_calendar_header_arrow_create(calendar);
	lv_calendar_set_showed_date(calendar, 2025, 10);

	return scr;
}
void delete_calendar_screen(void) 
{
	if (scr_calendar) 
	{
		lv_obj_del(scr_calendar);
		scr_calendar = NULL;
	}
}

