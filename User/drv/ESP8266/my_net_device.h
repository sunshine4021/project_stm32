#ifndef _NET_DEVICE_H_
#define _NET_DEVICE_H_


//=============================����==============================
//===========�����ṩRTOS���ڴ��������Ҳ����ʹ��C���=========
#include "FreeRTOS.h"

#define NET_MallocBuffer	pvPortMalloc

#define NET_FreeBuffer		vPortFree
//==========================================================


struct NET_SEND_LIST
{

	unsigned short dataLen;			//���ݳ���
	unsigned char *buf;				//����ָ��
	
	struct NET_SEND_LIST *next;		//��һ��

};


typedef struct
{
	
	char *cmd_resp;					//����ظ�ָ�롣�����ȡ����ص����ݣ�������ȡ��ָ���ڵ�����
	char *cmd_ipd;					//�ڽ���onenetǰͨ�������ȡ�����ݣ���������ʱ�䡢��ȡ����ip��
	char *cmd_hdl;					//������ڷ�������󣬻��ڷ��������������û�ָ���ķ�������
	
/*************************���Ͷ���*************************/
	struct NET_SEND_LIST *head, *end;
	
	unsigned int net_time;			//����ʱ��
	
	int data_bytes;					//���յ������ݳ���
	
	signed char signal;				//�ź�ֵ
	
	unsigned short send_time;		//ģ����������ȶ�����ʱ��������
	
	unsigned short err : 4; 		//��������
	unsigned short init_step : 4;	//��ʼ������
	unsigned short reboot : 1;		//����������־
	unsigned short net_work : 1;	//�������OK
	unsigned short device_ok : 1;	//�豸���
	unsigned short busy : 1;		//æ��־
	unsigned short send_count : 3;	//�����豸��ķ��ͳɹ�
	unsigned short reverse : 1;		//����

} NET_DEVICE_INFO;

extern NET_DEVICE_INFO net_device_info;


typedef struct
{

	char lon[16];
	char lat[16];
	
	_Bool flag;

} GPS_INFO;

extern GPS_INFO gps;


#define NET_DEVICE_CONNECTED	0	//������
#define NET_DEVICE_CONNECTING	1	//������
#define NET_DEVICE_CLOSED		2	//�ѶϿ�
#define NET_DEVICE_GOT_IP		3	//�ѻ�ȡ��IP
#define NET_DEVICE_NO_DEVICE	4	//���豸
#define NET_DEVICE_INITIAL		5	//��ʼ��״̬
#define NET_DEVICE_NO_CARD		6	//û��sim��
#define NET_DEVICE_BUSY			254	//æ
#define NET_DEVICE_NO_ERR		255 //�޴���


#define PHONE_AP_MODE			0	//1-ʹ���ֻ��ȵ�ģʽ������Χ��·��ʱ��AirKiss�޷�ʹ�ã���ʹ�ù̶���ʽ����
									//0-ʹ��AirKiss�ķ�ʽ����

#define NET_TIME_EN				1	//1-��ȡ����ʱ��		0-����ȡ


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
