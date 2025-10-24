#include "Dht11Screen.h"
static const char celsius_symbol[] = {0xE2, 0x84, 0x83, 0x00};  // ℃
lv_obj_t *scr_dht11 = NULL;		
static lv_obj_t *humdata_label = NULL;
static lv_obj_t *tempdata_label = NULL;
//任务屏幕
lv_obj_t *create_dht11_screen(void)
{
	LV_FONT_DECLARE(CelsiusSymbol);																		//自定义字体库	32px
	LV_IMG_DECLARE(lovegame); 																				//图片																	
	//创建屏幕
	lv_obj_t *scr = lv_obj_create(NULL);
	lv_obj_t *dht11_cont = lv_obj_create(scr);  											// 父容器
	lv_obj_set_size(dht11_cont, 240, 240);     							    			// 容器大小
	lv_obj_center(dht11_cont);                								    		// 居中
	lv_obj_clear_flag(dht11_cont, LV_OBJ_FLAG_SCROLLABLE); 	    			// 禁止滚动
	lv_obj_set_style_bg_color(dht11_cont, lv_color_hex(0xCFEDE7), 0);	//容器颜色
	lv_obj_set_style_bg_opa(dht11_cont, LV_OPA_COVER, 0);        			// 确保背景不透明
	lv_obj_set_style_border_width(dht11_cont, 0, 0);          				// 去容器边框 边框宽度 0
	//图片
	lv_obj_t *img_lovegame 		= lv_img_create(dht11_cont);  
	lv_img_set_src(img_lovegame, &lovegame);  
	lv_obj_align(img_lovegame, LV_ALIGN_BOTTOM_MID, 0, 0); 	
  //标签
	lv_obj_t *hum_label 	= NULL;
	lv_obj_t *temp_label 	= NULL;
	lv_obj_t *colon1 = lv_label_create(dht11_cont);
	lv_obj_t *colon2 = lv_label_create(dht11_cont);
	lv_obj_t *symbol1 = lv_label_create(dht11_cont);
	lv_obj_t *symbol2 = lv_label_create(dht11_cont);
	hum_label = lv_label_create(dht11_cont);					
	temp_label = lv_label_create(dht11_cont);	
	humdata_label = lv_label_create(dht11_cont);					
	tempdata_label = lv_label_create(dht11_cont);
	
	lv_obj_set_style_text_font(hum_label, &lv_font_montserrat_32, 0); 
	lv_obj_set_style_text_font(temp_label, &lv_font_montserrat_32, 0); 
	lv_obj_set_style_text_font(humdata_label, &lv_font_montserrat_32, 0); 
	lv_obj_set_style_text_font(tempdata_label, &lv_font_montserrat_32, 0); 
	lv_obj_set_style_text_font(colon1, &lv_font_montserrat_32, 0);  
	lv_obj_set_style_text_font(colon2, &lv_font_montserrat_32, 0);
	lv_obj_set_style_text_font(symbol1, &lv_font_montserrat_32, 0);  
	lv_obj_set_style_text_font(symbol2, &CelsiusSymbol, 0);
	
	lv_label_set_text(hum_label,	"Hum");
	lv_label_set_text(temp_label, "Temp");
	lv_label_set_text(humdata_label,	"00");
	lv_label_set_text(tempdata_label, "00");
	lv_label_set_text(colon1,	":");
	lv_label_set_text(colon2, ":");
	lv_label_set_text(symbol1, "%");	
	lv_label_set_text(symbol2, celsius_symbol);//℃

	lv_obj_align(hum_label, LV_ALIGN_TOP_LEFT, 0, 0); 
	lv_obj_align(temp_label, LV_ALIGN_TOP_LEFT, 0, 40); 
	lv_obj_set_pos(colon1,  100, 0); 
	lv_obj_set_pos(colon2,  100, 40); 
	lv_obj_align(humdata_label, LV_ALIGN_TOP_MID,20, 0); 
	lv_obj_align(tempdata_label, LV_ALIGN_TOP_MID, 20, 40); 
	lv_obj_set_pos(symbol1,  160, 0); 
	lv_obj_set_pos(symbol2,  160, 45); 
		
	return scr;
}
void delete_dht11_screen(void) 
{
	if (scr_dht11) 
	{
		lv_obj_del(scr_dht11);
		scr_dht11 = NULL;
	}
}
/* 更新DHT11屏幕显示的函数 */
void set_dht11_data(int hum_integer, int hum_decimal, int temp_integer, int temp_decimal)
{
	char buf[10];
	if(humdata_label) 
	{
		snprintf(buf, sizeof(buf), "%d", hum_integer);
		lv_label_set_text(humdata_label, buf);
	}
	if(tempdata_label) 
	{
		snprintf(buf, sizeof(buf), "%d", temp_integer);
		lv_label_set_text(tempdata_label, buf);
	}
	lv_timer_handler(); // 立即刷新LVGL显示
}