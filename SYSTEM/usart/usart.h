#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 

#define U1_RX_SIZE	1024		//接收缓冲区大小
#define U1_RX_MAX		256			//每一次最大接收字节数
#define NUM					10			
typedef struct
{
	uint8_t *start;						//指向接收缓冲区中，每次收到数据的开始位置
	uint8_t *end;							//指向接收缓冲区中，每次收到数据的结束位置
}UCB_URxBuffptr;

typedef struct
{
	uint16_t 				Counter;					//记录每一次接收多少个数据
	UCB_URxBuffptr 	URxDataPtr[NUM];	//缓冲区中每次的数据块
	UCB_URxBuffptr	*URxDataIN;				//每次数据块的起始
	UCB_URxBuffptr	*URxDataOUT;			//通讯结束，收到的最后一个数据块的位置
	UCB_URxBuffptr	*URxDataEND;			//最后一个数据块位置--->定义URxDataPtr[NUM]的最后一个数据块的位置
}UCB_CB;

#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
extern uint8_t U1_RxBuff[U1_RX_SIZE];//接收缓冲区
extern UCB_CB	U1_CB;								//串口控制块
void uart_init(u32 bound);
#endif


