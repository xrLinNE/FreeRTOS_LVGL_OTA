#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "exti_x.h"
#include "timerx.h"
#include "FreeRTOS_task.h"


int main(void)
{ 
 
	delay_init(168);		  //初始化延时函数
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置NVIC中断配置分组，为了适配FreeRTOS中断管理，4bit表示抢占，0bit表示响应
	LED_Init();		        //初始化LED端口
	KEY_Init();
	uart_init(115200);
	//EXTIX_Init();
//	TIM3_Int_Init(9999,8399);//1s定时
//	TIM4_Int_Init(9999,8399);
	FreeRTOS_task();
	while(1)
	{

	}
}


