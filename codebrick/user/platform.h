/******************************************************************************
 * @brief    �豸��ز���
 *
 * Copyright (c) 2020, <master_roger@sina.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs: 
 * Date           Author       Notes 
 *
 ******************************************************************************/

#ifndef _PLATFORM_DEVICE_H_
#define _PLATFORM_DEVICE_H_

#include "stm32f4xx.h"
#include "tty.h"
#include "spi_flash.h"
#include <stdbool.h>

#define SYS_TICK_INTERVAL                   10             /*ϵͳ�δ���(ms) */    

bool is_timeout(unsigned int start, unsigned int timeout);

#endif