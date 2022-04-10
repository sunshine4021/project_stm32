/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	spilcd.c
	*
	*	���ߣ� 		�ż���
	*
	*	���ڣ� 		2017-11-23
	*
	*	�汾�� 		V1.1
	*
	*	˵���� 		SPI TFT-LCD����
	*
	*	�޸ļ�¼��	V1.1�������Դ���ƣ�ͨ��spilcd.h��ĺ����ʹ�ܻ�ʹ��
	************************************************************
	************************************************************
	************************************************************
**/

//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//Ӳ������
#include "spilcd.h"
#include "delay.h"
#include "hwtimer.h"
#include "light.h"
#if(HW_SPI_EN == 1)
#include "spi.h"
#endif

//C��
#include <stdarg.h>
#include <stdio.h>


SPILCD_INFO spilcd_info;


/*
************************************************************
*	�������ƣ�	LCD_SendByte
*
*	�������ܣ�	����һ���ֽ�
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void LCD_SendByte(unsigned char byte)
{

#if(HW_SPI_EN == 0)
	unsigned char i = 0;
	
	for(; i < 8; i++)
	{
		if(byte & 0x80)
		{
			LCD_SDA_H;
		}
		else
		{
			LCD_SDA_L;
		}
		
		LCD_SCK_L;
		LCD_SCK_H;
		
		byte <<= 1;
	}
#else
	SPIx_WriteByte(SPILCD_IO, byte);
#endif

}

void LCD_WriteData(unsigned char data)
{

	LCD_CS_L;
	LCD_AO_H;
	LCD_SendByte(data);
	LCD_CS_H;

}

void LCD_WriteData16(unsigned short data)
{

	LCD_CS_L;
	LCD_AO_H;
	LCD_SendByte(data >> 8);
	LCD_SendByte(data & 0xff);
	LCD_CS_H;

}

void LCD_WriteReg(unsigned char reg)
{

	LCD_CS_L;
	LCD_AO_L;
	LCD_SendByte(reg);
	LCD_CS_H;

}

void LCD_WriteRegData(unsigned char reg, unsigned short data)
{

	LCD_WriteReg(reg);
	LCD_WriteData16(data);

}

/*
************************************************************
*	�������ƣ�	LCD_SetAddress
*
*	�������ܣ�	������ʾ��ַ����
*
*	��ڲ�����	xStart��X����ʼ��ַ
*				yStart��Y����ʼ��ַ
*				xEnd��X�������ַ
*				yEnd��Y�������ַ
*
*	���ز�����	��
*
*	˵����		�ڴ�������д����ʱ����ַ�Զ�����
************************************************************
*/
void LCD_SetAddress(unsigned short xStart, unsigned short yStart, unsigned short xEnd, unsigned short yEnd)
{

	LCD_WriteReg(0x2a);
	LCD_WriteData(xStart >> 8);
	LCD_WriteData(0x00FF & xStart);
	LCD_WriteData(xEnd >> 8);
	LCD_WriteData(0x00FF & xEnd);
  
	LCD_WriteReg(0x2b);
	LCD_WriteData(yStart >> 8);
	LCD_WriteData(0x00FF & yStart);
	LCD_WriteData(yEnd >> 8);
	LCD_WriteData(0x00FF & yEnd);

	LCD_WriteReg(0x2C);

}

void LCD_Rst(void)
{

	LCD_RST_ON;
	DelayXms(100);
	LCD_RST_OFF;
	DelayXms(50);

}

/*
************************************************************
*	�������ƣ�	LCD_Init
*
*	�������ܣ�	LCD��ʼ��
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		PA8-����	PB12-CS		PB14-MISO	PB15-MOSI
*				PC9-RST		PB13-CLK	
************************************************************
*/
void SPILCD_Init(void)
{

	GPIO_InitTypeDef gpio_initstruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);				//��ֹJTAG����
	
	//CS
	gpio_initstruct.GPIO_Mode = GPIO_Mode_Out_PP;
#if(HW_SPI_EN == 1)
	SPIx_TX_Init(SPILCD_IO);
	#if(DIS_BUFFER_EN == 1)
		SPIx_ResetMemoryBaseAddr(SPILCD_IO, (unsigned int)spilcd_info.dis_buffer, sizeof(spilcd_info.dis_buffer), SPI_TX_TYPE);
	#endif
	
	gpio_initstruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_14;
#else
	gpio_initstruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
#endif
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio_initstruct);
	
	//��λ
	gpio_initstruct.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOB, &gpio_initstruct);
	
	//����
	TIM3_PWM_Init(GPIOB, GPIO_Pin_1, 1799, 249);
	
	LCD_Rst();
	
	LCD_WriteReg(0x11);		//sleep out
	DelayXms(120);

	//------------------------------------ST7735S Frame Rate-----------------------------------------// 
	LCD_WriteReg(0xB1); 
	LCD_WriteData(0x05); 
	LCD_WriteData(0x3C); 
	LCD_WriteData(0x3C); 
	LCD_WriteReg(0xB2); 
	LCD_WriteData(0x05); 
	LCD_WriteData(0x3C); 
	LCD_WriteData(0x3C); 
	LCD_WriteReg(0xB3); 
	LCD_WriteData(0x05); 
	LCD_WriteData(0x3C); 
	LCD_WriteData(0x3C); 
	LCD_WriteData(0x05); 
	LCD_WriteData(0x3C); 
	LCD_WriteData(0x3C); 
	//------------------------------------End ST7735S Frame Rate-----------------------------------------// 
	LCD_WriteReg(0xB4); //Dot inversion 
	LCD_WriteData(0x03); 

	//------------------------------------ST7735S Power Sequence-----------------------------------------// 
	LCD_WriteReg(0xC0); 
	LCD_WriteData(0x28); 
	LCD_WriteData(0x08); 
	LCD_WriteData(0x04); 
	LCD_WriteReg(0xC1); 
	LCD_WriteData(0XC0); 
	LCD_WriteReg(0xC2); 
	LCD_WriteData(0x0D); 
	LCD_WriteData(0x00); 
	LCD_WriteReg(0xC3); 
	LCD_WriteData(0x8D); 
	LCD_WriteData(0x2A); 
	LCD_WriteReg(0xC4); 
	LCD_WriteData(0x8D); 
	LCD_WriteData(0xEE); 
	//---------------------------------End ST7735S Power Sequence-------------------------------------// 
	 
	LCD_WriteReg(0xC5); //VCOM 
	LCD_WriteData(0x1A); 
	 
	LCD_WriteReg(0x36); //MX, MY, RGB mode 
	LCD_WriteData(0x08); 
	//------------------------------------ST7735S Gamma Sequence-----------------------------------------// 
	 
	LCD_WriteReg(0xE0); 
	LCD_WriteData(0x04); 
	LCD_WriteData(0x22); 
	LCD_WriteData(0x07); 
	LCD_WriteData(0x0A); 
	LCD_WriteData(0x2E); 
	LCD_WriteData(0x30); 
	LCD_WriteData(0x25); 
	LCD_WriteData(0x2A); 
	LCD_WriteData(0x28); 
	LCD_WriteData(0x26); 
	LCD_WriteData(0x2E); 
	LCD_WriteData(0x3A); 
	LCD_WriteData(0x00); 

	LCD_WriteData(0x01); 
	LCD_WriteData(0x03); 
	LCD_WriteData(0x13); 
	LCD_WriteReg(0xE1); 
	LCD_WriteData(0x04); 
	LCD_WriteData(0x16); 
	LCD_WriteData(0x06); 
	LCD_WriteData(0x0D); 
	LCD_WriteData(0x2D); 
	LCD_WriteData(0x26); 
	LCD_WriteData(0x23); 
	LCD_WriteData(0x27); 
	LCD_WriteData(0x27); 
	LCD_WriteData(0x25); 
	LCD_WriteData(0x2D); 
	LCD_WriteData(0x3B); 
	LCD_WriteData(0x00); 
	LCD_WriteData(0x01); 
	LCD_WriteData(0x04); 
	LCD_WriteData(0x13); 
	//------------------------------------End ST7735S Gamma Sequence-----------------------------------------// 

	LCD_WriteReg(0x3A); //65k mode 
	LCD_WriteData(0x05); 
	LCD_WriteReg(0x29); //Display on 
	LCD_WriteReg(0x2c);
	
#if(DIS_BUFFER_EN == 1)
	LCD_SetAddress(0, 0, TFTLCD_X_SIZE - 1, TFTLCD_Y_SIZE - 1);
	
	DMA_Cmd(DMA1_Channel5, ENABLE);											//ʹ��DMAͨ��
	SPI_I2S_DMACmd(SPILCD_IO, SPI_I2S_DMAReq_Tx, ENABLE);
	
	LCD_CS_L;
	LCD_AO_H;
#endif
	
	SPILCD_BL_Ctl(250);

}

void SPILCD_BL_Ctl(unsigned char value)
{

	if(value > 250)
		value = 250;
	
	LCD_BL_SET(value);
	
	spilcd_info.blSta = value;

}

/*
************************************************************
*	�������ƣ�	SPILCD_BL_Ctl_Auto
*
*	�������ܣ�	���ݻ�����ǿ�Զ����ȵ���
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void SPILCD_BL_Ctl_Auto(void)
{

	unsigned char count = 0;
	unsigned char pwm = 0;
	unsigned char temp = 0;
	
	pwm = 75 * light_info.voltag;		//���������������

	if(spilcd_info.blSta > pwm)			//��С����
	{
		temp = spilcd_info.blSta - pwm;
		
		if(temp > 15)					//������15���ڵĲ�����
		{
			for(count = 0; count < temp; count++)	//��һ���������ȵĽ������
			{
				SPILCD_BL_Ctl(spilcd_info.blSta);	//����һ�ε�ֵ��ʼ��������
				if(--spilcd_info.blSta <= 5)		//���С�����������������
					break;
					
				RTOS_TimeDly(3);
			}
		}
	}
	else if(spilcd_info.blSta < pwm)	//��������
	{
		temp = pwm - spilcd_info.blSta;
		
		if(temp > 15)					//������15���ڵĲ�����
		{
			for(count = 0; count < temp; count++)	//��һ���������ȵĽ������
			{
				SPILCD_BL_Ctl(spilcd_info.blSta);	//����һ�ε�ֵ��ʼ��������
				if(++spilcd_info.blSta >= 250)		//��������������������
					break;
				
				RTOS_TimeDly(3);
			}
		}
	}
	
	SPILCD_BL_Ctl(spilcd_info.blSta);

}

/*
************************************************************
*	�������ƣ�	LCD_DrawPoint
*
*	�������ܣ�	����
*
*	��ڲ�����	x��X��λ��
*				y��Y��λ��
*				color����ɫֵ
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void SPILCD_DrawPoint(unsigned short x, unsigned short y, unsigned short color)
{
	
	if(x < TFTLCD_X_SIZE && y < TFTLCD_Y_SIZE)
	{
#if(DIS_BUFFER_EN == 1)
		unsigned short pos = (y * TFTLCD_X_SIZE + x) << 1;	//������ʾ�����Դ��е�λ��	����1����Ϊ�Դ���uchar������16λɫ
		
		spilcd_info.dis_buffer[pos] = color >> 8;
		spilcd_info.dis_buffer[pos + 1] = color & 0x00ff;
#else
		LCD_SetAddress(x, y, x, y);
		LCD_WriteData16(color);
#endif
	}

}

/*
************************************************************
*	�������ƣ�	SPILCD_FillRect
*
*	�������ܣ�	��Rect
*
*	��ڲ�����	xStart��X����ʼ��ַ
*				yStart��Y����ʼ��ַ
*				xEnd��X�������ַ
*				yEnd��Y�������ַ
*				color����ɫֵ
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void SPILCD_FillRect(unsigned short xStart, unsigned short yStart, unsigned short xEnd, unsigned short yEnd, unsigned short color)
{
	
	if(xEnd < TFTLCD_X_SIZE && yEnd < TFTLCD_Y_SIZE)
	{
#if(DIS_BUFFER_EN == 1)
		unsigned short pos = 0, i = 0, j = 0;
		
		for(i = yStart; i < yEnd; i++)
		{
			pos = ((yStart + i - xStart) * TFTLCD_X_SIZE + xStart) << 1;	//�����μ�����ε�һ��ÿ�������Դ����λ��
			
			for(j = xStart; j < xEnd; j++, pos++)
			{
				spilcd_info.dis_buffer[pos] = color >> 8;
				spilcd_info.dis_buffer[++pos] = color & 0x00ff;
			}
		}
#else
		unsigned short i = 0, j = 0;
		
		LCD_SetAddress(xStart, yStart, xEnd, yEnd);
		
		for(; i <= yEnd; i++)
		{
			for(j = 0; j <= xEnd; j++)
			{
				LCD_WriteData16(color);
			}
		}
#endif
	}

}

/*
************************************************************
*	�������ƣ�	SPILCD_Draw16BPP
*
*	�������ܣ�	��ͼ
*
*	��ڲ�����	x��X��λ��
*				y��Y��λ��
*				color����ɫֵ
*				xySize���������
*
*	���ز�����	��
*
*	˵����		ר��GUIʹ��
************************************************************
*/
void SPILCD_Draw16BPP(unsigned short x, unsigned short y, const unsigned short *color, const unsigned short *pTrans,
						int xySize, unsigned char mode)
{
	
#if(DIS_BUFFER_EN == 1)
	unsigned short pos = 0;
#else
	LCD_SetAddress(x, y, x + xySize, y);
#endif
	
	switch(mode)
	{
		case 0:
			
#if(DIS_BUFFER_EN == 1)
			pos = (y * TFTLCD_X_SIZE + x) << 1;
		
			for(; xySize > 0; xySize--, pos++)
			{
				spilcd_info.dis_buffer[pos] = *(*color + pTrans) >> 8;
				spilcd_info.dis_buffer[++pos] = *(*color + pTrans) & 0x00ff;
				color++;
			}
#else
			for(; xySize > 0; xySize--)
				LCD_WriteData16(*(*color++ + pTrans));
#endif
		
		break;
		
		case 1:
			
#if(DIS_BUFFER_EN == 1)
			pos = (y * TFTLCD_X_SIZE + x) << 1;
		
			for(; xySize > 0; xySize--, pos++)
			{
				spilcd_info.dis_buffer[pos] = *color >> 8;
				spilcd_info.dis_buffer[++pos] = *color & 0x00ff;
				color++;
			}
#else
			for(; xySize > 0; xySize--)
				LCD_WriteData16(*color++);
#endif
		
		break;
		
		case 2:
			
#if(DIS_BUFFER_EN == 1)
			pos = (y * TFTLCD_X_SIZE + x) << 1;
		
			for(; xySize > 0; xySize--, pos++)
			{
				if(*color)
				{
					spilcd_info.dis_buffer[pos] = *(*color + pTrans) >> 8;
					spilcd_info.dis_buffer[++pos] = *(*color + pTrans) & 0x00ff;
				}
				color++;
			}
#else
			for(; xySize > 0; xySize--)
			{
				if(*color)
					LCD_WriteData16(*(*color + pTrans));
				
				color++;
			}
#endif
		
		break;
			
		case 3:
			
#if(DIS_BUFFER_EN == 1)
			pos = (y * TFTLCD_X_SIZE + x) << 1;
		
			for(; xySize > 0; xySize--, pos++)
			{
				if(*color)
				{
					spilcd_info.dis_buffer[pos] = *color >> 8;
					spilcd_info.dis_buffer[++pos] = *color & 0x00ff;
				}
				color++;
			}
#else
			for(; xySize > 0; xySize--)
			{
				if(*color)
					LCD_WriteData16(*color);
				
				color++;
			}
#endif
		
		break;
	}

}

#if(USE_GUI == 0)

/***********************************************************************************************************
************************************************************************************************************


									����Ϊ��ʹ��ͼ��ϵͳʱר�õĺ���


************************************************************************************************************
************************************************************************************************************/


/*
************************************************************
*	�������ƣ�	LCD_Clear
*
*	�������ܣ�	�����Ļ
*
*	��ڲ�����	color����ɫֵ
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void SPILCD_Clear(unsigned short color)
{

#if(DIS_BUFFER_EN == 1)
	unsigned short i = 0;
	
	for(; i < sizeof(spilcd_info.dis_buffer); i++)
	{
		spilcd_info.dis_buffer[i] = color >> 8;
		spilcd_info.dis_buffer[++i] = color & 0x00ff;
	}
#else
	unsigned short i = 0, j = 0;
	
	LCD_SetAddress(0, 0, TFTLCD_X_SIZE - 1, TFTLCD_Y_SIZE - 1);
	
	for(; i < TFTLCD_X_SIZE; i++)
	{
		for(j = 0; j < TFTLCD_Y_SIZE; j++)
		{
			LCD_WriteData16(color);
		}
	}
#endif

}

/*
************************************************************
*	�������ƣ�	SPILCD_DrawLine
*
*	�������ܣ�	����Ļ�ϻ�һ����
*
*	��ڲ�����	x1��X����ʼ��
*				y1��Y����ʼ��
*				x2��X�������
*				y2��Y�������
*				color����ɫֵ
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void SPILCD_DrawLine(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned short color)
{
	
	unsigned short t;
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy, uRow, uCol;
	
	delta_x = x2 - x1;										//������������
	delta_y = y2 - y1;
	uRow = x1;
	uCol = y1;

	if(delta_x > 0)
		incx = 1;											//���õ�������
	else if(delta_x == 0)
		incx = 0;											//��ֱ��
	else
	{
		incx = -1;
		delta_x = -delta_x;
	}

	if(delta_y > 0)
		incy = 1;
	else if(delta_y == 0)
		incy = 0;											//ˮƽ��
	else
	{
		incy = -1;
		delta_y = -delta_y;
	}

	if(delta_x > delta_y)
		distance = delta_x;									//ѡȡ��������������
	else
		distance = delta_y;

	for(t = 0; t <= distance + 1; t++)						//�������
	{
		SPILCD_DrawPoint(uRow, uCol, color);				//����
		xerr += delta_x ;
		yerr += delta_y ;

		if(xerr > distance)
		{
			xerr -= distance;
			uRow += incx;
		}

		if(yerr > distance)
		{
			yerr -= distance;
			uCol += incy;
		}
	}
	
}

/*
************************************************************
*	�������ƣ�	SPILCD_DrawRectangle
*
*	�������ܣ�	����Ļ�ϻ�һ�����ľ���
*
*	��ڲ�����	x1��X����ʼ��
*				y1��Y����ʼ��
*				x2��X�������
*				y2��Y�������
*				color����ɫֵ
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void SPILCD_DrawRectangle(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned short color)
{
	
	SPILCD_DrawLine(x1, y1, x2, y1, color);
	SPILCD_DrawLine(x1, y1, x1, y2, color);
	SPILCD_DrawLine(x1, y2, x2, y2, color);
	SPILCD_DrawLine(x2, y1, x2, y2, color);
	
}

/*
************************************************************
*	�������ƣ�	SPILCD_DrawCircle
*
*	�������ܣ�	����Ļ�ϻ�һ��Բ��
*
*	��ڲ�����	x0��Բ��X������
*				y0��Բ��Y������
*				r���뾶
*				color����ɫֵ
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void SPILCD_DrawCircle(unsigned short x0, unsigned short y0, unsigned char r, unsigned short color)
{
	
	int a, b;
	int di;
	
	a = 0;
	b = r;
	di = 3 - (r << 1);       //�ж��¸���λ�õı�־

	while(a <= b)
	{
		SPILCD_DrawPoint(x0 + a, y0 - b, color);        //5
		SPILCD_DrawPoint(x0 + b, y0 - a, color);        //0
		SPILCD_DrawPoint(x0 + b, y0 + a, color);        //4
		SPILCD_DrawPoint(x0 + a, y0 + b, color);        //6
		SPILCD_DrawPoint(x0 - a, y0 + b, color);        //1
		SPILCD_DrawPoint(x0 - b, y0 + a, color);
		SPILCD_DrawPoint(x0 - a, y0 - b, color);        //2
		SPILCD_DrawPoint(x0 - b, y0 - a, color);        //7
		a++;

		//ʹ��Bresenham�㷨��Բ
		if(di < 0)
			di += 4 * a + 6;
		else
		{
			di += 10 + 4 * (a - b);
			b--;
		}
	}
}

/*
************************************************************
*	�������ƣ�	SPILCD_DrawPic
*
*	�������ܣ�	��ͼ
*
*	��ڲ�����	pic������ͼƬ��ָ�뻭ͼ
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void SPILCD_DrawPic(const unsigned char *pic)
{

#if(DIS_BUFFER_EN == 1)
	unsigned short i = 0;
	
	for(; i < sizeof(spilcd_info.dis_buffer); i += 2)
	{
		spilcd_info.dis_buffer[i + 1] = *pic++;
		spilcd_info.dis_buffer[i] = *pic++;
	}
#else
	unsigned short i = 0, j = 0;
	
	LCD_SetAddress(0, 0, TFTLCD_X_SIZE - 1, TFTLCD_Y_SIZE - 1);
	
	for(; i < TFTLCD_Y_SIZE; i++)
	{
		for(j = 0; j < TFTLCD_X_SIZE; j++)
		{
			LCD_WriteData16(*pic | (unsigned short)*(++pic) << 8);
			
			pic++;
		}
	}
#endif

}

extern const unsigned char asc2_1206[95][12];
extern const unsigned char asc2_1608[95][16];
extern const unsigned char asc2_2412[95][36];
/*
************************************************************
*	�������ƣ�	SPILCD_DisChar
*
*	�������ܣ�	��ʾһ��ASCII�ַ�
*
*	��ڲ�����	x��X����
*				y��Y����
*				size���ַ��ߴ�
*				color����ʾ����ɫ
*				mode��0-������ʾ		1-��������ʾ
*				Char���ַ�����
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void SPILCD_DisChar(unsigned short x, unsigned short y, unsigned char size, unsigned short color, _Bool mode, unsigned char Char)    //����ʾ��̬����  һ���ַ�ռ��12���ؿ�  24���ظ�
{

	unsigned char temp, t1, t;
	unsigned short y0 = y;
	unsigned char csize = ((size >> 3) + ((size % 8) ? 1 : 0)) * (size >> 1);	//�õ�����һ���ַ���Ӧ������ռ���ֽ���  12  16��û��  24��36

	Char -= 32;//�õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩

	for(t = 0; t < csize; t++)
	{
		if(size == 12)
			temp = asc2_1206[Char][t]; 	 //����1206����
		else if(size == 16)
			temp = asc2_1608[Char][t];	//����1608����
		else if(size == 24)
			temp = asc2_2412[Char][t];	//����2412����
		else
			return;						//û�е��ֿ�

		for(t1 = 0; t1 < 8; t1++)
		{
			if(mode)
				SPILCD_DrawPoint(x, y, BGC);    //д����ɫ����

			if(temp & 0x80)
				SPILCD_DrawPoint(x, y, color);

			temp <<= 1;
			y++;

			if((y - y0) == size)
			{
				y = y0;
				x++;
				break;
			}
		}
	}

}

/*
************************************************************
*	�������ƣ�	SPILCD_DisString
*
*	�������ܣ�	��ʾһ���ַ���
*
*	��ڲ�����	x��X����
*				y��Y����
*				size���ַ��ߴ�
*				color����ʾ����ɫ
*				mode��0-������ʾ		1-��������ʾ
*				...
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void SPILCD_DisString(unsigned short x, unsigned short y, unsigned char size, unsigned short color, unsigned char mode, char *fmt, ...)
{

	char LCD_Printf_Buf[256];
	va_list ap;
	char *pStr = LCD_Printf_Buf;

	va_start(ap, fmt);
	vsprintf(LCD_Printf_Buf, fmt, ap);
	va_end(ap);

	while(*pStr != 0)
	{
		SPILCD_DisChar(x, y, size, color, mode, *pStr++);

		x += size >> 1;
	}

}

/*
************************************************************
*	�������ƣ�	SPILCD_DisZW
*
*	�������ܣ�	��ʾһ���ֺ���
*
*	��ڲ�����	x��X����
*				y��Y����
*				color����ɫֵ
*				zw���ֿ�����ָ��
*
*	���ز�����	��
*
*	˵����		�˺���ֻ��ʾ16x16������ȡ���ĺ���(Ϊ�˺�OLEDʹ����ͬ���ֿ�����)
*				��ʾ��ʽ��16x16��2�ţ�ÿ��16�У�ÿ��ÿ��Ϊһ���ֽ�
*
*				B0								B0
*				B1								B1
*				B2 B							B2 B
*				B3 Y							B3 Y
*				B4 T	....................	B4 T
*				B5 E							B5 E
*				B6 0							B6 15
*				B7								B7
*				
*				B0								B0
*				B1								B1
*				B2 B							B2 B
*				B3 Y							B3 Y
*				B4 T	....................	B4 T
*				B5 E							B5 E
*				B6 16							B6 31
*				B7								B7
*
************************************************************
*/
void SPILCD_DisZW(unsigned short x, unsigned short y, unsigned short color, const unsigned char *zw)
{
	
	unsigned char i = 0, j = 0, k = 0;
	unsigned char byte = 0;
	
	for(; i < 2; i++)													//����
	{
		for(j = 0; j < 16; j++)											//����
		{
			byte = zw[j + (i << 4)];									//ȡһ���ֽ���ʾ��LCD�ϣ�16x16���֣���32�ֽ�
			for(k = 0; k < 8; k++)										//һ�������ֽ�����8λ��ÿλ����һ����
			{
				SPILCD_DrawPoint(x + j, y + k + (i << 3), BGC);			//д����ɫ����
				
				if(byte & 0x01)											//�˵������ݣ�����ʾ
					SPILCD_DrawPoint(x + j, y + k + (i << 3), color);
				
				byte >>= 1;												//��λ��ǰ
			}
		}
	}

}

#endif