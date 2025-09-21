#include "lcd_init.h"
#include "spi.h"

//SPI2 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI2_ReadWriteByte(u8 TxData)
{		 			 
 
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET){}//�ȴ���������  
	
	SPI_I2S_SendData(SPI2, TxData); //ͨ������SPIx����һ��byte  ����
		
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){} //�ȴ�������һ��byte  
 
	return SPI_I2S_ReceiveData(SPI2); //����ͨ��SPIx������յ�����	
 		    
}

void SPI2_Init(void)
{
	//��ʼ�� SCL SDA
	#if USE_HARDWARE_SPI
	
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
	RCC_AHB1PeriphClockCmd(LCD_SCL_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(LCD_Rem_CLK, ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);//ʹ��SPI2  APB1 42Mhz
	//SCL
	GPIO_InitStructure.GPIO_Pin = LCD_SCLK_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(LCD_SCL_Port, &GPIO_InitStructure);//��ʼ��
	//SDA
	GPIO_InitStructure.GPIO_Pin = LCD_SDA_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(LCD_Rem_Port, &GPIO_InitStructure);//��ʼ��
	
	GPIO_PinAFConfig(LCD_SCL_Port, LCD_SCLK_Source, GPIO_AF_SPI2);//��������
	GPIO_PinAFConfig(LCD_Rem_Port, LCD_SDA_Source, GPIO_AF_SPI2);
	
	//��λSPI
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, DISABLE);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//ֻ����ģʽ
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//����SPI����ģʽ������ģʽ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//����SPI���ݴ�С��8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//����ͬ��ʱ�ӿ���ʱSCLKλ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//����ͬ��ʱ�ӿյڶ���ʱ���ز���
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//NSS�ź����������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;//������Ԥ��Ƶֵ��������Ԥ��ƵֵΪ2
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//���ݴ����λ����
	SPI_InitStructure.SPI_CRCPolynomial = 7;//CRCֵ����Ķ���ʽ
	
	SPI_Init(SPI2,&SPI_InitStructure);//��ʼ��SPI
	SPI_Cmd(SPI2, ENABLE);//ʹ��SPI
	//SPI2_ReadWriteByte(0xff);//��������	
	#else
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(LCD_SCL_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(LCD_Rem_CLK, ENABLE);
	//SCL
  GPIO_InitStructure.GPIO_Pin = LCD_SCLK_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(LCD_SCL_Port, &GPIO_InitStructure);//��ʼ��
	GPIO_SetBits(LCD_SCL_Port,LCD_SCLK_Pin);	
	//SDA
  GPIO_InitStructure.GPIO_Pin = LCD_SDA_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(LCD_Rem_Port, &GPIO_InitStructure);//��ʼ��
	GPIO_SetBits(LCD_Rem_Port,LCD_SDA_Pin);	
	#endif
}



