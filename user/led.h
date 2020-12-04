/******************************************************************************
 * @brief    led控制
 *
 * Copyright (c) 2020, <morro_luo@163.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs: 
 * Date           Author       Notes 
 * 
 ******************************************************************************/
#ifndef _LED_H_
#define _LED_H_

/*led工作模式 ---------------------------------------------------------------*/
#define LED_MODE_OFF     0                            /*常灭*/
#define LED_MODE_ON      1                            /*常亮*/
#define LED_MODE_FAST    2                            /*快闪*/
#define LED_MODE_SLOW    3                            /*慢闪*/

/*led类型 --------------------------------------------------------------------*/
typedef enum {
    LED_TYPE_RED = 0,
    LED_TYPE_GREEN,
    LED_TYPE_BLUE,
    LED_TYPE_MAX
}led_type;

void led_ctrl(led_type type, int mode, int reapeat);

#endif
