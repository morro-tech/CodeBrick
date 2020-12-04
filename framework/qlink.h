/*******************************************************************************
* @brief	      链式队列(queue link)管理
*
* Copyright (c) 2017~2020, <morro_luo@163.com>
*
* SPDX-License-Identifier: Apache-2.0
* Change Logs 
* Date           Author       Notes 
* 2016-06-24     Morro        初始版
* 2018-03-17     Morro        增加队列元素统计功能
*******************************************************************************/
#ifndef _QLINK_H_
#define _QLINK_H_


#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h> 

/*链式队列结点 ---------------------------------------------------------------*/
struct qlink_node {
    struct qlink_node *next;
};

/*链式队列管理器 -------------------------------------------------------------*/
struct qlink {
    unsigned int  count;
    struct qlink_node *front, *rear;        
};

/*******************************************************************************
 * @brief       初始化链式队列
 * @param[in]   q     - 队列管理器
 * @return      none
 ******************************************************************************/
static inline void qlink_init(struct qlink *q)
{
    q->front = q->rear= NULL;
    q->count = 0;
}

/*******************************************************************************
 * @brief       入队操作
 * @param[in]   q     - 队列管理器
 * @return      nond
 ******************************************************************************/
static inline void qlink_put(struct qlink *q, struct qlink_node *n)
{   
    if (q->count == 0)
        q->front = n;
    else 
        q->rear->next = n;
    q->rear = n;
    n->next = NULL; 
    q->count++;
}

/*******************************************************************************
 * @brief       预出队操作(获取队首结点)
 * @param[in]   q     - 队列管理器
 * @return      nond
 ******************************************************************************/
static inline struct qlink_node *qlink_peek(struct qlink *q)
{
    return q->front;
}

/*******************************************************************************
 * @brief       出队操作
 * @param[in]   q     - 队列管理器
 * @return      nond
 ******************************************************************************/
static inline struct qlink_node *qlink_get(struct qlink *q)
{
    struct qlink_node *n;
    if (q->count == 0)
        return NULL;
    n = q->front;
    q->front = q->front->next;
    q->count--;
    return n;
}

/*******************************************************************************
 * @brief       队列元素个数
 * @param[in]   q     - 队列管理器
 * @return      nond
 ******************************************************************************/
static inline int qlink_count(struct qlink *q)
{
    return q->count;
}

#ifdef __cplusplus
}
#endif

#endif
