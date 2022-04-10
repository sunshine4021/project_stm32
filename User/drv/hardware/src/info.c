/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	info.c
	*
	*	���ߣ� 		�ż���
	*
	*	���ڣ� 		2017-02-23
	*
	*	�汾�� 		V1.1
	*
	*	˵���� 		V1.0��SSID��PSWD��DEVID��APIKEY��PROID��AUIF���漰��ȡ��
	*				V1.1��ȡ����SSID��PSWD�ı���Ͷ�д���滻Ϊ������������wifi���͵������豸�����Զ����档
	*
	*				��Ҫ��ֻ�е��ⲿ�洢������ʱ���Ŵ��ж�ȡ��Ϣ
	*					  �������ڣ����ȡ�̻��ڴ��������Ϣ
	*
	*	�޸ļ�¼��	
	************************************************************
	************************************************************
	************************************************************
**/

//Ӳ������
#include "info.h"
#include "at24c02.h"
#include "delay.h"
#include "usart.h"

//Э��
#include "onenet.h"

//�����豸
#include "net_device.h"

//C��
#include <string.h>
#include <stdlib.h>




/*
************************************************************
*	�������ƣ�	Info_Check
*
*	�������ܣ�	�����Ϣ�Ƿ����
*
*	��ڲ�����	��
*
*	���ز�����	�����
*
*	˵����		�ж�wifi��ssid��pswd�Ƿ����
*				0-ok	1-��ssid	2-��pswd
*				3-��devid	4-��apikey
************************************************************
*/
unsigned char Info_Check(void)
{
	
	unsigned char rData = 0;
	
	AT24C02_ReadByte(DEVID_ADDRESS, &rData);	//��ȡ����ֵ
	if(rData == 0 || rData >= 10)				//���Ϊ0�򳬳�
		return 1;
	
	AT24C02_ReadByte(AKEY_ADDRESS, &rData);		//��ȡ����ֵ
	if(rData == 0 || rData >= 30)				//���Ϊ0�򳬳�
		return 2;
	
	AT24C02_ReadByte(PROID_ADDRESS, &rData);	//��ȡ����ֵ
	if(rData == 0 || rData >= 10)				//���Ϊ0�򳬳�
		return 3;
	
	AT24C02_ReadByte(AUIF_ADDRESS, &rData);		//��ȡ����ֵ
	if(rData == 0 || rData >= 50)				//���Ϊ0�򳬳�
		return 4;
        
	return 0;

}

/*
************************************************************
*	�������ƣ�	Info_WifiLen
*
*	�������ܣ�	��ȡ��Ϣ����
*
*	��ڲ�����	sp����Ҫ������Ϣ-��˵��
*
*	���ز�����	�����
*
*	˵����		��ȡ0-ssid����	1-pswd����	
*				2-devid����		3-apikey����
************************************************************
*/
unsigned char Info_WifiLen(unsigned char sp)
{
	
	unsigned char len = 0;
    
    switch(sp)
    {
        case 1:
            AT24C02_ReadByte(DEVID_ADDRESS, &len);		//��ȡ����ֵ
			if(len == 0 || len >= 10)					//���Ϊ0�򳬳�
				return 1;
        break;
        
        case 2:
            AT24C02_ReadByte(AKEY_ADDRESS, &len);		//��ȡ����ֵ
			if(len == 0 || len >= 30)					//���Ϊ0�򳬳�
				return 1;
        break;
			
		case 3:
            AT24C02_ReadByte(PROID_ADDRESS, &len);		//��ȡ����ֵ
			if(len == 0 || len >= 10)					//���Ϊ0�򳬳�
				return 1;
        break;
			
		case 4:
            AT24C02_ReadByte(AUIF_ADDRESS, &len);		//��ȡ����ֵ
			if(len == 0 || len >= 50)					//���Ϊ0�򳬳�
				return 1;
        break;
    }
	
	return len;

}

/*
************************************************************
*	�������ƣ�	Info_CountLen
*
*	�������ܣ�	�����ֶγ���
*
*	��ڲ�����	info����Ҫ�����ֶ�
*
*	���ز�����	�ֶγ���
*
*	˵����		���㴮1���������ֶγ���   ��"\r\n"��β
************************************************************
*/
unsigned char Info_CountLen(char *info)
{

	unsigned char len = 0;
	char *buf = strstr(info, ":");		//�ҵ�':'
	
	buf++;								//ƫ�Ƶ���һ���ֽڣ������ֶ���Ϣ��ʼ
	while(1)
	{
		if(*buf == '\r')				//ֱ��'\r'Ϊֹ
			return len;
		
		buf++;
		len++;
	}

}

/*
************************************************************
*	�������ƣ�	Info_Read
*
*	�������ܣ�	��ȡssid��pswd��devid��apikey
*
*	��ڲ�����	��
*
*	���ز�����	��ȡ���
*
*	˵����		0-�ɹ�		1-ʧ��
************************************************************
*/
_Bool Info_Read(void)
{
	
    memset(onenet_info.dev_id, 0, sizeof(onenet_info.dev_id));											//���֮ǰ������
	AT24C02_ReadBytes(DEVID_ADDRESS + 1, (unsigned char *)onenet_info.dev_id, Info_WifiLen(1));		//��ȡdevid����  ��devid
    DelayXms(10);																					//��ʱ
                
    memset(onenet_info.api_key, 0, sizeof(onenet_info.api_key));										//���֮ǰ������
	AT24C02_ReadBytes(AKEY_ADDRESS + 1, (unsigned char *)onenet_info.api_key, Info_WifiLen(2));		//��ȡapikey����  ��apikey
    DelayXms(10);																					//��ʱ
	
	memset(onenet_info.pro_id, 0, sizeof(onenet_info.pro_id));											//���֮ǰ������
	AT24C02_ReadBytes(PROID_ADDRESS + 1, (unsigned char *)onenet_info.pro_id, Info_WifiLen(3));		//��ȡproid����  ��proid
    DelayXms(10);																					//��ʱ
	
	memset(onenet_info.auif, 0, sizeof(onenet_info.auif));											//���֮ǰ������
	AT24C02_ReadBytes(AUIF_ADDRESS + 1, (unsigned char *)onenet_info.auif, Info_WifiLen(4));			//��ȡauif����  ��auif

    return 0;

}

/*
************************************************************
*	�������ƣ�	Info_Alter
*
*	�������ܣ�	����wifi��Ϣ����Ŀ��Ϣ
*
*	��ڲ�����	��Ҫ������ֶ�
*
*	���ز�����	������
*
*	˵����		0-����Ҫ��������		1-��Ҫ��������
************************************************************
*/
_Bool Info_Alter(char *info)
{
    
    char *usart1Tmp;
    unsigned char usart1Count = 0;
	_Bool flag = 0;
        
	if((usart1Tmp = strstr(info, "DEVID:")) != (void *)0)								//��ȡdevid
	{
		usart1Count = Info_CountLen(usart1Tmp);											//���㳤��
        if(usart1Count > 0)
        {
            memset(onenet_info.dev_id, 0, sizeof(onenet_info.dev_id));						//���֮ǰ������
            strncpy(onenet_info.dev_id, usart1Tmp + 6, usart1Count);
            UsartPrintf(USART_DEBUG, "Tips:	Save DEVID: %s\r\n", onenet_info.dev_id);

			AT24C02_WriteByte(DEVID_ADDRESS, strlen(onenet_info.dev_id));					//����devid����
			RTOS_TimeDly(2);
			AT24C02_WriteBytes(DEVID_ADDRESS + 1,										//����devid
								(unsigned char *)onenet_info.dev_id,
								strlen(onenet_info.dev_id));
            
            flag = 1;
        }
	}
        
	if((usart1Tmp = strstr(info, "APIKEY:")) != (void *)0)								//��ȡapikey
	{
		usart1Count = Info_CountLen(usart1Tmp);											//���㳤��
        if(usart1Count > 0)
        {
            memset(onenet_info.api_key, 0, sizeof(onenet_info.api_key));					//���֮ǰ������
            strncpy(onenet_info.api_key, usart1Tmp + 7, usart1Count);
            UsartPrintf(USART_DEBUG, "Tips:	Save APIKEY: %s\r\n", onenet_info.api_key);

			AT24C02_WriteByte(AKEY_ADDRESS, strlen(onenet_info.api_key));					//����apikey����
			RTOS_TimeDly(2);
			AT24C02_WriteBytes(AKEY_ADDRESS + 1,										//����apikey
								(unsigned char *)onenet_info.api_key,
								strlen(onenet_info.api_key));
            
            flag = 1;
        }
	}
	
	if((usart1Tmp = strstr(info, "PROID:")) != (void *)0)								//��ȡproID
	{
		usart1Count = Info_CountLen(usart1Tmp);											//���㳤��
        if(usart1Count > 0)
        {
            memset(onenet_info.pro_id, 0, sizeof(onenet_info.pro_id));						//���֮ǰ������
            strncpy(onenet_info.pro_id, usart1Tmp + 6, usart1Count);
            UsartPrintf(USART_DEBUG, "Tips:	Save PROID: %s\r\n", onenet_info.pro_id);

			AT24C02_WriteByte(PROID_ADDRESS, strlen(onenet_info.pro_id));					//����proID����
			RTOS_TimeDly(2);
			AT24C02_WriteBytes(PROID_ADDRESS + 1,										//����proID
								(unsigned char *)onenet_info.pro_id,
								strlen(onenet_info.pro_id));
            
            flag = 1;
        }
	}
	
	if((usart1Tmp = strstr(info, "AUIF:")) != (void *)0)								//��ȡauif
	{
		usart1Count = Info_CountLen(usart1Tmp);											//���㳤��
        if(usart1Count > 0)
        {
            memset(onenet_info.auif, 0, sizeof(onenet_info.auif));						//���֮ǰ������
            strncpy(onenet_info.auif, usart1Tmp + 5, usart1Count);
            UsartPrintf(USART_DEBUG, "Tips:	Save AUIF: %s\r\n", onenet_info.auif);

			AT24C02_WriteByte(AUIF_ADDRESS, strlen(onenet_info.auif));					//����auif����
			RTOS_TimeDly(2);
			AT24C02_WriteBytes(AUIF_ADDRESS + 1,										//����auif
								(unsigned char *)onenet_info.auif,
								strlen(onenet_info.auif));
            
            flag = 1;
        }
	}
	
	return flag;

}
