#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "exti_x.h"
#include "timerx.h"
#include "lcd_init.h"
#include "lcd.h"
#include "dma.h"
#include "pic.h"
#include "FreeRTOS_task.h"

int main(void)
{ 
	delay_init(168);		  //初始化延时函数
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置NVIC中断配置分组，为了适配FreeRTOS中断管理，4bit表示抢占，0bit表示响应
	LED_Init();		        //初始化LED端口
	EXTIX_Init();					//按键和外部中断初始化
	uart_init(115200);
	//LCD
	LCD_Init();
	LCD_Fill(0,0,LCD_W,LCD_H,LIGHTBLUE);
	//LVGL
	lv_init();                            // LVGL 初始化
	lv_port_disp_init();                  // 注册LVGL的显示任务
	TIM3_Int_Init(9,8399);							 	//1ms定时,为LVGL提供时间基础
	//创建各个任务并开启任务调度
	delay_xms(100);												//延迟一会儿，等待硬件初始化完成
	FreeRTOS_task();
	while(1)
	{
	}
}


