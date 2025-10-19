/*
 * usb_com.c
 *
 *  Created on: April 28, 2025
 *      Author: Dat Hoang
 */
#include "usb_com.h"
#include "stdlib.h"
#include "usbd_cdc_if.h"
#include "flash.h"
#include "cJSON.h"
#include "util.h"
#include "basic.h"
#include "stdbool.h"

void usb_transmit(uint8_t *tx_buffer, uint16_t tx_len)
{
	// printf("Transmitting Frame to USB: ");
	CDC_Transmit_FS(tx_buffer, tx_len);
}

/*
	{
	"key": "c1281b6a24cc467bbc85c4c28eba5c8cf9b4a25919c3fb1a959e7be9fb862177",
	"nonce": "000102030405060708090a0b"
	}
*/

#define MAX_JSON_SIZE 256
#define JSON_START_MARKER '{'
#define JSON_END_MARKER '}'

static uint8_t json_buffer[MAX_JSON_SIZE];
static uint16_t json_buffer_pos = 0;
static bool json_receiving = false;

static void store_key_nonce(uint8_t* json, uint32_t len)
{
	crypto_info_t info = {};
    cJSON *response_json = cJSON_Parse((const char*)json);
    if(response_json != NULL){
      cJSON* key_item = cJSON_GetObjectItem(response_json, "key");
      if(cJSON_IsString(key_item) && (key_item->valuestring != NULL)){
        // printf("key: %s\n", key_item->valuestring);
		int key_len = hex_string_to_bytes(key_item->valuestring, info.key, sizeof(info.key));	
      }
      cJSON* nonce_item = cJSON_GetObjectItem(response_json, "nonce");
      if(cJSON_IsString(nonce_item) && (nonce_item->valuestring != NULL)){
        // printf("nonce: %s\n", nonce_item->valuestring);
		int nonce_len = hex_string_to_bytes(nonce_item->valuestring, info.nonce, sizeof(info.nonce));
      }
    //   printf("Storing crypto info to flash...\n");
	  store_crypto_info(&info);
    }
    //check again
    if(!response_json){
      cJSON_Delete(response_json);
    }
}

void USB_CDC_RxHandler(uint8_t* Buf, uint32_t Len)
{
    printf("Received %lu bytes from USB: ", Len);
    for(uint32_t i = 0; i < Len; i++) {
        printf("%c", Buf[i]);
    }
    printf("\n");
    
    for(uint32_t i = 0; i < Len; i++) {
        char current_char = Buf[i];
        
        // Check for JSON start
        if(current_char == JSON_START_MARKER && !json_receiving) {
            printf("JSON start detected\n");
            json_receiving = true;
            json_buffer_pos = 0;
            json_buffer[json_buffer_pos++] = current_char;
        }
        // If we're receiving JSON, continue collecting
        else if(json_receiving) {
            if(json_buffer_pos < MAX_JSON_SIZE - 1) {
                json_buffer[json_buffer_pos++] = current_char;
                
                // Check for JSON end
                if(current_char == JSON_END_MARKER) {
                    printf("JSON end detected\n");
                    json_buffer[json_buffer_pos] = '\0'; // Null terminate
                    
                    printf("Complete JSON received (%d bytes): %s\n", json_buffer_pos, json_buffer);
                    
                    // Process complete JSON
                    store_key_nonce(json_buffer, json_buffer_pos);
                    
                    // Reset for next JSON
                    json_receiving = false;
                    json_buffer_pos = 0;
                    break;
                }
            }
            else {
                printf("JSON buffer overflow! Resetting...\n");
                json_receiving = false;
                json_buffer_pos = 0;
            }
        }
    }
}