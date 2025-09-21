#include "lcd_init.h"
#include "spi.h"

//SPI2 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI2_ReadWriteByte(u8 TxData)
{		 			 
 
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET){}//等待发送区空  
	
	SPI_I2S_SendData(SPI2, TxData); //通过外设SPIx发送一个byte  数据
		
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){} //等待接收完一个byte  
 
	return SPI_I2S_ReceiveData(SPI2); //返回通过SPIx最近接收的数据	
 		    
}

void SPI2_Init(void)
{
	//初始化 SCL SDA
	#if USE_HARDWARE_SPI
	
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
	RCC_AHB1PeriphClockCmd(LCD_SCL_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(LCD_Rem_CLK, ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);//使能SPI2  APB1 42Mhz
	//SCL
	GPIO_InitStructure.GPIO_Pin = LCD_SCLK_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(LCD_SCL_Port, &GPIO_InitStructure);//初始化
	//SDA
	GPIO_InitStructure.GPIO_Pin = LCD_SDA_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(LCD_Rem_Port, &GPIO_InitStructure);//初始化
	
	GPIO_PinAFConfig(LCD_SCL_Port, LCD_SCLK_Source, GPIO_AF_SPI2);//复用配置
	GPIO_PinAFConfig(LCD_Rem_Port, LCD_SDA_Source, GPIO_AF_SPI2);
	
	//复位SPI
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, DISABLE);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//只发送模式
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//设置SPI工作模式：主机模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//设置SPI数据大小：8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//串行同步时钟空闲时SCLK位高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//串行同步时钟空第二个时钟沿捕获
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//NSS信号由软件管理
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;//波特率预分频值：波特率预分频值为2
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//数据传输高位先行
	SPI_InitStructure.SPI_CRCPolynomial = 7;//CRC值计算的多项式
	
	SPI_Init(SPI2,&SPI_InitStructure);//初始化SPI
	SPI_Cmd(SPI2, ENABLE);//使能SPI
	//SPI2_ReadWriteByte(0xff);//启动传输	
	#else
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(LCD_SCL_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(LCD_Rem_CLK, ENABLE);
	//SCL
  GPIO_InitStructure.GPIO_Pin = LCD_SCLK_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(LCD_SCL_Port, &GPIO_InitStructure);//初始化
	GPIO_SetBits(LCD_SCL_Port,LCD_SCLK_Pin);	
	//SDA
  GPIO_InitStructure.GPIO_Pin = LCD_SDA_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(LCD_Rem_Port, &GPIO_InitStructure);//初始化
	GPIO_SetBits(LCD_Rem_Port,LCD_SDA_Pin);	
	#endif
}



