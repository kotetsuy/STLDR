# External Flash loader sample code for STM32CubeProgrammer (STLDR)

## Abstruct

This is sample code for external flash loader for STM32CubeProgrammer.

## Software

To compile and run it, you need below tools.

 * IAR EWARM 8 (I use 32K limited and 8.42.2)
 * STM32CubeProgrammer (I use 2.3.0)

## Hardware

To run it, you need below boards.

 * Nucleo-F412ZG
 * Winbond W25Q128JV board

I bought the W25Q128JV board at here;
https://ja.aliexpress.com/item/33035168600.html

## Pins

|  W25Q128JV  |  Nucleo  |
| ---- | ---- |
|  IO1  |  PD12  |
|  IO0  |  PD11  |
|  CLK  |  PB2  |
|  CS  |  PB6  |
|  IO2  |  PE2  |
|  IO3  |  PD13  |

## How to compile and run

 * Open W25Q128_STM32F412ZG/EWARM/Project.eww by IAR EWARM (I use 8.42.2)
 * Build
 * W25Q128_STM32F412ZG.stldr is made in W25Q128_STM32F412ZG folder.
 * Copy this stldr to C:\Program Files (x86)\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\ExternalLoader
 * Start STM32CubeProgrammer
 * Select W25Q128_STM32F412ZG in external loader

You can use read, write, erase sector, mass erase, verify commands.
