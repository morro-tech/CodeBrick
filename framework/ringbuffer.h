/******************************************************************************
 * @brief    环形缓冲区管理(参考linux/kfifo)
 *
 * Copyright (c) 2016~2020, <morro_luo@163.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs: 
 * Date           Author       Notes 
 * 2016-05-30     Morro        初版完成
 ******************************************************************************/

#ifndef _RING_BUF_H_
#define _RING_BUF_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*环形缓冲区管理器*/
typedef struct {
     unsigned char *buf;    /*环形缓冲区        */
     unsigned int size;     /*环形缓冲区        */
     unsigned int front;    /*头指针            */
     unsigned int rear;     /*尾指针            */
}ring_buf_t;

bool ring_buf_init(ring_buf_t *r,unsigned char *buf,unsigned int size);

void ring_buf_clr(ring_buf_t *r);

unsigned int ring_buf_len(ring_buf_t *r);

unsigned int ring_buf_put(ring_buf_t *r,unsigned char *buf,unsigned int len);

unsigned int ring_buf_get(ring_buf_t *r,unsigned char *buf,unsigned int len);


#ifdef __cplusplus
}
#endif

#endif
