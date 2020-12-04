/******************************************************************************
 * @brief        异步作业
 *
 * Copyright (c) 2020, <morro_luo@163.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs: 
 * Date           Author       Notes 
 * 2020-09-22     Morro        Initial version. 
 ******************************************************************************/
#ifndef _ASYNC_WORK_H_
#define _ASYNC_WORK_H_

#include "qlink.h"
#include <stdbool.h>

/*异步作业管理器 -------------------------------------------------------------*/
typedef struct {
    struct qlink idle;                             /*空闲队列*/
    struct qlink ready;                            /*就绪队列*/
}async_work_t;

typedef void (*async_work_func_t)(async_work_t *w, void *object, void *params);

/*异步作业结点 ---------------------------------------------------------------*/
typedef struct {
    void *object;                                  /*作业对象*/
    void *params;                                  /*作业参数*/    
    async_work_func_t work;                        /*作业入口*/
    struct qlink_node node;                        /*链表节点*/
}work_node_t;

void async_work_init(async_work_t *w, work_node_t *nodes, int max_nodes);

bool async_work_add(async_work_t *w, void *object, void *params, 
                    async_work_func_t work);

void async_work_process(async_work_t *w);

#endif
