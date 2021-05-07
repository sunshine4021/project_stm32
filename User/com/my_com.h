#ifndef __MY_COM_H_
#define	__MY_COM_H_




typedef unsigned char uint8_t; 
typedef signed char int8_t; 

typedef unsigned short uint16_t; 
typedef signed short int16_t;

typedef unsigned int uint32_t; 
typedef signed  int int32_t;

typedef unsigned long long int uint64_t; 
typedef signed long long int int64_t;




typedef struct  
{
    uint16_t    mes_id;     //消息id
    int32_t     param_1;    //参数1
	  int32_t     param_2;    //参数2
	  int32_t     param_3;    //参数3
	  uint32_t    data_len;	  //数据长度
    void *      pvdata;     //数据指针，初始化主动赋值为NULL
}my_task_message_struct;





#endif /* __MY_COM_H_ */
