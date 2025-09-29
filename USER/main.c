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
	u16 i,j;
	double t=0;

	delay_init(168);		  //初始化延时函数
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置NVIC中断配置分组，为了适配FreeRTOS中断管理，4bit表示抢占，0bit表示响应
	LED_Init();		        //初始化LED端口
	EXTIX_Init();					//按键和外部中断初始化
	uart_init(115200);
	LCD_Init();
	LCD_Fill(0,0,LCD_W,LCD_H,YELLOW);


//	TIM3_Int_Init(9999,8399);//1s定时
//	TIM4_Int_Init(9999,8399);
//	FreeRTOS_task();
	while(1)
	{
		LED1(1);
		LCD_ShowPicture(0,0,240,240,gImage_2);
		delay_xms(1000);
		LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
		LCD_ShowChinese(0,0,"abcde",RED,WHITE,12,0);
		LCD_ShowString(0,40,"LCD_W:",RED,WHITE,16,0);
		LCD_ShowIntNum(48,40,LCD_W,3,RED,WHITE,16);
		LCD_ShowString(80,40,"LCD_H:",RED,WHITE,16,0);
		LCD_ShowIntNum(128,40,LCD_H,3,RED,WHITE,16);
		LCD_ShowString(80,40,"LCD_H:",RED,WHITE,16,0);
		LCD_ShowString(0,70,"Increaseing Nun:",RED,WHITE,16,0);
		LCD_ShowFloatNum1(128,70,t,4,RED,WHITE,16);
		t+=0.11;
		for(j=0;j<3;j++)
		{
			for(i=0;i<6;i++)
			{
				LCD_ShowPicture(40*i,120+j*40,40,40,gImage_1);
			}
		}
		LED1(0);
		delay_xms(1000);
		LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	}
}


