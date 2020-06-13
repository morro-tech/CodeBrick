/*******************************************************************************
* @brief	模块管理(包含系统初始化,时间片轮询系统)
*
* Change Logs: 
* Date           Author       Notes 
* 2016-06-24     Morro        初始版
* 2020-05-23     Morro        增加匿名类型,防止模块重名错误
*******************************************************************************/
#include "module.h"
 
/*
 * 根据编译器特性得到初始化及任务数据段的开始/结束地址
 */
#ifdef __CC_ARM                			 /* ARM Compiler 	*/	
    extern task_item_t        section_task_item$$Base;           
    extern task_item_t        section_task_item$$Limit;		
	#define task_tbl_start    &section_task_item$$Base    
	#define task_tbl_end      &section_task_item$$Limit 
	
    extern module_init_item_t section_sys_init$$Base;
    extern module_init_item_t section_sys_init$$Limit;		
	#define sys_init_start    &section_sys_init$$Base    
	#define sys_init_end      &section_sys_init$$Limit 
	
#elif defined (__ICCARM__)        		/* for IAR Compiler */
	#pragma section="section_task_item"
	#define task_tbl_start  __section_begin("section_task_item")        
	#define task_tbl_end    __section_end("section_task_item")  
	
	#pragma section="section_sys_init"
	#define sys_init_start  __section_begin("section_sys_init")
	#define sys_init_end    __section_end("section_sys_init")
    
	#pragma section="section_drv_init"
	#define drv_init_start  __section_begin("section_drv_init")
	#define drv_init_end    __section_end("section_drv_init")    
    
	#pragma section="section_app_init"
	#define app_init_start  __section_begin("section_app_init")
	#define app_init_end    __section_end("section_app_init")     

#endif


static volatile unsigned int tick;               //系统滴答计时

/*
 * @brief       增加系统节拍数(定时器中断中调用)
 */
void systick_increase(void)
{
	tick++;
}

/*
 * @brief       获取系统滴答时钟值(通常单位是1ms)
 */
unsigned int get_tick(void)
{
	return tick;
}


/*
 * @brief       模块初始处理
 *              初始化模块优化级 system_init > driver_init > module_init
 * @param[in]   none
 * @return      none
 */
void module_task_init(void)
{
    module_init_item_t *m;
    for (m = (module_init_item_t *)sys_init_start;
         m < (module_init_item_t *)sys_init_end; m++) {    //系统模块初始化
        m->init();
    } 
    for (m = (module_init_item_t *)drv_init_start;
         m < (module_init_item_t *)drv_init_end; m++) {    //驱动模块初始化
        m->init();
    } 
    for (m = (module_init_item_t *)app_init_start;
         m < (module_init_item_t *)app_init_end; m++) {    //应用模块初始化
        m->init();
    }     
}
 
/*
 * @brief       基于时间轮询任务调度
 * @param[in]   none
 * @return      none
 */
void module_task_poll(void)
{
    task_item_t *t;
    for (t = (task_item_t *)task_tbl_start; 
         t < (task_item_t*)task_tbl_end; t++) {      
        if  ((get_tick() - *t->timer) >= t->interval) {
            *t->timer = get_tick();
            t->handle();
        }
    } 
}





		
		
		