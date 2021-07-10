
# 1 "mcc_generated_files/fatfs/fatfs_demo.c"

# 16 "mcc_generated_files/fatfs/integer.h"
typedef int INT;
typedef unsigned int UINT;


typedef unsigned char BYTE;


typedef short SHORT;
typedef unsigned short WORD;
typedef unsigned short WCHAR;


typedef long LONG;
typedef unsigned long DWORD;

# 76 "mcc_generated_files/fatfs/ff.h"
typedef char TCHAR;

# 90
typedef DWORD FSIZE_t;

# 97
typedef struct {
BYTE fs_type;
BYTE pdrv;
BYTE n_fats;
BYTE wflag;
BYTE fsi_flag;
WORD id;
WORD n_rootdir;
WORD csize;

# 119
DWORD last_clst;
DWORD free_clst;

# 130
DWORD n_fatent;
DWORD fsize;
DWORD volbase;
DWORD fatbase;
DWORD dirbase;
DWORD database;
DWORD winsect;
BYTE win[512];
} FATFS;

# 144
typedef struct {
FATFS* fs;
WORD id;
BYTE attr;
BYTE stat;
DWORD sclust;
FSIZE_t objsize;

# 161
} FFOBJID;

# 167
typedef struct {
FFOBJID obj;
BYTE flag;
BYTE err;
FSIZE_t fptr;
DWORD clust;
DWORD sect;

DWORD dir_sect;
BYTE* dir_ptr;

# 184
} FIL;

# 190
typedef struct {
FFOBJID obj;
DWORD dptr;
DWORD clust;
DWORD sect;
BYTE* dir;
BYTE fn[12];

# 203
} FFDIR;

# 209
typedef struct {
FSIZE_t fsize;
WORD fdate;
WORD ftime;
BYTE fattrib;




TCHAR fname[12 + 1];

} FILINFO;

# 226
typedef enum {
FR_OK = 0,
FR_DISK_ERR,
FR_INT_ERR,
FR_NOT_READY,
FR_NO_FILE,
FR_NO_PATH,
FR_INVALID_NAME,
FR_DENIED,
FR_EXIST,
FR_INVALID_OBJECT,
FR_WRITE_PROTECTED,
FR_INVALID_DRIVE,
FR_NOT_ENABLED,
FR_NO_FILESYSTEM,
FR_MKFS_ABORTED,
FR_TIMEOUT,
FR_LOCKED,
FR_NOT_ENOUGH_CORE,
FR_TOO_MANY_OPEN_FILES,
FR_INVALID_PARAMETER
} FRESULT;

# 254
FRESULT f_open (FIL* fp, const TCHAR* path, BYTE mode);
FRESULT f_close (FIL* fp);
FRESULT f_read (FIL* fp, void* buff, UINT btr, UINT* br);
FRESULT f_write (FIL* fp, const void* buff, UINT btw, UINT* bw);
FRESULT f_lseek (FIL* fp, FSIZE_t ofs);
FRESULT f_truncate (FIL* fp);
FRESULT f_sync (FIL* fp);
FRESULT f_opendir (FFDIR* dp, const TCHAR* path);
FRESULT f_closedir (FFDIR* dp);
FRESULT f_readdir (FFDIR* dp, FILINFO* fno);
FRESULT f_findfirst (FFDIR* dp, FILINFO* fno, const TCHAR* path, const TCHAR* pattern);
FRESULT f_findnext (FFDIR* dp, FILINFO* fno);
FRESULT f_mkdir (const TCHAR* path);
FRESULT f_unlink (const TCHAR* path);
FRESULT f_rename (const TCHAR* path_old, const TCHAR* path_new);
FRESULT f_stat (const TCHAR* path, FILINFO* fno);
FRESULT f_chmod (const TCHAR* path, BYTE attr, BYTE mask);
FRESULT f_utime (const TCHAR* path, const FILINFO* fno);
FRESULT f_chdir (const TCHAR* path);
FRESULT f_chdrive (const TCHAR* path);
FRESULT f_getcwd (TCHAR* buff, UINT len);
FRESULT f_getfree (const TCHAR* path, DWORD* nclst, FATFS** fatfs);
FRESULT f_getlabel (const TCHAR* path, TCHAR* label, DWORD* vsn);
FRESULT f_setlabel (const TCHAR* label);
FRESULT f_forward (FIL* fp, UINT(*func)(const BYTE*,UINT), UINT btf, UINT* bf);
FRESULT f_expand (FIL* fp, FSIZE_t szf, BYTE opt);
FRESULT f_mount (FATFS* fs, const TCHAR* path, BYTE opt);
FRESULT f_mkfs (const TCHAR* path, BYTE opt, DWORD au, void* work, UINT len);
FRESULT f_fdisk (BYTE pdrv, const DWORD* szt, void* work);
FRESULT f_setcp (WORD cp);
int f_putc (TCHAR c, FIL* fp);
int f_puts (const TCHAR* str, FIL* cp);
int f_printf (FIL* fp, const TCHAR* str, ...);
TCHAR* f_gets (TCHAR* buff, int len, FIL* fp);

# 310
DWORD get_fattime (void);

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

# 27 "mcc_generated_files/fatfs/fatfs_demo.c"
static FATFS drive;
static FIL file;

void FatFsDemo_Tasks(void)
{
UINT actualLength;
char data[] = "Hello World!";
if( SD_SPI_IsMediaPresent() == 0)
{
return;
}

if (f_mount(&drive,"0:",1) == FR_OK)
{
if (f_open(&file, "HELLO.TXT", 0x02 | 0x04 ) == FR_OK)
{
f_write(&file, data, sizeof(data)-1, &actualLength );
f_close(&file);
}

f_mount(0,"0:",0);
}
}
