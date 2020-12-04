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
#include "key.h"
#include <stddef.h>

static key_t *keyhead = NULL;                              /*按键链表头结点*/

/*******************************************************************************
 * @brief       创建一个按键
 * @param[in]   key     - 按键管理器
 * @param[in]   readkey - 按键触发测试函数指针
 * @param[in]   event   - 按键事件处理函数指针
 * @return      true    - 创建失败, false - 创建成功
 ******************************************************************************/
bool key_create(key_t *key, int (*readkey)(void), 
               void (*event)(int type, unsigned int duration))
{
    key_t *keytail = keyhead;
    if (key == NULL || readkey == NULL || event == NULL)
        return 0;    
    key->event    = event;
    key->readkey  = readkey;
    key->next     = NULL;
    if (keyhead == NULL) {
        keyhead = key;
        return 1;
    }
    while (keytail->next != NULL)                          /*转至链尾*/
        keytail = keytail->next;
    keytail->next = key;
    return 1;
}

/*******************************************************************************
 * @brief       按键扫描处理
 * @return      none
 ******************************************************************************/
void key_scan_process(void)
{
    key_t *k;
    for (k = keyhead; k != NULL; k = k->next) {        
        if (k->readkey()) {
            if (k->tick) {
                if (is_timeout(k->tick, LONG_PRESS_TIME))    /*长按判断 */   
                    k->event(KEY_LONG_DOWN, get_tick() - k->tick);           
            } else {
                k->tick = get_tick();                        /*记录首次按下时间*/
            }
        } else if (k->tick) {              
            if (is_timeout(k->tick, LONG_PRESS_TIME)) {      /*长按释放 */            
                k->event(KEY_LONG_UP, get_tick() - k->tick);
            }
            
            /*短按释放操作 ---------------------------------------------------*/
            if (is_timeout(k->tick, KEY_DEBOUNCE_TIME) && 
                !is_timeout(k->tick, LONG_PRESS_TIME)) {
                k->event(KEY_PRESS, get_tick() - k->tick);
            }
            
            k->tick = 0;
        }        
    }
}

