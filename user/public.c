/******************************************************************************
 * @brief    ST MCU 通用外设配置
 *
 * Copyright (c) 2019, <morro_luo@163.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 ******************************************************************************/
#include "public.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* 
 * @brief       GPIO配置
 * @return      none
 */ 
void gpio_conf(GPIO_TypeDef* GPIOx, GPIOMode_TypeDef Mode, GPIOPuPd_TypeDef PuPd,
               uint32_t Pins)
{
    GPIO_InitTypeDef config;
    config.GPIO_Pin   = Pins;
    config.GPIO_Mode  = Mode;
    config.GPIO_OType = GPIO_OType_PP;
    config.GPIO_Speed = GPIO_Speed_50MHz;
    config.GPIO_PuPd  = PuPd;
    GPIO_Init(GPIOx, &config);
}


/* 
 * @brief       中断配置
 * @param[in]   - Channel     通道号
 * @param[in]   - Priority    优先级
 * @param[in]   - SubPriority 次优先级
 * @return      none
 */ 
void nvic_conf(int Channel, int Priority, int SubPriority)
{
    NVIC_InitTypeDef config = {0};    
    config.NVIC_IRQChannel = Channel; 
    config.NVIC_IRQChannelPreemptionPriority = Priority;  
    config.NVIC_IRQChannelSubPriority = SubPriority; 
    config.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&config);
}


/* 
 * @brief       外部中断配置
 * @param[in]   - Line       中断号
 * @param[in]   - Trigger    触发器(上升沿/下降沿)
 * @return      none
 */
void exti_conf(uint32_t Line, EXTITrigger_TypeDef Trigger, FunctionalState Cmd)
{
    EXTI_InitTypeDef  config = {0};    
    config.EXTI_Line = Line;
    config.EXTI_Mode = EXTI_Mode_Interrupt;
    config.EXTI_Trigger = Trigger;
    config.EXTI_LineCmd = Cmd;
    EXTI_Init(&config);
    EXTI_ClearITPendingBit(Line);
}

/* 
 * @brief       定时器配置
 * @param[in]   - TIMx       定时器
 * @param[in]   - hz         计数频率
 * @return      none
 */
void timer_conf(TIM_TypeDef* TIMx, unsigned int hz)
{
    TIM_TimeBaseInitTypeDef  config = {0};
    float Period;
    RCC_ClocksTypeDef Clocks;
    TIM_DeInit(TIMx); 
    RCC_GetClocksFreq(&Clocks);
    /* Time base configuration */
    config.TIM_Prescaler = 1;
    if (TIMx == TIM1 || TIMx == TIM8 || TIMx == TIM9 || 
        TIMx == TIM10|| TIMx == TIM11)
        Period = (float)(Clocks.PCLK2_Frequency / hz + 0.5);     
    else 
        Period = (float)(Clocks.PCLK1_Frequency / hz + 0.5);   

    config.TIM_Period = (uint32_t)Period - 1;    
    config.TIM_ClockDivision = 0;
    config.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIMx, &config);       
}

/* 
 * @brief       串口配置
 * @param[in]   - port       串口号(USART[1..6])
 * @param[in]   - baudrate   波特率
 * @return      none
 */

void uart_conf(USART_TypeDef *port, int baudrate)
{
    USART_InitTypeDef config;
    USART_DeInit(port);
    config.USART_BaudRate = baudrate;  
    config.USART_WordLength = USART_WordLength_8b;
    config.USART_StopBits = USART_StopBits_1;
    config.USART_Parity = USART_Parity_No;
    config.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    config.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(port, &config);
    USART_ITConfig(port, USART_IT_RXNE, ENABLE);
 	USART_ITConfig(port, USART_IT_ERR, ENABLE);

	USART_Cmd(port, ENABLE);      

}
