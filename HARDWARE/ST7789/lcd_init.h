#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "sys.h"


#define USE_SPI2 0	//Ӳ��SPI
#define USE_HARDWARE_SPI 1	//Ӳ��SPI
#define USE_DMA_LCD 		 1	//ʹ��DMA��������ֻ�е�ʹ��Ӳ��SPIʱ����ʹ��DMA�����򱨴�

#define USE_HORIZONTAL 0  //���ú�������������ʾ 0��1Ϊ���� 2��3Ϊ����.     NOTE:LVGLĬ�Ϻ�����ʽ!!!!,������ʹ�õ���Ļ�߶ȺͿ����ȣ����Ժ�������ν��


#define LCD_W 240
#define LCD_H 240

//										SPI2	 				SPI 1
//              GND   
//              VCC  
//              SCL   PB10	SCLK		PA5
//              SDA   PC3		MOSI		PA7
//              RES   PC4
//              DC    PC5
//              BLK   PC6

#if	USE_SPI2
//-----------------LCD�˿ڶ���---------------- 
#define LCD_SCL_CLK  			RCC_AHB1Periph_GPIOB
#define LCD_Rem_CLK  			RCC_AHB1Periph_GPIOC

#define LCD_SCL_Port 			GPIOB
#define LCD_Rem_Port 			GPIOC

#define LCD_SCLK_Pin 			GPIO_Pin_10
#define LCD_SCLK_Source 	GPIO_PinSource10

#define LCD_SDA_Pin 			GPIO_Pin_3
#define LCD_SDA_Source 		GPIO_PinSource3

#define LCD_RES_Pin 			GPIO_Pin_4
#define LCD_DC_Pin 				GPIO_Pin_5
#define LCD_BLK_Pin 			GPIO_Pin_6
//-----------------LCD�˿ڲ���---------------- 
#define LCD_SCLK_Clr() GPIO_ResetBits(LCD_SCL_Port,LCD_SCLK_Pin)//SCL=SCLK
#define LCD_SCLK_Set() GPIO_SetBits(LCD_SCL_Port,LCD_SCLK_Pin)

#define LCD_MOSI_Clr() GPIO_ResetBits(LCD_Rem_Port,LCD_SDA_Pin)//SDA=MOSI
#define LCD_MOSI_Set() GPIO_SetBits(LCD_Rem_Port,LCD_SDA_Pin)

#define LCD_RES_Clr()  GPIO_ResetBits(LCD_Rem_Port,LCD_RES_Pin)//RES
#define LCD_RES_Set()  GPIO_SetBits(LCD_Rem_Port,LCD_RES_Pin)

#define LCD_DC_Clr()   GPIO_ResetBits(LCD_Rem_Port,LCD_DC_Pin)//DC
#define LCD_DC_Set()   GPIO_SetBits(LCD_Rem_Port,LCD_DC_Pin)

#define LCD_BLK_Clr()  GPIO_ResetBits(LCD_Rem_Port,LCD_BLK_Pin)//BLK
#define LCD_BLK_Set()  GPIO_SetBits(LCD_Rem_Port,LCD_BLK_Pin)

#else

#define LCD_SCL_CLK  			RCC_AHB1Periph_GPIOA
#define LCD_Rem_CLK  			RCC_AHB1Periph_GPIOC

#define LCD_SCL_Port 			GPIOA
#define LCD_Rem_Port 			GPIOC

#define LCD_SCLK_Pin 			GPIO_Pin_5
#define LCD_SCLK_Source 	GPIO_PinSource5

#define LCD_SDA_Pin 			GPIO_Pin_7
#define LCD_SDA_Source 		GPIO_PinSource7

#define LCD_RES_Pin 			GPIO_Pin_4
#define LCD_DC_Pin 				GPIO_Pin_5
#define LCD_BLK_Pin 			GPIO_Pin_6
//-----------------LCD�˿ڲ���---------------- 
#define LCD_SCLK_Clr() GPIO_ResetBits(LCD_SCL_Port,LCD_SCLK_Pin)//SCL=SCLK
#define LCD_SCLK_Set() GPIO_SetBits(LCD_SCL_Port,LCD_SCLK_Pin)

#define LCD_MOSI_Clr() GPIO_ResetBits(LCD_SCL_Port,LCD_SDA_Pin)//SDA=MOSI
#define LCD_MOSI_Set() GPIO_SetBits(LCD_SCL_Port,LCD_SDA_Pin)

#define LCD_RES_Clr()  GPIO_ResetBits(LCD_Rem_Port,LCD_RES_Pin)//RES
#define LCD_RES_Set()  GPIO_SetBits(LCD_Rem_Port,LCD_RES_Pin)

#define LCD_DC_Clr()   GPIO_ResetBits(LCD_Rem_Port,LCD_DC_Pin)//DC
#define LCD_DC_Set()   GPIO_SetBits(LCD_Rem_Port,LCD_DC_Pin)

#define LCD_BLK_Clr()  GPIO_ResetBits(LCD_Rem_Port,LCD_BLK_Pin)//BLK
#define LCD_BLK_Set()  GPIO_SetBits(LCD_Rem_Port,LCD_BLK_Pin)

#endif



void LCD_GPIO_Init(void);//��ʼ��GPIO
void LCD_Writ_Bus(u8 dat);//ģ��SPIʱ��
void LCD_WR_DATA8(u8 dat);//д��һ���ֽ�
void LCD_WR_DATA(u16 dat);//д�������ֽ�
void LCD_WR_REG(u8 dat);//д��һ��ָ��
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2);//�������꺯��
void LCD_Init(void);//LCD��ʼ��
#endif




