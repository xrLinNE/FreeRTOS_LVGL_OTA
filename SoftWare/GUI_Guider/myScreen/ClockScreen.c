#include "ClockScreen.h"

static lv_obj_t *h1_label = NULL;
static lv_obj_t *h2_label = NULL;
static lv_obj_t *m1_label = NULL;
static lv_obj_t *m2_label = NULL;
static lv_obj_t *s1_label = NULL;
static lv_obj_t *s2_label = NULL;
static lv_obj_t *rethour_label = NULL;
static lv_obj_t *retmin_label	 = NULL;
static lv_obj_t *retsec_label	 = NULL;
static lv_obj_t *start_label	 = NULL;
lv_obj_t *scr_clock = NULL;							//闹钟任务屏幕
int8_t seclect_flag = 0;								//0：分个位 1：分十位 2：时个位 3：时十位
uint8_t g_clock_num[6] = {0, 0, 0, 0, 0, 0};  //分别对应 s1,s2,m1,m2,h1,h2
void update_select_border(uint8_t select_flag)
{
    // 清除所有标签的边框
    lv_obj_set_style_border_width(h1_label, 0, 0);
    lv_obj_set_style_border_width(h2_label, 0, 0);
    lv_obj_set_style_border_width(m1_label, 0, 0);
		lv_obj_set_style_border_width(m2_label, 0, 0);
		lv_obj_set_style_border_width(s1_label, 0, 0);
		lv_obj_set_style_border_width(s2_label, 0, 0);
		lv_obj_set_style_border_width(start_label, 0, 0);

    // 选中项加蓝色边框
    lv_color_t blue = lv_color_hex(0x1E90FF);

    switch(select_flag)
    {
        case 0: // 秒 个位
            lv_obj_set_style_border_width(s1_label, 2, 0);
            lv_obj_set_style_border_color(s1_label, blue, 0);
            lv_obj_set_style_border_opa(s1_label, LV_OPA_COVER, 0);
            break;
        case 1: // 秒	十位
            lv_obj_set_style_border_width(s2_label, 2, 0);
            lv_obj_set_style_border_color(s2_label, blue, 0);
            lv_obj_set_style_border_opa(s2_label, LV_OPA_COVER, 0);
            break;
        case 2: // 分 个位
            lv_obj_set_style_border_width(m1_label, 2, 0);
            lv_obj_set_style_border_color(m1_label, blue, 0);
            lv_obj_set_style_border_opa(m1_label, LV_OPA_COVER, 0);
            break;
				case 3: // 分  十位
            lv_obj_set_style_border_width(m2_label, 2, 0);
            lv_obj_set_style_border_color(m2_label, blue, 0);
            lv_obj_set_style_border_opa(m2_label, LV_OPA_COVER, 0);
            break;
				case 4: // 时  个位
            lv_obj_set_style_border_width(h1_label, 2, 0);
            lv_obj_set_style_border_color(h1_label, blue, 0);
            lv_obj_set_style_border_opa(h1_label, LV_OPA_COVER, 0);
            break;
				case 5: // 时  十位
            lv_obj_set_style_border_width(h2_label, 2, 0);
            lv_obj_set_style_border_color(h2_label, blue, 0);
            lv_obj_set_style_border_opa(h2_label, LV_OPA_COVER, 0);
            break;
				case 6: // START
            lv_obj_set_style_border_width(start_label, 2, 0);
            lv_obj_set_style_border_color(start_label, blue, 0);
            lv_obj_set_style_border_opa(start_label, LV_OPA_COVER, 0);
            break;
    }
}
//闹钟任务屏幕
lv_obj_t *create_clock_screen(void)
{
	LV_IMG_DECLARE(zz_img_64); 																				//图片
	LV_IMG_DECLARE(clock_img_64); 																				
	//创建闹钟屏幕
	lv_obj_t *scr = lv_obj_create(NULL);
	lv_obj_t *clock_cont = lv_obj_create(scr);  											// 父容器
	lv_obj_set_size(clock_cont, 240, 240);     							    			// 容器大小
	lv_obj_center(clock_cont);                								    		// 居中
	lv_obj_clear_flag(clock_cont, LV_OBJ_FLAG_SCROLLABLE); 	    			// 禁止滚动
	lv_obj_set_style_bg_color(clock_cont, lv_color_hex(0xCFEDE7), 0);	//容器颜色
	lv_obj_set_style_bg_opa(clock_cont, LV_OPA_COVER, 0);        			// 确保背景不透明
	lv_obj_set_style_border_width(clock_cont, 0, 0);          				// 去容器边框 边框宽度 0
	//图片
	lv_obj_t *img_zz 		= lv_img_create(clock_cont);  
	lv_obj_t *img_clock = lv_img_create(clock_cont); 	
	lv_img_set_src(img_zz, &zz_img_64);  
	//lv_obj_set_pos(img_zz,  6, 170);	
	lv_obj_align(img_zz, LV_ALIGN_TOP_LEFT, 0, 0); 	
	lv_img_set_src(img_clock, &clock_img_64);         													
	//lv_obj_set_pos(img_clock,  170, 6);
	lv_obj_align(img_clock, LV_ALIGN_BOTTOM_MID, 0, 0); 	
	//时间标签
	h1_label = lv_label_create(clock_cont);					//小时个位
	h2_label = lv_label_create(clock_cont);					//小时十位
	
	m1_label = lv_label_create(clock_cont);						
	m2_label = lv_label_create(clock_cont);				

	s1_label = lv_label_create(clock_cont);						
	s2_label = lv_label_create(clock_cont);		

	rethour_label = lv_label_create(clock_cont);
	retmin_label  = lv_label_create(clock_cont);
	retsec_label	= lv_label_create(clock_cont);
	
	start_label  	= lv_label_create(clock_cont);
	
	lv_obj_t *set_label = lv_label_create(clock_cont);
	lv_obj_t *ret_label = lv_label_create(clock_cont);
	//冒号
	lv_obj_t *colon1 = lv_label_create(clock_cont);
	lv_obj_t *colon2 = lv_label_create(clock_cont);
	lv_obj_t *colon3 = lv_label_create(clock_cont);
	lv_obj_t *colon4 = lv_label_create(clock_cont);
	//排列
	lv_obj_set_style_text_font(start_label, &lv_font_montserrat_32, 0); 
	lv_obj_set_style_text_color(start_label,lv_color_hex(0xFFA500), 0);
		
	lv_obj_set_style_text_font(set_label, &lv_font_montserrat_32, 0); 
	lv_obj_set_style_text_font(ret_label, &lv_font_montserrat_32, 0); 
	
	lv_obj_set_style_text_font(h1_label, &lv_font_montserrat_32, 0);
	lv_obj_set_style_text_font(h2_label, &lv_font_montserrat_32, 0); 
	lv_obj_set_style_text_font(m1_label, &lv_font_montserrat_32, 0);
	lv_obj_set_style_text_font(m2_label, &lv_font_montserrat_32, 0); 
	lv_obj_set_style_text_font(s1_label, &lv_font_montserrat_32, 0);
	lv_obj_set_style_text_font(s2_label, &lv_font_montserrat_32, 0); 
	
	lv_obj_set_style_text_font(rethour_label, &lv_font_montserrat_32, 0);
	lv_obj_set_style_text_font(retmin_label, &lv_font_montserrat_32, 0); 
	lv_obj_set_style_text_font(retsec_label, &lv_font_montserrat_32, 0); 

	lv_obj_set_style_text_font(colon1, &lv_font_montserrat_32, 0);  
	lv_obj_set_style_text_font(colon2, &lv_font_montserrat_32, 0); 
	lv_obj_set_style_text_font(colon3, &lv_font_montserrat_32, 0);  
	lv_obj_set_style_text_font(colon4, &lv_font_montserrat_32, 0); 
	
	lv_label_set_text(set_label,	 	"SET");
	lv_label_set_text(ret_label, 		"RET");
	lv_label_set_text(start_label, 	"START");
	
	char buf[2];
	sprintf(buf,"%d", g_clock_num[0]);
	lv_label_set_text(s1_label,buf);
	sprintf(buf,"%d", g_clock_num[1]);
	lv_label_set_text(s2_label,buf);
	sprintf(buf,"%d", g_clock_num[2]);
	lv_label_set_text(m1_label,buf);
	sprintf(buf,"%d", g_clock_num[3]);
	lv_label_set_text(m2_label,buf);
	sprintf(buf,"%d", g_clock_num[4]);
	lv_label_set_text(h1_label,buf);
	sprintf(buf,"%d", g_clock_num[5]);
	lv_label_set_text(h2_label,buf);
	
	lv_label_set_text(rethour_label, "00");
	lv_label_set_text(retmin_label,  "00");
	lv_label_set_text(retsec_label,  "00");

	lv_label_set_text(colon1, ":");
	lv_label_set_text(colon2, ":");
	lv_label_set_text(colon3, ":");
	lv_label_set_text(colon4, ":");

	lv_obj_set_pos(h2_label, 65,		86);			//十位
	lv_obj_set_pos(h1_label, 87,		86);			//个位
	lv_obj_set_pos(colon1,   112, 	84);
	lv_obj_set_pos(m2_label, 125, 	86); 			//十位
	lv_obj_set_pos(m1_label, 147, 	86); 			//个位
	lv_obj_set_pos(colon3,   172, 	84);
	lv_obj_set_pos(s2_label, 185, 	86); 			
	lv_obj_set_pos(s1_label, 207, 	86); 			
	
	lv_obj_set_pos(rethour_label, 65,		126); 
	lv_obj_set_pos(colon2,     		112, 	124);
	lv_obj_set_pos(retmin_label,  125, 	126); 
	lv_obj_set_pos(colon4,     		172, 	124);
	lv_obj_set_pos(retsec_label,  185, 	126); 

	lv_obj_set_pos(set_label, 0,	86);
	lv_obj_set_pos(ret_label, 0, 126);
	
	lv_obj_align(start_label, LV_ALIGN_TOP_RIGHT, 0, 0); 
	
	seclect_flag = 6;				//高亮START
	update_select_border(seclect_flag);
	
	return scr;
}
void delete_clock_screen(void) 
{
	if (scr_clock) 
	{
		lv_obj_del(scr_clock);
		scr_clock = NULL;
	}
}
//时间更新
void ret_set_time(uint8_t hour_decade, uint8_t hour_unit, uint8_t min_decade,  uint8_t min_unit, uint8_t sec_decade, uint8_t sec_unit)
{
    char buf[2];
    if(rethour_label) 
		{
			sprintf(buf, "%d%d", hour_decade, hour_unit);
			lv_label_set_text(rethour_label, buf);
		}
    if(retmin_label) 
		{
			sprintf(buf, "%d%d", min_decade, min_unit);
			lv_label_set_text(retmin_label, buf);
    }
    if(retsec_label) 
		{
			sprintf(buf, "%d%d", sec_decade, sec_unit);
			lv_label_set_text(retsec_label, buf);
    }
    lv_timer_handler(); // 立即刷新
}
void update_clock_digits(int8_t seclect_flag)
{
	char buf[2];
	switch(seclect_flag)
	{
		case 0:
			snprintf(buf, sizeof(buf), "%d", g_clock_num[0]);
			lv_label_set_text(s1_label, buf);
			break;
		case 1:
			snprintf(buf, sizeof(buf), "%d", g_clock_num[1]);
			lv_label_set_text(s2_label, buf);
			break;
		case 2:
			snprintf(buf, sizeof(buf), "%d", g_clock_num[2]);
			lv_label_set_text(m1_label, buf);
			break;
		case 3:
			snprintf(buf, sizeof(buf), "%d", g_clock_num[3]);
			lv_label_set_text(m2_label, buf);
			break;
		case 4:
			snprintf(buf, sizeof(buf), "%d", g_clock_num[4]);
			lv_label_set_text(h1_label, buf);
			break;
		case 5:
			snprintf(buf, sizeof(buf), "%d", g_clock_num[5]);
			lv_label_set_text(h2_label, buf);
			break;
	}
}
