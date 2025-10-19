#ifndef __FLASH_H__
#define __FLASH_H__

#include "stdio.h"
#include "stdlib.h"
#include "stm32f4xx_hal.h"

#include "memory_layout.h"
#ifdef __cplusplus
extern "C" {
#endif
void flash_erase_sector(uint8_t sector);
void flash_write_dword(uint32_t addr, uint64_t data);
uint32_t flash_write(uint32_t addr, uint8_t *data, size_t sz);
uint32_t flash_addr(uint16_t sector);
#ifdef __cplusplus
}
#endif

#endif /* __FLASH_H__ */