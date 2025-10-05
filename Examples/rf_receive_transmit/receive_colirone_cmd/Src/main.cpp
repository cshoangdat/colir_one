#include "main.h"
#include "../../ColirOne/Inc/colir_one.h"
#include "../../Crypto/ChaCha.h"

#define MAX_DATA_SIZE 32

uint8_t key[] = {
  0xC1, 0x28, 0x1B, 0x6A, 0x24, 0xCC, 0x46, 0x7B, 0xBC, 0x85,
  0xC4, 0xC2, 0x8E, 0xBA, 0x5C, 0x8C, 0xF9, 0xB4, 0xA2, 0x59,
  0x19, 0xC3, 0xFB, 0x1A, 0x95, 0x9E, 0x7B, 0xE9, 0xFB, 0x86,
  0x21, 0x77
};

uint8_t nonce[] = {
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
  0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B
};

const uint8_t RxAddress[] = {0xAA,0xDD,0xCC,0xBB,0xAA};

static bool decryptData(const uint8_t key[32], const uint8_t* input, uint8_t* output, size_t dataSize) {
    if (dataSize == 0 || dataSize > 32) {
        return false;
    }
    
    ChaCha chaCha;
    chaCha.setKey(key, 32);
    chaCha.setIV(nonce, 12);
    
    chaCha.decrypt(output, input, dataSize);
    return true;
}

int main(void){
  	ColirOne colirOne;
  	colirone_err_t err = colirOne.init();
    if(err != COLIRONE_OK){
        printf("Error initializing colirone: %d\n", err);
        return -1;
    }
    colirOne.rf.setRxAdress((uint8_t*)RxAddress);
    colirOne.rf.setRxMode();
    uint8_t rcv_data[32] = {0}; // Buffer to hold received data
    colirone_payload_cmd_t cmd = {};
	while(1){
        if(colirOne.rf.hasReceivedData()){
            colirOne.rf.getReceivedData(rcv_data);
            printf("Received data: ");
            for(int i = 0; i < 32; i++){
                printf("%02X ", rcv_data[i]);
            }
            printf("\n");
            uint8_t decryptedData[MAX_DATA_SIZE];
            bool res = decryptData(key, rcv_data, decryptedData, MAX_DATA_SIZE);
            if(res){
                printf("Decrypted data: ");
                for(int i = 0; i < sizeof(cmd); i++){
                    printf("%02X ", decryptedData[i]);
                }
                printf("\n");
            }
            else{
                printf("Decryption failed\n");
            }
            memcpy(&cmd, decryptedData, sizeof(cmd));
            printf("Lighter: %d, OpenShutes: %d, CloseShutes: %d, StartLogs: %d, WriteLogs: %d, ResetAltitude: %d, RemoveLogs: %d\n",
                   cmd.lighterLaunchNumber,
                   cmd.openShutes,
                   cmd.closeShutes,
                   cmd.startLogs,
                   cmd.writeLogs,
                   cmd.resetAltitude,
                   cmd.removeLogs);
        }
	}
	return 0;
}