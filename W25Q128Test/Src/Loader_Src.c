/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : Loader_Src.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "w25q128jv_ll.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
extern QSPI_HandleTypeDef hqspi;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
extern void SystemClock_Config(void);
extern void MX_QUADSPI_Init(void);
extern void MX_USART3_UART_Init(void);
extern void MX_GPIO_Init(void);

void *memset(void *buf, int ch, size_t n);


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

__IO static uint8_t dummy;

/**
  * @brief  The application entry point.
  * @retval int
  */
int Init(uint8_t MemMappedMode)
{
    /* USER CODE BEGIN 1 */
    LL_GPIO_InitTypeDef gpioInit = {0};

    hqspi.Instance = QUADSPI;
    __HAL_QSPI_DISABLE(&hqspi);
    __HAL_RCC_QSPI_FORCE_RESET();
    for (int32_t i = 0; i < 1000; i++) {
        dummy = i;
    }
    __HAL_RCC_QSPI_RELEASE_RESET();
    memset(&hqspi, 0, sizeof(QSPI_HandleTypeDef));
    HAL_QSPI_DeInit(&hqspi);
    HAL_DeInit();
    /* USER CODE END 1 */  

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_QUADSPI_Init();
    /* USER CODE BEGIN 2 */
    W25Q128JV_Init();
    gpioInit.Pin = LL_GPIO_PIN_14;
    gpioInit.Mode = LL_GPIO_MODE_OUTPUT;
    LL_GPIO_Init(GPIOB, &gpioInit);
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    return 1;
    /* USER CODE END 3 */
}

/* USER CODE BEGIN 4 */
#define QSPI_PAGESIZE 0x100
#define QSPI_FLASHSIZE (16*1024*1024)
#define QSPI_BLOCKSIZE 0x10000

int WriteEnable (void)
{
    return 1;
}

/*******************************************************************************
 Description :
 Write data to the device
 Inputs :
 				Address 	: Write location
 				Size 		: Length in bytes
 				buffer 		: Address where to get the data to write
 outputs :
 				"1" 	        : Operation succeeded
 Info :
 Note : Mandatory for all types except SRAM and PSRAM
********************************************************************************/
int Write (uint32_t Address, uint32_t Size, uint8_t* buffer)
{
    GPIO_InitTypeDef gpioInit = {0};
    uint32_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
    uint32_t   QSPI_DataNum = 0;
    if (Address >= 0x90000000) {
        Address -= 0x90000000;
    }
    gpioInit.Pin = GPIO_PIN_7;
    gpioInit.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOB, &gpioInit);
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);

    Addr = Address % QSPI_PAGESIZE;
    count = QSPI_PAGESIZE - Addr;
    NumOfPage =  Size / QSPI_PAGESIZE;
    NumOfSingle = Size % QSPI_PAGESIZE;

    if (Addr == 0) /*!< Address is QSPI_PAGESIZE aligned  */
    {
        if (NumOfPage == 0) /*!< NumByteToWrite < QSPI_PAGESIZE */
        {
            QSPI_DataNum = Size;
            W25Q128JV_Write(Address, buffer, QSPI_DataNum);
            W25Q128JV_AutoPollingMemReady();
        }
        else /*!< Size > QSPI_PAGESIZE */
        {
            while (NumOfPage--)
            {
                QSPI_DataNum = QSPI_PAGESIZE;
                W25Q128JV_Write(Address, buffer, QSPI_DataNum);
                W25Q128JV_AutoPollingMemReady();
                Address +=  QSPI_PAGESIZE;
                buffer += QSPI_PAGESIZE;
            }

            QSPI_DataNum = NumOfSingle;
            W25Q128JV_Write(Address, buffer, QSPI_DataNum);
            W25Q128JV_AutoPollingMemReady();
        }
    }
    else /*!< Address is not QSPI_PAGESIZE aligned  */
    {
        if (NumOfPage == 0) /*!< Size < QSPI_PAGESIZE */
        {
            if (NumOfSingle > count) /*!< (Size + Address) > QSPI_PAGESIZE */
            {
                temp = NumOfSingle - count;
                QSPI_DataNum = count;
                W25Q128JV_Write(Address, buffer, QSPI_DataNum);
                W25Q128JV_AutoPollingMemReady();
                Address +=  count;
                buffer += count;

                QSPI_DataNum = temp;
                W25Q128JV_Write(Address, buffer, QSPI_DataNum);
                W25Q128JV_AutoPollingMemReady();
            }
            else
            {
                QSPI_DataNum = Size;
                W25Q128JV_Write(Address, buffer, QSPI_DataNum);
                W25Q128JV_AutoPollingMemReady();
            }
        }
        else /*!< Size > QSPI_PAGESIZE */
        {
            Size -= count;
            NumOfPage =  Size / QSPI_PAGESIZE;
            NumOfSingle = Size % QSPI_PAGESIZE;

            QSPI_DataNum = count;

            W25Q128JV_Write(Address, buffer, QSPI_DataNum);
            W25Q128JV_AutoPollingMemReady();
            Address +=  count;
            buffer += count;

            while (NumOfPage--)
            {
                QSPI_DataNum = QSPI_PAGESIZE;

                W25Q128JV_Write(Address, buffer, QSPI_DataNum);
                W25Q128JV_AutoPollingMemReady();
                Address +=  QSPI_PAGESIZE;
                buffer += QSPI_PAGESIZE;
            }

            if (NumOfSingle != 0)
            {
                QSPI_DataNum = NumOfSingle;

                W25Q128JV_Write(Address, buffer, QSPI_DataNum);
                W25Q128JV_AutoPollingMemReady();
            }
        }
    }
    gpioInit.Pin = GPIO_PIN_7;
    gpioInit.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOB, &gpioInit);
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);

    return 1;
}

/*******************************************************************************
 Description :
 Erase a full sector in the device
 Inputs :
 				SectrorAddress	: Start of sector
 outputs :
 				"1" : Operation succeeded
 				"0" : Operation failure
 Note : Not Mandatory for SRAM PSRAM and NOR_FLASH
********************************************************************************/
int SectorErase (uint32_t EraseStartAddress ,uint32_t EraseEndAddress)
{
    GPIO_InitTypeDef gpioInit = {0};
    uint32_t BlockAddr;
        
    if (EraseStartAddress >= 0x90000000) {
        EraseStartAddress -= 0x90000000;
    }
    if (EraseEndAddress >= 0x90000000) {
        EraseEndAddress -= 0x90000000;
    }

    gpioInit.Pin = GPIO_PIN_7;
    gpioInit.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOB, &gpioInit);
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);

    EraseStartAddress = EraseStartAddress -  EraseStartAddress % 0x10000;

    while (EraseEndAddress>=EraseStartAddress)
    {
        BlockAddr = EraseStartAddress & 0x0FFFFFFF;
        W25Q128JV_EraseBlock( BlockAddr);
        W25Q128JV_AutoPollingMemReady();
        EraseStartAddress += 0x10000;
    }
    gpioInit.Pin = GPIO_PIN_7;
    gpioInit.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOB, &gpioInit);
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);

    return 1;
}

/*******************************************************************************
 Description :
 Erase a full sector in the device
 Inputs :
 				SectrorAddress	: Start of sector
 outputs :
 				"1" : Operation succeeded
 				"0" : Operation failure
 Note : Not Mandatory for SRAM PSRAM and NOR_FLASH
********************************************************************************/
int MassErase (void)
{
    GPIO_InitTypeDef gpioInit = {0};
    uint32_t BlockAddr = 0;

    gpioInit.Pin = GPIO_PIN_7;
    gpioInit.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOB, &gpioInit);
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);

    while (BlockAddr < QSPI_FLASHSIZE)
    {
        W25Q128JV_EraseBlock( BlockAddr);
        W25Q128JV_AutoPollingMemReady();
        BlockAddr += QSPI_BLOCKSIZE;
    }
    gpioInit.Pin = GPIO_PIN_7;
    gpioInit.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOB, &gpioInit);
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);

    return 1;
}

/*******************************************************************************
 Description :
 Read data from the device
 Inputs :
 				Address 	: Write location
 				Size 		: Length in bytes
 				buffer 		: Address where to get the data to write
 outputs :
 				"1" 		: Operation succeeded
 				"0" 		: Operation failure
 Note : Not Mandatory
********************************************************************************/
int Read (uint32_t Address, uint32_t Size, uint8_t* Buffer)
{
    GPIO_InitTypeDef gpioInit = {0};
#if 1
    int i = 0;

    W25Q128JV_MemoryMapped();
        
        
    for (i=0; i < Size;i++)
    {
        *(uint8_t*)Buffer++ = *(uint8_t*)Address;
        Address ++;
    }
#else
    int32_t l, size;
    if (Address >= 0x90000000) {
        Address -= 0x90000000;
    }
    size = (int32_t)Size;

    while (size > 0) {
        l = size % 0x10000;
        if (l == 0 && size >= 0x10000) {
            l = 0xffff;
        }
        W25Q128JV_ReadFast1Line(Address, Buffer, l);
        size -= l;
        Buffer += l;
        Address += l;
    }
#endif
    gpioInit.Pin = GPIO_PIN_0;
    gpioInit.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOB, &gpioInit);
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);

    return 1;
}

/*******************************************************************************
 Description :
 Verify the data
 Inputs :
 				MemoryAddr 	: Write location
 				RAMBufferAddr 	: RAM Address
 				Size 		: Length in bytes
 outputs :
 				"0" 		: Operation succeeded
 Note : Not Mandatory
********************************************************************************/
int Verify (uint32_t MemoryAddr, uint32_t RAMBufferAddr, uint32_t Size)
{
    GPIO_InitTypeDef gpioInit = {0};
    uint32_t VerifiedData = 0;
    Size*=4;

    W25Q128JV_MemoryMapped();

    while (Size>VerifiedData)
    {
        if ( *(uint8_t*)MemoryAddr++ != *((uint8_t*)RAMBufferAddr + VerifiedData))
            return (MemoryAddr + VerifiedData);

        VerifiedData++;
    }
    gpioInit.Pin = GPIO_PIN_0;
    gpioInit.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOB, &gpioInit);
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);

    return 1;
}

void *memset(void *buf, int ch, size_t n)
{
    char *p = buf;
    while (n-- > 0) {
        *p++ = (char)ch;
    }
    return p;
}

#if 0
void *__aeabi_memset(void *buf, unsigned int ch, int n)
{
    char *p = buf;
    while (n-- > 0) {
        *p++ = (char)ch;
    }
    return p;
}
#endif
/* USER CODE END 4 */


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
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
