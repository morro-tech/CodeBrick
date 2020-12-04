/******************************************************************************
 * @brief    系统模块管理(包含系统初始化,时间片轮询系统)
 *
 * Copyright (c) 2017~2020, <morro_luo@163.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs: 
 * Date           Author       Notes 
 * 2016-06-24     Morro        初版完成
 * 2020-05-23     Morro        增加匿名类型,防止模块重名错误
 * 2020-06-28     Morro        增加is_timeout超时判断接口
 * 2020-09-28     Morro        解决伪任务项未初始化timer，导致引用了空指针的问题！
 *                             
 ******************************************************************************/
#include "module.h"


static volatile unsigned int tick;               //系统滴答计时

/*
 * @brief   增加系统节拍数(定时器中断中调用,1ms 1次)
 */
void systick_increase(unsigned int ms)
{
	tick += ms;
}

/*
 * @brief       获取系统滴答时钟值(通常单位是1ms)
 */
unsigned int get_tick(void)
{
	return tick;
}

/*
 * @brief       超时判断
 * @param[in]   start   - 起始时间
 * @param[in]   timeout - 超时时间(ms)
 */
bool is_timeout(unsigned int start, unsigned int timeout)
{
    return get_tick() - start > timeout;
}

/*
 * @brief       空处理,用于定位段入口
 */
static void nop_process(void) {}
    
//第一个初始化项
const init_item_t init_tbl_start SECTION("init.item.0") = {     
    "", nop_process
};
//最后个初始化项
const init_item_t init_tbl_end SECTION("init.item.4") = {       
    "", nop_process
};

//第一个任务项
const task_item_t task_tbl_start SECTION("task.item.0") = {     
    "", nop_process
};
//最后个任务项
const task_item_t task_tbl_end SECTION("task.item.2") = {       
    "", nop_process
};

/*
 * @brief       模块初始处理
 *              初始化模块优化级 system_init > driver_init > module_init
 * @param[in]   none
 * @return      none
 */
void module_task_init(void)
{
    const init_item_t *it = &init_tbl_start;
    while (it < &init_tbl_end) {
        it++->init();
    }
}

/*
 * @brief       任务轮询处理
 * @param[in]   none
 * @return      none
 */
void module_task_process(void)
{
    const task_item_t *t;
    for (t = &task_tbl_start + 1; t < &task_tbl_end; t++) {
        if  ((get_tick() - *t->timer) >= t->interval) {
            *t->timer = get_tick();
            t->handle();
        }
    }
}
