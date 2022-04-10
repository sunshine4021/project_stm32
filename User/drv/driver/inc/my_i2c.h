#ifndef _MY_I2C_H_
#define _MY_I2C_H_


//单片机相关组件
#include "my_mcu_gpio.h"


#define IIC_OK		0

#define IIC_Err		1


//SDA		PB11
//SCL		PB10
#define SDA_H	i2c_gpio_list[1].gpio_group->BSRR = i2c_gpio_list[1].gpio_pin
#define SDA_L	i2c_gpio_list[1].gpio_group->BRR = i2c_gpio_list[1].gpio_pin
#define SDA_R	MCU_GPIO_Input_Read(i2c_gpio_list[1].gpio_name)

#define SCL_H	i2c_gpio_list[0].gpio_group->BSRR = i2c_gpio_list[0].gpio_pin
#define SCL_L	i2c_gpio_list[0].gpio_group->BRR = i2c_gpio_list[0].gpio_pin


extern const GPIO_LIST i2c_gpio_list[2];


typedef struct
{

	unsigned short speed;

} IIC_INFO;

extern IIC_INFO iic_info;


void IIC_Init(void);

void IIC_SpeedCtl(unsigned short speed);

_Bool I2C_WriteByte(unsigned char slaveAddr, unsigned char regAddr, unsigned char *byte);

_Bool I2C_ReadByte(unsigned char slaveAddr, unsigned char regAddr, unsigned char *val);

_Bool I2C_WriteBytes(unsigned char slaveAddr, unsigned char regAddr, unsigned char *buf, unsigned char num);

_Bool I2C_ReadBytes(unsigned char slaveAddr, unsigned char regAddr, unsigned char *buf, unsigned char num);

void IIC_Start(void);

void IIC_Stop(void);

_Bool IIC_WaitAck(unsigned int timeOut);

void IIC_Ack(void);

void IIC_NAck(void);

void IIC_SendByte(unsigned char byte);

unsigned char IIC_RecvByte(void);


#endif
