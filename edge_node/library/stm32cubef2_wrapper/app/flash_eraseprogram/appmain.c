/**
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#include <ubinos.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>

#include "main.h"

#if !(UBINOS__BSP__BOARD_MODEL == UBINOS__BSP__BOARD_MODEL__NUCLEOF207ZG)
    #error "Unsupported UBINOS__BSP__BOARD_MODEL"
#endif

//
extern char __flash2_start__; /* Set by linker.  */
extern char __flash2_size__; /* Set by linker.  */

#define FLASH_USER_START_ADDR   ((intptr_t) &__flash2_start__)  /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     ((intptr_t) &__flash2_start__  +  (intptr_t) &__flash2_size__ - 1) /* End @ of user Flash area : sector start address + sector size - 1 */

#define DATA_32                 ((uint32_t) 0x12345678)

uint32_t FirstSector = 0, NbOfSectors = 0;
uint32_t Address = 0, SECTORError = 0;
__IO uint32_t data32 = 0, MemoryProgramStatus = 0;

static FLASH_EraseInitTypeDef EraseInitStruct;
static uint32_t GetSector(uint32_t Address);
static uint32_t GetSectorSize(uint32_t Sector);

//
static void root_func(void *arg);
static void task1_func(void *arg);
static void task2_func(void *arg);

int appmain(int argc, char *argv[])
{
    int r;
    (void) r;

    srand(time(NULL));

    r = task_create(NULL, root_func, NULL, task_getmiddlepriority(), 0, "root");
    ubi_assert(r == 0);

    ubik_comp_start();

    return 0;
}

static void root_func(void *arg)
{
    int r;
    (void) r;

    printf("\n\n\n");
    printf("================================================================================\n");
    printf("flash_eraseprogram (build time: %s %s)\n", __TIME__, __DATE__);
    printf("================================================================================\n");
    printf("\n");

    printf("FLASH_USER_START_ADDR = 0x%08x\n", FLASH_USER_START_ADDR);
    printf("FLASH_USER_END_ADDR   = 0x%08x\n", FLASH_USER_END_ADDR);
    printf("\n");

    r = task_create(NULL, task1_func, NULL, task_getmiddlepriority(), 0, "task1");
    ubi_assert(r == 0);

    r = task_create(NULL, task2_func, NULL, task_getmiddlepriority(), 0, "task2");
    ubi_assert(r == 0);

    /* Initialize LED1, LED2 and LED3 */
    BSP_LED_Init(LED1);
    BSP_LED_Init(LED2);
    BSP_LED_Init(LED3);

    /* Unlock the Flash to enable the flash control register access *************/
    HAL_FLASH_Unlock();

    /* Erase the user Flash area
     (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

    /* Get the 1st sector to erase */
    FirstSector = GetSector(FLASH_USER_START_ADDR);
    /* Get the number of sector to erase from 1st sector*/
    NbOfSectors = GetSector(FLASH_USER_END_ADDR) - FirstSector + 1;
    /* Fill EraseInit structure*/
    EraseInitStruct.TypeErase = TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange = VOLTAGE_RANGE_3;
    EraseInitStruct.Sector = FirstSector;
    EraseInitStruct.NbSectors = NbOfSectors;

    /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
     you have to make sure that these data are rewritten before they are accessed during code
     execution. If this cannot be done safely, it is recommended to flush the caches by setting the
     DCRST and ICRST bits in the FLASH_CR register. */
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
    {
        /*
         Error occurred while sector erase.
         User can add here some code to deal with this error.
         SECTORError will contain the faulty sector and then to know the code error on this sector,
         user can call function 'HAL_FLASH_GetError()'
         */
        /* Infinite loop */
        printf("Fail\n");
        while (1)
        {
            BSP_LED_On(LED3);
        }
    }

    /* Program the user Flash area word by word
     (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

    Address = FLASH_USER_START_ADDR;

    while (Address < FLASH_USER_END_ADDR)
    {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, DATA_32) == HAL_OK)
        {
            Address = Address + 4;
        }
        else
        {
            /* Error occurred while writing data in Flash memory.
             User can add here some code to deal with this error */
            printf("Fail\n");
            while (1)
            {
                BSP_LED_On(LED3);
            }
        }
    }

    /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
    HAL_FLASH_Lock();

    /* Check if the programmed data is OK
     MemoryProgramStatus = 0: data programmed correctly
     MemoryProgramStatus != 0: number of words not programmed correctly ******/
    Address = FLASH_USER_START_ADDR;
    MemoryProgramStatus = 0x0;

    while (Address < FLASH_USER_END_ADDR)
    {
        data32 = *(__IO uint32_t*) Address;

        if (data32 != DATA_32)
        {
            MemoryProgramStatus++;
        }
        Address = Address + 4;
    }

    /*Check if there is an issue to program data*/
    if (MemoryProgramStatus == 0)
    {
        /* No error detected. Switch on LED1*/
        printf("Success\n");
        BSP_LED_On(LED1);
    }
    else
    {
        /* Error detected. Switch on LED2*/
        printf("Fail\n");
        BSP_LED_On(LED2);
    }
}

static void task1_func(void *arg)
{
    unsigned int delayms;

    task_sleepms(1000);

    for (unsigned int i = 0;; i++)
    {
        delayms = (rand() % 10 + 1) * 200;
        printf("1: hello world ! (%u) (delay = %4d ms)\n", i, delayms);
        task_sleepms(delayms);
    }
}

static void task2_func(void *arg)
{
    unsigned int delayms;

    task_sleepms(1000);

    for (unsigned int i = 0;; i++)
    {
        delayms = (rand() % 10 + 1) * 200;
        printf("2: hello world ! (%u) (delay = %4d ms)\n", i, delayms);
        task_sleepms(delayms);
    }
}

/**
 * @brief  Gets the sector of a given address
 * @param  None
 * @retval The sector of a given address
 */
static uint32_t GetSector(uint32_t Address)
{
    uint32_t sector = 0;

    if ((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
    {
        sector = FLASH_SECTOR_0;
    }
    else if ((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
    {
        sector = FLASH_SECTOR_1;
    }
    else if ((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
    {
        sector = FLASH_SECTOR_2;
    }
    else if ((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
    {
        sector = FLASH_SECTOR_3;
    }
    else if ((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
    {
        sector = FLASH_SECTOR_4;
    }
    else if ((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
    {
        sector = FLASH_SECTOR_5;
    }
    else if ((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
    {
        sector = FLASH_SECTOR_6;
    }
    else if ((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
    {
        sector = FLASH_SECTOR_7;
    }
    else if ((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
    {
        sector = FLASH_SECTOR_8;
    }
    else if ((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
    {
        sector = FLASH_SECTOR_9;
    }
    else if ((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
    {
        sector = FLASH_SECTOR_10;
    }
    else /* (Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11) */
    {
        sector = FLASH_SECTOR_11;
    }

    return sector;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
/**
 * @brief  Gets sector Size
 * @param  None
 * @retval The size of a given sector
 */
static uint32_t GetSectorSize(uint32_t Sector)
{
    uint32_t sectorsize = 0x00;
    if ((Sector == FLASH_SECTOR_0) || (Sector == FLASH_SECTOR_1) || (Sector == FLASH_SECTOR_2) || (Sector == FLASH_SECTOR_3))
    {
        sectorsize = 16 * 1024;
    }
    else if ((Sector == FLASH_SECTOR_4))
    {
        sectorsize = 64 * 1024;
    }
    else
    {
        sectorsize = 128 * 1024;
    }
    return sectorsize;
}

#pragma GCC diagnostic pop

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}
#endif

