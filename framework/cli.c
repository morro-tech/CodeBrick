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
#include "cli.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>

static const cmd_item_t cmd_tbl_start SECTION("cli.cmd.0") = {0};
static const cmd_item_t cmd_tbl_end SECTION("cli.cmd.4") = {0};    
/*
 * @brief       查找命令
 * @param[in]   keyword - 命令关键字
 * @return      命令项
 */ 
static const cmd_item_t *find_cmd(const char *keyword)
{                   
	const cmd_item_t *it;
    for (it = &cmd_tbl_start + 1; it < &cmd_tbl_end; it++) {
		if (!strcasecmp(keyword, it->name))
		    return it;
    }
	return NULL;
}

/*******************************************************************************
 * @brief      字符串分割  - 在源字符串查找出所有由separator指定的分隔符
 *                            (如',')并替换成字符串结束符'\0'形成子串，同时令list
 *                            指针列表中的每一个指针分别指向一个子串
 * @example 
 *             input=> s = "abc,123,456,,fb$"  
 *             separator = ",$"
 *            
 *             output=>s = abc'\0' 123'\0' 456'\0' '\0' fb'\0''\0'
 *             list[0] = "abc"
 *             list[1] = "123"
 *             list[2] = "456"
 *             list[3] = ""
 *             list[4] = "fb"
 *             list[5] = ""
 *
 * @param[in] str             - 源字符串
 * @param[in] separator       - 分隔字符串 
 * @param[in] list            - 字符串指针列表
 * @param[in] len             - 列表长度
 * @return    list指针列表项数，如上例所示则返回6
 ******************************************************************************/  
static size_t strsplit(char *s, const char *separator, char *list[],  size_t len)
{
    size_t count = 0;      
    if (s == NULL || list == NULL || len == 0) 
        return 0;     
        
    list[count++] = s;    
    while(*s && count < len) {       
        if (strchr(separator, *s) != NULL) {
            *s = '\0';                                       
            list[count++] = s + 1;                           /*指向下一个子串*/
        }
        s++;        
    }    
    return count;
}

/*
 *@brief 打印一个格式化字符串到串口控制台
 *@retval 
 */
static void cli_print(cli_obj_t *obj, const char *format, ...)
{
	va_list args;
    int len;
	char buf[CLI_MAX_CMD_LEN + CLI_MAX_CMD_LEN / 2];
	va_start (args, format);
	len = vsnprintf (buf, sizeof(buf), format, args);
    obj->write(buf, len);
}


/*
 * @brief       cli 初始化
 * @param[in]   p - cli驱动接口
 * @return      none
 */
void cli_init(cli_obj_t *obj, const cli_port_t *p)
{
    obj->read   = p->read;
    obj->write  = p->write;
    obj->print  = cli_print;
    obj->enable = true;
}

/*
 * @brief       进入cli命令模式(cli此时自动处理用户输入的命令)
 * @param[in]   none
 * @return      none
 **/
void cli_enable(cli_obj_t *obj)
{
    obj->enable = true;
}

/*
 * @brief       退出cli命令模式(cli此时不再处理用户输入的命令)
 * @param[in]   none
 * @return      none
 **/
void cli_disable (cli_obj_t *obj)
{
    obj->enable = false;
}

/*
 * @brief       处理行
 * @param[in]   line - 命令行
 * @return      none
 **/
static void process_line(cli_obj_t *obj)
{
    char *argv[CLI_MAX_ARGS];
    int   argc;    
    const cmd_item_t *it;
    argc = strsplit(obj->recvbuf, " ,",argv, CLI_MAX_ARGS);
    if ((it = find_cmd(argv[0])) == NULL) {
        obj->print(obj, "Unknown command '%s' - try 'help'\r\n", argv[0]);
        return;
    }
    it->handler(obj, argc, argv);
}

/*
 * @brief       执行一行命令(无论cli是否运行,都会执行)
 * @param[in]   none
 * @return      none
 **/
void cli_exec_cmd(cli_obj_t *obj, const char *cmd)
{
    snprintf(obj->recvbuf, CLI_MAX_CMD_LEN, "%s", cmd);
    process_line(obj);
}

/*
 * @brief       命令行处理程序
 * @param[in]   none
 * @return      none
 **/
void cli_process(cli_obj_t *obj)
{
    
    int i;
    if (!obj->read || !obj->enable)
        return;
    i = obj->recvcnt;
    obj->recvcnt += obj->read(&obj->recvbuf[i], CLI_MAX_CMD_LEN - i);
    while (i < obj->recvcnt) {
        if (obj->recvbuf[i] == '\r' || obj->recvbuf[i] == '\n') {    /*读取1行*/
            obj->recvbuf[i] = '\0';
            process_line(obj);
            obj->recvcnt = 0;
        }
        i++;
    }
}


/*******************************************************************************
* @brief	   命令比较器
* @param[in]   none
* @return 	   参考strcmp
*******************************************************************************/
static int cmd_item_comparer(const void *item1,const void *item2)
{
    cmd_item_t *it1 = *((cmd_item_t **)item1); 
    cmd_item_t *it2 = *((cmd_item_t **)item2);    
    return strcmp(it1->name, it2->name);
}

/*
 * @brief	   帮助命令
 */
static int do_help (struct cli_obj *s, int argc, char *argv[])
{
	int i,j, count;
    const cmd_item_t *item_start = &cmd_tbl_start + 1; 
    const cmd_item_t *item_end   = &cmd_tbl_end;	
	const cmd_item_t *cmdtbl[CLI_MAX_CMDS];
    
    if (argc == 2) {
        if ((item_start = find_cmd(argv[1])) != NULL) 
        {
            s->print(s, item_start->brief);                  /*命令使用信息----*/
            s->print(s, "\r\n");   
        }
        return 0;
    }
    for (i = 0; i < item_end - item_start && i < CLI_MAX_ARGS; i++)
        cmdtbl[i] = &item_start[i];
    count = i;
    /*对命令进行排序 ---------------------------------------------------------*/
    qsort(cmdtbl, i, sizeof(cmd_item_t*), cmd_item_comparer);    		        
    s->print(s, "\r\n");
    for (i = 0; i < count; i++) {
        s->print(s, cmdtbl[i]->name);                        /*打印命令名------*/
        /*对齐调整*/
        j = strlen(cmdtbl[i]->name);
        if (j < 10)
            j = 10 - j;
            
        while (j--)
            s->print(s, " ");
            
        s->print(s, "- "); 
		s->print(s, cmdtbl[i]->brief);                       /*命令使用信息----*/
        s->print(s, "\r\n");        
    }
	return 1;
}
 /*注册帮助命令 ---------------------------------------------------------------*/
cmd_register("help", do_help, "list all command.");   
cmd_register("?",    do_help, "alias for 'help'");

