/**
	************************************************************
	************************************************************
	************************************************************
	*	文件名： 	spi.c
	*
	*	作者： 		张继瑞
	*
	*	日期： 		2017-02-27
	*
	*	版本： 		V1.0
	*
	*	说明： 		SPI驱动
	*
	*	修改记录：	
	************************************************************
	************************************************************
	************************************************************
**/

//单片机相关组件
#include "my_mcu_spi.h"
#include "my_mcu_dma.h"

//硬件驱动
#include "my_spi.h"


/*
SPI1:	NSS---PA4
		SCK---PA5
		MISO---PA6
		MOSI---PA7

SPI2:	NSS---PB12
		SCK---PB13
		MISO---PB14
		MOSI---PB15
*/

/*
************************************************************
*	函数名称：	SPIx_Init
*
*	函数功能：	SPIx初始化
*
*	入口参数：	SPIx：SPI1或者SPI2
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void SPIx_Init(SPI_TypeDef *SPIx)
{
	
	MCU_SPIx_Init(SPIx, SPI_Direction_2Lines_FullDuplex, SPI_Mode_Master, SPI_DataSize_8b,
					SPI_CPOL_High, SPI_CPHA_2Edge, SPI_NSS_Soft, SPI_BaudRatePrescaler_2, SPI_FirstBit_MSB, 7);
 
	SPI_Cmd(SPIx, ENABLE);												//使能SPI外设
	
#if(SPI_DMA_TX_EN == 1)
	MCU_DMA_Init(DMA1, DMA1_Channel3, (unsigned int)&SPIx->DR, 0, DMA_DIR_PeripheralDST, 0,
					DMA_PeripheralInc_Disable, DMA_MemoryInc_Enable, DMA_PeripheralDataSize_Byte,
					DMA_MemoryDataSize_Byte, DMA_Mode_Normal, DMA_Priority_Medium, DMA_M2M_Disable);
	
	SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, DISABLE);					//失能SPI-DMA
#endif

#if(SPI_DMA_RX_EN == 1)
	MCU_DMA_Init(DMA1, DMA1_Channel2, (unsigned int)&SPIx->DR, 0, DMA_DIR_PeripheralSRC, 0,
					DMA_PeripheralInc_Disable, DMA_MemoryInc_Enable, DMA_PeripheralDataSize_Byte,
					DMA_MemoryDataSize_Byte, DMA_Mode_Normal, DMA_Priority_Medium, DMA_M2M_Disable);
	
	SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Rx, DISABLE);					//失能SPI-DMA
#endif

}

/*
************************************************************
*	函数名称：	SPIx_TX_Init
*
*	函数功能：	SPIx初始化
*
*	入口参数：	SPIx：SPI1或者SPI2
*				addr：内存地址
*				size：缓存大小
*
*	返回参数：	无
*
*	说明：		只发送模式
************************************************************
*/
void SPIx_TX_Init(SPI_TypeDef *SPIx)
{
	
	MCU_SPIx_Init(SPIx, SPI_Direction_1Line_Tx, SPI_Mode_Master, SPI_DataSize_8b,
					SPI_CPOL_High, SPI_CPHA_2Edge, SPI_NSS_Soft, SPI_BaudRatePrescaler_2, SPI_FirstBit_MSB, 7);
 
	SPI_Cmd(SPIx, ENABLE);												//使能SPI外设
	
#if(SPI_DIS_BUFFER_EN == 1)
	MCU_DMA_Init(DMA1, DMA1_Channel5, (unsigned int)&SPIx->DR, 0, DMA_DIR_PeripheralDST, 0,
					DMA_PeripheralInc_Disable, DMA_MemoryInc_Enable, DMA_PeripheralDataSize_Byte,
					DMA_MemoryDataSize_Byte, DMA_Mode_Circular, DMA_Priority_Medium, DMA_M2M_Disable);
	
	SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, DISABLE);					//失能SPI-DMA
#endif

}

/*
************************************************************
*	函数名称：	SPIx_ResetMemoryBaseAddr
*
*	函数功能：	重设DMA内存地址并使能发送
*
*	入口参数：	SPIx：SPI组
*				mAddr：内存地址值
*				num：本次发送的数据长度(字节)
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void SPIx_ResetMemoryBaseAddr(SPI_TypeDef *SPIx, unsigned int mAddr, unsigned short num, _Bool type)
{

#if(SPI_DMA_TX_EN == 1)
	if(type == SPI_TX_TYPE)
	{
		if(SPIx == SPI1)
			MCU_DMA_ResetMemoryBaseAddr(DMA1_Channel3, mAddr, num);
		else if(SPIx == SPI2)
			MCU_DMA_ResetMemoryBaseAddr(DMA1_Channel5, mAddr, num);
	}
#endif
	
#if(SPI_DMA_RX_EN == 1)
	if(type == SPI_RX_TYPE)
	{
		if(SPIx == SPI1)
			MCU_DMA_ResetMemoryBaseAddr(DMA1_Channel2, mAddr, num);
		else if(SPIx == SPI2)
			MCU_DMA_ResetMemoryBaseAddr(DMA1_Channel4, mAddr, num);
	}
#endif

}

/*
************************************************************
*	函数名称：	SPIx_SetSpeed
*
*	函数功能：	SPI 速度设置函数
*
*	入口参数：	SPIx：SPI1或者SPI2
*				speed：速度，见说明
*
*	返回参数：	无
*
*	说明：		SPI_BaudRatePrescaler_2   2分频   (SPI 36M@sys 72M)
*				SPI_BaudRatePrescaler_8   8分频   (SPI 9M@sys 72M)
*				SPI_BaudRatePrescaler_16  16分频  (SPI 4.5M@sys 72M)
*				SPI_BaudRatePrescaler_256 256分频 (SPI 281.25K@sys 72M)
************************************************************
*/
void SPIx_SetSpeed(SPI_TypeDef *SPIx, unsigned char speed)
{
	
	
	
}

/*
************************************************************
*	函数名称：	SPIx_ReadWriteByte
*
*	函数功能：	读写一个字节
*
*	入口参数：	SPIx：SPI1或者SPI2
*				TxData:要写入的字节
*
*	返回参数：	读取到的字节
*
*	说明：		
************************************************************
*/
unsigned char SPIx_ReadWriteByte(SPI_TypeDef *SPIx, unsigned char TxData)
{
	
	unsigned char retry = 0;
	
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET)	//检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		retry++;
		if(retry > 200)
			return 0;
	}
	
	SPI_I2S_SendData(SPIx, TxData);									//通过外设SPIx发送一个数据
	retry = 0;

	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET)	//检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
		retry++;
		if(retry > 200)
			return 0;
	}
	
	return SPI_I2S_ReceiveData(SPIx);								//返回通过SPIx最近接收的数据
		
}

/*
************************************************************
*	函数名称：	SPIx_DMA_ReadWriteByte
*
*	函数功能：	读写一个字节
*
*	入口参数：	SPIx：SPI1或者SPI2
*				TxData:要写入的字节
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void SPIx_DMA_ReadWriteByte(SPI_TypeDef *SPIx, unsigned char *tx_buf, unsigned char *rx_buf, unsigned short tx_size, unsigned short rx_size)
{
	
	SPIx_ResetMemoryBaseAddr(SPIx, (unsigned int)tx_buf, tx_size, SPI_TX_TYPE);
	SPIx_ResetMemoryBaseAddr(SPIx, (unsigned int)rx_buf, rx_size, SPI_RX_TYPE);
	
	SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, ENABLE);
	SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Rx, ENABLE);
	
	if(SPIx == SPI1)										//等待接收完成
	{
		while(DMA_GetFlagStatus(DMA1_IT_TC2) == RESET);
		DMA_ClearFlag(DMA1_IT_TC2);
	}
	else if(SPIx == SPI2)
	{
		while(DMA_GetFlagStatus(DMA1_IT_TC4) == RESET);
		DMA_ClearFlag(DMA1_IT_TC4);
	}
	
	SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, DISABLE);
	SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Rx, DISABLE);
		
}

/*
************************************************************
*	函数名称：	SPIx_WriteByte
*
*	函数功能：	写一个字节
*
*	入口参数：	SPIx：SPI1或者SPI2
*				TxData:要写入的字节
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void SPIx_WriteByte(SPI_TypeDef *SPIx, unsigned char TxData)
{
	
	SPI_I2S_SendData(SPIx, TxData);									//通过外设SPIx发送一个数据
	
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);	//检查指定的SPI标志位设置与否:发送缓存空标志位
		
}
