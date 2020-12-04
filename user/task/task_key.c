/******************************************************************************
 * @brief    按键任务(演示key模块使用)
 *
 * Copyright (c) 2020, <morro_luo@163.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs: 
 * Date           Author       Notes 
 * 2020/07/03     roger.luo    
 ******************************************************************************/
#include "led.h"
#include "key.h"
#include "module.h"
#include "public.h"

static key_t key;                                    /*按键定义*/

static void key_event(int type, unsigned int duration);

/* 
 * @brief       读取按键电平状态
 * @return      0 | 1
 */ 
static int readkey(void)
{
    return GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4) == 0;
}

/* 
 * @brief       按键 io初始化
 *              PC4 -> key;
 * @return      none
 */ 
static void key_io_init(void)
{
    gpio_conf(GPIOC, GPIO_Mode_IN, GPIO_PuPd_UP, GPIO_Pin_4);
    key_create(&key, readkey, key_event);            /*创建按键*/
}

/* 
 * @brief       按键事件处理
 * @return      type - 
 */
static void key_event(int type, unsigned int duration)
{
    if (type == KEY_PRESS)
        led_ctrl(LED_TYPE_GREEN, LED_MODE_FAST, 3);  /*短按,绿灯闪3下*/
    else if (type == KEY_LONG_DOWN)
        led_ctrl(LED_TYPE_GREEN, LED_MODE_ON, 0);    /*长按,绿灯常亮*/
    else if (type == KEY_LONG_UP)
        led_ctrl(LED_TYPE_GREEN, LED_MODE_OFF, 0);   /*长按后释放,绿灯关闭*/
}

driver_init("key", key_io_init);                     /*按键初始化*/
task_register("key", key_scan_process, 20);          /*按键扫描任务, 20ms轮询1次*/
