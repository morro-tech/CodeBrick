/******************************************************************************
 * @brief    具有闪烁特性(dev, motor, buzzer)的设备(dev, motor, buzzer)管理
 *
 * Copyright (c) 2019, <morro_luo@163.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs: 
 * Date           Author       Notes 
 * 2019-04-01     Morro        Initial version
 ******************************************************************************/
#include "module.h"                      /*get_tick()*/
#include "blink.h"                       
#include <stddef.h>
#include <string.h>

static blink_dev_t *head = NULL;         /*头结点 -*/

/*
 * @brief       创建blink设备
 * @param[in]   dev    - 设备
 * @param[in]   ioctrl - IO控制函数
 * @return      none
 */
void blink_dev_create(blink_dev_t *dev, void (*ioctrl)(bool enable))
{
    blink_dev_t *tail = head;  
    memset(dev, 0, sizeof(blink_dev_t));
    dev->ioctrl = ioctrl;    
    dev->next = NULL;
    if (head == NULL) {
        head = dev;
        return;
    }
    while (tail->next != NULL)
        tail = tail->next;
    tail->next = dev;
}


/* 
 * @brief       blink 设备控制
 * @param[in]   name    - 设备名称
 * @param[in]   ontime - 开启时间(如果该值为0则永久关闭)
 * @param[in]   offtime- 关闭时间
 * @param[in]   repeats- 重复次数(0表示无限循环)
 * @return      none
 */
void blink_dev_ctrl(blink_dev_t *dev, int ontime, int offtime, int repeats)
{
    dev->ontime  = ontime;
    dev->offtime = offtime + ontime;                  
    dev->repeats = repeats;
    dev->tick    = get_tick();
    dev->count   = 0;
    if (ontime  == 0) {
        dev->ioctrl(false);
        dev->enable  = false;
    }        
}

/*
 * @brief       blink设备管理
 * @param[in]   none
 * @return      none
 */
void blink_dev_process(void)
{
    blink_dev_t *dev;
    for (dev = head; dev != NULL; dev = dev->next) {
        if (dev->ontime == 0) {
            continue;
        } else if (get_tick() - dev->tick < dev->ontime) {
            if (!dev->enable) {
                dev->enable = true;
                dev->ioctrl(true);
            }
        } else if(get_tick() - dev->tick < dev->offtime) {    /**/
            if (dev->enable) {
                dev->enable = false;
                dev->ioctrl(false);
            }
        } else {
            dev->tick = get_tick();
			if (dev->repeats) {
				if (++dev->count >= dev->repeats) {
					dev->ontime = 0;
					dev->ioctrl(false);
					dev->enable = false;
				}
			}
        }
    }
}
