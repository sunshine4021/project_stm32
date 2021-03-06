#ifndef _SPILCD_H_
#define _SPILCD_H_


#define HW_SPI_EN			1		//1-使用硬件SPI		0-使用软件SPI


#if(HW_SPI_EN == 1)					//使能硬件SPI才能使用显存方式
//注意：	1.使能显存模式，还需要打开 spi.h 12行的宏开关
//		2.使能显存模式，还需要关闭 usart.h 13行的宏开关，因USART1-DMA-RX与其冲突
#define DIS_BUFFER_EN		0		//1-使能显存模式		0-失能显存模式
#endif


#define TFTLCD_X_SIZE		128		//X轴像素

#define TFTLCD_Y_SIZE		128		//Y轴像素


typedef struct
{

	unsigned char blSta;
	
#if(DIS_BUFFER_EN == 1)
	unsigned char dis_buffer[TFTLCD_X_SIZE * TFTLCD_Y_SIZE * 2];
#endif

} SPILCD_INFO;

extern SPILCD_INFO spilcd_info;


#define USE_GUI		0


#define SPILCD_IO	SPI2


#define LCD_CS_H		GPIOB->BSRR = 1UL << 12
#define LCD_CS_L		GPIOB->BSRR = 1UL << 28

#if(HW_SPI_EN == 0)

#define LCD_SCK_H		GPIOB->BSRR = 1UL << 13
#define LCD_SCK_L		GPIOB->BSRR = 1UL << 29

#define LCD_SDA_H		GPIOB->BSRR = 1UL << 15
#define LCD_SDA_L		GPIOB->BSRR = 1UL << 31

#endif

#define LCD_AO_H		GPIOB->BSRR = 1UL << 14
#define LCD_AO_L		GPIOB->BSRR = 1UL << 30

#define LCD_RST_OFF		GPIOB->BSRR = 1UL << 4
#define LCD_RST_ON		GPIOB->BSRR = 1UL << 20

#define LCD_BL_SET(x)	TIM_SetCompare4(TIM3, x)


void SPILCD_Init(void);

void SPILCD_BL_Ctl(unsigned char value);

void SPILCD_BL_Ctl_Auto(void);

void SPILCD_DrawPoint(unsigned short x, unsigned short y, unsigned short color);

void SPILCD_FillRect(unsigned short xStart, unsigned short yStart, unsigned short xEnd, unsigned short yEnd, unsigned short color);

void SPILCD_Draw16BPP(unsigned short x, unsigned short y, const unsigned short *color, const unsigned short *pTrans,
						int xySize, unsigned char mode);


#if(USE_GUI == 0)

/***********************************************************************************************************
************************************************************************************************************


									以下为不使用图形系统时专用的函数


************************************************************************************************************
************************************************************************************************************/

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
#define LIGHTGREEN     	 0X841F //浅绿色
#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

#define BGC WHITE				//背景色


void SPILCD_DrawPic(const unsigned char *pic);

void SPILCD_Clear(unsigned short color);

void SPILCD_DrawLine(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned short color);

void SPILCD_DrawRectangle(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned short color);

void SPILCD_DrawCircle(unsigned short x0, unsigned short y0, unsigned char r, unsigned short color);

void SPILCD_DisString(unsigned short x, unsigned short y, unsigned char size, unsigned short color, unsigned char mode, char *fmt, ...);

void SPILCD_DisZW(unsigned short x, unsigned short y, unsigned short color, const unsigned char *zw);

#endif

#endif
