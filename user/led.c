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
#include "blink.h"
#include "module.h"
#include "public.h"
#include "led.h"

static blink_dev_t led[LED_TYPE_MAX];               /*定义led设备 ------------*/

/* 
 * @brief       红色LED控制(PB4)
 * @return      none
 */ 
static void red_led_ctrl(bool level)
{
    if (level)
        GPIOC->ODR |= 1 << 4;
    else 
        GPIOC->ODR &= ~(1 << 4);
}

/* 
 * @brief       绿色LED控制(PB5)
 * @return      none
 */ 
static void green_led_ctrl(bool level)
{
    if (level)
        GPIOC->ODR |= 1 << 5;
    else 
        GPIOC->ODR &= ~(1 << 5);    
}

/* 
 * @brief       蓝色LED控制(PB6)
 * @return      none
 */ 
static void blue_led_ctrl(bool level)
{
    if (level)
        GPIOB->ODR |= 1 << 6;
    else 
        GPIOB->ODR &= ~(1 << 6);    
}
/* 
 * @brief       led io初始化
 *              PB4 -> red; PB5 -> green; PB6-> blue;
 * @return      none
 */ 
static void led_io_init(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);
    
    gpio_conf(GPIOB, GPIO_Mode_OUT, GPIO_PuPd_NOPULL, GPIO_Pin_4);
    gpio_conf(GPIOB, GPIO_Mode_OUT, GPIO_PuPd_NOPULL, GPIO_Pin_5);
    gpio_conf(GPIOB, GPIO_Mode_OUT, GPIO_PuPd_NOPULL, GPIO_Pin_6);
    
    blink_dev_create(&led[LED_TYPE_RED], red_led_ctrl);
    blink_dev_create(&led[LED_TYPE_GREEN], green_led_ctrl);
    blink_dev_create(&led[LED_TYPE_BLUE], blue_led_ctrl);

    /*开机闪3下*/
    led_ctrl(LED_TYPE_RED, LED_MODE_FAST, 3);
    led_ctrl(LED_TYPE_GREEN, LED_MODE_FAST, 3);
    led_ctrl(LED_TYPE_BLUE, LED_MODE_FAST, 3);
    
}

/* 
 * @brief       led控制
 * @param[in]   type    - led类型(LED_TYPE_XXX)
 * @param[in]   mode    - 工作模式(LED_MODE_XXX)
 * @param[in]   reapeat - 闪烁次数,0表示无限循环
 * @return      none
 */ 
void led_ctrl(led_type type, int mode, int reapeat)
{
    int ontime = 0, offtime = 0;
    
    switch (mode) {                     /*根据工作模式得到led开关周期 ---------*/
    case LED_MODE_OFF:
        ontime = offtime = 0;
        break;
    case LED_MODE_ON:
        ontime  = 1;
        offtime = 0;
        break;
    case LED_MODE_FAST:
        ontime  = 100;
        offtime = 100;
        break;
    case LED_MODE_SLOW:
        ontime  = 500;
        offtime = 1000;
        break;        
    }
    blink_dev_ctrl(&led[type], ontime, offtime, reapeat);
}


driver_init("led", led_io_init);                     /*驱动初始化*/
task_register("led", blink_dev_process, 10);         /*led任务, 10ms轮询1次*/
