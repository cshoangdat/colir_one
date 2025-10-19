#include "basic.h"
#include "flash.h"
#include "cJSON.h"

// Simple CRC calculation for data integrity
uint32_t calculate_crc(const uint8_t* data, size_t length) {
    uint32_t crc = 0xFFFFFFFF;
    for (size_t i = 0; i < length; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xEDB88320;
            } else {
                crc >>= 1;
            }
        }
    }
    return ~crc;
}

uint8_t is_device_info_valid(crypto_info_t* info) {
    if (info->magic_number != DEVICE_INFO_MAGIC) {
        return 0;
    }
    
    // Calculate CRC excluding the CRC field itself
    uint32_t calculated_crc = calculate_crc((uint8_t*)info, sizeof(crypto_info_t) - sizeof(uint32_t));
    return (calculated_crc == info->crc);
}

void load_crypto_info(crypto_info_t* info) {
    memcpy(info, (crypto_info_t*)(SECRET_ADDRESS), sizeof(crypto_info_t));
    
    // Validate data
    if (!is_device_info_valid(info)) {
        printf("[WARNING] Device info invalid or corrupted, initializing defaults\n");
        memset(info, 0, sizeof(crypto_info_t));
        info->magic_number = DEVICE_INFO_MAGIC;
        info->crc = calculate_crc((uint8_t*)info, sizeof(crypto_info_t) - sizeof(uint32_t));
    } else {
        printf("[INFO] Device info loaded successfully\n");
        printf("key: ");
        for(int i = 0; i < 32; i++) {
            printf("%02X, ", info->key[i]);
        }
        printf("\n");
        printf("nonce: ");
        for(int i = 0; i < 12; i++) {
            printf("%02X, ", info->nonce[i]);
        }
        printf("\n");
    }
}

void store_crypto_info(crypto_info_t* info) {
    // Set magic number and calculate CRC
    info->magic_number = DEVICE_INFO_MAGIC;
    info->crc = calculate_crc((uint8_t*)info, sizeof(crypto_info_t) - sizeof(uint32_t));

    printf("[INFO] Storing device info to flash at 0x%08lX\n", SECRET_ADDRESS);
    
    flash_write(SECRET_ADDRESS, (uint8_t*)info, sizeof(crypto_info_t));
    
    printf("[INFO] Device info stored successfully\n");
}

void remove_crypto_info(void) {
    printf("[INFO] Removing device info from flash\n");
    flash_erase_sector(SECRET_SECTOR);
}