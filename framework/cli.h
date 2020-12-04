/******************************************************************************
 * @brief    命令行处理
 *
 * Copyright (c) 2015-2020, <morro_luo@163.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs: 
 * Date           Author       Notes 
* 2015-06-09      Morro        初版
*                             
* 2017-07-04      Morro        优化字段分割处理
* 
* 2020-07-05      Morro        使用cli_obj_t对象, 支持多个命令源处理
 ******************************************************************************/
#ifndef _CMDLINE_H_
#define _CMDLINE_H_

#include "comdef.h"

#define CLI_MAX_CMD_LEN           64             /*命令行长度*/                 
#define CLI_MAX_ARGS              16             /*最大参数个数*/
#define CLI_MAX_CMDS              32             /*最大允许定义的命令个数*/

struct cli_obj;

/*命令项定义*/
typedef struct {
	char	   *name;		                         /*命令名*/
    /*命令处理程序*/   
	int        (*handler)(struct cli_obj *o, int argc, char *argv[]);   
    const char *brief;                               /*命令简介*/
}cmd_item_t;

#define __cmd_register(name,handler,brief)\
    USED ANONY_TYPE(const cmd_item_t,__cli_cmd_##handler)\
    SECTION("cli.cmd.1") = {name, handler, brief}
    
/*******************************************************************************
 * @brief     命令注册
 * @params    name      - 命令名
 * @params    handler   - 命令处理程序
 *            类型:int (*handler)(struct cli_obj *s, int argc, char *argv[]);   
 * @params    brief     - 使用说明
 */                 
#define cmd_register(name,handler,brief)\
    __cmd_register(name,handler,brief)

/*cli 接口定义 -------------------------------------------------------------*/
typedef struct {
    unsigned int (*write)(const void *buf, unsigned int len);
    unsigned int (*read) (void *buf, unsigned int len);
}cli_port_t;

/*命令行对象*/
typedef struct cli_obj {
    unsigned int (*write)(const void *buf, unsigned int len);
    unsigned int (*read) (void *buf, unsigned int len);
    void         (*print)(struct cli_obj *this, const char *fmt, ...); 
    char           recvbuf[CLI_MAX_CMD_LEN + 1];  /*命令接收缓冲区*/
    unsigned short recvcnt;                       /*最大接收长度*/    
    unsigned       enable : 1;
}cli_obj_t;

void cli_init(cli_obj_t *obj, const cli_port_t *p);

void cli_enable(cli_obj_t *obj);

void cli_disable (cli_obj_t *obj);

void cli_exec_cmd(cli_obj_t *obj, const char *cmd);

void cli_process(cli_obj_t *obj);



#endif	/* __CMDLINE_H */
