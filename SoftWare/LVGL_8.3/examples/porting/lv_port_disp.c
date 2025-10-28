/**
 * @file lv_port_disp_templ.c
 *
 */

/*Copy this file as "lv_port_disp.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_disp.h"
#include <stdbool.h>
//�����Լ���Ļ����
#include "lcd_init.h"
#include "lcd.h"
#include "dma.h"

/*********************
 *      DEFINES
 *********************/
#ifndef MY_DISP_HOR_RES
    //#warning Please define or replace the macro MY_DISP_HOR_RES with the actual screen width, default value 320 is used for now.
    #define MY_DISP_HOR_RES    LCD_W
#endif

#ifndef MY_DISP_VER_RES
    //#warning Please define or replace the macro MY_DISP_HOR_RES with the actual screen height, default value 240 is used for now.
    #define MY_DISP_VER_RES    LCD_H
#endif

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_init(void);

static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);
//static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
//        const lv_area_t * fill_area, lv_color_t color);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
#define	SINGLE_BUFF	0
#define	BUFF_ROWS	40
 #if SINGLE_BUFF
 //单缓冲
static lv_disp_draw_buf_t draw_buf_dsc_1;
static lv_color_t buf_1[MY_DISP_HOR_RES * BUFF_ROWS];                          /*A buffer for 60 rows*/
#else
//双缓冲
static lv_disp_draw_buf_t draw_buf_dsc_1;
static lv_color_t buf_1[MY_DISP_HOR_RES * BUFF_ROWS];                        /*A buffer for 60 rows*/
static lv_color_t buf_2[MY_DISP_HOR_RES * BUFF_ROWS];                        /*An other buffer for 60 rows*/
#endif

void lv_port_disp_init(void)
{	
    /*-------------------------
     * Initialize your display
     * -----------------------*/
		MYDMA_Config1(DMA2_Stream5, DMA_Channel_3, (u32)&SPI1->DR, (uint32_t)buf_1, sizeof(buf_1),1);//16位DMA SPI1，这里配置的内存地址无所谓，最终的地址是在flush中
    disp_init();

    /*-----------------------------
     * Create a buffer for drawing
     *----------------------------*/

    /**
     * LVGL requires a buffer where it internally draws the widgets.
     * Later this buffer will passed to your display driver's `flush_cb` to copy its content to your display.
     * The buffer has to be greater than 1 display row
     *
     * There are 3 buffering configurations:
     * 1. Create ONE buffer:
     *      LVGL will draw the display's content here and writes it to your display
     *
     * 2. Create TWO buffer:
     *      LVGL will draw the display's content to a buffer and writes it your display.
     *      You should use DMA to write the buffer's content to the display.
     *      It will enable LVGL to draw the next part of the screen to the other buffer while
     *      the data is being sent form the first buffer. It makes rendering and flushing parallel.
     *
     * 3. Double buffering
     *      Set 2 screens sized buffers and set disp_drv.full_refresh = 1.
     *      This way LVGL will always provide the whole rendered screen in `flush_cb`
     *      and you only need to change the frame buffer's address.
     */


		#if SINGLE_BUFF
    lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL, MY_DISP_HOR_RES * BUFF_ROWS);   /*Initialize the display buffer*/
		#else
		lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, buf_2, MY_DISP_HOR_RES * BUFF_ROWS);   /*Initialize the display buffer*/
		#endif

//    /* Example for 2) */
//    static lv_disp_draw_buf_t draw_buf_dsc_2;
//    static lv_color_t buf_2_1[MY_DISP_HOR_RES * 10];                        /*A buffer for 10 rows*/
//    static lv_color_t buf_2_2[MY_DISP_HOR_RES * 10];                        /*An other buffer for 10 rows*/
//    lv_disp_draw_buf_init(&draw_buf_dsc_2, buf_2_1, buf_2_2, MY_DISP_HOR_RES * 10);   /*Initialize the display buffer*/

//    /* Example for 3) also set disp_drv.full_refresh = 1 below*/
//    static lv_disp_draw_buf_t draw_buf_dsc_3;
//    static lv_color_t buf_3_1[MY_DISP_HOR_RES * MY_DISP_VER_RES];            /*A screen sized buffer*/
//    static lv_color_t buf_3_2[MY_DISP_HOR_RES * MY_DISP_VER_RES];            /*Another screen sized buffer*/
//    lv_disp_draw_buf_init(&draw_buf_dsc_3, buf_3_1, buf_3_2,
//                          MY_DISP_VER_RES * LV_VER_RES_MAX);   /*Initialize the display buffer*/

    /*-----------------------------------
     * Register the display in LVGL
     *----------------------------------*/

    static lv_disp_drv_t disp_drv;                         /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);                    /*Basic initialization*/

    /*Set up the functions to access to your display*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = MY_DISP_HOR_RES;
    disp_drv.ver_res = MY_DISP_VER_RES;

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = disp_flush;

    /*Set a display buffer*/
    disp_drv.draw_buf = &draw_buf_dsc_1;

    /*Required for Example 3)*/
    //disp_drv.full_refresh = 1;

    /* Fill a memory array with a color if you have GPU.
     * Note that, in lv_conf.h you can enable GPUs that has built-in support in LVGL.
     * But if you have a different GPU you can use with this callback.*/
    //disp_drv.gpu_fill_cb = gpu_fill;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*Initialize your display and the required peripherals.*/
static void disp_init(void)
{
    /*You code here*/
}

volatile bool disp_flush_enabled = true;

/* Enable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_enable_update(void)
{
    disp_flush_enabled = true;
}

/* Disable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_disable_update(void)
{
    disp_flush_enabled = false;
}

/*Flush the content of the internal buffer the specific area on the display
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_disp_flush_ready()' has to be called when finished.*/
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    if(disp_flush_enabled) 
		{
			int16_t w = (area->x2 - area->x1 + 1);
			int16_t h = (area->y2 - area->y1 + 1);
			uint32_t draw_size = w * h;
			uint32_t num_left = draw_size;
			u32 offset = 0;
			u16 batch_size;
			SPI_DataSizeConfig(SPI1, SPI_DataSize_8b);//8位 SPI发送坐标
			LCD_Address_Set(area->x1, area->y1, area->x2, area->y2);
			SPI_DataSizeConfig(SPI1, SPI_DataSize_16b);//16位 SPI 发送DMA数据

			while (num_left > 0)
			{
					if (num_left > 65534)
							batch_size = 65534;
					else
							batch_size = num_left;

					DMA2_Stream5->M0AR = (u32)&color_p[offset];         //寄存器设置内存地址
					SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);		//使能SPI DMA传输
				
					DMA_Cmd(DMA2_Stream5, DISABLE);                     //关闭DMA传输 
					while (DMA_GetCmdStatus(DMA2_Stream5) != DISABLE);	//确保DMA可以被设置
					DMA_SetCurrDataCounter(DMA2_Stream5,batch_size);  	//数据传输量  
					DMA_Cmd(DMA2_Stream5, ENABLE);                      //开启DMA传输 
					
					while (DMA_GetFlagStatus(DMA2_Stream5, DMA_FLAG_TCIF5) == RESET);
					DMA_ClearFlag(DMA2_Stream5, DMA_FLAG_TCIF5);

					offset += batch_size;
					num_left -= batch_size;
			}

			while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
			while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
    }

    /*IMPORTANT!!!
     *Inform the graphics library that you are ready with the flushing*/
    lv_disp_flush_ready(disp_drv);
}

/*OPTIONAL: GPU INTERFACE*/

/*If your MCU has hardware accelerator (GPU) then you can use it to fill a memory with a color*/
//static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
//                    const lv_area_t * fill_area, lv_color_t color)
//{
//    /*It's an example code which should be done by your GPU*/
//    int32_t x, y;
//    dest_buf += dest_width * fill_area->y1; /*Go to the first line*/
//
//    for(y = fill_area->y1; y <= fill_area->y2; y++) {
//        for(x = fill_area->x1; x <= fill_area->x2; x++) {
//            dest_buf[x] = color;
//        }
//        dest_buf+=dest_width;    /*Go to the next line*/
//    }
//}

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
