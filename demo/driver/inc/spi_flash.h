/******************************************************************************
 * @brief    SPI flash 驱动(M25PE16)
 *
 * Copyright (c) 2017~2020, <master_roger@sina.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs: 
 * Date           Author       Notes 
 * 2017-08-01     Morro        初版
 ******************************************************************************/
#ifndef	_SPI_FLASH_H
#define	_SPI_FLASH_H

#include <stdbool.h>

#define MAX_FLASH_PAGE_SIZE      0x100
#define MAX_FLASH_SECTOR_SIZE    0x1000
#define MAX_FLASH_BLOCK_SIZE     0x10000
#define MAX_FLASH_SIZE           4 * 1024 * 1024           /*4M*/

/*flash 命令表 ---------------------------------------------------------------*/
#define CMD_WREN		    0x06
#define CMD_WRDI		    0x04
#define CMD_RDSR		    0x05
#define CMD_READ_ID		    0x9F
#define CMD_WRSR		    0x01
#define CMD_WRLR		    0xE5
#define CMD_RDLR		    0xE8
#define CMD_READ		    0x03

#define CMD_FAST_READ		0x0B
#define CMD_RDSFDB		    0x5A
#define CMD_RES		        0xAB   //device ID
#define CMD_REMS		    0x90   //manufacturer ID\ device ID
#define CMD_DREAD		    0x3B 

#define CMD_PAGE_PROG		0x02
#define CMD_PAGE_ERASE		0xDB
#define CMD_PAGE_WRITE		0x0A

#define CMD_BLOCK_64k_ERASE 0xD8
#define CMD_BLOCK_32K_ERASE	0x52
#define CMD_BLOCK_ERASE		CMD_BLOCK_64k_ERASE	//For M25PE16, it is the command of sector erase

#define CMD_SECTOR_ERASE	0x20	//For M25PE16, it is the command of subsector erase
#define CMD_CHIP_ERASE		0x60	//For M25PE16, it is the same meaning of bulk erase
#define CMD_DEEP_PWDN		0xB9
#define CMD_RELS_PWDN		0xAB

/*flash 信息 -----------------------------------------------------------------*/
typedef struct {
	unsigned char manufacture;
	unsigned char type;
	unsigned char capacity;
}flash_info_t;                      
    
void spi_flash_init(void);
void spi_flash_get_info(flash_info_t *info);

bool spi_flash_write(unsigned int addr, const void *buf ,unsigned int len);
bool spi_flash_read(unsigned int addr, void *buf ,unsigned int len);
bool spi_flash_sector_erase(unsigned int addr);
bool spi_flash_block_erase(unsigned int addr);
bool spi_flash_chip_erase(void);

#endif


