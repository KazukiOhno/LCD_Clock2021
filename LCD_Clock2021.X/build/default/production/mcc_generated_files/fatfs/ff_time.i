
# 1 "mcc_generated_files/fatfs/ff_time.c"

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

# 21 "mcc_generated_files/fatfs/../../main.h"
uint8_t SlideSWStatus = 0x0f;

# 84
typedef struct {
uint16_t x;
uint16_t y;
uint16_t xw;
uint16_t yw;
uint8_t font;
uint8_t mode;
} rbox;

# 100
rbox RTime[3] = {
{ 20, 35, (4*20+6+2)*2, 27*2+4, 2, 2 },
{ 0, 15, (4*20+6+2)*2, 27*2+4, 1, 0 },
{ 100, 140, 90, 90, 2, 0 },
};
rbox RDate[3] = {
{ 30, 10, 5*8*2 + 3*8*2 + 5*8*2, 8*2+2, 2, 0 },
{ 0, 0, 5*8*2 + 3*8*2 + 5*8*2, 8*2+2, 2, 0 },
{ 0, 0, 5*8*2 + 3*8*2 + 5*8*2, 8*2+2, 2, 0 },
};
rbox RYear[3] = {
{ 30, 10, 5*8*2, 8*2+2, 2, 0 },
{ 0, 0, 5*8*2, 8*2+2, 2, 0 },
{ 0, 0, 5*8*2, 8*2+2, 2, 0 },
};
rbox RMonth[3] = {
{ 30 + 5*8*2, 10, 3*8*2, 8*2+2, 2, 0 },
{ 5*8*2, 0, 3*8*2, 8*2+2, 2, 0 },
{ 5*8*2, 0, 3*8*2, 8*2+2, 2, 0 },
};
rbox RDay[3] = {
{ 30 + 5*8*2 + 3*8*2, 10, 5*8*2, 8*2+2, 2, 0 },
{ 5*8*2 + 3*8*2, 0, 5*8*2, 8*2+2, 2, 0 },
{ 5*8*2 + 3*8*2, 0, 5*8*2, 8*2+2, 2, 0 },
};
rbox RTemp[3] = {
{ 230, 100, 50, 15, 2, 0 },
{ 230, 0, 50, 15, 2, 0 },
{ 230, 0, 50, 15, 2, 0 },
};
rbox RHumidity[3] = {
{ 240, 120, 50, 15, 2, 0 },
{ 240, 17, 50, 15, 2, 0 },
{ 240, 17, 50, 15, 2, 0 },
};
rbox RAlarm[3] = {
{ 50, 120, 100, 8*2+2, 2, 0 },
{ 50+100, 35, 100, 8*2+2, 2, 0 },
{ 15, 23, 100, 8*2+2, 0x13, 0 },
};

rbox RPrevMonthCalendar[3] = {
{ 10, 160, 100, 80, 2, 0 },
{ 20, 65, 320, 200, 2, 0 },
{ 320-100, 40, 100, 80, 2, 0 },
};
rbox RThisMonthCalendar[3] = {
{ 10+100, 160, 100, 80, 2, 0 },
{ 20, 65, 320, 200, 2, 0 },
{ 320-100, 40+80, 100, 80, 2, 0 },
};
rbox RPostMonthCalendar[3] = {
{ 10+100*2, 160, 100, 80, 2, 0 },
{ 20, 65, 320, 200, 2, 0 },
{ 320-100, 40+80*2, 100, 80, 2, 0 },
};

# 166
rbox ButtonObj3[] = {
{ 30, 10, 5*8*2, 8*2+2 },
{ 30 + 5*8*2, 10, 3*8*2, 8*2+2 },
{ 30 + 5*8*2 + 3*8*2, 10, 5*8*2, 8*2+2, },
{ 20, 35, (4*20+6+2)*2, 27*2+4, },
{ 250, 10, 65, 50, },
{ 250, 70, 65, 50, },
{ 10, 160, 100*2+100, 80, },
};



uint8_t TouchStatus = 0;

# 187
uint16_t TouchX, TouchY;
uint16_t Test_x, Test_y;

# 195
uint8_t DateTime[7] = {0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x21};

uint8_t preDateTime[3];


uint8_t TmpTime[7];

uint8_t AlarmTime[3] = {0, 0, 0x7f};
uint8_t SmoothAlarmTime[3] = {0, 0, 0x7f};

uint16_t AlarmColor[2];

uint8_t FirstDraw = 1;



char WeekDays[][4] = {
"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat",

};




int16_t Temp, Humidity;

# 18 "mcc_generated_files/fatfs/../../RTC8025.h"
uint8_t Hex2Bcd(uint8_t x);
uint8_t Bcd2Hex(uint8_t x);

void RTC_resetAlarm(void);

void RTC_setAlarmTime(uint8_t *alarmtime);
void IncDecTime(int16_t dmm, int8_t *hh, int8_t *mm);
void RTC_setTime(uint8_t *datetime);
void RTC_setDate(uint8_t *datetime);
void dateErrorCheck(uint8_t *dateTime);
uint8_t getWeekdays(uint8_t *year, uint8_t *month, uint8_t *day);
void RTC_read(uint8_t *datetime);
void resetCTFG(void);
void readReg(void);

void init_RTC(uint8_t *datetime);

# 66 "mcc_generated_files/fatfs/ff_time.c"
static DWORD decimalToFatTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t seconds)
{
DWORD fatTime;

fatTime = (seconds >> 1);
fatTime |= ( ((DWORD)minute) << 5 );
fatTime |= ( ((DWORD)hour) << 11 );
fatTime |= ( ((DWORD)day) << 16 );
fatTime |= ( ((DWORD)month) << 21 );
fatTime |= ( ((DWORD)(year - 1980)) << 25 );

return fatTime;
}

DWORD get_fattime (void)
{

return decimalToFatTime(2000+Bcd2Hex(DateTime[6]), Bcd2Hex(DateTime[5]), Bcd2Hex(DateTime[4]), Bcd2Hex(DateTime[2]), Bcd2Hex(DateTime[1]), Bcd2Hex(DateTime[0]));
}
