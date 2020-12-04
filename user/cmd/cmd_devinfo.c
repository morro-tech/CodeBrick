/******************************************************************************
 * @brief    设备相关命令
 *
 * Copyright (c) 2020, <morro_luo@163.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs: 
 * Date           Author       Notes 
 * 2020/07/11     Morro        
 ******************************************************************************/
#include "cli.h"
#include "config.h"
#include "public.h"
#include <stdio.h>
/* 
 * @brief       显示系统信息
 */ 
static int do_sysinfo_handler(struct cli_obj *o, int argc, char *argv[])
{
    printf("|*************************************************************|\r\n");  
    printf("|                            \\\\\\|///                          |\r\n");
    printf("|                            \\- - -/                          |\r\n");
    printf("|                           (  @ @  )                         |\r\n");
    printf("|  +----------------------o00o-(_)-o00o---------------------+ |\r\n");
    printf("|  |                                                        | |\r\n");
    printf("|  | Project Name :        CODEBRICK                        | |\r\n");
    printf("|  |--------------------------------------------------------| |\r\n");
    printf("|  | Author      :  Morro                                   | |\r\n");
    printf("|  | Contact     :  <morro_luo@163.com>                 | |\r\n");
    printf("|  |                                                        | |\r\n");
    printf("|  |                               Oooo                     | |\r\n");  
    printf("|  +---------------------- oooO---(   )---------------------+ |\r\n");
    printf("|                          (   )   ) /                        |\r\n");
    printf("|                           \\ (   (_/                         |\r\n");
    printf("|                            \\_)                              |\r\n");
    printf("|*************************************************************|\r\n\r\n");
    
    printf( "Build in %s %s\r\n",__DATE__,__TIME__);
    printf( "Software version: %s\r\n", SW_VER);
    printf("System clock: %d hz\r\n", SystemCoreClock);   
    return 1;
}cmd_register("sysinfo", do_sysinfo_handler, "show system infomation.");

/* 
 * @brief       复位命令
 */ 
int do_cmd_reset(struct cli_obj *o, int argc, char *argv[])
{
    NVIC_SystemReset();
    return 0;
}cmd_register("reset",do_cmd_reset, "reset system");
