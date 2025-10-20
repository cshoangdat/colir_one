#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <Crypto.h>
#include <ChaCha.h>
#include <RNG.h>
#include <string.h>

#define MAX_DATA_SIZE  32

//create an RF24 object
RF24 radio(4, 5);  // CE, CSN

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

uint8_t TxAddress[] = {0xAA,0xDD,0xCC,0xBB,0xAA};

static bool encryptData(const uint8_t key[32], const uint8_t* input, uint8_t* output, size_t dataSize){
    if (dataSize == 0 || dataSize > 32) {
        return false;
    }
    
    ChaCha chaCha;
    chaCha.setKey(key, 32);
    chaCha.setIV(nonce, 12);
    
    chaCha.encrypt(output, input, dataSize);
    return true;
}

void setup()
{
  Serial.begin(115200);
  
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.setRetries(5,5);
  radio.disableCRC();

  radio.openWritingPipe(TxAddress);
  radio.stopListening();
}

void loop()
{
  const char text[] = "Hello World";
  uint8_t encryptedData[MAX_DATA_SIZE];
  bool res = encryptData(key, (uint8_t*)text, encryptedData, MAX_DATA_SIZE);
  if(res){
    bool success = radio.write(&encryptedData, MAX_DATA_SIZE);
    if(success){
      Serial.print("Transmit data: ");
      for (size_t i = 0; i < MAX_DATA_SIZE; i++) {
          if (encryptedData[i] < 0x10) Serial.print("0");
          Serial.print(eryptedData[i], HEX);
          if (i < MAX_DATA_SIZE - 1) Serial.print(" ");
      }
      Serial.println();
    }
  }
  delay(1000);
}