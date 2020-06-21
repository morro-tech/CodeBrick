/*******************************************************************************
* @brief         具有闪烁特性(led, motor, buzzer)的设备(led, motor, buzzer)管理
*
* Change Logs: 
* Date           Author       Notes 
* 2019-04-01     Morro        Initial version. 
*******************************************************************************/
#ifndef _BLINK_H_
#define _BLINK_H_

#include "framework.h"
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif

/*Blink 设备定义*/
typedef struct {
    unsigned short ontime;                                 
    unsigned short offtime;
    unsigned short  repeats;
    unsigned char  count;
    unsigned char  enable;    
    unsigned int   tick;
    void (*ioctrl)(bool enable);
}blink_dev_t;

#define blink_dev_register(name, ioctrl) \
        (blink_dev_t name) \
        SECTION("__section_blink_dev") UNUSED = \
        {0,0,0,0,ioctrl}  

void blink_dev_ctrl(blink_dev_t *dev, int ontime, int offtime, int repeat);
void blink_dev_process(void);

#ifdef __cplusplus
}
#endif

#endif
