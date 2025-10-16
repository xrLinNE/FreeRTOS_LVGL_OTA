#include "ShowTime_task.h"

//任务句柄
extern TaskHandle_t xShowMenuTaskHandle;
extern TaskHandle_t xShowTimeTaskHandle;
extern TaskHandle_t xShowWoodenFishTaskHandle;
extern TaskHandle_t xShowFlashLightTaskHandle;
extern TaskHandle_t xShowSettingTaskHandle;
extern TaskHandle_t xShowCalendarTaskHandle;
extern TaskHandle_t xShowClockTaskHandle;
extern TaskHandle_t xShowDHT11TaskHandle;
//队列
extern QueueHandle_t g_xQueueMenu;

/* some data */
#define BOX_R 1
uint8_t time_flag = 0;

uint8_t sec_unit, sec_decade, min_unit, min_decade, hour_unit, hour_decade;
typedef struct Time_param{
    int x[4];
	int y;
	int w;
	int h;
	int x_arg;
}T;
T time = { {8, 35, 71, 98}, 15, 20, 40, 98};
//Image Box1 = {62, 22, 4, 4,};
//Image Box2 = {62, 39, 4, 4,};

void ShowTimeTask(void *params)
{
	//xSemaphoreTake(g_xSemTicks, portMAX_DELAY);

	/* suspend_other_task */
	vTaskSuspend(xShowMenuTaskHandle);
	//vTaskSuspend(xShowWoodenFishTaskHandle);
	vTaskSuspend(xShowFlashLightTaskHandle);
	vTaskSuspend(xShowSettingTaskHandle);
	vTaskSuspend(xShowClockTaskHandle);
	vTaskSuspend(xShowCalendarTaskHandle);
	vTaskSuspend(xShowDHT11TaskHandle);

	//创建队列
	g_xQueueMenu = xQueueCreate(1, 4);//队列长度1，队列大小 1*sizeof(Key_data) = 4字节
	/* u8g2 Start */
//	u8g2_t u8g2;
//	u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2,U8G2_R0, u8x8_byte_hw_i2c, u8g2_stm32_delay);
//	u8g2_InitDisplay(&u8g2); // send init sequence to the display, display is in sleep mode after this,
//	u8g2_SetPowerSave(&u8g2, 0); // wake up display
//	u8g2_ClearDisplay(&u8g2);
////	u8g2_SetFont(&u8g2, u8g2_font_wqy16_t_chinese1);
//	u8g2_SetFont(&u8g2, u8g2_font_fur35_tf);
	
	/* receive queue */
	 Key_data	key_data;

	while(1)
	{	
//		u8g2_ClearBuffer(&u8g2);
//		
//		/* draw */
//		u8g2_DrawXBMP(&u8g2, 0, 0, 23, 10, ShowPower);
//		u8g2_DrawXBMP(&u8g2, 105, 0, 23, 10, ShowGame);
//		/* draw time */
//		u8g2_DrawXBMP(&u8g2, time.x[3], time.y, time.w, time.h, BigNum[sec_unit]);
//		u8g2_DrawXBMP(&u8g2, time.x[2], time.y, time.w, time.h, BigNum[sec_decade]);
//		u8g2_DrawRBox(&u8g2, Box1.x, Box1.y, Box1.w, Box1.h, BOX_R);
//		u8g2_DrawRBox(&u8g2, Box2.x, Box2.y, Box2.w, Box2.h, BOX_R);		
//		u8g2_DrawXBMP(&u8g2, time.x[1], time.y, time.w, time.h, BigNum[min_unit]);
//		u8g2_DrawXBMP(&u8g2, time.x[0], time.y, time.w, time.h, BigNum[min_decade]);

//		u8g2_DrawXBMP(&u8g2, 56, 2, 6, 8, Num_6x8[hour_decade]);

//		u8g2_DrawXBMP(&u8g2, 66, 2, 6, 8, Num_6x8[hour_unit]);
//				
//		u8g2_SendBuffer(&u8g2);

		vTaskDelay(250);
		/* handle queue data */
		if(time_flag == 0)
		{
			//pdPASS == xQueueReceive(g_xQueueMenu, &key_data, 0);
		}
		/* task scheduling */
		if(key_data.updata == 1)
		{		                     
			vTaskResume(xShowMenuTaskHandle);
			vTaskSuspend(NULL);
			key_data.updata = 0;
		}			
	}
}

///******************TimerCallBackFun*******************/
//void TimerCallBackFun(TimerHandle_t xTimer)
//{  
//	/* handle time data */
//	sec_unit++;		
//	if(sec_unit>9){sec_unit = 0; sec_decade++;}			//秒个位
//	if(sec_decade>5){sec_decade = 0; min_unit++;}		//秒十位
//	if(min_unit>9){min_unit = 0; min_decade++;}			//分个位
//	if(min_decade>5){min_decade = 0; hour_unit++;}	//分十位
//	if(hour_unit>5){hour_unit = 0; hour_decade++;}	//时
//}

