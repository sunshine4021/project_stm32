#ifndef _MY_SPI_H_
#define _MY_SPI_H_


#include "stm32f10x.h"


#define SPI_DMA_TX_EN		1		//1-使能DMA发送		0-失能DMA发送

#define SPI_DMA_RX_EN		1		//1-使能DMA接收		0-失能DMA接收

#define SPI_DIS_BUFFER_EN	0		//1-使能SPI_LCD显存模式		0-失能


#define SPI_TX_TYPE			0

#define SPI_RX_TYPE			1


void SPIx_Init(SPI_TypeDef *SPIx);

void SPIx_TX_Init(SPI_TypeDef *SPIx);

void SPIx_ResetMemoryBaseAddr(SPI_TypeDef *SPIx, unsigned int mAddr, unsigned short num, _Bool type);

void SPIx_SetSpeed(SPI_TypeDef *SPIx, unsigned char SpeedSet);

unsigned char SPIx_ReadWriteByte(SPI_TypeDef *SPIx, unsigned char TxData);

void SPIx_DMA_ReadWriteByte(SPI_TypeDef *SPIx, unsigned char *tx_buf, unsigned char *rx_buf, unsigned short tx_size, unsigned short rx_size);

void SPIx_WriteByte(SPI_TypeDef *SPIx, unsigned char TxData);


#endif
