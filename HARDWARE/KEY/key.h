#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 

//引脚定义
#define KEY1_GPIO_PORT                  GPIOA
#define KEY1_GPIO_PIN                   GPIO_Pin_1

#define KEY2_GPIO_PORT                  GPIOA
#define KEY2_GPIO_PIN                   GPIO_Pin_2

#define KEY3_GPIO_PORT                  GPIOA
#define KEY3_GPIO_PIN                   GPIO_Pin_3

#define KEY4_GPIO_PORT                  GPIOA
#define KEY4_GPIO_PIN                   GPIO_Pin_4

/*下面的方式是通过直接操作库函数方式读取IO*/
#define KEY1 		GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY1_GPIO_PIN) 
#define KEY2 		GPIO_ReadInputDataBit(KEY2_GPIO_PORT,KEY2_GPIO_PIN)	
#define KEY3 		GPIO_ReadInputDataBit(KEY3_GPIO_PORT,KEY3_GPIO_PIN) 
#define KEY4 		GPIO_ReadInputDataBit(KEY4_GPIO_PORT,KEY4_GPIO_PIN)	

#define KEY1_PRES 	1
#define KEY2_PRES		2
#define KEY3_PRES 	3
#define KEY4_PRES		4


void KEY_Init(void);	//IO初始化
u8 KEY_Scan(u8);  		//按键扫描函数	

#endif
