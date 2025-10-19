#ifndef __BASIC_H__
#define __BASIC_H__

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "flash.h"
#include "memory_layout.h"

#define MAX_DEVICE_ID_LENGTH 64
#define MAX_SERIAL_NUMBER_LENGTH 64

// Magic number to validate stored data
#define DEVICE_INFO_MAGIC 0xDEADBEEF
#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    uint8_t key[32];         // 256-bit encryption key
    uint8_t nonce[12];       // 96-bit nonce
    uint32_t magic_number;   // Validation magic number
    uint32_t crc;            // CRC for data integrity
} crypto_info_t;

uint32_t calculate_crc(const uint8_t* data, size_t length);
void load_crypto_info(crypto_info_t* info);
void store_crypto_info(crypto_info_t* info);
uint8_t is_device_info_valid(crypto_info_t* info);
void remove_crypto_info(void);

#ifdef __cplusplus
}
#endif

#endif /* __BASIC_H__ */