/******************************************************************************
 * @brief    平台相关初始化
 *
 * Copyright (c) 2020, <master_roger@sina.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 ******************************************************************************/
#include "module.h"
#include "public.h"
#include "platform.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "tty.h"

/*******************************************************************************
* @brief	   系统滴答中断
* @param[in]   none
* @return 	   none
*******************************************************************************/
void SysTick_Handler(void)
{
    systick_increase(SYS_TICK_INTERVAL);
}

/*******************************************************************************
* @brief	   硬件驱动初始化
* @param[in]   none
* @return 	   none
*******************************************************************************/
static void bsp_init(void)
{    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  /*设置优先级分组 --------*/
    tty.init(115200);
    SystemCoreClockUpdate();
 	SysTick_Config(SystemCoreClock / SYS_TICK_INTERVAL);
	NVIC_SetPriority(SysTick_IRQn, 0);		         // Set SysTick IRQ priority
    
}system_init("bsp", bsp_init); 
