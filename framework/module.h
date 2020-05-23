/*******************************************************************************
* @brief	模块管理(包含系统初始化,时间片轮询系统)
*
* Change Logs: 
* Date           Author       Notes 
* 2016-06-24     魔罗卜士     Initial version. 
*******************************************************************************/

#ifndef _MODULE_H_
#define _MODULE_H_

#include "comdef.h"

/*模块初始化项*/
typedef struct {
    const char *name;               //模块名称
    void (*init)(void);             //初始化接口
}module_init_item_t;

/*任务处理项*/
typedef struct {
    const char *name;               //模块名称    
    void (*handle)(void);           //初始化接口
    unsigned int interval;          //轮询间隔
    unsigned int *timer;            //指向定时器指针
}task_item_t;

#define __module_initialize(name,func,section_name)     \
RESERVE ANONY_TYPE(const module_init_item_t,func)       \
SECTION("section_sys_init") UNUSED = {name,func}

/*
 * @brief       任务注册
 * @param[in]   name    - 任务名称 
 * @param[in]   handle  - 初始化处理(void func(void){...})
  * @param[in]  interval- 轮询间隔(ms)
 */
#define task_register(name, handle,interval)           \
    RESERVE static unsigned int __task_timer_##handle; \
    RESERVE ANONY_TYPE(const task_item_t,handle)       \
    SECTION("section_task_item") UNUSED =              \
    {name,handle, interval, &__task_timer_##handle}

/*
 * @brief       模块初始化注册
 * @param[in]   name    - 模块名称 
 * @param[in]   func    - 初始化入口函数(void func(void){...})
 */
#define system_init(name,func)  __module_initialize(name,func,"section_sys_init")
#define driver_init(name,func)  __module_initialize(name,func,"section_drv_init")
#define module_init(name,func)  __module_initialize(name,func,"section_app_init")

void systick_increase(void);
unsigned int get_tick(void);
void module_task_init(void);
void module_task_poll(void);

    
#endif
