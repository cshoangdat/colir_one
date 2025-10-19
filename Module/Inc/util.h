/*
 * util.h
 *
 *  Created on: April 28, 2025
 *      Author: Dat Hoang
 */
#ifndef __UTIL_H__
#define __UTIL_H__

#include "stdio.h"
#ifdef __cplusplus
extern "C" {
#endif
void dump_hex(uint8_t * buf, int size);
int hex_string_to_bytes(const char* hex_string, uint8_t* output, uint16_t max_output_len);
#ifdef __cplusplus
}
#endif
#endif