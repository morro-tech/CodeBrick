/******************************************************************************
 * @brief    独立按键管理
 *
 * Copyright (c) 2017~2020, <morro_luo@163.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs: 
 * Date           Author       Notes 
 * 2017-08-10     Morro        Initial version
 ******************************************************************************/

#ifndef _KEY_H_
#define _KEY_H_

#include "module.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LONG_PRESS_TIME     1500                   /*长按确认时间 ------------*/
#define KEY_DEBOUNCE_TIME   20                     /*按键消抖时间 ------------*/

#define KEY_PRESS           0                      /*短按 --------------------*/
#define KEY_LONG_DOWN       1                      /*长按按下 ----------------*/
#define KEY_LONG_UP         2                      /*长按释放 ----------------*/

/*按键管理器 -----------------------------------------------------------------*/
typedef struct key_t {
    /*
     *@brief     按键触发读取接口
     *@param[in] none
     *@return    0  - 按键未按下, 非0 - 按键按下
     */
    int  (*readkey)(void);  
    /*
     *@brief     按键事件触发处理
     *@param[in] type - 事件类型(KEY_SHORT_PRESS - 短按, KEY_LONG_PRESS - 长按)
     *@param[in] duration 持续时间,长按有效
     *@return    none
     */    
    void (*event)(int type, unsigned int duration);/*按键事件处理 ------------*/
    unsigned int tick;                             /*滴答计时器 --------------*/
    struct key_t *next;                            /*连接下一个按键并构成链表 */
}key_t;

bool key_create(key_t *key, int (*readkey)(void),  /*创建按键*/
               void (*event)(int type, unsigned int duration));
void key_scan_process(void);                       /*按键扫描处理*/

#ifdef __cplusplus
}
#endif

#endif
