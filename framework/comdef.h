/*******************************************************************************
* @brief	通用宏定义
*
* Change Logs: 
* Date           Author       Notes 
* 2018-03-18     魔罗卜士     Initial version. 
*******************************************************************************/

#ifndef _COM_DEF_H_
#define _COM_DEF_H_

#ifdef __cplusplus
extern "C" {
#endif

/*匿名类型定义 -----------------------------------------------------------*/
#define  ANONY_CONN(type, var, line)  type  var##line
#define  ANONY_DEF(type,prefix,line)  ANONY_CONN(type, prefix, line)
#define  ANONY_TYPE(type,prefix)      ANONY_DEF(type,prefix, __LINE__)

/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:	the pointer to the member.
 * @type:	the type of the container struct this is embedded in.
 * @member:	the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) ( \
	(type *)( (char *)(ptr) - offsetof(type,member) ))


#ifdef __CC_ARM                          /* ARM Compiler    */
    #define SECTION(x)                  __attribute__((section(x)))
    #define UNUSED                      __attribute__((unused))
    #define RESERVE                     
#elif defined (__ICCARM__)              /* for IAR Compiler */
    #define SECTION(x)                  @ x
    #define UNUSED
    #define RESERVE                     __root
#elif defined (__GNUC__)                /* GNU GCC Compiler */
    #define SECTION(x)                  __attribute__((section(x)))
    #define UNUSED                      __attribute__((unused))
    #define RESERVE 
#endif

#ifdef __cplusplus
}
#endif

#endif
