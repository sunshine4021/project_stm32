/*******************************************************

* �ļ�����my_log_main.h

* ���ߣ�  luyangguang

* ������ �����ռ����

* �޸��ˣ���¬���⡵

* �޸�ʱ�䣺2021-04-24

* �޸����ݣ�����
*******************************************************/

/*******************  ͷ�ļ����� ***********************/
/******************************************************/
#include "my_log_main.h"

#include "stdlib.h"
#include "string.h"
#include "stdarg.h"
#include "stdio.h"

#include "my_log_main.h"
#include "my_drv_task.h"
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

/******************************************************************************
 * Function  -  my_log_send_to_uart
 * Purpose   -  ������Ϣ��drv��������ݷ��ͳ�ȥ
 * Params 
 *   @uint8_t *data  ���ݻ�����
 *   @uint16_t data_len ���ݵĳ���
 * Return
 *   @  
 * Limitation -   
 * ----------------------------------------
 * Modification history
 * luyangguang create  @2022-04-10
 ******************************************************************************/
static void my_log_send_to_uart(uint8_t *data , uint16_t data_len)
{
		my_task_message_struct message = {0};
		uint8_t *send_data = NULL;
		
		send_data = (uint8_t *) pvPortMalloc(data_len + 1);
		
		memset(send_data,0,data_len + 1);
		memcpy(send_data,data,data_len);
						
		message.mes_id = MY_DRV_UART_DATA_SEND;
		message.data_len = data_len;
		message.pvdata = send_data;
		message.param_1 = 1;
		
		xQueueSendToBack(my_drv_task_get_queuue_handle(),&message,0);

		//����ʧ��Ҫ�жϵģ�Ȼ���ͷ���Դ

}
	

/******************************************************************************
 * Function  -  my_pre_log
 * Purpose   -  ��ʽ��LOG��ӡ���
 * Params 
 *   @ void
 * Return
 *   @  
 * Limitation -   
 * ----------------------------------------
 * Modification history
 * luyangguang create  @2022-04-10
 ******************************************************************************/
void my_pre_log(const char *prefix,const char *format,...)
{
		char line[1024] = {0};	
	  va_list args;
		strncpy(line,prefix,strlen(prefix));
	  va_start(args,format);
		vsnprintf(line + strlen(line),1024 - strlen(line),format,args);	
	  va_end(args);
		my_log_send_to_uart((uint8_t *)line,strlen(line));		
}




