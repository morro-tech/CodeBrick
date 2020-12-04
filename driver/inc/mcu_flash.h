/******************************************************************************
 * @brief    ST mcu Æ¬ÉÏflash²Ù×÷
 *
 * Copyright (c) 2018~2020, <morro_luo@163.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs: 
 * Date           Author       Notes 
 * 2018-09-09     Morro         Initial version
 ******************************************************************************/
#ifndef _MCU_FLASH_H_
#define _MCU_FLASH_H_

#include <stddef.h>

int mcu_flash_erase(unsigned int addr, size_t size);
int mcu_flash_write(unsigned int addr ,const void *buf, size_t size);
int mcu_flash_read(unsigned int addr ,void *buf, size_t size);

#endif
