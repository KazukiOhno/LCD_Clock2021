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

   
//表示位置のリソース  X,Y座標と、X/Yサイズ
#define TimeX       20
#define TimeY       35
#define TimeXw      (4*20+6+2)*2
#define TimeYw      27*2+4
#define TimebtnYw   30  //Timeのボタン高さ

#define YearX       30
#define YearY       10
#define YearXw      5*8*2     //4+1桁x8ドットの2倍サイズ
#define YearYw      8*2+2   //8ドットの2倍+2ドット
#define YMDbtnYw    30  //YMDのボタン高さ=共通
    
#define MonthX      YearX + YearXw
#define MonthY      YearY
#define MonthXw     3*8*2   //3桁x8ドットx2
#define MonthYw     YearYw

#define DayX        MonthX + MonthXw
#define DayY        YearY
#define DayXw       5*8*2   //2+3桁x8ドットx2
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

//アナログ時計の座標は、時計の中心座標
#define AnalogClockX    100
#define AnalogClockY    140
#define AnalogClockR    90


typedef struct {
    uint16_t x;     //左上の座標
    uint16_t y;
    uint16_t xw;    //リソースのサイズ
    uint16_t yw;
//    uint8_t font;   //使用フォント、X/Yサイズ、X/Yピッチの情報を取得
} rbox;

//表示モード
typedef enum {
    DisplayMode1,   //0x00    //時計表示大きく、3か月カレンダー表示
            DisplayMode2,    //0x01    //カレンダー大きい表示
            DisplayMode3,    //0x02    //アナログ時計表示
            DispSetting,     //0x03    //設定用
            MODENUM,         //3       //モードの種類
};

// 上位4ビットで動作モード、下位4ビットでその動作モードの中で使う項目など
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

//表示モードによって、位置やサイズが変わるので、配列変数化して、プログラムしやすく
//リソース
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
//カレンダーの表示位置
//基本座標と、X,Ystepがあれば情報としては十分
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
            
            ButtonNum   //ボタンの数
} Button;

//ボタン座標データ: 左上の座標と、W, Hを指定
//Calendarはメイン画面でのみ使用
//それ以外は、設定画面でのみ使用
rbox ButtonObj3[ButtonNum] = {
    { CalendarX, CalendarY, CalendarXstep*2+CalendarXw, CalendarYw, },     //Calendar (押すと表示モード変更)
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


//RTC処理用
//Sec, Min, Hour, Wday, Day, Month, Yearを配列化
//値は、BCD10進とすることに注意。先頭は、Secのレジスタアドレス
//初期値:2021/1/1 00:00:00
// 並びは、RTCと合わせて、秒、分、時、曜日、日、月、年
uint8_t DateTime[7] = {0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x21};
//時刻表示は、変化のあった所だけにするようにする
uint8_t preDateTime[3]; //年月日を保持しておき、変化をチェックできるようにする


#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

