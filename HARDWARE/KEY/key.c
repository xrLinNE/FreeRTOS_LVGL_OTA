#include <stm32f4xx.h>
#include "key.h"
#include "delay.h" 

//按键初始化函数
void KEY_Init(void)
{
	//四个操作按键
	
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
	
  GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN|KEY2_GPIO_PIN|KEY3_GPIO_PIN|KEY4_GPIO_PIN; //KEY 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);//初始化GPIO
} 
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY0按下
//2，KEY1按下
//3，KEY2按下 
//4，WKUP按下 WK_UP
//注意此函数有响应优先级
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	
	if(mode) key_up=1;  //支持连按		
  
	if(key_up && (KEY1==0||KEY2==0))
	{
		delay_xms(10);//去抖动 
		key_up=0;
		if(KEY1==0)				return 1;
		else if(KEY2==0)	return 2;
		else if(KEY3==0)	return 3;
		else if(KEY4==0)	return 4;
	}else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1)	key_up=1; 	    
 	return 0;// 无按键按下
}

