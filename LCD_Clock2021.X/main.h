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
    

uint8_t SlideSWStatus = 0x0f;   //どの状態でもない値を設定しておく

//表示位置のリソース
//表示モードによって、位置やサイズが変わるので、配列変数化して、プログラムしやすく
#define TimeX       20
#define TimeY       35
#define TimeXw      (4*20+6+2)*2
#define TimeYw      27*2+4

#define YearX       30
#define YearY       10
#define YearXw      5*8*2     //4+1桁x8ドットの2倍サイズ
#define YearYw      8*2+2   //8ドットの2倍+2ドット

#define MonthX      YearX + YearXw
#define MonthY      YearY
#define MonthXw     3*8*2   //3桁x8ドットx2
#define MonthYw     YearYw

#define DayX        MonthX + MonthXw
#define DayY        YearY
#define DayXw       5*8*2   //2+3桁x8ドットx2
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
    uint16_t x;     //左上の座標
    uint16_t y;
    uint16_t xw;    //リソースのサイズ
    uint16_t yw;
    uint8_t font;   //使用フォント、X/Yサイズ、X/Yピッチの情報を取得
    uint8_t mode;   //表示形式
} rbox;

//表示モード
#define MODENUM     3       //モードの種類
#define DisplayMode1    0x00    //時計表示大きく、3か月カレンダー表示
#define DisplayMode2    0x01    //カレンダー大きい表示
#define DisplayMode3    0x02    //アナログ時計表示

//リソース
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
//小さなカレンダーの表示位置
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

//ボタン座標データ: 左上の座標と、W, Hを指定
rbox ButtonObj3[] = {
    { YearX, YearY, YearXw, YearYw }, // Year
    { MonthX, MonthY, MonthXw, MonthYw }, // Month
    { DayX, DayY, DayXw, DayYw, }, // Day
    { TimeX, TimeY, TimeXw, TimeYw, }, // Time
    { CancelX, CancelY, CancelXw, CancelYw, },       //Cancel
    { OKX, OKY, OKXw, OKYw, },      //OK
    { CalendarX, CalendarY, CalendarXstep*2+CalendarXw, CalendarYw, },     //Calendar (押すと表示モード変更)
};


//タッチ処理用
uint8_t TouchStatus = 0;
//0: タッチしていない
//1: タッチしなくなった直後の処理完了
//2: タッチ短押し
//3-4: タッチ短押し処理完了　カウントが100に行くまで、1-3のどれかの状態は維持
//5: タッチ長押し
//6-: タッチ長押し処理完了
//タッチすると0になる

//RTC処理用
//Sec, Min, Hour, Wday, Day, Month, Yearを配列化
//値は、BCD10進とすることに注意。先頭は、Secのレジスタアドレス
//初期値:2021/1/1 00:00:00
// 並びは、RTCと合わせて、秒、分、時、曜日、日、月、年
uint8_t DateTime[7] = {0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x21};
//時刻表示は、変化のあった所だけにするようにする
uint8_t preDateTime[3]; //年月日を保持しておき、変化をチェックできるようにする

//アラーム時刻、mm,hh, アラームが有効な曜日　RTCと同じ順序
uint8_t TmpTime[7]; //設定途中の時刻を保持

uint8_t AlarmTime[3] = {0, 0, 0x7f};
uint8_t SmoothAlarmTime[3] = {0, 0, 0x7f};

uint16_t AlarmColor[2];// = {0x4208, WHITE};   //0: SlideSW off時の色、1: on時の色
// 0x4208;    //灰色
uint8_t FirstDraw = 1;  // 表示モード変更後、最初の描画=1かどうか

//uint8_t debug[2];

char WeekDays[][4] = {
    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat",
//    "0x80", "0x81", "0x82", "0x83", "0x84", "0x85", "0x86",
};

//LCD関係

//温湿度センサ
int16_t Temp, Humidity;



#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

