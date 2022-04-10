/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	spi.c
	*
	*	���ߣ� 		�ż���
	*
	*	���ڣ� 		2017-02-27
	*
	*	�汾�� 		V1.0
	*
	*	˵���� 		SPI����
	*
	*	�޸ļ�¼��	
	************************************************************
	************************************************************
	************************************************************
**/

//��Ƭ��������
#include "my_mcu_spi.h"
#include "my_mcu_dma.h"

//Ӳ������
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
*	�������ƣ�	SPIx_Init
*
*	�������ܣ�	SPIx��ʼ��
*
*	��ڲ�����	SPIx��SPI1����SPI2
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void SPIx_Init(SPI_TypeDef *SPIx)
{
	
	MCU_SPIx_Init(SPIx, SPI_Direction_2Lines_FullDuplex, SPI_Mode_Master, SPI_DataSize_8b,
					SPI_CPOL_High, SPI_CPHA_2Edge, SPI_NSS_Soft, SPI_BaudRatePrescaler_2, SPI_FirstBit_MSB, 7);
 
	SPI_Cmd(SPIx, ENABLE);												//ʹ��SPI����
	
#if(SPI_DMA_TX_EN == 1)
	MCU_DMA_Init(DMA1, DMA1_Channel3, (unsigned int)&SPIx->DR, 0, DMA_DIR_PeripheralDST, 0,
					DMA_PeripheralInc_Disable, DMA_MemoryInc_Enable, DMA_PeripheralDataSize_Byte,
					DMA_MemoryDataSize_Byte, DMA_Mode_Normal, DMA_Priority_Medium, DMA_M2M_Disable);
	
	SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, DISABLE);					//ʧ��SPI-DMA
#endif

#if(SPI_DMA_RX_EN == 1)
	MCU_DMA_Init(DMA1, DMA1_Channel2, (unsigned int)&SPIx->DR, 0, DMA_DIR_PeripheralSRC, 0,
					DMA_PeripheralInc_Disable, DMA_MemoryInc_Enable, DMA_PeripheralDataSize_Byte,
					DMA_MemoryDataSize_Byte, DMA_Mode_Normal, DMA_Priority_Medium, DMA_M2M_Disable);
	
	SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Rx, DISABLE);					//ʧ��SPI-DMA
#endif

}

/*
************************************************************
*	�������ƣ�	SPIx_TX_Init
*
*	�������ܣ�	SPIx��ʼ��
*
*	��ڲ�����	SPIx��SPI1����SPI2
*				addr���ڴ��ַ
*				size�������С
*
*	���ز�����	��
*
*	˵����		ֻ����ģʽ
************************************************************
*/
void SPIx_TX_Init(SPI_TypeDef *SPIx)
{
	
	MCU_SPIx_Init(SPIx, SPI_Direction_1Line_Tx, SPI_Mode_Master, SPI_DataSize_8b,
					SPI_CPOL_High, SPI_CPHA_2Edge, SPI_NSS_Soft, SPI_BaudRatePrescaler_2, SPI_FirstBit_MSB, 7);
 
	SPI_Cmd(SPIx, ENABLE);												//ʹ��SPI����
	
#if(SPI_DIS_BUFFER_EN == 1)
	MCU_DMA_Init(DMA1, DMA1_Channel5, (unsigned int)&SPIx->DR, 0, DMA_DIR_PeripheralDST, 0,
					DMA_PeripheralInc_Disable, DMA_MemoryInc_Enable, DMA_PeripheralDataSize_Byte,
					DMA_MemoryDataSize_Byte, DMA_Mode_Circular, DMA_Priority_Medium, DMA_M2M_Disable);
	
	SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, DISABLE);					//ʧ��SPI-DMA
#endif

}

/*
************************************************************
*	�������ƣ�	SPIx_ResetMemoryBaseAddr
*
*	�������ܣ�	����DMA�ڴ��ַ��ʹ�ܷ���
*
*	��ڲ�����	SPIx��SPI��
*				mAddr���ڴ��ֵַ
*				num�����η��͵����ݳ���(�ֽ�)
*
*	���ز�����	��
*
*	˵����		
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
*	�������ƣ�	SPIx_SetSpeed
*
*	�������ܣ�	SPI �ٶ����ú���
*
*	��ڲ�����	SPIx��SPI1����SPI2
*				speed���ٶȣ���˵��
*
*	���ز�����	��
*
*	˵����		SPI_BaudRatePrescaler_2   2��Ƶ   (SPI 36M@sys 72M)
*				SPI_BaudRatePrescaler_8   8��Ƶ   (SPI 9M@sys 72M)
*				SPI_BaudRatePrescaler_16  16��Ƶ  (SPI 4.5M@sys 72M)
*				SPI_BaudRatePrescaler_256 256��Ƶ (SPI 281.25K@sys 72M)
************************************************************
*/
void SPIx_SetSpeed(SPI_TypeDef *SPIx, unsigned char speed)
{
	
	
	
}

/*
************************************************************
*	�������ƣ�	SPIx_ReadWriteByte
*
*	�������ܣ�	��дһ���ֽ�
*
*	��ڲ�����	SPIx��SPI1����SPI2
*				TxData:Ҫд����ֽ�
*
*	���ز�����	��ȡ�����ֽ�
*
*	˵����		
************************************************************
*/
unsigned char SPIx_ReadWriteByte(SPI_TypeDef *SPIx, unsigned char TxData)
{
	
	unsigned char retry = 0;
	
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET)	//���ָ����SPI��־λ�������:���ͻ���ձ�־λ
	{
		retry++;
		if(retry > 200)
			return 0;
	}
	
	SPI_I2S_SendData(SPIx, TxData);									//ͨ������SPIx����һ������
	retry = 0;

	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET)	//���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
	{
		retry++;
		if(retry > 200)
			return 0;
	}
	
	return SPI_I2S_ReceiveData(SPIx);								//����ͨ��SPIx������յ�����
		
}

/*
************************************************************
*	�������ƣ�	SPIx_DMA_ReadWriteByte
*
*	�������ܣ�	��дһ���ֽ�
*
*	��ڲ�����	SPIx��SPI1����SPI2
*				TxData:Ҫд����ֽ�
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void SPIx_DMA_ReadWriteByte(SPI_TypeDef *SPIx, unsigned char *tx_buf, unsigned char *rx_buf, unsigned short tx_size, unsigned short rx_size)
{
	
	SPIx_ResetMemoryBaseAddr(SPIx, (unsigned int)tx_buf, tx_size, SPI_TX_TYPE);
	SPIx_ResetMemoryBaseAddr(SPIx, (unsigned int)rx_buf, rx_size, SPI_RX_TYPE);
	
	SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, ENABLE);
	SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Rx, ENABLE);
	
	if(SPIx == SPI1)										//�ȴ��������
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
*	�������ƣ�	SPIx_WriteByte
*
*	�������ܣ�	дһ���ֽ�
*
*	��ڲ�����	SPIx��SPI1����SPI2
*				TxData:Ҫд����ֽ�
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void SPIx_WriteByte(SPI_TypeDef *SPIx, unsigned char TxData)
{
	
	SPI_I2S_SendData(SPIx, TxData);									//ͨ������SPIx����һ������
	
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);	//���ָ����SPI��־λ�������:���ͻ���ձ�־λ
		
}
