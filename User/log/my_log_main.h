/*******************************************************

* 文件名：my_log_main.h

* 作者：  luyangguang

* 描述： 调试日记输出

* 修改人：〈卢阳光〉

* 修改时间：2021-04-24

* 修改内容：创建
*******************************************************/

/*******************  头文件包含 ***********************/
/******************************************************/

#ifndef __MY_LOG_MAIN_H_
#define	__MY_LOG_MAIN_H_

#include "my_com.h"
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

/******************************************************/
/*******************  全局变量定义 *********************/
/******************************************************/

/******************************************************/
/*******************  数据类型定义 *********************/
/******************************************************/
typedef enum
{
	LOG_DEBUG = 1,
	LOG_WARN,
  LOG_ERROR,
	LOG_FATAL
}my_log_level_enmu;

static  my_log_level_enmu my_log_level = LOG_DEBUG;

//#define LOG(level,prefix,format,...) do{ if(my_log_level <= level) my_pre_log(prefix,"%s:%d(%s)\""format"\"\n",__FILE__,__LINE__,__FUNCTION__,##__VA_ARGS__);}while(0)

//使用不带文件名的，简洁一点
#define LOG(level,prefix,format,...) do{ if(my_log_level <= level) my_pre_log(prefix,"%s(): "format"\n",__FUNCTION__,##__VA_ARGS__);}while(0)

#define LOG_DEBUG(fmt,...) LOG(LOG_DEBUG,"[DEBUG]",fmt,##__VA_ARGS__)
#define LOG_WARN(fmt,...)  LOG(LOG_WARN, "[WARN]", fmt,##__VA_ARGS__)
#define LOG_ERROR(fmt,...) LOG(LOG_ERROR,"[ERROR]",fmt,##__VA_ARGS__)
#define LOG_FATAL(fmt,...) LOG(LOG_FATAL,"[FATAL]",fmt,##__VA_ARGS__)

/******************************************************/
/*******************  外部变量引用 *********************/
/******************************************************/

/******************************************************/
/*******************  本地函数声明 *********************/
/******************************************************/

/******************************************************/
/*******************  外部函数引用 *********************/
/******************************************************/




#endif /* __MY_LOG_MAIN_H_ */