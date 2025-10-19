/*
 * util.c
 *
 *  Created on: April 28, 2025
 *      Author: Dat Hoang
 */
#include <stdint.h>
#include <stdio.h>
#include "string.h"

void dump_hex(uint8_t * buf, int size)
{
    while(size--)
    {
        printf("%02x ", *buf++);
    }
    printf("\n");
}

/**
 * @brief Convert hex character to its numeric value
 * @param c: hex character (0-9, a-f, A-F)
 * @return: numeric value (0-15) or -1 if invalid
 */
static int hex_char_to_value(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    } else if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    }
    return -1; // Invalid hex character
}

/**
 * @brief Convert hex string to byte array
 * @param hex_string: input hex string (e.g., "c1281b6a...")
 * @param output: output byte array
 * @param max_output_len: maximum length of output array
 * @return: number of bytes converted, or -1 if error
 */
int hex_string_to_bytes(const char* hex_string, uint8_t* output, uint16_t max_output_len) {
    if (!hex_string || !output) {
        return -1;
    }
    
    uint16_t hex_len = strlen(hex_string);
    
    // Check if hex string length is even
    if (hex_len % 2 != 0) {
        printf("Error: Hex string length must be even\n");
        return -1;
    }
    
    uint16_t byte_count = hex_len / 2;
    
    // Check if output buffer is large enough
    if (byte_count > max_output_len) {
        printf("Error: Output buffer too small\n");
        return -1;
    }
    
    // Convert each pair of hex characters to a byte
    for (uint16_t i = 0; i < byte_count; i++) {
        int high_nibble = hex_char_to_value(hex_string[i * 2]);
        int low_nibble = hex_char_to_value(hex_string[i * 2 + 1]);
        
        if (high_nibble == -1 || low_nibble == -1) {
            printf("Error: Invalid hex character at position %d\n", i * 2);
            return -1;
        }
        
        output[i] = (high_nibble << 4) | low_nibble;
    }
    
    return byte_count;
}
