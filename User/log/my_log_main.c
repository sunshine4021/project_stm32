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

void my_log_uart_send_drv(uint8_t *data,uint16_t data_len)
{
		my_task_message_struct message = {0};
		uint8_t *send_data = NULL;
		
		send_data = (uint8_t *) pvPortMalloc(data_len + 1);
		
		memcpy(send_data,data,data_len);
						
		message.mes_id = MY_DRV_UART_DATA_SEND;
		message.data_len = data_len;
		message.pvdata = send_data;
		message.param_1 = 1;

		xQueueSendToBack(my_drv_task_get_queuue_handle(),&message,0);
		
}

void my_log_send_to_uart(uint8_t *data , uint16_t data_len)
{
		my_log_uart_send_drv(data,data_len);
}
	
void my_log_stdout(const char *prefix,const char *format,va_list args)
{
		char line[1024] = {0};
		strcpy(line,prefix);
		strcpy(line + strlen(line),"");
		vsnprintf(line + strlen(line),1024 - strlen(line),format,args);
		my_log_send_to_uart((uint8_t *)line,strlen(line));
		
}


void my_pre_log(const char *prefix,const char *format,...)
{
		va_list args;
		va_start(args,format);
	  my_log_stdout(prefix,format,args);
	  va_end(args);
}




