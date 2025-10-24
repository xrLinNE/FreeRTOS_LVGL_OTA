#include "dht11.h"
#define OUT	1
#define	IN	0

void DHT11_Mode(u8 mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	if(mode)
	{
		GPIO_InitStructure.GPIO_Pin 	= DHT11_GPIO_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_OType	=	GPIO_OType_PP;			//推挽
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	}
	else
	{
		GPIO_InitStructure.GPIO_Pin =  DHT11_GPIO_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	}
	GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStructure);
}


void DHT11_Rst(void)	   
{                 
	DHT11_Mode(OUT); 																		//输出
	GPIO_ResetBits(DHT11_GPIO_PORT, DHT11_GPIO_PIN);    //主机拉低
	delay_ms(20);    																		//主机拉低18~30ms
	GPIO_SetBits(DHT11_GPIO_PORT, DHT11_GPIO_PIN);     	//主机拉高
	delay_us(13);     																	//主机拉高10~35us
}
//dht11应答
u8 DHT11_Check(void) 	   
{   
	u8 retry=0;
	DHT11_Mode(IN);																													//输入模式
	while (GPIO_ReadInputDataBit(DHT11_GPIO_PORT,DHT11_GPIO_PIN)&&retry<100)//DHT11会拉低40~80us
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=100)return 1;
	else retry=0;
  while (!GPIO_ReadInputDataBit(DHT11_GPIO_PORT,DHT11_GPIO_PIN)&&retry<100)//DHT11拉低后会再次拉高40~80us
	{
		retry++;
		delay_us(1);
	};
	if(retry>=100)return 1;	    
	return 0;
}
//读bit
u8 DHT11_Read_Bit(void) 			 
{
 	u8 retry=0;
	while(GPIO_ReadInputDataBit(DHT11_GPIO_PORT,DHT11_GPIO_PIN)&&retry<100)//等待变为低电平
	{
		retry++;
		delay_us(1);
	}
	retry=0;
	while(!GPIO_ReadInputDataBit(DHT11_GPIO_PORT,DHT11_GPIO_PIN)&&retry<100)//等待变高电平
	{
		retry++;
		delay_us(1);
	}
	delay_us(40);//等待40us
	if(GPIO_ReadInputDataBit(DHT11_GPIO_PORT,DHT11_GPIO_PIN))return 1;
	else return 0;		   
}
u8 DHT11_Read_Byte(void)    
{        
	u8 i,dat;
	dat=0;
	for (i=0;i<8;i++) 
	{
		dat<<=1; 
		dat|=DHT11_Read_Bit();
	}						    
	return dat;
}
u8 DHT11_Read_Data(u8 *humi,u8 *humd, u8 *tempi,u8 *tempd)    
{        
 	u8 buf[5];
	u8 i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//读取40位数据
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0];
			*humd=buf[1];
			*tempi=buf[2];
			*tempd=buf[3];
		}
	}
	else return 1;
	return 0;	    
}
u8 DHT11_Init(void)
{
  /*Configure GPIO pin : PA1 */
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
	
  GPIO_InitStructure.GPIO_Pin 	= DHT11_GPIO_PIN; 		
	GPIO_InitStructure.GPIO_OType	=	GPIO_OType_PP;			
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;			//输出模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//
  GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStructure);		//初始化GPIO
	GPIO_SetBits(DHT11_GPIO_PORT,DHT11_GPIO_PIN);				
	DHT11_Rst();  																			//复位DHT11
	return DHT11_Check();																//等待DHT11的回应
}

