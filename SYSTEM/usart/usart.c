#include "sys.h"
#include "usart.h"	
#include "dma.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "FreeRTOS.h"					//FreeRTOS使用	  
#endif
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕
	USART1->DR = (u8) ch;      
	return ch;
}
#endif

uint8_t U1_RxBuff[U1_RX_SIZE];//接收缓冲区
UCB_CB	U1_CB;								//串口控制块
void U1Rx_PtrInit(void)
{
	U1_CB.URxDataIN = &U1_CB.URxDataPtr[0];					//开始指向第一个数据块
	U1_CB.URxDataOUT = &U1_CB.URxDataPtr[0];
	U1_CB.URxDataEND = &U1_CB.URxDataPtr[NUM - 1];	//指向最后一个数据块
	U1_CB.URxDataIN->start = U1_RxBuff;							//第一个数据块保存的起始位置为缓冲区的起始位置
	U1_CB.Counter = 0;
}
//初始化IO 串口1 
//bound:波特率
void uart_init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	
	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

	//USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
	#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);					//开启空闲中断
	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;				//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=5;	//抢占优先级5
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;				//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);													//根据指定的参数初始化VIC寄存器中断
	#endif
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);					//使能串口DMA接收通道
	//初始指针
	U1Rx_PtrInit();
	//由于是利用串口空闲中断来判断一次数据的接收，不是利用DMA一次搬运完中断来接受，所以DMA的搬运次数要比一次最大接收的数据数多，以避免DMA中断
	MYDMA_Config(DMA2_Stream2, DMA_Channel_4, (u32)&USART1->DR, (u32)U1_RxBuff, (U1_RX_MAX+1));		//DMA通道初始化
	MYDMA_Enable(DMA2_Stream2);	//开启DMA通道
	USART_Cmd(USART1, ENABLE);  //使能串口1 
	//USART_ClearFlag(USART1, USART_FLAG_TC);
}

void USART1_IRQHandler(void)                							//串口1中断服务程序
{
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)  //空闲中断中断
	{
		//清零标志位方法 1.读入 USART_SR 寄存器；2.读入 USART_DR 寄存器
		USART1->SR;
		USART1->DR;		
		U1_CB.Counter += (U1_RX_MAX + 1 -DMA_GetCurrDataCounter(DMA2_Stream2));//记录这次收到的数据量【这次DMA的传输量】
		U1_CB.URxDataIN->end = &U1_RxBuff[U1_CB.Counter-1];										 //指向第一个数据块的结束位置
		U1_CB.URxDataIN++;																										 //每接收完一个数据块，指针自增
		if(U1_CB.URxDataIN == U1_CB.URxDataEND)
		{
			U1_CB.URxDataIN = &U1_CB.URxDataPtr[0];																//如果到了最后一个数据块了，重新指回第一个数据块 	 	
		}
		if( (U1_RX_SIZE - U1_CB.Counter) >= U1_RX_MAX )													//判断缓冲区剩余大小是否支持一次的最大搬运量
		{
			U1_CB.URxDataIN->start = &U1_RxBuff[U1_CB.Counter];	
		}
		else
		{
			U1_CB.URxDataIN->start = U1_RxBuff;																		//大小不够，指针回到缓冲区开始
			U1_CB.Counter = 0;
		}
		MYDMA_Config(DMA2_Stream2, DMA_Channel_4, (u32)&USART1->DR, (u32)U1_CB.URxDataIN->start, (U1_RX_MAX+1));		//重新设置DMA通道
		MYDMA_Enable(DMA2_Stream2);	//开启DMA通道  
	} 
} 



