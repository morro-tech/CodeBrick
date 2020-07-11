/******************************************************************************
 * @brief    设备相关操作
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

#include <stdbool.h>

#define SYS_TICK_INTERVAL                   10             /*系统滴答间隔(ms) */    

bool is_timeout(unsigned int start, unsigned int timeout);

#endif
