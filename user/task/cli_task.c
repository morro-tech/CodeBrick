/******************************************************************************
 * @brief    命令行任务
 *
 * Copyright (c) 2020, <morro_luo@163.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs: 
 * Date           Author       Notes 
 * 2020/07/11     Morro        
 ******************************************************************************/
#include "module.h"
#include "tty.h"
#include "cli.h"

static cli_obj_t cli;                               /*命令行对象 */

/* 
 * @brief       命令行任务初始化
 * @return      none
 */ 
static void cli_task_init(void)
{
    cli_port_t p = {tty.write, tty.read};           /*读写接口 */
    
    cli_init(&cli, &p);                             /*初始化命令行对象 */
     
    cli_enable(&cli);
    
    cli_exec_cmd(&cli,"sysinfo");                   /*显示系统信息*/
}

/* 
 * @brief       命令行任务处理
 * @return      none
 */ 
static void cli_task_process(void)
{
    cli_process(&cli);
}

module_init("cli", cli_task_init);                  
task_register("cli", cli_task_process, 10);          /*注册命令行任务*/
