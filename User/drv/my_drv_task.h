/*******************************************************

* 文件名：my_drv_task.h

* 作者：  luyangguang

* 描述： 底层驱动任务头文件

* 修改人：〈卢阳光〉

* 修改时间：2021-04-24

* 修改内容：创建
*******************************************************/

/*******************  头文件包含 ***********************/
/******************************************************/

#ifndef __MY_DRV_TASK_H_
#define	__MY_DRV_TASK_H_

#include "my_com.h"
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


/******************************************************/
/*******************  全局变量定义 *********************/
/******************************************************/


/******************************************************/
/*******************  数据类型定义 *********************/
/******************************************************/

/******************************************************/
/*******************  外部变量引用 *********************/
/******************************************************/

/******************************************************/
/*******************  本地函数声明 *********************/
/******************************************************/

/******************************************************
* 函数名 : my_drv_task_init
* 函数功能描述 : drv任务初始化
* 函数参数 : 
* 函数返回值 : 
* 作者 : luyangguang
* 版本 : 1.0
* 历史版本 : 无
*******************************************************/
void my_drv_task_init( void * pvParameters );

/******************************************************
* 函数名 : my_drv_task_get_queuue_handle
* 函数功能描述 : 获取drv任务的队列句柄
* 函数参数 : 
* 函数返回值 : 
* 作者 : luyangguang
* 版本 : 1.0
* 历史版本 : 无
*******************************************************/
QueueHandle_t my_drv_task_get_queuue_handle(void);

/******************************************************/
/*******************  外部函数引用 *********************/
/******************************************************/


























#endif /* __MY_DRV_TASK_H_ */
