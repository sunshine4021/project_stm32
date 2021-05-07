/*******************************************************

* �ļ�����my_uart.c

* ���ߣ�  luyangguang

* ������ ����������ʼ��

* �޸��ˣ���¬���⡵

* �޸�ʱ�䣺2021-04-24

* �޸����ݣ�����
*******************************************************/

/*******************  ͷ�ļ����� ***********************/
/******************************************************/

#include "my_uart.h"
#include "my_master_task.h"

/******************************************************/
/*******************  ȫ�ֱ������� *********************/
/******************************************************/

/******************************************************/
/*******************  �������Ͷ��� *********************/
/******************************************************/

/******************************************************/
/*******************  �ⲿ�������� *********************/
/******************************************************/

/******************************************************/
/*******************  ���غ������� *********************/
/******************************************************/


/******************************************************/
/*******************  �ⲿ�������� *********************/
/******************************************************/

 /**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Ƕ�������жϿ�������ѡ�� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* ����USARTΪ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = pal_USART_IRQ;
  /* �������ȼ�*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* �����ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

 /**
  * @brief  USART GPIO ����,������������
  * @param  ��
  * @retval ��
  */
void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// �򿪴���GPIO��ʱ��
	pal_USART_GPIO_APBxClkCmd(pal_USART_GPIO_CLK, ENABLE);
	
	// �򿪴��������ʱ��
	pal_USART_APBxClkCmd(pal_USART_CLK, ENABLE);

	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = pal_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(pal_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  // ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = pal_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(pal_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = pal_USART_BAUDRATE;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(pal_USARTx, &USART_InitStructure);
	
	// �����ж����ȼ�����
	NVIC_Configuration();
	
	// ʹ�ܴ��ڽ����ж�
	USART_ITConfig(pal_USARTx, USART_IT_RXNE, ENABLE);	
	
	// ʹ�ܴ���
	USART_Cmd(pal_USARTx, ENABLE);	    
}

/*****************  ����һ���ֽ� **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/****************** ����8λ������ ************************/
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
  uint8_t i;
	
	for(i=0; i<num; i++)
  {
	    /* ����һ���ֽ����ݵ�USART */
	    Usart_SendByte(pUSARTx,array[i]);	
  
  }
	/* �ȴ�������� */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

/*****************  �����ַ��� **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* �ȴ�������� */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/*****************  ����һ��16λ�� **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	
	/* ȡ���߰�λ */
	temp_h = (ch&0XFF00)>>8;
	/* ȡ���Ͱ�λ */
	temp_l = ch&0XFF;
	
	/* ���͸߰�λ */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
	/* ���͵Ͱ�λ */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}




void my_drv_uart_send_mastter(uint8_t *data,uint16_t data_len)
{
		my_task_message_struct message = {0};
		uint8_t *send_data = NULL;
		
		send_data = (uint8_t *) pvPortMalloc(data_len + 1);
		
		memcpy(send_data,data,data_len);
						
		message.mes_id = MY_MASTER_UART_DATA_RECV;
		message.data_len = data_len;
		message.pvdata = send_data;
		message.param_1 = 1;

		xQueueSendToBackFromISR(my_master_task_get_queuue_handle(),&message,0);
		
}



uint8_t rx_buf[1024];

uint16_t rx_len = 0;

// �����жϷ�����
// �ѽ��յ������ݴ���һ�����黺�������棬�����յ��ĵ�ֵ����0XFFʱ����ֵ����

void pal_USART_IRQHandler(void)
{        

		if(USART_GetITStatus(pal_USARTx,USART_IT_RXNE)!=RESET)
		{        

				USART_ClearITPendingBit(pal_USARTx,USART_IT_RXNE); //����жϱ�־
			
				rx_buf[rx_len] = USART_ReceiveData(pal_USARTx);

				if(( rx_buf[rx_len] == '\n' && rx_buf[rx_len - 1] == '\r') ||  rx_len >=  1024 )
				{
							my_drv_uart_send_mastter(rx_buf,rx_len); 
							rx_len = 0;
				}
				else
				{
							rx_len ++;
				}  

		}         

}





















