/******************************************************************************
 * @brief    tty串口打印驱动
 *
 * Copyright (c) 2020, <master_roger@sina.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs: 
 * Date           Author       Notes 
 *
 ******************************************************************************/

#ifndef	_TTY_H_
#define	_TTY_H_

#define TTY_RXBUF_SIZE		 256
#define TTY_TXBUF_SIZE		 1024

/*接口声明 --------------------------------------------------------------------*/
typedef struct {
    void (*init)(int baudrate);                                    
    unsigned int (*write)(const void *buf, unsigned int len);    
    unsigned int (*read)(void *buf, unsigned int len);
}tty_t;

extern const  tty_t tty;

#endif
