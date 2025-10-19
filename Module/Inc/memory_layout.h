#ifndef __MEMORY_LAYOUT_H__
#define __MEMORY_LAYOUT_H__

#include "flash.h"

#define FLASH_SECTOR_SIZE_0_3   16 * 1024
#define FLASH_SECTOR_SIZE_4     64 * 1024
#define FLASH_SECTOR_SIZE_5_7   128 * 1024
#define SECTORS                 8

#define SECRET_SECTOR       (SECTORS - 1)
#define SECRET_ADDRESS      (flash_addr(SECRET_SECTOR))

#endif