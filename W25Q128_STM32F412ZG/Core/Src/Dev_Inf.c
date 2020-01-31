#include "Dev_Inf.h"

/* This structure containes information used by ST-LINK Utility to program and erase the device */
#if defined (__ICCARM__)
__root struct StorageInfo const StorageInfo  =  {
#else
struct StorageInfo const StorageInfo  =  {
#endif
   "W25Q128_STM32F412ZG", 	 					// Device Name + EVAL Borad name
   NOR_FLASH,                   					// Device Type
   0x90000000,                						// Device Start Address
   0x1000000,                 						// Device Size in Bytes (16MBytes)
   0x100,                    						// Programming Page Size 256Bytes
   0xFF,                       						// Initial Content of Erased Memory
// Specify Size and Address of Sectors (view example below)
   {{0x00000010, 0x00010000,},     				 		// Sector Num : 10 ,Sector Size: 64KBytes
   {0x00000000, 0x00000000,},}
}; 

