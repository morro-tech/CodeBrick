/******************************************************************************
 * @brief    led����
 *
 * Copyright (c) 2020, <master_roger@sina.com>
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

static blink_dev_t led[LED_TYPE_MAX];               /*����led�豸 ------------*/

/* 
 * @brief       ��ɫLED����(PB4)
 * @return      none
 */ 
static void red_led_ctrl(bool level)
{
    if (level)
        GPIOB->ODR |= 1 << 4;
    else 
        GPIOB->ODR &= ~(1 << 4);
}

/* 
 * @brief       ��ɫLED����(PB5)
 * @return      none
 */ 
static void green_led_ctrl(bool level)
{
    if (level)
        GPIOB->ODR |= 1 << 5;
    else 
        GPIOB->ODR &= ~(1 << 5);    
}

/* 
 * @brief       ��ɫLED����(PB6)
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
 * @brief       led io��ʼ��
 *              PB4 -> red; PB5 -> green; PB6-> blue;
 * @return      none
 */ 
static void led_io_init(void)
{
    gpio_conf(GPIOB, GPIO_Mode_OUT, GPIO_PuPd_NOPULL, GPIO_Pin_4);
    gpio_conf(GPIOB, GPIO_Mode_OUT, GPIO_PuPd_NOPULL, GPIO_Pin_5);
    gpio_conf(GPIOB, GPIO_Mode_OUT, GPIO_PuPd_NOPULL, GPIO_Pin_6);
    
    blink_dev_create(&led[LED_TYPE_RED], red_led_ctrl);
    blink_dev_create(&led[LED_TYPE_GREEN], green_led_ctrl);
    blink_dev_create(&led[LED_TYPE_BLUE], blue_led_ctrl);
}

/* 
 * @brief       led����
 * @param[in]   type    - led����(LED_TYPE_XXX)
 * @param[in]   mode    - ����ģʽ(LED_MODE_XXX)
 * @param[in]   reapeat - ��˸����,0��ʾ����ѭ��
 * @return      none
 */ 
void led_ctrl(led_type type, int mode, int reapeat)
{
    int ontime, offtime;
    
    switch (mode) {                     /*���ݹ���ģʽ�õ�led�������� ---------*/
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


driver_init("led", led_io_init);                     /*������ʼ��*/
task_register("led", blink_dev_process, 100);        /*led����, 100ms��ѯ1��*/