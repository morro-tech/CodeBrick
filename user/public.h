/******************************************************************************
 * @brief    ST MCU 通用外设配置
 *
 * Copyright (c) 2019, <morro_luo@163.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 ******************************************************************************/
#ifndef _PUBLIC_H_
#define _PUBLIC_H_

#include "stm32f4xx.h"

void gpio_conf(GPIO_TypeDef* GPIOx, GPIOMode_TypeDef Mode,GPIOPuPd_TypeDef PuPd,
               uint32_t Pins);

void nvic_conf(int Channel, int Priority, int SubPriority);

void exti_conf(uint32_t Line, EXTITrigger_TypeDef Trigger, FunctionalState Cmd);

void timer_conf(TIM_TypeDef* TIMx, unsigned int hz);

void uart_conf(USART_TypeDef *port, int baudrate);

#endif
