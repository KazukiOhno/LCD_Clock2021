
# 1 "mcc_generated_files/fatfs/diskio.c"

# 16 "mcc_generated_files/fatfs/integer.h"
typedef int INT;
typedef unsigned int UINT;


typedef unsigned char BYTE;


typedef short SHORT;
typedef unsigned short WORD;
typedef unsigned short WCHAR;


typedef long LONG;
typedef unsigned long DWORD;

# 16 "mcc_generated_files/fatfs/diskio.h"
typedef BYTE DSTATUS;


typedef enum {
RES_OK = 0,
RES_ERROR,
RES_WRPRT,
RES_NOTRDY,
RES_PARERR
} DRESULT;

# 32
DSTATUS disk_initialize (BYTE pdrv);
DSTATUS disk_status (BYTE pdrv);
DRESULT disk_read (BYTE pdrv, BYTE* buff, DWORD sector, UINT count);
DRESULT disk_write (BYTE pdrv, const BYTE* buff, DWORD sector, UINT count);
DRESULT disk_ioctl (BYTE pdrv, BYTE cmd, void* buff);

# 13 "C:\Program Files\Microchip\xc8\v2.20\pic\include\c90\stdint.h"
typedef signed char int8_t;

# 20
typedef signed int int16_t;

# 28
typedef __int24 int24_t;

# 36
typedef signed long int int32_t;

# 52
typedef unsigned char uint8_t;

# 58
typedef unsigned int uint16_t;

# 65
typedef __uint24 uint24_t;

# 72
typedef unsigned long int uint32_t;

# 88
typedef signed char int_least8_t;

# 96
typedef signed int int_least16_t;

# 109
typedef __int24 int_least24_t;

# 118
typedef signed long int int_least32_t;

# 136
typedef unsigned char uint_least8_t;

# 143
typedef unsigned int uint_least16_t;

# 154
typedef __uint24 uint_least24_t;

# 162
typedef unsigned long int uint_least32_t;

# 181
typedef signed char int_fast8_t;

# 188
typedef signed int int_fast16_t;

# 200
typedef __int24 int_fast24_t;

# 208
typedef signed long int int_fast32_t;

# 224
typedef unsigned char uint_fast8_t;

# 230
typedef unsigned int uint_fast16_t;

# 240
typedef __uint24 uint_fast24_t;

# 247
typedef unsigned long int uint_fast32_t;

# 268
typedef int32_t intmax_t;

# 282
typedef uint32_t uintmax_t;

# 289
typedef int16_t intptr_t;




typedef uint16_t uintptr_t;

# 15 "C:\Program Files\Microchip\xc8\v2.20\pic\include\c90\stdbool.h"
typedef unsigned char bool;

# 86 "mcc_generated_files/fatfs/../../mcc_generated_files/sd_spi/sd_spi.h"
bool SD_SPI_IsMediaInitialized(void);
bool SD_SPI_MediaInitialize(void);
bool SD_SPI_IsMediaPresent(void);
bool SD_SPI_IsWriteProtected(void);
uint16_t SD_SPI_GetSectorSize(void);
uint32_t SD_SPI_GetSectorCount(void);

# 123
bool SD_SPI_SectorRead(uint32_t sector_address, uint8_t* buffer, uint16_t sector_count);

# 154
bool SD_SPI_SectorWrite(uint32_t sector_address, const uint8_t* buffer, uint16_t sector_count);

# 15 "mcc_generated_files/fatfs/diskio.c"
enum DRIVER_LIST{
DRVA = 0,
};

# 23
DSTATUS disk_status (
BYTE pdrv
)
{
DSTATUS stat = 0x01;

switch (pdrv) {

case DRVA:
if ( SD_SPI_IsMediaPresent() == 0)
{
stat = 0x02;
}
else if ( SD_SPI_IsMediaInitialized() == 1)
{
stat &= ~0x01;
}

if ( SD_SPI_IsWriteProtected() == 1)
{
stat |= 0x04;
}

break;

default:
break;
}
return stat;
}

# 60
DSTATUS disk_initialize (
BYTE pdrv
)
{
DSTATUS stat = 0x01;

switch (pdrv) {
case DRVA :
if(SD_SPI_MediaInitialize() == 1)
{
stat = RES_OK;
}
else
{
stat = RES_ERROR;
}
break;
default:
break;
}

return stat;
}

# 90
DRESULT disk_read (
BYTE pdrv,
BYTE *buff,
DWORD sector,
UINT count
)
{
DRESULT res = RES_PARERR;

switch (pdrv) {
case DRVA :
if(SD_SPI_SectorRead(sector, buff, count) == 1)
{
res = RES_OK;
}
else
{
res = RES_ERROR;
}
break;

default:
break;
}

return res;
}

# 124
DRESULT disk_write (
BYTE pdrv,
const BYTE *buff,
DWORD sector,
UINT count
)
{
DRESULT res = RES_PARERR;

switch (pdrv) {
case DRVA :
if(SD_SPI_SectorWrite(sector, buff, count) == 1)
{
res = RES_OK;
}
else
{
res = RES_ERROR;
}
break;

default:
break;
}

return res;
}

# 158
DRESULT disk_ioctl (
BYTE pdrv,
BYTE cmd,
void *buff
)
{
DRESULT res = RES_OK;

switch (pdrv) {
case DRVA :
return res;

default:
break;
}

return RES_PARERR;
}

