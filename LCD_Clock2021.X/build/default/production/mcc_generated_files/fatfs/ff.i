
# 1 "mcc_generated_files/fatfs/ff.c"

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

# 431 "mcc_generated_files/fatfs/ff.c"
static FATFS* FatFs[1];
static WORD Fsid;

# 554
static const BYTE DbcTbl[] = {0x81, 0x9F, 0xE0, 0xFC, 0x40, 0x7E, 0x80, 0xFC, 0x00, 0x00};

# 572
static WORD ld_word (const BYTE* ptr)
{
WORD rv;

rv = ptr[1];
rv = rv << 8 | ptr[0];
return rv;
}

static DWORD ld_dword (const BYTE* ptr)
{
DWORD rv;

rv = ptr[3];
rv = rv << 8 | ptr[2];
rv = rv << 8 | ptr[1];
rv = rv << 8 | ptr[0];
return rv;
}

# 610
static void st_word (BYTE* ptr, WORD val)
{
*ptr++ = (BYTE)val; val >>= 8;
*ptr++ = (BYTE)val;
}

static void st_dword (BYTE* ptr, DWORD val)
{
*ptr++ = (BYTE)val; val >>= 8;
*ptr++ = (BYTE)val; val >>= 8;
*ptr++ = (BYTE)val; val >>= 8;
*ptr++ = (BYTE)val;
}

# 646
static void mem_cpy (void* dst, const void* src, UINT cnt)
{
BYTE *d = (BYTE*)dst;
const BYTE *s = (const BYTE*)src;

if (cnt != 0) {
do {
*d++ = *s++;
} while (--cnt);
}
}



static void mem_set (void* dst, int val, UINT cnt)
{
BYTE *d = (BYTE*)dst;

do {
*d++ = (BYTE)val;
} while (--cnt);
}



static int mem_cmp (const void* dst, const void* src, UINT cnt)
{
const BYTE *d = (const BYTE *)dst, *s = (const BYTE *)src;
int r = 0;

do {
r = *d++ - *s++;
} while (--cnt && r == 0);

return r;
}



static int chk_chr (const char* str, int chr)
{
while (*str && *str != chr) str++;
return *str;
}



static int dbc_1st (BYTE c)
{

# 701
if (c >= DbcTbl[0]) {
if (c <= DbcTbl[1]) return 1;
if (c >= DbcTbl[2] && c <= DbcTbl[3]) return 1;
}

# 708
return 0;
}



static int dbc_2nd (BYTE c)
{

# 722
if (c >= DbcTbl[4]) {
if (c <= DbcTbl[5]) return 1;
if (c >= DbcTbl[6] && c <= DbcTbl[7]) return 1;
if (c >= DbcTbl[8] && c <= DbcTbl[9]) return 1;
}

# 730
return 0;
}

# 1034
static FRESULT sync_window (
FATFS* fs
)
{
FRESULT res = FR_OK;


if (fs->wflag) {
if (disk_write(fs->pdrv, fs->win, fs->winsect, 1) == RES_OK) {
fs->wflag = 0;
if (fs->winsect - fs->fatbase < fs->fsize) {
if (fs->n_fats == 2) disk_write(fs->pdrv, fs->win, fs->winsect + fs->fsize, 1);
}
} else {
res = FR_DISK_ERR;
}
}
return res;
}



static FRESULT move_window (
FATFS* fs,
DWORD sector
)
{
FRESULT res = FR_OK;


if (sector != fs->winsect) {

res = sync_window(fs);

if (res == FR_OK) {
if (disk_read(fs->pdrv, fs->win, sector, 1) != RES_OK) {
sector = 0xFFFFFFFF;
res = FR_DISK_ERR;
}
fs->winsect = sector;
}
}
return res;
}

# 1087
static FRESULT sync_fs (
FATFS* fs
)
{
FRESULT res;


res = sync_window(fs);
if (res == FR_OK) {
if (fs->fs_type == 3 && fs->fsi_flag == 1) {

mem_set(fs->win, 0, ((UINT)512));
st_word(fs->win + 510, 0xAA55);
st_dword(fs->win + 0, 0x41615252);
st_dword(fs->win + 484, 0x61417272);
st_dword(fs->win + 488, fs->free_clst);
st_dword(fs->win + 492, fs->last_clst);

fs->winsect = fs->volbase + 1;
disk_write(fs->pdrv, fs->win, fs->winsect, 1);
fs->fsi_flag = 0;
}

if (disk_ioctl(fs->pdrv, 0, 0) != RES_OK) res = FR_DISK_ERR;
}

return res;
}

# 1124
static DWORD clst2sect (
FATFS* fs,
DWORD clst
)
{
clst -= 2;
if (clst >= fs->n_fatent - 2) return 0;
return fs->database + fs->csize * clst;
}

# 1141
static DWORD get_fat (
FFOBJID* obj,
DWORD clst
)
{
UINT wc, bc;
DWORD val;
FATFS *fs = obj->fs;


if (clst < 2 || clst >= fs->n_fatent) {
val = 1;

} else {
val = 0xFFFFFFFF;

switch (fs->fs_type) {
case 1 :
bc = (UINT)clst; bc += bc / 2;
if (move_window(fs, fs->fatbase + (bc / ((UINT)512))) != FR_OK) break;
wc = fs->win[bc++ % ((UINT)512)];
if (move_window(fs, fs->fatbase + (bc / ((UINT)512))) != FR_OK) break;
wc |= fs->win[bc % ((UINT)512)] << 8;
val = (clst & 1) ? (wc >> 4) : (wc & 0xFFF);
break;

case 2 :
if (move_window(fs, fs->fatbase + (clst / (((UINT)512) / 2))) != FR_OK) break;
val = ld_word(fs->win + clst * 2 % ((UINT)512));
break;

case 3 :
if (move_window(fs, fs->fatbase + (clst / (((UINT)512) / 4))) != FR_OK) break;
val = ld_dword(fs->win + clst * 4 % ((UINT)512)) & 0x0FFFFFFF;
break;

# 1202
default:
val = 1;
}
}

return val;
}

# 1218
static FRESULT put_fat (
FATFS* fs,
DWORD clst,
DWORD val
)
{
UINT bc;
BYTE *p;
FRESULT res = FR_INT_ERR;


if (clst >= 2 && clst < fs->n_fatent) {
switch (fs->fs_type) {
case 1 :
bc = (UINT)clst; bc += bc / 2;
res = move_window(fs, fs->fatbase + (bc / ((UINT)512)));
if (res != FR_OK) break;
p = fs->win + bc++ % ((UINT)512);
*p = (clst & 1) ? ((*p & 0x0F) | ((BYTE)val << 4)) : (BYTE)val;
fs->wflag = 1;
res = move_window(fs, fs->fatbase + (bc / ((UINT)512)));
if (res != FR_OK) break;
p = fs->win + bc % ((UINT)512);
*p = (clst & 1) ? (BYTE)(val >> 4) : ((*p & 0xF0) | ((BYTE)(val >> 8) & 0x0F));
fs->wflag = 1;
break;

case 2 :
res = move_window(fs, fs->fatbase + (clst / (((UINT)512) / 2)));
if (res != FR_OK) break;
st_word(fs->win + clst * 2 % ((UINT)512), (WORD)val);
fs->wflag = 1;
break;

case 3 :

# 1256
res = move_window(fs, fs->fatbase + (clst / (((UINT)512) / 4)));
if (res != FR_OK) break;
if (!0 || fs->fs_type != 4) {
val = (val & 0x0FFFFFFF) | (ld_dword(fs->win + clst * 4 % ((UINT)512)) & 0xF0000000);
}
st_dword(fs->win + clst * 4 % ((UINT)512), val);
fs->wflag = 1;
break;
}
}
return res;
}

# 1408
static FRESULT remove_chain (
FFOBJID* obj,
DWORD clst,
DWORD pclst
)
{
FRESULT res = FR_OK;
DWORD nxt;
FATFS *fs = obj->fs;

# 1424
if (clst < 2 || clst >= fs->n_fatent) return FR_INT_ERR;


if (pclst != 0 && (!0 || fs->fs_type != 4 || obj->stat != 2)) {
res = put_fat(fs, pclst, 0xFFFFFFFF);
if (res != FR_OK) return res;
}


do {
nxt = get_fat(obj, clst);
if (nxt == 0) break;
if (nxt == 1) return FR_INT_ERR;
if (nxt == 0xFFFFFFFF) return FR_DISK_ERR;
if (!0 || fs->fs_type != 4) {
res = put_fat(fs, clst, 0);
if (res != FR_OK) return res;
}
if (fs->free_clst < fs->n_fatent - 2) {
fs->free_clst++;
fs->fsi_flag |= 1;
}

# 1464
clst = nxt;
} while (clst < fs->n_fatent);

# 1493
return FR_OK;
}

# 1503
static DWORD create_chain (
FFOBJID* obj,
DWORD clst
)
{
DWORD cs, ncl, scl;
FRESULT res;
FATFS *fs = obj->fs;


if (clst == 0) {
scl = fs->last_clst;
if (scl == 0 || scl >= fs->n_fatent) scl = 1;
}
else {
cs = get_fat(obj, clst);
if (cs < 2) return 1;
if (cs == 0xFFFFFFFF) return cs;
if (cs < fs->n_fatent) return cs;
scl = clst;
}
if (fs->free_clst == 0) return 0;

# 1552
{
ncl = 0;
if (scl == clst) {
ncl = scl + 1;
if (ncl >= fs->n_fatent) ncl = 2;
cs = get_fat(obj, ncl);
if (cs == 1 || cs == 0xFFFFFFFF) return cs;
if (cs != 0) {
cs = fs->last_clst;
if (cs >= 2 && cs < fs->n_fatent) scl = cs;
ncl = 0;
}
}
if (ncl == 0) {
ncl = scl;
for (;;) {
ncl++;
if (ncl >= fs->n_fatent) {
ncl = 2;
if (ncl > scl) return 0;
}
cs = get_fat(obj, ncl);
if (cs == 0) break;
if (cs == 1 || cs == 0xFFFFFFFF) return cs;
if (ncl == scl) return 0;
}
}
res = put_fat(fs, ncl, 0xFFFFFFFF);
if (res == FR_OK && clst != 0) {
res = put_fat(fs, clst, ncl);
}
}

if (res == FR_OK) {
fs->last_clst = ncl;
if (fs->free_clst <= fs->n_fatent - 2) fs->free_clst--;
fs->fsi_flag |= 1;
} else {
ncl = (res == FR_DISK_ERR) ? 0xFFFFFFFF : 1;
}

return ncl;
}

# 1636
static FRESULT dir_clear (
FATFS *fs,
DWORD clst
)
{
DWORD sect;
UINT n, szb;
BYTE *ibuf;


if (sync_window(fs) != FR_OK) return FR_DISK_ERR;
sect = clst2sect(fs, clst);
fs->winsect = sect;
mem_set(fs->win, 0, ((UINT)512));

# 1660
{
ibuf = fs->win; szb = 1;
for (n = 0; n < fs->csize && disk_write(fs->pdrv, ibuf, sect + n, szb) == RES_OK; n += szb) ;
}
return (n == fs->csize) ? FR_OK : FR_DISK_ERR;
}

# 1675
static FRESULT dir_sdi (
FFDIR* dp,
DWORD ofs
)
{
DWORD csz, clst;
FATFS *fs = dp->obj.fs;


if (ofs >= (DWORD)((0 && fs->fs_type == 4) ? 0x10000000 : 0x200000) || ofs % 32) {
return FR_INT_ERR;
}
dp->dptr = ofs;
clst = dp->obj.sclust;
if (clst == 0 && fs->fs_type >= 3) {
clst = fs->dirbase;
if (0) dp->obj.stat = 0;
}

if (clst == 0) {
if (ofs / 32 >= fs->n_rootdir) return FR_INT_ERR;
dp->sect = fs->dirbase;

} else {
csz = (DWORD)fs->csize * ((UINT)512);
while (ofs >= csz) {
clst = get_fat(&dp->obj, clst);
if (clst == 0xFFFFFFFF) return FR_DISK_ERR;
if (clst < 2 || clst >= fs->n_fatent) return FR_INT_ERR;
ofs -= csz;
}
dp->sect = clst2sect(fs, clst);
}
dp->clust = clst;
if (dp->sect == 0) return FR_INT_ERR;
dp->sect += ofs / ((UINT)512);
dp->dir = fs->win + (ofs % ((UINT)512));

return FR_OK;
}

# 1723
static FRESULT dir_next (
FFDIR* dp,
int stretch
)
{
DWORD ofs, clst;
FATFS *fs = dp->obj.fs;


ofs = dp->dptr + 32;
if (dp->sect == 0 || ofs >= (DWORD)((0 && fs->fs_type == 4) ? 0x10000000 : 0x200000)) return FR_NO_FILE;

if (ofs % ((UINT)512) == 0) {
dp->sect++;

if (dp->clust == 0) {
if (ofs / 32 >= fs->n_rootdir) {
dp->sect = 0; return FR_NO_FILE;
}
}
else {
if ((ofs / ((UINT)512) & (fs->csize - 1)) == 0) {
clst = get_fat(&dp->obj, dp->clust);
if (clst <= 1) return FR_INT_ERR;
if (clst == 0xFFFFFFFF) return FR_DISK_ERR;
if (clst >= fs->n_fatent) {

if (!stretch) {
dp->sect = 0; return FR_NO_FILE;
}
clst = create_chain(&dp->obj, dp->clust);
if (clst == 0) return FR_DENIED;
if (clst == 1) return FR_INT_ERR;
if (clst == 0xFFFFFFFF) return FR_DISK_ERR;
if (dir_clear(fs, clst) != FR_OK) return FR_DISK_ERR;
if (0) dp->obj.stat |= 4;

# 1763
}
dp->clust = clst;
dp->sect = clst2sect(fs, clst);
}
}
}
dp->dptr = ofs;
dp->dir = fs->win + ofs % ((UINT)512);

return FR_OK;
}

# 1783
static FRESULT dir_alloc (
FFDIR* dp,
UINT nent
)
{
FRESULT res;
UINT n;
FATFS *fs = dp->obj.fs;


res = dir_sdi(dp, 0);
if (res == FR_OK) {
n = 0;
do {
res = move_window(fs, dp->sect);
if (res != FR_OK) break;



if (dp->dir[0] == 0xE5 || dp->dir[0] == 0) {

if (++n == nent) break;
} else {
n = 0;
}
res = dir_next(dp, 1);
} while (res == FR_OK);
}

if (res == FR_NO_FILE) res = FR_DENIED;
return res;
}

# 1825
static DWORD ld_clust (
FATFS* fs,
const BYTE* dir
)
{
DWORD cl;

cl = ld_word(dir + 26);
if (fs->fs_type == 3) {
cl |= (DWORD)ld_word(dir + 20) << 16;
}

return cl;
}



static void st_clust (
FATFS* fs,
BYTE* dir,
DWORD cl
)
{
st_word(dir + 26, (WORD)cl);
if (fs->fs_type == 3) {
st_word(dir + 20, (WORD)(cl >> 16));
}
}

# 2403
static FRESULT dir_find (
FFDIR* dp
)
{
FRESULT res;
FATFS *fs = dp->obj.fs;
BYTE c;

# 2414
res = dir_sdi(dp, 0);
if (res != FR_OK) return res;

# 2440
do {
res = move_window(fs, dp->sect);
if (res != FR_OK) break;
c = dp->dir[0];
if (c == 0) { res = FR_NO_FILE; break; }

# 2467
dp->obj.attr = dp->dir[11] & 0x3F;
if (!(dp->dir[11] & 0x08) && !mem_cmp(dp->dir, dp->fn, 11)) break;

res = dir_next(dp, 0);
} while (res == FR_OK);

return res;
}

# 2484
static FRESULT dir_register (
FFDIR* dp
)
{
FRESULT res;
FATFS *fs = dp->obj.fs;

# 2561
res = dir_alloc(dp, 1);




if (res == FR_OK) {
res = move_window(fs, dp->sect);
if (res == FR_OK) {
mem_set(dp->dir, 0, 32);
mem_cpy(dp->dir + 0, dp->fn, 11);

# 2574
fs->wflag = 1;
}
}

return res;
}

# 2818
static FRESULT create_name (
FFDIR* dp,
const TCHAR** path
)
{

# 2945
BYTE c, d, *sfn;
UINT ni, si, i;
const char *p;


p = *path; sfn = dp->fn;
mem_set(sfn, ' ', 11);
si = i = 0; ni = 8;

# 2966
for (;;) {
c = (BYTE)p[si++];
if (c <= ' ') break;
if (c == '/' || c == '\\') {
while (p[si] == '/' || p[si] == '\\') si++;
break;
}
if (c == '.' || i >= ni) {
if (ni == 11 || c != '.') return FR_INVALID_NAME;
i = 8; ni = 11;
continue;
}

# 2987
if (dbc_1st(c)) {
d = (BYTE)p[si++];
if (!dbc_2nd(d) || i >= ni - 1) return FR_INVALID_NAME;
sfn[i++] = c;
sfn[i++] = d;
} else {
if (chk_chr("\"*+,:;<=>\?[]|\x7F", c)) return FR_INVALID_NAME;
if (((c) >= 'a' && (c) <= 'z')) c -= 0x20;
sfn[i++] = c;
}
}
*path = p + si;
if (i == 0) return FR_INVALID_NAME;

if (sfn[0] == 0xE5) sfn[0] = 0x05;
sfn[11] = (c <= ' ') ? 0x04 : 0;

return FR_OK;

}

# 3015
static FRESULT follow_path (
FFDIR* dp,
const TCHAR* path
)
{
FRESULT res;
BYTE ns;
FATFS *fs = dp->obj.fs;

# 3030
{
while (*path == '/' || *path == '\\') path++;
dp->obj.sclust = 0;
}

# 3051
if ((UINT)*path < ' ') {
dp->fn[11] = 0x80;
res = dir_sdi(dp, 0);

} else {
for (;;) {
res = create_name(dp, &path);
if (res != FR_OK) break;
res = dir_find(dp);
ns = dp->fn[11];
if (res != FR_OK) {
if (res == FR_NO_FILE) {
if (0 && (ns & 0x20)) {
if (!(ns & 0x04)) continue;
dp->fn[11] = 0x80;
res = FR_OK;
} else {
if (!(ns & 0x04)) res = FR_NO_PATH;
}
}
break;
}
if (ns & 0x04) break;

if (!(dp->obj.attr & 0x10)) {
res = FR_NO_PATH; break;
}

# 3086
{
dp->obj.sclust = ld_clust(fs, fs->win + dp->dptr % ((UINT)512));
}
}
}

return res;
}

# 3102
static int get_ldnumber (
const TCHAR** path
)
{
const TCHAR *tp, *tt;
TCHAR tc;
int i, vol = -1;

# 3114
tt = tp = *path;
if (!tp) return vol;
do tc = *tt++; while ((UINT)tc >= (0 ? ' ' : '!') && tc != ':');

if (tc == ':') {
i = 1;
if (((*tp) >= '0' && (*tp) <= '9') && tp + 2 == tt) {
i = (int)*tp - '0';
}

# 3136
if (i < 1) {
vol = i;
*path = tt;
}
return vol;
}

# 3164
vol = 0;

return vol;
}

# 3176
static BYTE check_fs (
FATFS* fs,
DWORD sect
)
{
fs->wflag = 0; fs->winsect = 0xFFFFFFFF;
if (move_window(fs, sect) != FR_OK) return 4;

if (ld_word(fs->win + 510) != 0xAA55) return 3;

# 3189
if (fs->win[0] == 0xE9 || fs->win[0] == 0xEB || fs->win[0] == 0xE8) {
if (!mem_cmp(fs->win + 54, "FAT", 3)) return 0;
if (!mem_cmp(fs->win + 82, "FAT32", 5)) return 0;
}
return 2;
}

# 3203
static FRESULT find_volume (
const TCHAR** path,
FATFS** rfs,
BYTE mode
)
{
BYTE fmt, *pt;
int vol;
DSTATUS stat;
DWORD bsect, fasize, tsect, sysect, nclst, szbfat, br[4];
WORD nrsv;
FATFS *fs;
UINT i;



*rfs = 0;
vol = get_ldnumber(path);
if (vol < 0) return FR_INVALID_DRIVE;


fs = FatFs[vol];
if (!fs) return FR_NOT_ENABLED;

# 3229
*rfs = fs;

mode &= (BYTE)~0x01;
if (fs->fs_type != 0) {
stat = disk_status(fs->pdrv);
if (!(stat & 0x01)) {
if (!0 && mode && (stat & 0x04)) {
return FR_WRITE_PROTECTED;
}
return FR_OK;
}
}




fs->fs_type = 0;
fs->pdrv = (BYTE)(vol);
stat = disk_initialize(fs->pdrv);
if (stat & 0x01) {
return FR_NOT_READY;
}
if (!0 && mode && (stat & 0x04)) {
return FR_WRITE_PROTECTED;
}

# 3260
bsect = 0;
fmt = check_fs(fs, bsect);
if (fmt == 2 || (fmt < 2 && 0 != 0)) {
for (i = 0; i < 4; i++) {
pt = fs->win + (446 + i * 16);
br[i] = pt[4] ? ld_dword(pt + 8) : 0;
}
i = 0;
if (i != 0) i--;
do {
bsect = br[i];
fmt = bsect ? check_fs(fs, bsect) : 3;
} while (0 == 0 && fmt >= 2 && ++i < 4);
}
if (fmt == 4) return FR_DISK_ERR;
if (fmt >= 2) return FR_NO_FILESYSTEM;

# 3326
{
if (ld_word(fs->win + 11) != ((UINT)512)) return FR_NO_FILESYSTEM;

fasize = ld_word(fs->win + 22);
if (fasize == 0) fasize = ld_dword(fs->win + 36);
fs->fsize = fasize;

fs->n_fats = fs->win[16];
if (fs->n_fats != 1 && fs->n_fats != 2) return FR_NO_FILESYSTEM;
fasize *= fs->n_fats;

fs->csize = fs->win[13];
if (fs->csize == 0 || (fs->csize & (fs->csize - 1))) return FR_NO_FILESYSTEM;

fs->n_rootdir = ld_word(fs->win + 17);
if (fs->n_rootdir % (((UINT)512) / 32)) return FR_NO_FILESYSTEM;

tsect = ld_word(fs->win + 19);
if (tsect == 0) tsect = ld_dword(fs->win + 32);

nrsv = ld_word(fs->win + 14);
if (nrsv == 0) return FR_NO_FILESYSTEM;


sysect = nrsv + fasize + fs->n_rootdir / (((UINT)512) / 32);
if (tsect < sysect) return FR_NO_FILESYSTEM;
nclst = (tsect - sysect) / fs->csize;
if (nclst == 0) return FR_NO_FILESYSTEM;
fmt = 0;
if (nclst <= 0x0FFFFFF5) fmt = 3;
if (nclst <= 0xFFF5) fmt = 2;
if (nclst <= 0xFF5) fmt = 1;
if (fmt == 0) return FR_NO_FILESYSTEM;


fs->n_fatent = nclst + 2;
fs->volbase = bsect;
fs->fatbase = bsect + nrsv;
fs->database = bsect + sysect;
if (fmt == 3) {
if (ld_word(fs->win + 42) != 0) return FR_NO_FILESYSTEM;
if (fs->n_rootdir != 0) return FR_NO_FILESYSTEM;
fs->dirbase = ld_dword(fs->win + 44);
szbfat = fs->n_fatent * 4;
} else {
if (fs->n_rootdir == 0) return FR_NO_FILESYSTEM;
fs->dirbase = fs->fatbase + fasize;
szbfat = (fmt == 2) ?
fs->n_fatent * 2 : fs->n_fatent * 3 / 2 + (fs->n_fatent & 1);
}
if (fs->fsize < (szbfat + (((UINT)512) - 1)) / ((UINT)512)) return FR_NO_FILESYSTEM;



fs->last_clst = fs->free_clst = 0xFFFFFFFF;
fs->fsi_flag = 0x80;

if (fmt == 3
&& ld_word(fs->win + 48) == 1
&& move_window(fs, bsect + 1) == FR_OK)
{
fs->fsi_flag = 0;
if (ld_word(fs->win + 510) == 0xAA55
&& ld_dword(fs->win + 0) == 0x41615252
&& ld_dword(fs->win + 484) == 0x61417272)
{

fs->free_clst = ld_dword(fs->win + 488);


fs->last_clst = ld_dword(fs->win + 492);

}
}


}

fs->fs_type = fmt;
fs->id = ++Fsid;

# 3418
return FR_OK;
}

# 3428
static FRESULT validate (
FFOBJID* obj,
FATFS** rfs
)
{
FRESULT res = FR_INVALID_OBJECT;


if (obj && obj->fs && obj->fs->fs_type && obj->id == obj->fs->id) {

# 3448
if (!(disk_status(obj->fs->pdrv) & 0x01)) {
res = FR_OK;
}

}
*rfs = (res == FR_OK) ? obj->fs : 0;
return res;
}

# 3472
FRESULT f_mount (
FATFS* fs,
const TCHAR* path,
BYTE opt
)
{
FATFS *cfs;
int vol;
FRESULT res;
const TCHAR *rp = path;



vol = get_ldnumber(&rp);
if (vol < 0) return FR_INVALID_DRIVE;
cfs = FatFs[vol];

if (cfs) {

# 3496
cfs->fs_type = 0;
}

if (fs) {
fs->fs_type = 0;

# 3504
}
FatFs[vol] = fs;

if (opt == 0) return FR_OK;

res = find_volume(&path, &fs, 0);
return res;
}

# 3520
FRESULT f_open (
FIL* fp,
const TCHAR* path,
BYTE mode
)
{
FRESULT res;
FFDIR dj;
FATFS *fs;

DWORD dw, cl, bcs, clst, sc;
FSIZE_t ofs;




if (!fp) return FR_INVALID_OBJECT;


mode &= 0 ? 0x01 : 0x01 | 0x02 | 0x08 | 0x04 | 0x10 | 0x30;
res = find_volume(&path, &fs, mode);
if (res == FR_OK) {
dj.obj.fs = fs;
;
res = follow_path(&dj, path);

if (res == FR_OK) {
if (dj.fn[11] & 0x80) {
res = FR_INVALID_NAME;
}

# 3555
}

if (mode & (0x08 | 0x10 | 0x04)) {
if (res != FR_OK) {
if (res == FR_NO_FILE) {



res = dir_register(&dj);

}
mode |= 0x08;
}
else {
if (dj.obj.attr & (0x01 | 0x10)) {
res = FR_DENIED;
} else {
if (mode & 0x04) res = FR_EXIST;
}
}
if (res == FR_OK && (mode & 0x08)) {

# 3594
{

cl = ld_clust(fs, dj.dir);
st_dword(dj.dir + 14, get_fattime());
dj.dir[11] = 0x20;
st_clust(fs, dj.dir, 0);
st_dword(dj.dir + 28, 0);
fs->wflag = 1;
if (cl != 0) {
dw = fs->winsect;
res = remove_chain(&dj.obj, cl, 0);
if (res == FR_OK) {
res = move_window(fs, dw);
fs->last_clst = cl - 1;
}
}
}
}
}
else {
if (res == FR_OK) {
if (dj.obj.attr & 0x10) {
res = FR_NO_FILE;
} else {
if ((mode & 0x02) && (dj.obj.attr & 0x01)) {
res = FR_DENIED;
}
}
}
}
if (res == FR_OK) {
if (mode & 0x08) mode |= 0x40;
fp->dir_sect = fs->winsect;
fp->dir_ptr = dj.dir;

# 3632
}

# 3645
if (res == FR_OK) {

# 3654
{
fp->obj.sclust = ld_clust(fs, dj.dir);
fp->obj.objsize = ld_dword(dj.dir + 28);
}

# 3661
fp->obj.fs = fs;
fp->obj.id = fs->id;
fp->flag = mode;
fp->err = 0;
fp->sect = 0;
fp->fptr = 0;

# 3671
if ((mode & 0x20) && fp->obj.objsize > 0) {
fp->fptr = fp->obj.objsize;
bcs = (DWORD)fs->csize * ((UINT)512);
clst = fp->obj.sclust;
for (ofs = fp->obj.objsize; res == FR_OK && ofs > bcs; ofs -= bcs) {
clst = get_fat(&fp->obj, clst);
if (clst <= 1) res = FR_INT_ERR;
if (clst == 0xFFFFFFFF) res = FR_DISK_ERR;
}
fp->clust = clst;
if (res == FR_OK && ofs % ((UINT)512)) {
if ((sc = clst2sect(fs, clst)) == 0) {
res = FR_INT_ERR;
} else {
fp->sect = sc + (DWORD)(ofs / ((UINT)512));

# 3689
}
}
}

}

;
}

if (res != FR_OK) fp->obj.fs = 0;

return res;
}

# 3710
FRESULT f_read (
FIL* fp,
void* buff,
UINT btr,
UINT* br
)
{
FRESULT res;
FATFS *fs;
DWORD clst, sect;
FSIZE_t remain;
UINT rcnt, cc, csect;
BYTE *rbuff = (BYTE*)buff;


*br = 0;
res = validate(&fp->obj, &fs);
if (res != FR_OK || (res = (FRESULT)fp->err) != FR_OK) return res;
if (!(fp->flag & 0x01)) return FR_DENIED;
remain = fp->obj.objsize - fp->fptr;
if (btr > remain) btr = (UINT)remain;

for ( ; btr;
btr -= rcnt, *br += rcnt, rbuff += rcnt, fp->fptr += rcnt) {
if (fp->fptr % ((UINT)512) == 0) {
csect = (UINT)(fp->fptr / ((UINT)512) & (fs->csize - 1));
if (csect == 0) {
if (fp->fptr == 0) {
clst = fp->obj.sclust;
} else {

# 3745
{
clst = get_fat(&fp->obj, fp->clust);
}
}
if (clst < 2) { fp->err = (BYTE)(FR_INT_ERR); return FR_INT_ERR; };
if (clst == 0xFFFFFFFF) { fp->err = (BYTE)(FR_DISK_ERR); return FR_DISK_ERR; };
fp->clust = clst;
}
sect = clst2sect(fs, fp->clust);
if (sect == 0) { fp->err = (BYTE)(FR_INT_ERR); return FR_INT_ERR; };
sect += csect;
cc = btr / ((UINT)512);
if (cc > 0) {
if (csect + cc > fs->csize) {
cc = fs->csize - csect;
}
if (disk_read(fs->pdrv, rbuff, sect, cc) != RES_OK) { fp->err = (BYTE)(FR_DISK_ERR); return FR_DISK_ERR; };


if (fs->wflag && fs->winsect - sect < cc) {
mem_cpy(rbuff + ((fs->winsect - sect) * ((UINT)512)), fs->win, ((UINT)512));
}

# 3773
rcnt = ((UINT)512) * cc;
continue;
}

# 3787
fp->sect = sect;
}
rcnt = ((UINT)512) - (UINT)fp->fptr % ((UINT)512);
if (rcnt > btr) rcnt = btr;

if (move_window(fs, fp->sect) != FR_OK) { fp->err = (BYTE)(FR_DISK_ERR); return FR_DISK_ERR; };
mem_cpy(rbuff, fs->win + fp->fptr % ((UINT)512), rcnt);

# 3797
}

return FR_OK;
}

# 3810
FRESULT f_write (
FIL* fp,
const void* buff,
UINT btw,
UINT* bw
)
{
FRESULT res;
FATFS *fs;
DWORD clst, sect;
UINT wcnt, cc, csect;
const BYTE *wbuff = (const BYTE*)buff;


*bw = 0;
res = validate(&fp->obj, &fs);
if (res != FR_OK || (res = (FRESULT)fp->err) != FR_OK) return res;
if (!(fp->flag & 0x02)) return FR_DENIED;


if ((!0 || fs->fs_type != 4) && (DWORD)(fp->fptr + btw) < (DWORD)fp->fptr) {
btw = (UINT)(0xFFFFFFFF - (DWORD)fp->fptr);
}

for ( ; btw;
btw -= wcnt, *bw += wcnt, wbuff += wcnt, fp->fptr += wcnt, fp->obj.objsize = (fp->fptr > fp->obj.objsize) ? fp->fptr : fp->obj.objsize) {
if (fp->fptr % ((UINT)512) == 0) {
csect = (UINT)(fp->fptr / ((UINT)512)) & (fs->csize - 1);
if (csect == 0) {
if (fp->fptr == 0) {
clst = fp->obj.sclust;
if (clst == 0) {
clst = create_chain(&fp->obj, 0);
}
} else {

# 3850
{
clst = create_chain(&fp->obj, fp->clust);
}
}
if (clst == 0) break;
if (clst == 1) { fp->err = (BYTE)(FR_INT_ERR); return FR_INT_ERR; };
if (clst == 0xFFFFFFFF) { fp->err = (BYTE)(FR_DISK_ERR); return FR_DISK_ERR; };
fp->clust = clst;
if (fp->obj.sclust == 0) fp->obj.sclust = clst;
}

if (fs->winsect == fp->sect && sync_window(fs) != FR_OK) { fp->err = (BYTE)(FR_DISK_ERR); return FR_DISK_ERR; };

# 3868
sect = clst2sect(fs, fp->clust);
if (sect == 0) { fp->err = (BYTE)(FR_INT_ERR); return FR_INT_ERR; };
sect += csect;
cc = btw / ((UINT)512);
if (cc > 0) {
if (csect + cc > fs->csize) {
cc = fs->csize - csect;
}
if (disk_write(fs->pdrv, wbuff, sect, cc) != RES_OK) { fp->err = (BYTE)(FR_DISK_ERR); return FR_DISK_ERR; };


if (fs->winsect - sect < cc) {
mem_cpy(fs->win, wbuff + ((fs->winsect - sect) * ((UINT)512)), ((UINT)512));
fs->wflag = 0;
}

# 3890
wcnt = ((UINT)512) * cc;
continue;
}

if (fp->fptr >= fp->obj.objsize) {
if (sync_window(fs) != FR_OK) { fp->err = (BYTE)(FR_DISK_ERR); return FR_DISK_ERR; };
fs->winsect = sect;
}

# 3905
fp->sect = sect;
}
wcnt = ((UINT)512) - (UINT)fp->fptr % ((UINT)512);
if (wcnt > btw) wcnt = btw;

if (move_window(fs, fp->sect) != FR_OK) { fp->err = (BYTE)(FR_DISK_ERR); return FR_DISK_ERR; };
mem_cpy(fs->win + fp->fptr % ((UINT)512), wbuff, wcnt);
fs->wflag = 1;

# 3917
}

fp->flag |= 0x40;

return FR_OK;
}

# 3931
FRESULT f_sync (
FIL* fp
)
{
FRESULT res;
FATFS *fs;
DWORD tm;
BYTE *dir;


res = validate(&fp->obj, &fs);
if (res == FR_OK) {
if (fp->flag & 0x40) {

# 3951
tm = get_fattime();

# 3983
{
res = move_window(fs, fp->dir_sect);
if (res == FR_OK) {
dir = fp->dir_ptr;
dir[11] |= 0x20;
st_clust(fp->obj.fs, dir, fp->obj.sclust);
st_dword(dir + 28, (DWORD)fp->obj.objsize);
st_dword(dir + 22, tm);
st_word(dir + 18, 0);
fs->wflag = 1;
res = sync_fs(fs);
fp->flag &= (BYTE)~0x40;
}
}
}
}

return res;
}

# 4012
FRESULT f_close (
FIL* fp
)
{
FRESULT res;
FATFS *fs;


res = f_sync(fp);
if (res == FR_OK)

{
res = validate(&fp->obj, &fs);
if (res == FR_OK) {




fp->obj.fs = 0;

# 4035
}
}
return res;
}

# 4226
FRESULT f_lseek (
FIL* fp,
FSIZE_t ofs
)
{
FRESULT res;
FATFS *fs;
DWORD clst, bcs, nsect;
FSIZE_t ifptr;

# 4239
res = validate(&fp->obj, &fs);
if (res == FR_OK) res = (FRESULT)fp->err;

# 4246
if (res != FR_OK) return res;

# 4301
{

# 4305
if (ofs > fp->obj.objsize && (0 || !(fp->flag & 0x02))) {
ofs = fp->obj.objsize;
}
ifptr = fp->fptr;
fp->fptr = nsect = 0;
if (ofs > 0) {
bcs = (DWORD)fs->csize * ((UINT)512);
if (ifptr > 0 &&
(ofs - 1) / bcs >= (ifptr - 1) / bcs) {
fp->fptr = (ifptr - 1) & ~(FSIZE_t)(bcs - 1);
ofs -= fp->fptr;
clst = fp->clust;
} else {
clst = fp->obj.sclust;

if (clst == 0) {
clst = create_chain(&fp->obj, 0);
if (clst == 1) { fp->err = (BYTE)(FR_INT_ERR); return FR_INT_ERR; };
if (clst == 0xFFFFFFFF) { fp->err = (BYTE)(FR_DISK_ERR); return FR_DISK_ERR; };
fp->obj.sclust = clst;
}

fp->clust = clst;
}
if (clst != 0) {
while (ofs > bcs) {
ofs -= bcs; fp->fptr += bcs;

if (fp->flag & 0x02) {
if (0 && fp->fptr > fp->obj.objsize) {
fp->obj.objsize = fp->fptr;
fp->flag |= 0x40;
}
clst = create_chain(&fp->obj, clst);
if (clst == 0) {
ofs = 0; break;
}
} else

{
clst = get_fat(&fp->obj, clst);
}
if (clst == 0xFFFFFFFF) { fp->err = (BYTE)(FR_DISK_ERR); return FR_DISK_ERR; };
if (clst <= 1 || clst >= fs->n_fatent) { fp->err = (BYTE)(FR_INT_ERR); return FR_INT_ERR; };
fp->clust = clst;
}
fp->fptr += ofs;
if (ofs % ((UINT)512)) {
nsect = clst2sect(fs, clst);
if (nsect == 0) { fp->err = (BYTE)(FR_INT_ERR); return FR_INT_ERR; };
nsect += (DWORD)(ofs / ((UINT)512));
}
}
}
if (!0 && fp->fptr > fp->obj.objsize) {
fp->obj.objsize = fp->fptr;
fp->flag |= 0x40;
}
if (fp->fptr % ((UINT)512) && nsect != fp->sect) {

# 4373
fp->sect = nsect;
}
}

return res;
}

