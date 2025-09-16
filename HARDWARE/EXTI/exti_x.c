#include "exti_x.h"
#include "delay.h" 
#include "led.h" 
#include "key.h"
#include "FreeRTOS.h"
#include "task.h"

//extern TaskHandle_t task1_handler;
////外部中断8服务程序
//void EXTI3_IRQHandler(void)
//{
//	BaseType_t xYieldRequired;
//	delay_xms(10);	//消抖，注意不能调用系统提供的vTaskDelay，否则会卡死
//	if(KEY1 == 0)	  
//	{				 
//		xYieldRequired = xTaskResumeFromISR(task1_handler); //中断中恢复
//		printf("task1在中断中恢复.\r\n");
//		if(xYieldRequired == pdTRUE)
//		{
//			portYIELD_FROM_ISR( xYieldRequired );
//		}			
//	}		 
//	 EXTI_ClearITPendingBit(EXTI_Line3);//清除LINE3上的中断标志位 
//}

//外部中断初始化程序
//初始化PE2~4,PA0为中断输入.
void EXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	//KEY_Init(); //按键对应的IO口初始化
 
	//EXTI只有16个口，但是有很多GPIO口，怎么一一对应？SYSCFG会进行选择，从大量的GPIO中选择一个进入EXTI
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟，这个就是选择器的时钟
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);//PE3 连接到中断线3

  /* 配置EXTI_Line3 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line3;//Line3
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE8
  EXTI_Init(&EXTI_InitStructure);//配置
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;//外部中断3
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x05;//抢占优先级5
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;//子优先级0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
}

