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

   
//�\���ʒu�̃��\�[�X  X,Y���W�ƁAX/Y�T�C�Y
#define TimeX       20
#define TimeY       35
#define TimeXw      (4*20+6+2)*2
#define TimeYw      27*2+4
#define TimebtnYw   30  //Time�̃{�^������

#define YearX       30
#define YearY       10
#define YearXw      5*8*2     //4+1��x8�h�b�g��2�{�T�C�Y
#define YearYw      8*2+2   //8�h�b�g��2�{+2�h�b�g
#define YMDbtnYw    30  //YMD�̃{�^������=����
    
#define MonthX      YearX + YearXw
#define MonthY      YearY
#define MonthXw     3*8*2   //3��x8�h�b�gx2
#define MonthYw     YearYw

#define DayX        MonthX + MonthXw
#define DayY        YearY
#define DayXw       5*8*2   //2+3��x8�h�b�gx2
#define DayYw       YearYw

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

#define AboutX       250
#define AboutY       170
#define AboutXw      65
#define AboutYw      50

#define CalendarX    10
#define CalendarY   160
#define CalendarXw   100
#define CalendarYw   80
#define CalendarXstep  100

//�A�i���O���v�̍��W�́A���v�̒��S���W
#define AnalogClockX    100
#define AnalogClockY    140
#define AnalogClockR    90


typedef struct {
    uint16_t x;     //����̍��W
    uint16_t y;
    uint16_t xw;    //���\�[�X�̃T�C�Y
    uint16_t yw;
//    uint8_t font;   //�g�p�t�H���g�AX/Y�T�C�Y�AX/Y�s�b�`�̏����擾
} rbox;

//�\�����[�h
typedef enum {
    DisplayMode1,   //0x00    //���v�\���傫���A3�����J�����_�[�\��
            DisplayMode2,    //0x01    //�J�����_�[�傫���\��
            DisplayMode3,    //0x02    //�A�i���O���v�\��
            DispSetting,     //0x03    //�ݒ�p
            MODENUM,         //3       //���[�h�̎��
};

// ���4�r�b�g�œ��샂�[�h�A����4�r�b�g�ł��̓��샂�[�h�̒��Ŏg�����ڂȂ�
#define NormalInit       0x00
#define Normal           0x01
#define Setting          0x10
#define SettingYear      0x11
#define SettingMonth     0x12
#define SettingDay       0x13
#define SettingTime      0x14
#define SettingCancel    0x15
#define SettingOK        0x16
#define SettingAlarmTime 0x17

//�\�����[�h�ɂ���āA�ʒu��T�C�Y���ς��̂ŁA�z��ϐ������āA�v���O�������₷��
//���\�[�X
rbox RTime[] = {
    { TimeX, TimeY, TimeXw, TimeYw, },
    { 0,    15,     TimeXw, TimeYw, },
    { AnalogClockX, AnalogClockY, AnalogClockR, AnalogClockR, },
    { TimeX,  130, TimeXw, TimeYw, },
};
rbox RYear[] = {
    { YearX, YearY, YearXw, YearYw, },
    { 0,    0,      YearXw, YearYw, },
    { 0,    0,      YearXw, YearYw, },
    { YearX,  40,    YearXw,  30, },
};

rbox RTemp[] = {
    { TempX, TempY, TempXw, TempYw, },
    { TempX, 0,     TempXw, TempYw, },
    { TempX, 0,     TempXw, TempYw, },
};
rbox RHumidity[] = {
    { HumidityX, HumidityY, HumidityXw, HumidityYw, },
    { HumidityX, 17,        HumidityXw, HumidityYw, },
    { HumidityX, 17,        HumidityXw, HumidityYw, },
};
rbox RAlarm[] = {
    { AlarmX,     AlarmY, AlarmXw, AlarmYw, },
    { AlarmX+100, 35,     AlarmXw, AlarmYw, },
    { 15,         23,     AlarmXw, AlarmYw, },
};
//�J�����_�[�̕\���ʒu
//��{���W�ƁAX,Ystep������Ώ��Ƃ��Ă͏\��
rbox MonthCalendar[] = {
    { CalendarX,          CalendarY,   CalendarXstep, CalendarYw, },
    { 20,                 65,          320,           200, },
    { 320-CalendarXstep,  40,          CalendarXstep, CalendarYw, },    //
};

typedef enum {
    BtnCalendar, //0
            BtnYear,     //1
            BtnMonth,    //2
            BtnDay,      //3
            BtnTime,     //4
            BtnCancel,   //5
            BtnOK,       //6
            BtnAbout,    //7
            BtnYearUp,      //8
            BtnYearDown,    //9
            BtnMonthUp,     //10
            BtnMonthDown,   //11
            BtnDayUp,       //12
            BtnDayDown,     //13
            BtnTimeUp,      //14
            BtnTimeDown,    //15
            
            ButtonNum   //�{�^���̐�
} Button;

//�{�^�����W�f�[�^: ����̍��W�ƁAW, H���w��
//Calendar�̓��C����ʂł̂ݎg�p
//����ȊO�́A�ݒ��ʂł̂ݎg�p
rbox ButtonObj3[ButtonNum] = {
    { CalendarX, CalendarY, CalendarXstep*2+CalendarXw, CalendarYw, },     //Calendar (�����ƕ\�����[�h�ύX)
    { YearX,  40,    YearXw,  30, }, // Year
    { MonthX, 40,    MonthXw, 30 }, // Month
    { DayX,   40,    DayXw,   30, }, // Day
    { TimeX, 130,    TimeXw, TimeYw, }, // Time
    { CancelX, CancelY, CancelXw, CancelYw, },       //Cancel
    { OKX, OKY, OKXw, OKYw, },      //OK
    { AboutX, AboutY, AboutXw, AboutYw, },     //About
    { YearX,  40-YMDbtnYw-7,    YearXw, YMDbtnYw, },     //Year up
    { YearX,  40+YearYw+3,      YearXw, YMDbtnYw, },     //Year down
    { MonthX, 40-YMDbtnYw-7,    MonthXw, YMDbtnYw, },     //Month up
    { MonthX, 40+MonthYw+3,     MonthXw, YMDbtnYw, },     //Month down
    { DayX,   40-YMDbtnYw-7,    DayXw,   YMDbtnYw, },     //Day up
    { DayX,   40+DayYw+3,       DayXw,  YMDbtnYw, },     //Day down
    { TimeX,  130-TimebtnYw-2,  TimeXw, TimebtnYw, },     //Time up
    { TimeX,  130+TimeYw+5,     TimeXw, TimebtnYw, },     //Time down
};


//RTC�����p
//Sec, Min, Hour, Wday, Day, Month, Year��z��
//�l�́ABCD10�i�Ƃ��邱�Ƃɒ��ӁB�擪�́ASec�̃��W�X�^�A�h���X
//�����l:2021/1/1 00:00:00
// ���т́ARTC�ƍ��킹�āA�b�A���A���A�j���A���A���A�N
uint8_t DateTime[7] = {0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x21};
//�����\���́A�ω��̂������������ɂ���悤�ɂ���
uint8_t preDateTime[3]; //�N������ێ����Ă����A�ω����`�F�b�N�ł���悤�ɂ���


#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

