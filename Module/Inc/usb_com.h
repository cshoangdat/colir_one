/*
 * usb_com.h
 *
 *  Created on: April 28, 2025
 *      Author: Dat Hoang
 */
#ifndef __USB_COM_H__
#define __USB_COM_H__

#include <stdint.h>
#include <stdio.h>

#define USB_DEBUG(log_msg) (printf("USB_Debug: %s\n", log_msg))

typedef struct {
    char device_id[64];
    char serial_number[64];
} secret_info_t;

#ifdef __cplusplus
extern "C" {
#endif
void usb_transmit(uint8_t *tx_buffer, uint16_t tx_len);
void USB_CDC_RxHandler(uint8_t*, uint32_t);
#ifdef __cplusplus
}
#endif

#endif