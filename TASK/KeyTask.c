#include "KeyTask.h"
//队列
extern QueueHandle_t g_xQueueMenu;
//二值信号量
extern SemaphoreHandle_t xKeySems[4];

void KeyTask(void * params )
{
	uint8_t keyIndex = *(uint8_t*)params;
	struct Key_data	key_data;
	
	while(1)
	{					
		// 阻塞等待二值信号量
		if(xSemaphoreTake(xKeySems[keyIndex], portMAX_DELAY) == pdTRUE)
		{
			// 消抖延时
			vTaskDelay(pdMS_TO_TICKS(20));
			// 再次读取按键状态确认按下
			switch(keyIndex)
			{
					case 0: 
						if( KEY1 == 0 ) 
						{
							key_data.ldata = 1;
							xQueueSend(g_xQueueMenu, &key_data, 0);
						}
						break;
					case 1: 
						if( KEY2 == 0 ) 
						{
							key_data.rdata = 1;
							xQueueSend(g_xQueueMenu, &key_data, 0);
						}
						break;
					break;
					case 2: 
						if( KEY3 == 0 ) 
						{
							key_data.updata = 1;
							xQueueSend(g_xQueueMenu, &key_data, 0);
						}
						break;
					case 3: 
						if( KEY4 == 0 ) 
						{
							key_data.exdata = 1;
							xQueueSend(g_xQueueMenu, &key_data, 0);
						}
						break;
					default:
						break;
			}
		}
	}
}


