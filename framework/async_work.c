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
#include "async_work.h"
#include "comdef.h"
#include <stdbool.h>

/*******************************************************************************
 * @brief       作业入队
 * @param[in]   qlink     - 工作链
 * @param[in]   new_item -  工作项
 * @return      none
 ******************************************************************************/
static inline void workqueue_put(struct qlink *q, work_node_t *n)
{
	qlink_put(q, &n->node);                                  /*加入到就绪链表*/
}


/*******************************************************************************
 * @brief       作业出队
 * @param[in]   qlink     - 工作链
 * @return      工作项
 ******************************************************************************/
static inline work_node_t *workqueue_get(struct qlink *q)
{
	struct qlink_node *n = qlink_get(q);
	return n ? container_of(n, work_node_t, node) : NULL;
}

/*******************************************************************************
 * @brief       作业预出队
 * @param[in]   qlink     - 工作链
 * @return      工作项
 ******************************************************************************/
static inline work_node_t * workqueue_peek(struct qlink *q)
{
	struct qlink_node *n = qlink_peek(q);
	return n ? container_of(n, work_node_t, node) : NULL;
}


/*
 * @brief       异常作业初始化
 * @param[in]   w        - 作业管理器
 * @param[in]   node_tbl - 作业节点表
 * @param[in]   count    - node_tbl个数
 */
void async_work_init(async_work_t *w, work_node_t *node_tbl, int count)
{
    qlink_init(&w->idle);
    qlink_init(&w->ready);
    while (count--) {
        qlink_put(&w->idle, &node_tbl->node);
        node_tbl++;
    }
}

/*
 * @brief       增加作业到队列
 * @param[in]   w        - 作业管理器
 * @param[in]   params   - 作业参数
 * @param[in]   work     - 作业入口
 */
bool async_work_add(async_work_t *w, void *object, void *params, 
                    async_work_func_t work)
{
    work_node_t *n = workqueue_get(&w->idle);
    if (n == NULL)
        return false;
    n->object = object;
    n->params = params;l;;;;;;;;;;;;;;;;;;;;;,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
    n->work   = work;
    
    workqueue_put(&w->ready, n);               /*加入到就绪链表*/          
    return true;
}


/*
 * @brief       异步作业处理
 * @param[in]   w         - 作业管理器
 */
void async_work_process(async_work_t *w)
{
    work_node_t *n;
    if ((n = workqueue_get(&w->ready)) == NULL)
        return;
    n->work(w, n->object, n->params);
    workqueue_put(&w->idle, n);
}

