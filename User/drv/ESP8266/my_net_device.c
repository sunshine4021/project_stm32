/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	net_device.c
	*
	*	���ߣ� 		�ż���
	*
	*	���ڣ� 		2018-02-01
	*
	*	�汾�� 		V1.3
	*
	*	˵���� 		�����豸Ӧ�ò�
	*
	*	�޸ļ�¼��	V1.1��1.ƽ̨IP��PORTͨ����������ķ�ʽȷ��������˲�ͬЭ�������豸������ͨ�õ����⡣
	*					  2.ȡ�����ֶ��������磬�ϵ�ȴ�wifiģ���Զ����ӣ������ɹ���ʹ��OneNET���ںŽ���������
	*					  3.NET_DEVICE_SendCmd����������mode���������Ƿ������������ķ���ֵ��
	*				V1.2��1.ȡ��V1.1�еĹ���3��
	*					  2.���������ݻ�ȡ�ӿڣ�����rb���ơ�
	*					  3.ȡ����͸��ģʽ��
	*				V1.3��1.���ӡ�æ����־�жϡ�
	*					  2.����ģ�鷢��ʱ����������ģ�����ܶ�����
	*					  3.�޸��� NET_DEVICE_Check �������ơ�
	************************************************************
	************************************************************
	************************************************************
**/

//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//��Ƭ��������
#include "mcu_gpio.h"

//�����豸
#include "net_device.h"
#include "net_io.h"

//Ӳ������
#include "delay.h"
#include "usart.h"

#if(NET_TIME_EN == 1)
#include <time.h>
#endif

//C��
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


NET_DEVICE_INFO net_device_info = {NULL, NULL, NULL, NULL, NULL,
									0, 0, 0, 60,
									0, 0, 0, 0, 0, 0, 1, 0};


GPS_INFO gps;


//Ϊ��ͨ���ԣ�gpio�豸�б����name�̶���ôд
const static GPIO_LIST net_device_gpio_list[] = {
													{GPIOA, GPIO_Pin_0, "nd_rst"},
												};


//==========================================================
//	�������ƣ�	NET_DEVICE_IO_Init
//
//	�������ܣ�	��ʼ�������豸IO��
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		��ʼ�������豸�Ŀ������š������շ����ܵ�
//==========================================================
void NET_DEVICE_IO_Init(void)
{

	MCU_GPIO_Init(net_device_gpio_list[0].gpio_group, net_device_gpio_list[0].gpio_pin, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, net_device_gpio_list[0].gpio_name);
	
	NET_IO_Init();											//�����豸����IO���ʼ��
	
	net_device_info.reboot = 0;

}

//==========================================================
//	�������ƣ�	NET_DEVICE_GetSerial
//
//	�������ܣ�	��ȡһ��Ψһ����
//
//	��ڲ�����	serial��ָ���ַ
//
//	���ز�����	0-�ɹ�	1-ʧ��
//
//	˵����		Ψһ���ţ�GSMģ�������IMEI��ΨһID��WIFIģ����Ի�ȡMAC��ַ��
//==========================================================
_Bool NET_DEVICE_GetSerial(char **serial)
{
	
	_Bool result = 1;
	char *data_ptr = NULL, *data_ptr_t = NULL;
	unsigned char len = 0, i = 0;

	if(net_device_info.busy)
		return result;
	
	if(!NET_DEVICE_SendCmd("AT+CWJAP?\r\n", "OK"))
	{
		//+CSQ: +CWJAP:"ONENET","xx:xx:xx:xx:xx:xx",6,-28\r\n\r\nOK
		data_ptr = net_device_info.cmd_resp;
		
		//�ҵ�serial��ͷ
		while(*data_ptr != '\0' && i < 3)
		{
			if(*data_ptr++ == '"')
				i++;
		}
		
		if(i == 3)
		{
			data_ptr_t = data_ptr;
		
			//����serial����
			while(*data_ptr_t != '"')
			{
				len++;
				
				if(*data_ptr_t++ == '\0')
					return result;
			}
			
			*serial = (char *)NET_MallocBuffer(len + 1);
			if(*serial == NULL)
				return result;
			
			memset(*serial, 0, len + 1);
			
			//��������
			memcpy(*serial, data_ptr, len);
			
			UsartPrintf(USART_DEBUG, "Serial: %s\r\n", *serial);
			
			result = 0;
		}
	}
	
	return result;

}

//==========================================================
//	�������ƣ�	NET_DEVICE_GetSignal
//
//	�������ܣ�	��ȡ�ź�ֵ
//
//	��ڲ�����	��
//
//	���ز�����	�����ź�ֵ
//
//	˵����		
//==========================================================
signed char NET_DEVICE_GetSignal(void)
{
	
	char *data_ptr = NULL;
	char num_buf[4] = {0, 0, 0, 0};
	unsigned char i = 0;
	
	if(!net_device_info.net_work)
		return 0;

	if(net_device_info.busy)
		return 0;
	
	net_device_info.busy = 1;
	
	//+CSQ: +CWJAP:"ONENET","xx:xx:xx:xx:xx:xx",6,-28\r\n\r\nOK
	if(NET_DEVICE_SendCmd("AT+CWJAP?\r\n", "OK") == 0)
	{
		data_ptr = net_device_info.cmd_resp;
		
		while(*data_ptr != '\0' && i < 3)
		{
			if(*data_ptr++ == ',')
				i++;
		}
		
		if(i == 3)
		{
			i = 0;
			while(*data_ptr != '\r')
				num_buf[i++] = *data_ptr++;
			
			net_device_info.signal = (signed char)atoi(num_buf);
		}
		else
			net_device_info.signal = 0;
	}
	else
		net_device_info.signal = 0;
	
	net_device_info.busy = 0;
	
	return net_device_info.signal;

}

//==========================================================
//	�������ƣ�	NET_DEVICE_GetTime
//
//	�������ܣ�	��ȡ����ʱ��
//
//	��ڲ�����	��
//
//	���ز�����	UTC��ֵ
//
//	˵����		������NTPЭ���������
//				NTP��������UDPЭ�飬�˿�123
//
//				�Ѳ��Կ��õ�NTP������-2017-11-07
//				1.cn.pool.ntp.org		��Դ�����ϳ��ģ���֪��������(ע�⡰1.��������ţ���������һ����)
//				cn.ntp.org.cn			��Դ���й�
//				edu.ntp.org.cn			��Դ���й�������
//				tw.ntp.org.cn			��Դ���й�̨��
//				us.ntp.org.cn			��Դ������
//				sgp.ntp.org.cn			��Դ���¼���
//				kr.ntp.org.cn			��Դ������
//				de.ntp.org.cn			��Դ���¹�
//				jp.ntp.org.cn			��Դ���ձ�
//==========================================================
unsigned int NET_DEVICE_GetTime(void)
{
	
	unsigned int second = 0;
	
#if(NET_TIME_EN == 1)
	struct tm *local_time;
	unsigned char time_out = 200;
	char *data_ptr = NULL;
	
	unsigned char time_buffer[48];

	NET_DEVICE_Close();
	
	if(NET_DEVICE_Connect("UDP", "edu.ntp.org.cn", "123") == 0)
	{
		memset(time_buffer, 0, sizeof(time_buffer));
		
		time_buffer[0] = 0xE3;							//LI, Version, Mode
		time_buffer[1] = 0;								//��ʾ����ʱ�ӵĲ��ˮƽ
		time_buffer[2] = 6;								//��λsigned integer����ʾ������Ϣ֮��������
		time_buffer[3] = 0xEB;							//��ʾ����ʱ�Ӿ��ȣ���ȷ�����ƽ����
		
		NET_DEVICE_SendData(time_buffer, sizeof(time_buffer));
		net_device_info.cmd_ipd = NULL;
		
		while(--time_out)
		{
			if(net_device_info.cmd_ipd != NULL)
				break;
			
			RTOS_TimeDly(2);
		}
		
		if(net_device_info.cmd_ipd)
		{
			data_ptr = net_device_info.cmd_ipd;
			
			NET_DEVICE_Close();
			
			if(((*data_ptr >> 6) & 0x03) == 3)			//bit6��bit7ͬΪ1��ʾ��ǰ���ɶ�ʱ����������������״̬��
				return second;
			
			second = *(data_ptr + 40) << 24 | *(data_ptr + 41) << 16 | *(data_ptr + 42) << 8 | *(data_ptr + 43);
			second -= 2208960000UL;						//ʱ������
			
			local_time = localtime(&second);
			
			UsartPrintf(USART_DEBUG, "UTC Time: %d-%d-%d %d:%d:%d\r\n",
									local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday,
									local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
		}
	}
#endif
	
	return second;

}

//==========================================================
//	�������ƣ�	NET_DEVICE_Exist
//
//	�������ܣ�	�����豸���ڼ��
//
//	��ڲ�����	��
//
//	���ز�����	���ؽ��
//
//	˵����		0-�ɹ�		1-ʧ��
//==========================================================
_Bool NET_DEVICE_Exist(void)
{

	unsigned char time_out = 10;
	unsigned char config_time = 0;
	_Bool status = 1;
	unsigned char key_count = 0, wait_count = 0, wifi_status = 0;
	
	while(time_out--)												//�ȴ�
	{
		RTOS_TimeDly(200);											//����ȴ�
		
		if(MCU_GPIO_Input_Read("key1") == 0)						//������������ģʽ
			key_count++;
		else
			key_count = 0;
		
		wifi_status = NET_DEVICE_Check();							//��ȡ״̬
		if(wifi_status == NET_DEVICE_NO_DEVICE)
			wait_count++;
		
		if(wifi_status || key_count >= 2)							//����յ�����
		{
			if(wifi_status == NET_DEVICE_GOT_IP)
			{
				status = 0;
				break;
			}
			else if(wait_count >= 10 || key_count >= 2)
			{
				wait_count = 0;
				key_count = 0;
#if(PHONE_AP_MODE == 0)
				NET_DEVICE_SendCmd("AT+CWSMARTSTART=2\r\n", "OK");
				UsartPrintf(USART_DEBUG, "��ʹ��OneNET΢�Ź��ں�����SSID��PSWD\r\n");
				
				while(1)
				{
					MCU_GPIO_Output_Ctl("led1", 0);MCU_GPIO_Output_Ctl("led2", 0);
					MCU_GPIO_Output_Ctl("led3", 0);MCU_GPIO_Output_Ctl("led4", 0);
					
					if(strstr((char *)net_device_info.cmd_resp, "SMART SUCCESS"))
					{
						UsartPrintf(USART_DEBUG, "�յ�:\r\n%s\r\n", strstr((char *)net_device_info.cmd_resp, "SSID:"));
						status = 1;
						
						MCU_GPIO_Output_Ctl("led1", 1);MCU_GPIO_Output_Ctl("led2", 1);
						MCU_GPIO_Output_Ctl("led3", 1);MCU_GPIO_Output_Ctl("led4", 1);
						
						break;
					}
					else
					{
						if(++config_time >= 15)													//��ʱʱ��--15s
						{
							config_time = 0;
							
							MCU_GPIO_Output_Ctl("led1", 1);MCU_GPIO_Output_Ctl("led2", 1);
							MCU_GPIO_Output_Ctl("led3", 1);MCU_GPIO_Output_Ctl("led4", 1);
							
							break;
						}
					}
					
					RTOS_TimeDly(100);
					
					MCU_GPIO_Output_Ctl("led1", 1);MCU_GPIO_Output_Ctl("led2", 1);
					MCU_GPIO_Output_Ctl("led3", 1);MCU_GPIO_Output_Ctl("led4", 1);
					
					RTOS_TimeDly(100);
				}
				
				if(config_time != 0)															//���Ϊ0�����ǳ�ʱ�˳�
				{
					config_time = 0;
					while(NET_DEVICE_Check() != NET_DEVICE_GOT_IP)								//�ȴ�WIFI����
					{
						if(++config_time >= 10)
						{
							UsartPrintf(USART_DEBUG, "���볬ʱ,����WIFI����\r\n");
							break;
						}
						
						RTOS_TimeDly(200);
					}
				}
#else
				UsartPrintf(USART_DEBUG, "STA Tips:	Link Wifi\r\n");
				
				while(NET_DEVICE_SendCmd("AT+CWMODE=1\r\n", "OK"))
					RTOS_TimeDly(100);
				
				while(NET_DEVICE_SendCmd("AT+CWJAP=\"ONENET\",\"IOT@Chinamobile123\"\r\n", "GOT IP"))
					RTOS_TimeDly(100);
				
				status = 0;
				
				break;
#endif
			}
			else
				status = 1;
		}
	}
	
	return status;

}

//==========================================================
//	�������ƣ�	NET_DEVICE_Init
//
//	�������ܣ�	�����豸��ʼ��
//
//	��ڲ�����	��
//
//	���ز�����	���س�ʼ�����
//
//	˵����		0-�ɹ�		1-ʧ��
//==========================================================
_Bool NET_DEVICE_Init(void)
{
	
	_Bool status = 1;
	
	net_device_info.net_work = 0;

	switch(net_device_info.init_step)
	{
		case 0:
			
#if(NET_TIME_EN == 1)
			if(!net_device_info.net_time)
			{
				net_device_info.net_time = NET_DEVICE_GetTime();
				if(++net_device_info.err >= 3)
				{
					net_device_info.err = 0;
					net_device_info.init_step++;
				}
				
				RTOS_TimeDly(100);
			}
			else
#endif
				net_device_info.init_step++;
		break;
			
		case 1:
		
			net_device_info.send_count = 0;
			UsartPrintf(USART_DEBUG, "Tips:	ESP8266 STA_Mode OK\r\n");
			net_device_info.init_step++;
		
		break;
		
		default:
			
			status = 0;
			net_device_info.net_work = 1;
		
		break;
	}
	
	return status;

}

//==========================================================
//	�������ƣ�	NET_DEVICE_Reset
//
//	�������ܣ�	�����豸��λ
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void NET_DEVICE_Reset(void)
{

	UsartPrintf(USART_DEBUG, "Tips:	NET_DEVICE_Reset\r\n");
	
	MCU_GPIO_Output_Ctl("nd_rst", 0);		//��λ
	RTOS_TimeDly(50);
	
	MCU_GPIO_Output_Ctl("nd_rst", 1);		//������λ
	RTOS_TimeDly(200);

}

//==========================================================
//	�������ƣ�	NET_DEVICE_Close
//
//	�������ܣ�	�ر���������
//
//	��ڲ�����	��
//
//	���ز�����	0-�ɹ�	1-ʧ��
//
//	˵����		
//==========================================================
_Bool NET_DEVICE_Close(void)
{
	
	_Bool result = 1;
	
	UsartPrintf(USART_DEBUG, "Tips:	CLOSE\r\n");

	result = NET_DEVICE_SendCmd("AT+CIPCLOSE\r\n", "OK");
	
	RTOS_TimeDly(30);
	
	return result;

}

//==========================================================
//	�������ƣ�	NET_DEVICE_Connect
//
//	�������ܣ�	����ƽ̨
//
//	��ڲ�����	type��TCP �� UDP
//				ip��IP��ַ����ָ��
//				port���˿ڻ���ָ��
//
//	���ز�����	�������ӽ��
//
//	˵����		0-�ɹ�		1-ʧ��
//==========================================================
_Bool NET_DEVICE_Connect(char *type, char *ip, char *port)
{
	
	char cmd_buf[48];
	
	snprintf(cmd_buf, sizeof(cmd_buf), "AT+CIPSTART=\"%s\",\"%s\",%s\r\n", type, ip, port);
	
	UsartPrintf(USART_DEBUG, "Tips:	%s", cmd_buf);

	return NET_DEVICE_SendCmd(cmd_buf, "CONNECT");

}

//==========================================================
//	�������ƣ�	NET_DEVICE_CmdHandle
//
//	�������ܣ�	���������Ƿ���ȷ
//
//	��ڲ�����	cmd����Ҫ���͵�����
//
//	���ز�����	��
//
//	˵����		�����ɹ���ָ����NULL
//==========================================================
void NET_DEVICE_CmdHandle(char *cmd)
{
	
	if(strstr(cmd, net_device_info.cmd_hdl) != NULL)
		net_device_info.cmd_hdl = NULL;
	
	net_device_info.cmd_resp = cmd;

}

//==========================================================
//	�������ƣ�	NET_DEVICE_SendCmd
//
//	�������ܣ�	�������豸����һ��������ȴ���ȷ����Ӧ
//
//	��ڲ�����	cmd����Ҫ���͵�����
//				res����Ҫ��������Ӧ
//
//	���ز�����	�������ӽ��
//
//	˵����		0-�ɹ�		1-ʧ��
//==========================================================
_Bool NET_DEVICE_SendCmd(char *cmd, char *res)
{
	
	unsigned char time_out = 200;
	
	NET_IO_Send((unsigned char *)cmd, strlen(cmd));			//д��������豸
	
	if(res == NULL)											//���Ϊ�գ���ֻ�Ƿ���
		return 0;
	
	net_device_info.cmd_hdl = res;							//��Ҫ���ѵĹؼ���
	
	while((net_device_info.cmd_hdl != NULL) && --time_out)	//�ȴ�
		RTOS_TimeDly(2);
	
	if(time_out > 0)
		return 0;
	else
		return 1;

}

//==========================================================
//	�������ƣ�	NET_DEVICE_SendData
//
//	�������ܣ�	ʹ�����豸�������ݵ�ƽ̨
//
//	��ڲ�����	data����Ҫ���͵�����
//				len�����ݳ���
//
//	���ز�����	0-�������	1-����ʧ��
//
//	˵����		
//==========================================================
_Bool NET_DEVICE_SendData(unsigned char *data, unsigned short len)
{
	
	_Bool result = 1;
	char cmd_buf[40];
	
	if(net_device_info.busy)
		return result;
	
	net_device_info.busy = 1;
	
	net_device_info.send_count++;

	sprintf(cmd_buf, "AT+CIPSEND=%d\r\n", len);		//��������
	if(!NET_DEVICE_SendCmd(cmd_buf, ">")) 			//�յ���>��ʱ���Է�������
	{
		NET_IO_Send(data, len);  					//�����豸������������
		
		result = 0;
	}
	
	RTOS_TimeDly(10);
	
	net_device_info.busy = 0;
	
	return result;

}

//==========================================================
//	�������ƣ�	NET_DEVICE_Read
//
//	�������ܣ�	��ȡһ֡����
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
unsigned char *NET_DEVICE_Read(void)
{

	return NET_IO_Read();

}

//==========================================================
//	�������ƣ�	NET_DEVICE_GetIPD
//
//	�������ܣ�	��ȡƽ̨���ص�����
//
//	��ڲ�����	data_ptr��ԭʼ����ָ��
//
//	���ز�����	ƽ̨���ص�ԭʼ����
//
//	˵����		��ͬ�����豸���صĸ�ʽ��ͬ����Ҫȥ����
//				��ESP8266�ķ��ظ�ʽΪ	"+IPD,x:yyy"	x�������ݳ��ȣ�yyy����������
//==========================================================
unsigned char *NET_DEVICE_GetIPD(unsigned char *data_ptr)
{
	
	char *data_ptr_t = (char *)data_ptr;
	
	if(data_ptr_t != NULL)
	{
		data_ptr_t = strstr(data_ptr_t, "IPD,");					//������IPD��ͷ
		if(data_ptr_t == NULL)										//���û�ҵ���������IPDͷ���ӳ٣�������Ҫ�ȴ�һ�ᣬ�����ᳬ���趨��ʱ��
		{
			//UsartPrintf(USART_DEBUG, "\"IPD\" not found\r\n");
		}
		else
		{
			if(sscanf(data_ptr_t, "IPD,%d", &net_device_info.data_bytes) == 1)
			{
				data_ptr_t = strchr(data_ptr_t, ':');
				
				if(data_ptr_t != NULL)
				{
					data_ptr_t++;
					return (unsigned char *)(data_ptr_t);
				}
			}
		}
	}

	return NULL;													//��ʱ��δ�ҵ������ؿ�ָ��

}

//==========================================================
//	�������ƣ�	NET_DEVICE_Check
//
//	�������ܣ�	��������豸����״̬
//
//	��ڲ�����	��
//
//	���ز�����	����״̬
//
//	˵����		
//==========================================================
unsigned char NET_DEVICE_Check(void)
{
	
	unsigned char status = NET_DEVICE_NO_DEVICE, err_count = 3, time_out = 200;
	int status_result = 255;
	char *data_ptr = NULL;
	
	while(net_device_info.busy && --time_out)
		RTOS_TimeDly(2);
	
	if(time_out == 0)
		return NET_DEVICE_BUSY;
	
	net_device_info.busy = 1;
	
	while(--err_count)
	{
		if(NET_DEVICE_SendCmd("AT+CIPSTATUS\r\n", "OK") == 0)					//����״̬���
		{
			data_ptr = strstr(net_device_info.cmd_resp, "STATUS:");
			if(sscanf(data_ptr, "STATUS:%d", &status_result) == 1)
			{
				switch(status_result)
				{
					case 2:
						UsartPrintf(USART_DEBUG, "Tips:	NET_DEVICE Got IP\r\n");
						status = NET_DEVICE_GOT_IP;
					break;
					
					case 3:
						UsartPrintf(USART_DEBUG, "Tips:	NET_DEVICE Connect OK\r\n");
						status = NET_DEVICE_CONNECTED;
					break;
					
					case 4:
						UsartPrintf(USART_DEBUG, "Tips:	NET_DEVICE Lost Connect\r\n");
						status = NET_DEVICE_CLOSED;
					break;
					
					case 5:
						//UsartPrintf(USART_DEBUG, "Tips:	NET_DEVICE Lost\r\n");		//�豸��ʧ
						status = NET_DEVICE_NO_DEVICE;
					break;
				}
				
				break;
			}
		}
		
		RTOS_TimeDly(20);
	}
	
	net_device_info.busy = 0;
	
	return status;

}

//==========================================================
//	�������ƣ�	NET_DEVICE_ReConfig
//
//	�������ܣ�	�豸�����豸��ʼ���Ĳ���
//
//	��ڲ�����	����ֵ
//
//	���ز�����	��
//
//	˵����		�ú������õĲ����������豸��ʼ������õ�
//==========================================================
void NET_DEVICE_ReConfig(unsigned char step)
{

	net_device_info.init_step = step;

}

/******************************************************************************************
										��Ϣ����
******************************************************************************************/

//==========================================================
//	�������ƣ�	NET_DEVICE_CheckListHead
//
//	�������ܣ�	��鷢������ͷ�Ƿ�Ϊ��
//
//	��ڲ�����	��
//
//	���ز�����	0-��	1-��Ϊ��
//
//	˵����		
//==========================================================
_Bool NET_DEVICE_CheckListHead(void)
{

	if(net_device_info.head == NULL)
		return 0;
	else
		return 1;

}

//==========================================================
//	�������ƣ�	NET_DEVICE_GetListHeadBuf
//
//	�������ܣ�	��ȡ��������Ҫ���͵�����ָ��
//
//	��ڲ�����	��
//
//	���ز�����	��ȡ��������Ҫ���͵�����ָ��
//
//	˵����		
//==========================================================
unsigned char *NET_DEVICE_GetListHeadBuf(void)
{

	return net_device_info.head->buf;

}

//==========================================================
//	�������ƣ�	NET_DEVICE_GetListHeadLen
//
//	�������ܣ�	��ȡ��������Ҫ���͵����ݳ���
//
//	��ڲ�����	��
//
//	���ز�����	��ȡ��������Ҫ���͵����ݳ���
//
//	˵����		
//==========================================================
unsigned short NET_DEVICE_GetListHeadLen(void)
{

	return net_device_info.head->dataLen;

}

//==========================================================
//	�������ƣ�	NET_DEVICE_AddDataSendList
//
//	�������ܣ�	�ڷ�������β����һ����������
//
//	��ڲ�����	buf����Ҫ���͵�����
//				dataLen�����ݳ���
//
//	���ز�����	0-�ɹ�	����-ʧ��
//
//	˵����		�첽���ͷ�ʽ
//==========================================================
unsigned char NET_DEVICE_AddDataSendList(unsigned char *buf ,unsigned short dataLen)
{
	
	struct NET_SEND_LIST *current = (struct NET_SEND_LIST *)NET_MallocBuffer(sizeof(struct NET_SEND_LIST));
																//�����ڴ�
	
	if(current == NULL)
		return 1;
	
	current->buf = (unsigned char *)NET_MallocBuffer(dataLen);	//�����ڴ�
	if(current->buf == NULL)
	{
		NET_FreeBuffer(current);								//ʧ�����ͷ�
		return 2;
	}
	
	if(net_device_info.head == NULL)								//���headΪNULL
		net_device_info.head = current;							//headָ��ǰ������ڴ���
	else														//���head��ΪNULL
		net_device_info.end->next = current;							//��endָ��ǰ������ڴ���
	
	memcpy(current->buf, buf, dataLen);							//��������
	current->dataLen = dataLen;
	current->next = NULL;										//��һ��ΪNULL
	
	net_device_info.end = current;								//endָ��ǰ������ڴ���
	
	return 0;

}

//==========================================================
//	�������ƣ�	NET_DEVICE_DeleteDataSendList
//
//	�������ܣ�	������ͷɾ��һ������
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
_Bool NET_DEVICE_DeleteDataSendList(void)
{
	
	struct NET_SEND_LIST *next = net_device_info.head->next;	//��������ͷ����һ�����ݵ�ַ
	
	net_device_info.head->dataLen = 0;
	net_device_info.head->next = NULL;
	NET_FreeBuffer(net_device_info.head->buf);				//�ͷ��ڴ�
	NET_FreeBuffer(net_device_info.head);						//�ͷ��ڴ�
	
	net_device_info.head = next;								//����ͷָ����һ������
	
	return 0;

}
