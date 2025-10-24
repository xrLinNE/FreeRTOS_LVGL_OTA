#ifndef __DHT11_H
#define	__DHT11_H	   
#include "sys.h"
#include "stm32f4xx_dma.h"  
#include "delay.h"
#include "usart.h"
#include "stm32f4xx.h" 

#define DHT11_GPIO_PORT                  GPIOA
#define DHT11_GPIO_PIN                   GPIO_Pin_6

u8 DHT11_Init(void);
u8 DHT11_Read_Data(u8 *humi,u8 *humd, u8 *tempi,u8 *tempd);

#endif



