/**
	************************************************************
	************************************************************
	************************************************************
	*	文件名： 	net_device.c
	*
	*	作者： 		张继瑞
	*
	*	日期： 		2018-02-01
	*
	*	版本： 		V1.3
	*
	*	说明： 		网络设备应用层
	*
	*	修改记录：	V1.1：1.平台IP和PORT通过参数传入的方式确定，解决了不同协议网络设备驱动不通用的问题。
	*					  2.取消了手动配置网络，上电等待wifi模块自动连接，若不成功则使用OneNET公众号进行配网。
	*					  3.NET_DEVICE_SendCmd新增参数“mode”，决定是否清除本次命令的返回值。
	*				V1.2：1.取消V1.1中的功能3。
	*					  2.更改了数据获取接口，适配rb机制。
	*					  3.取消了透传模式。
	*				V1.3：1.增加“忙”标志判断。
	*					  2.增加模组发送时间间隔，根据模组性能而定。
	*					  3.修改了 NET_DEVICE_Check 函数机制。
	************************************************************
	************************************************************
	************************************************************
**/

//单片机头文件
#include "stm32f10x.h"

//单片机相关组件
#include "mcu_gpio.h"

//网络设备
#include "net_device.h"
#include "net_io.h"

//硬件驱动
#include "delay.h"
#include "usart.h"

#if(NET_TIME_EN == 1)
#include <time.h>
#endif

//C库
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


NET_DEVICE_INFO net_device_info = {NULL, NULL, NULL, NULL, NULL,
									0, 0, 0, 60,
									0, 0, 0, 0, 0, 0, 1, 0};


GPS_INFO gps;


//为了通用性，gpio设备列表里的name固定这么写
const static GPIO_LIST net_device_gpio_list[] = {
													{GPIOA, GPIO_Pin_0, "nd_rst"},
												};


//==========================================================
//	函数名称：	NET_DEVICE_IO_Init
//
//	函数功能：	初始化网络设备IO层
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		初始化网络设备的控制引脚、数据收发功能等
//==========================================================
void NET_DEVICE_IO_Init(void)
{

	MCU_GPIO_Init(net_device_gpio_list[0].gpio_group, net_device_gpio_list[0].gpio_pin, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, net_device_gpio_list[0].gpio_name);
	
	NET_IO_Init();											//网络设备数据IO层初始化
	
	net_device_info.reboot = 0;

}

//==========================================================
//	函数名称：	NET_DEVICE_GetSerial
//
//	函数功能：	获取一个唯一串号
//
//	入口参数：	serial：指针地址
//
//	返回参数：	0-成功	1-失败
//
//	说明：		唯一串号：GSM模组可以是IMEI等唯一ID；WIFI模组可以获取MAC地址等
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
		
		//找到serial开头
		while(*data_ptr != '\0' && i < 3)
		{
			if(*data_ptr++ == '"')
				i++;
		}
		
		if(i == 3)
		{
			data_ptr_t = data_ptr;
		
			//计算serial长度
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
			
			//复制数据
			memcpy(*serial, data_ptr, len);
			
			UsartPrintf(USART_DEBUG, "Serial: %s\r\n", *serial);
			
			result = 0;
		}
	}
	
	return result;

}

//==========================================================
//	函数名称：	NET_DEVICE_GetSignal
//
//	函数功能：	获取信号值
//
//	入口参数：	无
//
//	返回参数：	返回信号值
//
//	说明：		
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
//	函数名称：	NET_DEVICE_GetTime
//
//	函数功能：	获取网络时间
//
//	入口参数：	无
//
//	返回参数：	UTC秒值
//
//	说明：		可搜索NTP协议相关资料
//				NTP服务器：UDP协议，端口123
//
//				已测试可用的NTP服务器-2017-11-07
//				1.cn.pool.ntp.org		来源：网上抄的，不知道哪来的(注意“1.”不是序号，是域名的一部分)
//				cn.ntp.org.cn			来源：中国
//				edu.ntp.org.cn			来源：中国教育网
//				tw.ntp.org.cn			来源：中国台湾
//				us.ntp.org.cn			来源：美国
//				sgp.ntp.org.cn			来源：新加坡
//				kr.ntp.org.cn			来源：韩国
//				de.ntp.org.cn			来源：德国
//				jp.ntp.org.cn			来源：日本
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
		time_buffer[1] = 0;								//表示本地时钟的层次水平
		time_buffer[2] = 6;								//八位signed integer，表示连续信息之间的最大间隔
		time_buffer[3] = 0xEB;							//表示本地时钟精度，精确到秒的平方级
		
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
			
			if(((*data_ptr >> 6) & 0x03) == 3)			//bit6和bit7同为1表示当前不可对时（服务器处于闰秒状态）
				return second;
			
			second = *(data_ptr + 40) << 24 | *(data_ptr + 41) << 16 | *(data_ptr + 42) << 8 | *(data_ptr + 43);
			second -= 2208960000UL;						//时区修正
			
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
//	函数名称：	NET_DEVICE_Exist
//
//	函数功能：	网络设备存在检查
//
//	入口参数：	无
//
//	返回参数：	返回结果
//
//	说明：		0-成功		1-失败
//==========================================================
_Bool NET_DEVICE_Exist(void)
{

	unsigned char time_out = 10;
	unsigned char config_time = 0;
	_Bool status = 1;
	unsigned char key_count = 0, wait_count = 0, wifi_status = 0;
	
	while(time_out--)												//等待
	{
		RTOS_TimeDly(200);											//挂起等待
		
		if(MCU_GPIO_Input_Read("key1") == 0)						//长按进入配网模式
			key_count++;
		else
			key_count = 0;
		
		wifi_status = NET_DEVICE_Check();							//获取状态
		if(wifi_status == NET_DEVICE_NO_DEVICE)
			wait_count++;
		
		if(wifi_status || key_count >= 2)							//如果收到数据
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
				UsartPrintf(USART_DEBUG, "请使用OneNET微信公众号配置SSID和PSWD\r\n");
				
				while(1)
				{
					MCU_GPIO_Output_Ctl("led1", 0);MCU_GPIO_Output_Ctl("led2", 0);
					MCU_GPIO_Output_Ctl("led3", 0);MCU_GPIO_Output_Ctl("led4", 0);
					
					if(strstr((char *)net_device_info.cmd_resp, "SMART SUCCESS"))
					{
						UsartPrintf(USART_DEBUG, "收到:\r\n%s\r\n", strstr((char *)net_device_info.cmd_resp, "SSID:"));
						status = 1;
						
						MCU_GPIO_Output_Ctl("led1", 1);MCU_GPIO_Output_Ctl("led2", 1);
						MCU_GPIO_Output_Ctl("led3", 1);MCU_GPIO_Output_Ctl("led4", 1);
						
						break;
					}
					else
					{
						if(++config_time >= 15)													//超时时间--15s
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
				
				if(config_time != 0)															//如果为0，则是超时退出
				{
					config_time = 0;
					while(NET_DEVICE_Check() != NET_DEVICE_GOT_IP)								//等待WIFI接入
					{
						if(++config_time >= 10)
						{
							UsartPrintf(USART_DEBUG, "接入超时,请检查WIFI配置\r\n");
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
//	函数名称：	NET_DEVICE_Init
//
//	函数功能：	网络设备初始化
//
//	入口参数：	无
//
//	返回参数：	返回初始化结果
//
//	说明：		0-成功		1-失败
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
//	函数名称：	NET_DEVICE_Reset
//
//	函数功能：	网络设备复位
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void NET_DEVICE_Reset(void)
{

	UsartPrintf(USART_DEBUG, "Tips:	NET_DEVICE_Reset\r\n");
	
	MCU_GPIO_Output_Ctl("nd_rst", 0);		//复位
	RTOS_TimeDly(50);
	
	MCU_GPIO_Output_Ctl("nd_rst", 1);		//结束复位
	RTOS_TimeDly(200);

}

//==========================================================
//	函数名称：	NET_DEVICE_Close
//
//	函数功能：	关闭网络连接
//
//	入口参数：	无
//
//	返回参数：	0-成功	1-失败
//
//	说明：		
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
//	函数名称：	NET_DEVICE_Connect
//
//	函数功能：	重连平台
//
//	入口参数：	type：TCP 或 UDP
//				ip：IP地址缓存指针
//				port：端口缓存指针
//
//	返回参数：	返回连接结果
//
//	说明：		0-成功		1-失败
//==========================================================
_Bool NET_DEVICE_Connect(char *type, char *ip, char *port)
{
	
	char cmd_buf[48];
	
	snprintf(cmd_buf, sizeof(cmd_buf), "AT+CIPSTART=\"%s\",\"%s\",%s\r\n", type, ip, port);
	
	UsartPrintf(USART_DEBUG, "Tips:	%s", cmd_buf);

	return NET_DEVICE_SendCmd(cmd_buf, "CONNECT");

}

//==========================================================
//	函数名称：	NET_DEVICE_CmdHandle
//
//	函数功能：	检查命令返回是否正确
//
//	入口参数：	cmd：需要发送的命令
//
//	返回参数：	无
//
//	说明：		命令处理成功则将指针置NULL
//==========================================================
void NET_DEVICE_CmdHandle(char *cmd)
{
	
	if(strstr(cmd, net_device_info.cmd_hdl) != NULL)
		net_device_info.cmd_hdl = NULL;
	
	net_device_info.cmd_resp = cmd;

}

//==========================================================
//	函数名称：	NET_DEVICE_SendCmd
//
//	函数功能：	向网络设备发送一条命令，并等待正确的响应
//
//	入口参数：	cmd：需要发送的命令
//				res：需要检索的响应
//
//	返回参数：	返回连接结果
//
//	说明：		0-成功		1-失败
//==========================================================
_Bool NET_DEVICE_SendCmd(char *cmd, char *res)
{
	
	unsigned char time_out = 200;
	
	NET_IO_Send((unsigned char *)cmd, strlen(cmd));			//写命令到网络设备
	
	if(res == NULL)											//如果为空，则只是发送
		return 0;
	
	net_device_info.cmd_hdl = res;							//需要所搜的关键词
	
	while((net_device_info.cmd_hdl != NULL) && --time_out)	//等待
		RTOS_TimeDly(2);
	
	if(time_out > 0)
		return 0;
	else
		return 1;

}

//==========================================================
//	函数名称：	NET_DEVICE_SendData
//
//	函数功能：	使网络设备发送数据到平台
//
//	入口参数：	data：需要发送的数据
//				len：数据长度
//
//	返回参数：	0-发送完成	1-发送失败
//
//	说明：		
//==========================================================
_Bool NET_DEVICE_SendData(unsigned char *data, unsigned short len)
{
	
	_Bool result = 1;
	char cmd_buf[40];
	
	if(net_device_info.busy)
		return result;
	
	net_device_info.busy = 1;
	
	net_device_info.send_count++;

	sprintf(cmd_buf, "AT+CIPSEND=%d\r\n", len);		//发送命令
	if(!NET_DEVICE_SendCmd(cmd_buf, ">")) 			//收到‘>’时可以发送数据
	{
		NET_IO_Send(data, len);  					//发送设备连接请求数据
		
		result = 0;
	}
	
	RTOS_TimeDly(10);
	
	net_device_info.busy = 0;
	
	return result;

}

//==========================================================
//	函数名称：	NET_DEVICE_Read
//
//	函数功能：	读取一帧数据
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
unsigned char *NET_DEVICE_Read(void)
{

	return NET_IO_Read();

}

//==========================================================
//	函数名称：	NET_DEVICE_GetIPD
//
//	函数功能：	获取平台返回的数据
//
//	入口参数：	data_ptr：原始数据指针
//
//	返回参数：	平台返回的原始数据
//
//	说明：		不同网络设备返回的格式不同，需要去调试
//				如ESP8266的返回格式为	"+IPD,x:yyy"	x代表数据长度，yyy是数据内容
//==========================================================
unsigned char *NET_DEVICE_GetIPD(unsigned char *data_ptr)
{
	
	char *data_ptr_t = (char *)data_ptr;
	
	if(data_ptr_t != NULL)
	{
		data_ptr_t = strstr(data_ptr_t, "IPD,");					//搜索“IPD”头
		if(data_ptr_t == NULL)										//如果没找到，可能是IPD头的延迟，还是需要等待一会，但不会超过设定的时间
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

	return NULL;													//超时还未找到，返回空指针

}

//==========================================================
//	函数名称：	NET_DEVICE_Check
//
//	函数功能：	检查网络设备连接状态
//
//	入口参数：	无
//
//	返回参数：	返回状态
//
//	说明：		
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
		if(NET_DEVICE_SendCmd("AT+CIPSTATUS\r\n", "OK") == 0)					//发送状态监测
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
						//UsartPrintf(USART_DEBUG, "Tips:	NET_DEVICE Lost\r\n");		//设备丢失
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
//	函数名称：	NET_DEVICE_ReConfig
//
//	函数功能：	设备网络设备初始化的步骤
//
//	入口参数：	步骤值
//
//	返回参数：	无
//
//	说明：		该函数设置的参数在网络设备初始化里边用到
//==========================================================
void NET_DEVICE_ReConfig(unsigned char step)
{

	net_device_info.init_step = step;

}

/******************************************************************************************
										消息队列
******************************************************************************************/

//==========================================================
//	函数名称：	NET_DEVICE_CheckListHead
//
//	函数功能：	检查发送链表头是否为空
//
//	入口参数：	无
//
//	返回参数：	0-空	1-不为空
//
//	说明：		
//==========================================================
_Bool NET_DEVICE_CheckListHead(void)
{

	if(net_device_info.head == NULL)
		return 0;
	else
		return 1;

}

//==========================================================
//	函数名称：	NET_DEVICE_GetListHeadBuf
//
//	函数功能：	获取链表里需要发送的数据指针
//
//	入口参数：	无
//
//	返回参数：	获取链表里需要发送的数据指针
//
//	说明：		
//==========================================================
unsigned char *NET_DEVICE_GetListHeadBuf(void)
{

	return net_device_info.head->buf;

}

//==========================================================
//	函数名称：	NET_DEVICE_GetListHeadLen
//
//	函数功能：	获取链表里需要发送的数据长度
//
//	入口参数：	无
//
//	返回参数：	获取链表里需要发送的数据长度
//
//	说明：		
//==========================================================
unsigned short NET_DEVICE_GetListHeadLen(void)
{

	return net_device_info.head->dataLen;

}

//==========================================================
//	函数名称：	NET_DEVICE_AddDataSendList
//
//	函数功能：	在发送链表尾新增一个发送链表
//
//	入口参数：	buf：需要发送的数据
//				dataLen：数据长度
//
//	返回参数：	0-成功	其他-失败
//
//	说明：		异步发送方式
//==========================================================
unsigned char NET_DEVICE_AddDataSendList(unsigned char *buf ,unsigned short dataLen)
{
	
	struct NET_SEND_LIST *current = (struct NET_SEND_LIST *)NET_MallocBuffer(sizeof(struct NET_SEND_LIST));
																//分配内存
	
	if(current == NULL)
		return 1;
	
	current->buf = (unsigned char *)NET_MallocBuffer(dataLen);	//分配内存
	if(current->buf == NULL)
	{
		NET_FreeBuffer(current);								//失败则释放
		return 2;
	}
	
	if(net_device_info.head == NULL)								//如果head为NULL
		net_device_info.head = current;							//head指向当前分配的内存区
	else														//如果head不为NULL
		net_device_info.end->next = current;							//则end指向当前分配的内存区
	
	memcpy(current->buf, buf, dataLen);							//复制数据
	current->dataLen = dataLen;
	current->next = NULL;										//下一段为NULL
	
	net_device_info.end = current;								//end指向当前分配的内存区
	
	return 0;

}

//==========================================================
//	函数名称：	NET_DEVICE_DeleteDataSendList
//
//	函数功能：	从链表头删除一个链表
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
_Bool NET_DEVICE_DeleteDataSendList(void)
{
	
	struct NET_SEND_LIST *next = net_device_info.head->next;	//保存链表头的下一段数据地址
	
	net_device_info.head->dataLen = 0;
	net_device_info.head->next = NULL;
	NET_FreeBuffer(net_device_info.head->buf);				//释放内存
	NET_FreeBuffer(net_device_info.head);						//释放内存
	
	net_device_info.head = next;								//链表头指向下一段数据
	
	return 0;

}
