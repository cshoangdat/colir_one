#include "flash.h"
#include "string.h"
#include "stdlib.h"

uint32_t flash_addr(uint16_t sector) {
    if(sector >= 0 && sector <= 3) {
        return 0x08000000 + (sector * FLASH_SECTOR_SIZE_0_3);
    } else if(sector == 4) {
        return 0x08000000 + (4 * FLASH_SECTOR_SIZE_0_3);
    } else if(sector >= 5 && sector <= 7) {
        return 0x08000000 + (4 * FLASH_SECTOR_SIZE_0_3) + FLASH_SECTOR_SIZE_4 + ((sector - 5) * FLASH_SECTOR_SIZE_5_7);
    } else {
        // Invalid sector
        return 0xFFFFFFFF;
    }
}

void flash_erase_sector(uint8_t sector)
{
    FLASH_EraseInitTypeDef EraseInitStruct = {0};
    uint32_t SectorError = 0;
    
    HAL_FLASH_Unlock();
    
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.Banks = FLASH_BANK_1; // Assume Bank 1, adjust if needed
    EraseInitStruct.Sector = sector;
    EraseInitStruct.NbSectors = 1;
    EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
    
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK) {
        printf("erase NOT successful, error: %lx\r\n", HAL_FLASH_GetError());
    }
    
    HAL_FLASH_Lock();
}

void flash_write_dword(uint32_t addr, uint64_t data)
{
    HAL_FLASH_Unlock();
    
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr, data) != HAL_OK) {
        printf("program NOT successful, error: %lx\r\n", HAL_FLASH_GetError());
    }
    
    HAL_FLASH_Lock();
}

uint32_t GetSector(uint32_t Address)
{
    uint32_t sector = 0;
    
    if((Address < 0x08004000) && (Address >= 0x08000000))
    {
        sector = FLASH_SECTOR_0;
    }
    else if((Address < 0x08008000) && (Address >= 0x08004000))
    {
        sector = FLASH_SECTOR_1;
    }
    else if((Address < 0x0800C000) && (Address >= 0x08008000))
    {
        sector = FLASH_SECTOR_2;
    }
    else if((Address < 0x08010000) && (Address >= 0x0800C000))
    {
        sector = FLASH_SECTOR_3;
    }
    else if((Address < 0x08020000) && (Address >= 0x08010000))
    {
        sector = FLASH_SECTOR_4;
    }
    else if((Address < 0x08040000) && (Address >= 0x08020000))
    {
        sector = FLASH_SECTOR_5;
    }
    else if((Address < 0x08060000) && (Address >= 0x08040000))
    {
        sector = FLASH_SECTOR_6;
    }
    else if((Address < 0x08080000) && (Address >= 0x08060000))
    {
        sector = FLASH_SECTOR_7;
    }
    
    return sector;
}

uint32_t flash_write_words(uint32_t StartSectorAddress, uint32_t *Data, uint32_t numberofwords)
{
    static FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t SECTORError;
    int sofar = 0;
    /* Unlock the Flash to enable the flash control register access */
    HAL_FLASH_Unlock();

    /* Erase the user Flash area */
    /* Get the number of sector to erase from 1st sector */
    uint32_t StartSector = GetSector(StartSectorAddress);
    uint32_t EndSectorAddress = StartSectorAddress + numberofwords * 4;
    uint32_t EndSector = GetSector(EndSectorAddress);

    /* Fill EraseInit structure*/
    EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.Sector        = StartSector;
    EraseInitStruct.NbSectors     = (EndSector - StartSector) + 1;

    /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
       you have to make sure that these data are rewritten before they are accessed during code
       execution. If this cannot be done safely, it is recommended to flush the caches by setting the
       DCRST and ICRST bits in the FLASH_CR register. */
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
    {
        printf("Erase failed, error: 0x%lx\n", HAL_FLASH_GetError());
        HAL_FLASH_Lock();
        return HAL_FLASH_GetError();
    }

    /* Program the user Flash area word by word */
    while (sofar < numberofwords)
    {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, StartSectorAddress, Data[sofar]) == HAL_OK)
        {
            StartSectorAddress += 4;  // Move to next word (4 bytes)
            sofar++;
        }
        else
        {
            /* Error occurred while writing data in Flash memory*/
            printf("Program failed at word %d, addr 0x%08lx, error: 0x%lx\n", 
                   sofar, StartSectorAddress, HAL_FLASH_GetError());
            HAL_FLASH_Lock();
            return HAL_FLASH_GetError();
        }
    }

    /* Lock the Flash to disable the flash control register access */
    HAL_FLASH_Lock();
    
    printf("Programming completed successfully!\n");
    return 0;
}

uint32_t flash_write(uint32_t addr, uint8_t *data, size_t sz)
{
    // Word align address
    addr &= ~(0x03);
    
    // Calculate number of words needed (round up)
    uint32_t numberofwords = (sz + 3) / 4;
    
    // Create word-aligned buffer
    uint32_t *word_buffer = (uint32_t*)malloc(numberofwords * 4);
    if (word_buffer == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    // Initialize buffer with 0xFF
    memset(word_buffer, 0xFF, numberofwords * 4);
    
    // Copy data to word buffer
    memcpy(word_buffer, data, sz);
    
    // Write to flash
    uint32_t result = flash_write_words(addr, word_buffer, numberofwords);
    
    // Clean up
    free(word_buffer);
    
    return result;
}