#ifndef _NET_DEVICE_H_
#define _NET_DEVICE_H_


//=============================配置==============================
//===========可以提供RTOS的内存管理方案，也可以使用C库的=========
#include "FreeRTOS.h"

#define NET_MallocBuffer	pvPortMalloc

#define NET_FreeBuffer		vPortFree
//==========================================================


struct NET_SEND_LIST
{

	unsigned short dataLen;			//数据长度
	unsigned char *buf;				//数据指针
	
	struct NET_SEND_LIST *next;		//下一个

};


typedef struct
{
	
	char *cmd_resp;					//命令回复指针。比如获取命令返回的数据，可以提取此指针内的数据
	char *cmd_ipd;					//在接入onenet前通过网络获取的数据，比如网络时间、获取接入ip等
	char *cmd_hdl;					//命令处理。在发送命令后，会在返回内容里搜索用户指定的返回数据
	
/*************************发送队列*************************/
	struct NET_SEND_LIST *head, *end;
	
	unsigned int net_time;			//网络时间
	
	int data_bytes;					//接收到的数据长度
	
	signed char signal;				//信号值
	
	unsigned short send_time;		//模组最短数据稳定发送时间间隔发送
	
	unsigned short err : 4; 		//错误类型
	unsigned short init_step : 4;	//初始化步骤
	unsigned short reboot : 1;		//死机重启标志
	unsigned short net_work : 1;	//网络访问OK
	unsigned short device_ok : 1;	//设备检测
	unsigned short busy : 1;		//忙标志
	unsigned short send_count : 3;	//网络设备层的发送成功
	unsigned short reverse : 1;		//保留

} NET_DEVICE_INFO;

extern NET_DEVICE_INFO net_device_info;


typedef struct
{

	char lon[16];
	char lat[16];
	
	_Bool flag;

} GPS_INFO;

extern GPS_INFO gps;


#define NET_DEVICE_CONNECTED	0	//已连接
#define NET_DEVICE_CONNECTING	1	//连接中
#define NET_DEVICE_CLOSED		2	//已断开
#define NET_DEVICE_GOT_IP		3	//已获取到IP
#define NET_DEVICE_NO_DEVICE	4	//无设备
#define NET_DEVICE_INITIAL		5	//初始化状态
#define NET_DEVICE_NO_CARD		6	//没有sim卡
#define NET_DEVICE_BUSY			254	//忙
#define NET_DEVICE_NO_ERR		255 //无错误


#define PHONE_AP_MODE			0	//1-使用手机热点模式。当周围无路由时，AirKiss无法使用，则使用固定方式配置
									//0-使用AirKiss的方式配网

#define NET_TIME_EN				1	//1-获取网络时间		0-不获取


void NET_DEVICE_IO_Init(void);

_Bool NET_DEVICE_GetSerial(char **serial);

signed char NET_DEVICE_GetSignal(void);

_Bool NET_DEVICE_Exist(void);

_Bool NET_DEVICE_Init(void);

void NET_DEVICE_Reset(void);

_Bool NET_DEVICE_Close(void);

_Bool NET_DEVICE_Connect(char *type, char *ip, char *port);

void NET_DEVICE_CmdHandle(char *cmd);

_Bool NET_DEVICE_SendCmd(char *cmd, char *res);

_Bool NET_DEVICE_SendData(unsigned char *data, unsigned short len);

unsigned char *NET_DEVICE_Read(void);

unsigned char *NET_DEVICE_GetIPD(unsigned char *data_ptr);

unsigned char NET_DEVICE_Check(void);

void NET_DEVICE_ReConfig(unsigned char step);

_Bool NET_DEVICE_CheckListHead(void);

unsigned char *NET_DEVICE_GetListHeadBuf(void);

unsigned short NET_DEVICE_GetListHeadLen(void);

unsigned char NET_DEVICE_AddDataSendList(unsigned char *buf ,unsigned short dataLen);

_Bool NET_DEVICE_DeleteDataSendList(void);

#endif
