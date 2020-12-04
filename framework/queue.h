/******************************************************************************
 * @brief    通用队列管理
 *
 * Copyright (c) 2017~2020, <morro_luo@163.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs: 
 * Date           Author       Notes 
 * 2017-03-19     Morro        Initial version. 
 ******************************************************************************/
#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void *base;                                  /*队列容器基地址*/
    unsigned short element_size;                 /*队列元素大小*/
    unsigned short size;                         /*队列容器可容纳元素个数*/
    unsigned short front;                        /*队首*/
    unsigned short rear;                         /*队尾*/
}queue_t;

bool queue_create(queue_t *q,  void *container, int size, int element_size); 
bool queue_peek  (queue_t *q,  void **element);
bool queue_put   (queue_t *q,  void *element);
bool queue_get   (queue_t *q,  void *element);
bool queue_del   (queue_t *q);
void queue_clr   (queue_t *q);
int  queue_len   (queue_t *q);

#ifdef __cplusplus
}
#endif

#endif
