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
#include "dht11.h"
#include "FreeRTOS_task.h"

//队列
QueueHandle_t g_xQueueMenu;						//传递按键数据	
int main(void)
{ 
	//创建队列
	g_xQueueMenu = xQueueCreate(1, 4);
	/*系统初始化*/
	delay_init(168);		 													 	//初始化延时函数
	uart_init(115200);															//串口
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	//设置NVIC中断配置分组，为了适配FreeRTOS中断管理，4bit表示抢占，0bit表示响应
	/* 硬件驱动初始化 */
	LED_Init();		        													//初始化LED端口
	EXTIX_Init();																		//按键和外部中断初始化
	LCD_Init();																			//LCD
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);								//画屏幕
	lv_init();                            					// LVGL 初始化
	lv_port_disp_init();                  					// 注册LVGL的显示任务
	TIM3_Int_Init(9,8399);							 						//1ms定时,为LVGL提供时间基础
	delay_xms(50);																	//延迟一会
	/* 进入FreeRTOS */
	FreeRTOS_task();	//创建各个任务并开启任务调度
	while(1)
	{
	}
}


