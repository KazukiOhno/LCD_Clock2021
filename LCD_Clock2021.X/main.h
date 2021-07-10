/* 
 * File:   main.h
 * Author: K.Ohno
 *
 * Created on January 3, 2021, 12:33 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#ifdef	__cplusplus
extern "C" {
#endif

   
#define HHMMSS  0
#define HHMM    1
#define HHMMss  2
    

uint8_t SlideSWStatus = 0x0f;   //�ǂ̏�Ԃł��Ȃ��l��ݒ肵�Ă���

//�\���ʒu�̃��\�[�X
//�\�����[�h�ɂ���āA�ʒu��T�C�Y���ς��̂ŁA�z��ϐ������āA�v���O�������₷��
#define TimeX       20
#define TimeY       35
#define TimeXw      (4*20+6+2)*2
#define TimeYw      27*2+4

#define YearX       30
#define YearY       10
#define YearXw      5*8*2     //4+1��x8�h�b�g��2�{�T�C�Y
#define YearYw      8*2+2   //8�h�b�g��2�{+2�h�b�g

#define MonthX      YearX + YearXw
#define MonthY      YearY
#define MonthXw     3*8*2   //3��x8�h�b�gx2
#define MonthYw     YearYw

#define DayX        MonthX + MonthXw
#define DayY        YearY
#define DayXw       5*8*2   //2+3��x8�h�b�gx2
#define DayYw       YearYw

#define DateX       YearX
#define DateY       YearY
#define DateXw      YearXw + MonthXw + DayXw
#define DateYw      YearYw

#define AlarmX      50
#define AlarmY      120
#define AlarmXw     100
#define AlarmYw     8*2+2
#define TempX       230
#define TempY       100
#define TempXw      50
#define TempYw      15
#define HumidityX   240
#define HumidityY   120
#define HumidityXw  50
#define HumidityYw  15

#define CancelX       250
#define CancelY       10
#define CancelXw      65
#define CancelYw      50

#define OKX       250
#define OKY       70
#define OKXw      65
#define OKYw      50

#define CalendarX    10
#define CalendarY   160
#define CalendarXw   100
#define CalendarYw   80
#define CalendarXstep  100

#define AnalogClockX    100
#define AnalogClockY    140
#define AnalogClockR    90


typedef struct {
    uint16_t x;     //����̍��W
    uint16_t y;
    uint16_t xw;    //���\�[�X�̃T�C�Y
    uint16_t yw;
    uint8_t font;   //�g�p�t�H���g�AX/Y�T�C�Y�AX/Y�s�b�`�̏����擾
    uint8_t mode;   //�\���`��
} rbox;

//�\�����[�h
#define MODENUM     3       //���[�h�̎��
#define DisplayMode1    0x00    //���v�\���傫���A3�����J�����_�[�\��
#define DisplayMode2    0x01    //�J�����_�[�傫���\��
#define DisplayMode3    0x02    //�A�i���O���v�\��

//���\�[�X
rbox RTime[MODENUM] = {
    { TimeX, TimeY, TimeXw, TimeYw, 2, HHMMss },
    { 0,    15,     TimeXw, TimeYw, 1, HHMMSS },
    { AnalogClockX, AnalogClockY, AnalogClockR, AnalogClockR, 2, 0 },
};
rbox RDate[MODENUM] = {
    { DateX, DateY, DateXw, DateYw, 2, 0 },
    { 0,    0,      DateXw, DateYw, 2, 0 },
    { 0,    0,      DateXw, DateYw, 2, 0 },
};
rbox RYear[MODENUM] = {
    { YearX, YearY, YearXw, YearYw, 2, 0 },
    { 0,    0,      YearXw, YearYw, 2, 0 },
    { 0,    0,      YearXw, YearYw, 2, 0 },
};
rbox RMonth[MODENUM] = {
    { MonthX, MonthY, MonthXw, MonthYw, 2, 0 },
    { YearXw, 0,      MonthXw, MonthYw, 2, 0 },
    { YearXw, 0,      MonthXw, MonthYw, 2, 0 },
};
rbox RDay[MODENUM] = {
    { DayX,             DayY, DayXw, DayYw, 2, 0 },
    { YearXw + MonthXw, 0,    DayXw, DayYw, 2, 0 },
    { YearXw + MonthXw, 0,    DayXw, DayYw, 2, 0 },
};
rbox RTemp[MODENUM] = {
    { TempX, TempY, TempXw, TempYw, 2, 0 },
    { TempX, 0,     TempXw, TempYw, 2, 0 },
    { TempX, 0,     TempXw, TempYw, 2, 0 },
};
rbox RHumidity[MODENUM] = {
    { HumidityX, HumidityY, HumidityXw, HumidityYw, 2, 0 },
    { HumidityX, 17,        HumidityXw, HumidityYw, 2, 0 },
    { HumidityX, 17,        HumidityXw, HumidityYw, 2, 0 },
};
rbox RAlarm[MODENUM] = {
    { AlarmX,     AlarmY, AlarmXw, AlarmYw, 2, 0 },
    { AlarmX+100, 35,     AlarmXw, AlarmYw, 2, 0 },
    { 15,         23,     AlarmXw, AlarmYw, 0x13, 0 },
};
//�����ȃJ�����_�[�̕\���ʒu
rbox RPrevMonthCalendar[MODENUM] = {
    { CalendarX,          CalendarY,   CalendarXstep, CalendarYw, 2, 0 },
    { 20,                 65,          320,           200, 2, 0 },
    { 320-CalendarXstep,  40,          CalendarXstep, CalendarYw, 2, 0 },    //
};
rbox RThisMonthCalendar[MODENUM] = {
    { CalendarX+CalendarXstep, CalendarY,     CalendarXstep, CalendarYw, 2, 0 },
    { 20,                      65,            320,           200, 2, 0 },
    { 320-CalendarXstep,       40+CalendarYw, CalendarXstep, CalendarYw, 2, 0 },    //
};
rbox RPostMonthCalendar[MODENUM] = {
    { CalendarX+CalendarXstep*2, CalendarY,     CalendarXstep, CalendarYw, 2, 0 },
    { 20,                        65,            320,           200, 2, 0 },
    { 320-CalendarXstep,         40+CalendarYw*2, CalendarXstep, CalendarYw, 2, 0 },    //
};

#define BtnYear     0
#define BtnMonth    1
#define BtnDay      2
#define BtnTime     3
#define BtnCancel   4
#define BtnOK       5
#define BtnCalendar 6

//�{�^�����W�f�[�^: ����̍��W�ƁAW, H���w��
rbox ButtonObj3[] = {
    { YearX, YearY, YearXw, YearYw }, // Year
    { MonthX, MonthY, MonthXw, MonthYw }, // Month
    { DayX, DayY, DayXw, DayYw, }, // Day
    { TimeX, TimeY, TimeXw, TimeYw, }, // Time
    { CancelX, CancelY, CancelXw, CancelYw, },       //Cancel
    { OKX, OKY, OKXw, OKYw, },      //OK
    { CalendarX, CalendarY, CalendarXstep*2+CalendarXw, CalendarYw, },     //Calendar (�����ƕ\�����[�h�ύX)
};


//�^�b�`�����p
uint8_t TouchStatus = 0;
//0: �^�b�`���Ă��Ȃ�
//1: �^�b�`���Ȃ��Ȃ�������̏�������
//2: �^�b�`�Z����
//3-4: �^�b�`�Z�������������@�J�E���g��100�ɍs���܂ŁA1-3�̂ǂꂩ�̏�Ԃ͈ێ�
//5: �^�b�`������
//6-: �^�b�`��������������
//�^�b�`�����0�ɂȂ�

//RTC�����p
//Sec, Min, Hour, Wday, Day, Month, Year��z��
//�l�́ABCD10�i�Ƃ��邱�Ƃɒ��ӁB�擪�́ASec�̃��W�X�^�A�h���X
//�����l:2021/1/1 00:00:00
// ���т́ARTC�ƍ��킹�āA�b�A���A���A�j���A���A���A�N
uint8_t DateTime[7] = {0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x21};
//�����\���́A�ω��̂������������ɂ���悤�ɂ���
uint8_t preDateTime[3]; //�N������ێ����Ă����A�ω����`�F�b�N�ł���悤�ɂ���

//�A���[�������Amm,hh, �A���[�����L���ȗj���@RTC�Ɠ�������
uint8_t TmpTime[7]; //�ݒ�r���̎�����ێ�

uint8_t AlarmTime[3] = {0, 0, 0x7f};
uint8_t SmoothAlarmTime[3] = {0, 0, 0x7f};

uint16_t AlarmColor[2];// = {0x4208, WHITE};   //0: SlideSW off���̐F�A1: on���̐F
// 0x4208;    //�D�F
uint8_t FirstDraw = 1;  // �\�����[�h�ύX��A�ŏ��̕`��=1���ǂ���

//uint8_t debug[2];

char WeekDays[][4] = {
    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat",
//    "0x80", "0x81", "0x82", "0x83", "0x84", "0x85", "0x86",
};

//LCD�֌W

//�����x�Z���T
int16_t Temp, Humidity;



#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

