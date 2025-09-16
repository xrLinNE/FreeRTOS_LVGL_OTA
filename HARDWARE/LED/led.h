#ifndef __LED_H
#define __LED_H
#include "sys.h"

//LED端口
#define LED0_GPIO_PORT                  GPIOF
#define LED0_GPIO_PIN                   GPIO_Pin_9

#define LED1_GPIO_PORT                  GPIOF
#define LED1_GPIO_PIN                   GPIO_Pin_10

//LED开关   
#define LED0(x)   do{ x ? \
                      GPIO_WriteBit(LED0_GPIO_PORT, LED0_GPIO_PIN, Bit_RESET) : \
                      GPIO_WriteBit(LED0_GPIO_PORT, LED0_GPIO_PIN, Bit_SET); \
                  }while(0)   
#define LED1(x)   do{ x ? \
                      GPIO_WriteBit(LED1_GPIO_PORT, LED1_GPIO_PIN, Bit_RESET) : \
                      GPIO_WriteBit(LED1_GPIO_PORT, LED1_GPIO_PIN, Bit_SET); \
                  }while(0) 

/* LED翻转 */
#define LED0_TOGGLE()    do{ GPIO_ToggleBits(LED0_GPIO_PORT, LED0_GPIO_PIN); }while(0)       /* LED0 = !LED0 */
#define LED1_TOGGLE()    do{ GPIO_ToggleBits(LED1_GPIO_PORT, LED1_GPIO_PIN); }while(0)       /* LED1 = !LED1 */

void LED_Init(void);//初始化		

#endif

