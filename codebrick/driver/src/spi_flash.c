/******************************************************************************
 * @brief    SPI flash 驱动
 *
 * Copyright (c) 2017~2020, <master_roger@sina.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs: 
 * Date           Author       Notes 
 * 2017-08-01     Morro        初版
 ******************************************************************************/
#include "stm32f4xx.h"
#include "spi_flash.h"
#include "public.h"
#include <string.h>
#include <intrinsics.h>

/* flash 状态字---------------------------------------------------------------*/
typedef union  {
	unsigned char status;
	struct {
		unsigned char WIP	: 1;
		unsigned char WEL	: 1;
		unsigned char BP0	: 1;
		unsigned char BP1	: 1;
		unsigned char BP2	: 1;
		unsigned char BP3	: 1;
		unsigned char WPDIS	: 1;		
		unsigned char SRP	: 1;
	};
}flash_status_t;

/*片选控制*/
void spi_flash_diselect(void)
{
    GPIO_SetBits(GPIOB, GPIO_Pin_10); 
    __no_operation();
}
void spi_flash_select(void)
{
    GPIO_ResetBits(GPIOB, GPIO_Pin_10); 
    __no_operation();
}
//spi写
static unsigned char spi_write_byte(unsigned char byte)
{      
    bool timeout = 10000;
    while((SPI_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) && (timeout--));
        SPI_SendData(SPI1, byte);
    timeout = 10000;
    while((SPI_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)&& (timeout--));      
        return SPI_ReceiveData(SPI1);
}

//spi读
static unsigned char spi_read_byte(void)
{
    return (spi_write_byte(0x55));
}

/*-----------------------------------------------------------------------------*/
/*端口配置PB3->SPI1_CLK, PB4->SPI1_MISO,PB5->SPI1->MOSI,PB10->SPI1_CS*/
static void spi_port_init(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    gpio_conf(GPIOB, GPIO_Mode_AF, GPIO_PuPd_NOPULL, GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);
    
    /*片选信号配置*/
    gpio_conf(GPIOB, GPIO_Mode_OUT, GPIO_PuPd_UP, GPIO_Pin_10);
 
}

/*
 * @brief	    SPI 初始化(84/4 = 21Mhz)
 * @param[in]   none
 * @return 	    none
 */
static void spi_init(void)
{
    SPI_InitTypeDef  SPI_InitStructure;    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);  
    SPI_DeInit(SPI1);
    spi_flash_diselect();    
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		                //设置SPI工作模式:设置为主SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		            //设置SPI的数据大小:SPI发送接收8位帧结构
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		                    //选择了串行时钟的稳态:时钟悬空高
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	                    //数据捕获于第二个时钟沿
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		                    //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;	//定义波特率预分频的值:波特率预分频值为4
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	                //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI_InitStructure.SPI_CRCPolynomial = 7;	                        //CRC值计算的多项式
    SPI_Init(SPI1, &SPI_InitStructure);                                 //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
    SPI_Cmd(SPI1, ENABLE);                                              //使能SPI外设
    spi_flash_diselect();
}
/*
 * @brief	    SPI flash 初始化
 * @param[in]   none
 * @return 	    none
 */
void spi_flash_init(void)
{
    spi_port_init();
    spi_init();
}
//读状态
static flash_status_t read_flash_status(void)
{
    flash_status_t status;
    spi_flash_select() ;
    spi_write_byte(CMD_RDSR);
    status.status = spi_read_byte();		
    spi_flash_diselect() ;
    return status;
}
//等待flash空闲
static bool wait_complete(void)
{
    int retry;
    for (retry = 0; retry < 30000; retry++) {
        if (!read_flash_status().WIP)
          return true;        
    }	
    return false;
}
//发送地址
static void send_addr(unsigned int addr)
{
    spi_write_byte(addr >> 16);
    spi_write_byte(addr >> 8);
    spi_write_byte(addr);
}
/*发送写使能*/
static void send_write_enable(void)
{
	spi_flash_select() ;
	spi_write_byte(CMD_WREN);
	spi_flash_diselect();
}

/*
 * @brief       flash 写使能
 * @param[in]   none
 * @return      none
 */
static bool write_enable(void)
{
	int retry;     
	for (retry = 0; retry < 10; retry++) {
        send_write_enable(); 
        if (read_flash_status().WEL)
            return true;        
    }  
	return false;
}

/*
 * @brief       获取flash的硬件信息(manufacture,type,capacity etc.)
 * @param[in]   info - flash信息
 * @return      none
 */
void spi_flash_get_info(flash_info_t *info)
{         
	spi_flash_select();
	spi_write_byte(CMD_READ_ID);
    info->manufacture = spi_read_byte();
    info->type        = spi_read_byte();
    info->capacity    = spi_read_byte();
	spi_flash_diselect();
}

/*
 * @brief       页编程
 * @param[in]   addr  - 物理地址
 * @param[in]   buf   - 缓冲区指针
 * @param[in]   len   - 缓冲区数据长度(<256,否则将覆盖前面已经写入的数据)
 * @return      true - 执行成功, false - 执行失败
 */
static bool flash_PageProg(unsigned int addr, unsigned char *buf, unsigned int len)
{   
	if (len == 0 || buf == NULL)
        return false;
    if(addr >= MAX_FLASH_SIZE)
        return false;		
	spi_flash_select() ;
	spi_write_byte(CMD_PAGE_PROG);                        /*发送写命令 ----------*/
	send_addr(addr);	
	while (len--)
		spi_write_byte(*buf++);
	spi_flash_diselect();	
	return wait_complete();

}
/*
 * @brief       flash 写(写入的区域必须保存已经擦除为ff)
 * @param[in]   addr  - 物理地址
 * @param[in]   buf   - 缓冲区指针
 * @param[in]   len   - 缓冲区数据长度(<256,否则将覆盖前面已经写入的数据)
 * @return      true - 执行成功, false - 执行失败
 */
bool spi_flash_write(unsigned int addr, const void *buf ,unsigned int len)
{
    unsigned char *p = (unsigned char *)buf;
	/*addr所在页剩余的空间(单位为字节)-----------------------------------------*/
	unsigned short page_left = MAX_FLASH_PAGE_SIZE - (addr % MAX_FLASH_PAGE_SIZE);
    if(addr >= MAX_FLASH_SIZE || !write_enable())
        return false;
	/*判断addr所在页是否可以容纳待写入的数据*/
	if (page_left >= len)  
		page_left = len;
	while (1) {
        if (!flash_PageProg(addr, p, page_left))
          return false;
		if (len ==  page_left)
            break;    
		else {
			addr += page_left;		/*偏移到下一页*/
			p    += page_left;     //数据地址增
			len  -= page_left;    			
			page_left = (len > MAX_FLASH_PAGE_SIZE) ? MAX_FLASH_PAGE_SIZE : len;
		}
	}
    return true;
}

/*
 * @brief       flash 读
 * @param[in]   addr  - 物理地址
 * @param[out]  buf   - 缓冲区指针
 * @param[in]   len   - 读取字节数
 * @return      true - 执行成功, false - 执行失败
 */
bool spi_flash_read(unsigned int addr, void *buf ,unsigned int len)
{
    unsigned char *p = (unsigned char *)buf;
    if(buf == NULL || len == 0)
        return false;
	spi_flash_select();
	spi_write_byte(CMD_READ);
	send_addr(addr);
	while (len--)
		*p++ = spi_read_byte();
	spi_flash_diselect();
	return true;
}

/*
 * @brief       扇区擦除
 * @param[in]   addr  - 扇区所在地址
 * @return      true - 执行成功, false - 执行失败
 */
bool spi_flash_sector_erase(unsigned int addr)
{
	if(addr >= MAX_FLASH_SIZE) 
        return false;	

	spi_flash_select();
	spi_write_byte(CMD_SECTOR_ERASE);
	send_addr(addr);
	spi_flash_diselect();
	return wait_complete();
}

/*
 * @brief       扇区擦除
 * @param[in]   addr  - 块所在地址
 * @return      true - 执行成功, false - 执行失败
 */
bool spi_flash_block_erase(unsigned int addr)
{
	if(addr >= MAX_FLASH_SIZE) 
      return false;	
    
	spi_flash_select() ;
	spi_write_byte(CMD_BLOCK_ERASE);
	send_addr(addr);
	spi_flash_diselect();    
    return wait_complete();
}

/*
 * @brief       芯片擦除
 * @param[in]   addr  - 块所在地址
 * @return      true - 执行成功, false - 执行失败
 */
bool spi_flash_chip_erase(void)
{		
	spi_flash_select();    
	spi_write_byte(CMD_CHIP_ERASE);
	spi_flash_diselect();
	return wait_complete();
}
