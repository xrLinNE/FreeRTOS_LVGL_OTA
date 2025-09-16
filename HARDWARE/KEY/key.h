#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 

//引脚定义
#define KEY0_GPIO_PORT                  GPIOE
#define KEY0_GPIO_PIN                   GPIO_Pin_4

#define KEY1_GPIO_PORT                  GPIOE
#define KEY1_GPIO_PIN                   GPIO_Pin_3


/*下面的方式是通过直接操作库函数方式读取IO*/
#define KEY0 		GPIO_ReadInputDataBit(KEY0_GPIO_PORT,KEY0_GPIO_PIN) 
#define KEY1 		GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY1_GPIO_PIN)	

#define KEY0_PRES 	1
#define KEY1_PRES		2


void KEY_Init(void);	//IO初始化
u8 KEY_Scan(u8);  		//按键扫描函数	

#endif
