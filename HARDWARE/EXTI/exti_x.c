#include "exti_x.h"
#include "delay.h" 
#include "led.h" 
#include "key.h"
#include "FreeRTOS.h"
#include "FreeRTOS_task.h"
#include "task.h"

//extern 	BaseType_t end_flag;
//extern	BaseType_t seclect_end;
extern	QueueHandle_t g_xQueueMenu;	

//外部中断初始化程序
void EXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	KEY_Init(); //按键对应的IO口初始化
 
	//EXTI只有16个口，但是有很多GPIO口，怎么一一对应？SYSCFG会进行选择，从大量的GPIO中选择一个进入EXTI
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟，这个就是选择器的时钟
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource1);//PA1 连接到中断线1
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource2);//PA2 连接到中断线2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource3);//PA3 连接到中断线3
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource4);//PA4 连接到中断线4

  /* 配置EXTI_LineX */
  EXTI_InitStructure.EXTI_Line = EXTI_Line1|EXTI_Line2|EXTI_Line3|EXTI_Line4;//Line
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE
  EXTI_Init(&EXTI_InitStructure);//配置
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;//外部中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x05;//抢占优先级5
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;//子优先级0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x05;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x05;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x05;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//KEY1中断
void EXTI1_IRQHandler(void)
{
	BaseType_t  RM_Flag;
	Key_data 		key_data;
	if(EXTI_GetITStatus(EXTI_Line1) != RESET)//判断，否则会进入两次中断
	{
		delay_xms(10);	//消抖，注意不能调用系统提供的vTaskDelay，否则会卡死
		if(KEY1 == 0)	  
		{	
//			if(end_flag == 1&&seclect_end == 0)
//			{
				RM_Flag = 1;
				key_data.ldata = RM_Flag;
				xQueueSendToBackFromISR(g_xQueueMenu, &key_data, NULL);
				RM_Flag = 0;			
//			}
			LED1_TOGGLE();			 
			printf("key1 it test..\r\n");
		}		 
		EXTI_ClearITPendingBit(EXTI_Line1);//清除LINE上的中断标志位 
	}
}
//KEY2中断
void EXTI2_IRQHandler(void)
{
	BaseType_t  LM_Flag;
	Key_data 		key_data;
	if(EXTI_GetITStatus(EXTI_Line2) != RESET)
	{
		delay_xms(10);	//消抖，注意不能调用系统提供的vTaskDelay，否则会卡死
		if(KEY2 == 0)	  
		{
//			if(end_flag == 1&&seclect_end == 0)
//			{
				LM_Flag = 1;
				key_data.rdata = LM_Flag;
				xQueueSendToBackFromISR(g_xQueueMenu, &key_data, NULL);
				LM_Flag = 0;			
//			}			
			LED1_TOGGLE();			
			printf("key2 it test..\r\n");
		}		
		EXTI_ClearITPendingBit(EXTI_Line2);//清除LINE上的中断标志位 
	}
}
//KEY3中断
void EXTI3_IRQHandler(void)
{
	BaseType_t  EN_Flag;
	Key_data 		key_data;
	if(EXTI_GetITStatus(EXTI_Line3) != RESET)
	{
		delay_xms(10);	//消抖，注意不能调用系统提供的vTaskDelay，否则会卡死
		if(KEY3 == 0)	  
		{
//			if(end_flag == 1&&seclect_end == 0)
//			{
				EN_Flag = 1;
				key_data.updata = EN_Flag;
				xQueueSendToBackFromISR(g_xQueueMenu, &key_data, NULL);
				EN_Flag = 0;			
//			}			
			LED1_TOGGLE();	
			printf("key3 it test..\r\n");
		}		 
		EXTI_ClearITPendingBit(EXTI_Line3);//清除LINE上的中断标志位 
	}
}
//KEY4中断
void EXTI4_IRQHandler(void)
{
	BaseType_t  EX_Flag;
	Key_data 		key_data;
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)
	{
		delay_xms(10);	//消抖，注意不能调用系统提供的vTaskDelay，否则会卡死
		if(KEY4 == 0)	  
		{
//			if(end_flag == 1&&seclect_end == 0)
//			{
				EX_Flag = 1;
				key_data.exdata = EX_Flag;
				xQueueSendToBackFromISR(g_xQueueMenu, &key_data, NULL);
				EX_Flag = 0;			
//			}			
			LED1_TOGGLE();			
			printf("key4 it test..\r\n");
		}		 
		EXTI_ClearITPendingBit(EXTI_Line4);//清除LINE上的中断标志位 
	}
}
