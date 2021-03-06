/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Author: K.Ohno

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC18F26K22
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

/*
 * PIC18F26K22  16MHz x 4 PLL
 *                   MCLR   RB7 PGD
 *     LCD_CS    (O) RA0    RB6 PGC
 *     LCD_RESET (O) RA1    RB5 (I) G1PPS 
 *     LCD_DCRS  (O) RA2    RB4 (I) RTC_INTB (WPU) IOC
 * AN3 PhotoDiode(I) RA3    RB3 (O) P2A
 *     T_IRQ     (I) RA4    RB2 (IO) SDA2
 *     T_CS      (O) RA5    RB1 (IO) SCL2
 *                   VSS    RB0 (I) RTC_INTA INT0
 *     ALMSW     (I) RA7    VDD
 *     SDCard_CS (O) RA6    VSS
 *     ROT_B     (I) RC0    RC7 (I) RX1
 *     ROT_A     (I) RC1    RC6 (O)(TX1)
 *     LCD_BL P1A(I) RC2    RC5 (O) SDO1
 *     SCK1      (O) RC3    RC4 (I) SDI1
 * 
 * TMR0: アラーム音の鳴っている時間を測定 10ms毎の割込み
 * TMR1: ロータリーエンコーダのサンプリング　100us毎の割込み
 * TMR2: 圧電スピーカ用に4KHzのECCP2 PWMを発生させるためのタイマ  250us
 * TMR3: ADCをCCP5モードで使用して、TMR3=4msを利用
 * TMR5: タッチ、スライドSW、GPSの状態チェックの割り込み用タイマ　10ms
 * TMR6: バックライトのPWM制御用タイマ 256us
 * 
 * SPIインタフェース：LCD、タッチ、SDカードの3つで共用
 * 　MSSP1=SPI (SCK1, SDI1, SDO1)+LCD_CS, T_CS, SDCard_CS
 *   　LCD_RESET, LCD_DCRS、T_IRQ
 *   LCDとは最大スピードの8MHz、タッチは2MHzで通信、SDカードは、低速400kHz、高速8MHz
 * SPIMASTERは、開いた時デフォルトで、MODE3になるので、MODE0に変更要
 * 修正しておかないと、Generateした後動かないと騒ぐことになる
 * 
 * 照度センサ(PhotoDiode)は、アナログで電圧取り込み RA3=AN3=CCP5=PhotoDiode
 * LCDのバックライトはPWM制御を使って明るさ調整
 * 　　ECCP1: バックライトのPWM (Timer6) P1A=RC2
 * 　　照度センサの結果に基づき、EPWM1_LoadDutyValue()で調整
 * 
 * I2Cインタフェース:RTCと温度センサで共用
 * 　MSSP2=I2C (SCL2、SDA2) 100KHz
 * 　RTCは、/INTA, /INTB も使用
 *     INTA=RB0=INT0は、RTC基板上でpullup、1秒ごとに割り込み入る。EXT_INTで処理
 *     INTB=RB4は、PICでWPU、状態変化割り込みでアラームを処理
 * 
 * ロータリーエンコーダ:RC0/RC1の2ピンのデジタルInputを使って判断
 * 　　RC0=ROT_B, RC1=ROT_A
 * 
 * アラームOn/Off用SW　　RA7=ALMSW
 * 圧電スピーカ: ECCP2=PWM (Timer2)　P2A=RB3　　　MCC開き直すと、P2Aが設定から消えてしまうので注意
 * GPSとのシリアル通信: RX1のみで非同期通信(EUSART1)　9600bps, 8bit, parityなし、Stop1bit
 * GPSの1PPS: SWと同様に10ms毎にチェックのする処理で、RB5から取り込む
 * 
 * 動作確認用LED: RA6=LED
 * ピンが不足したので、RA6をSDCard_CSと共用
 * 
 * Revision:
 * Rev4: リソース化して、画面デザインを自由化
 * GPSv2: GPSユニットの通信を割り込みで処理
 * GPSv3: SDカード対応
 * GPSv21: サポートルーチンの定義を整理(何をGlobal変数にして、どの関数を共用化するか)
 * GPSv22: ボタンオブジェクトの定義を使いやすいように再定義
 * GPS4: SDカード書き込み時の問題解消。LCDをSleep_Outさせて解決
 * LCD_Clock2021に名称変更: GitHubにupload。このファイルはそのclone
 * 
 */
     
#include <string.h>

#include "mcc_generated_files/mcc.h"
#include "main.h"
#include "LCD320x240color.h"
#include "RTC8025.h"
#include "Draw.h"
#include "TouchXT2046.h"
#include "AlarmSound.h"
#include "TempHumidityAM2320.h"

#define hi(a) ((a) >> 8)
#define lo(a) ((a) & 0xff)

const char HEX[] = "0123456789ABCDEF";

uint8_t Mode;   //動作状態を示す
//表示モード
uint8_t DisplayMode;    // 表示用モード

uint8_t UpdateFlag = 0;
#define UpdateTime          0x01
#define UpdateTimeDisp      0x02
#define UpdateTempHumidity  0x04
#define UpdateBrightness    0x08
#define UpdateAlarmW        0x10
#define UpdateDate          0x20
#define UpdateMsg           0x80

#define AdjustTouch      0x20

//アラーム時刻、mm,hh, アラームが有効な曜日　RTCと同じ順序
uint8_t AlarmTime[3] = {0, 0, 0x7f};    //mm, hh, wday
uint8_t SmoothAlarmTime[3] = {0, 0, 0x7f};
uint8_t TmpTime[7]; //設定途中の時刻を保持

//温湿度センサ
int16_t Temp, Humidity;

//タッチ処理用
uint8_t TouchStatus = 0;
//0: タッチしていない
//1: タッチしなくなった直後の処理完了
//2: タッチ短押し
//3-4: タッチ短押し処理完了　カウントが100に行くまで、1-3のどれかの状態は維持
//5: タッチ長押し
//6-: タッチ長押し処理完了
//タッチすると0になる

uint8_t TouchCount;
uint16_t TouchX, TouchY;
uint16_t Test_x, Test_y;

int8_t RotCount = 0; //回してクリックした回数
int8_t Accel = 1;   //高速回転させた時に、倍速等

uint16_t Brightness = 1023;
uint16_t BackLight = 100;

uint8_t SlideSWStatus = 0x0f;   //どの状態でもない値を設定しておく

uint8_t AlarmStatus = 0;    //アラーム鳴動中は1にして、smooth処理などを行う
uint8_t SmoothCount = 0;    //何回スヌーズしたか

#define STABLE1PPS  20  // 何回取得したら安定とみなすか
uint8_t Count1PPS;  //1PPS割り込みの発生回数をカウント。最大STABLE1PPSになる
uint8_t GetGPS;     //電源投入時、1日1回くらいの取得必要になったら1にする
//0: GPS取得停止、1:GPS取得中、2:1行分のデータ取得完了
char Buffer[100];   //GPSとのシリアル通信用バッファ
char * BufferP;     //バッファのデータ位置を示すポインタ

#define GPS_Stop    0
#define GPS_Start   1
#define GPS_GotData 2

//EEPROM内データ配置　　電源落ちた後、ここから設定情報読み出す
#define AddressInit         0x00    //1バイト　　初期値FF、書き込んだら55
#define AddressDisplayMode  0x07    //1バイト
//日付
#define AddressYMD          0x08    //4バイト　曜日、日、月、年の順
#define AddressAlarm        0x0c    //3バイト　分、時、曜日の順
//#define AddressAlarmNo      0x0f     //1バイト　　アラームパターンの変更用
#define AddressTouch        0x18    //8バイト　　タッチ補正用の2か所の座標

//SDカード関係
uint8_t SDcardStatus = 0;
FATFS drive;
FIL file;

//プロトタイプ宣言
void OpeningScreen(uint8_t waitTouch);

/*
 * 10ms毎に割り込み
 * Touchと、スライドSWの状態チェック、GPSユニットの起動チェック
 * チャタリング防止
 */
void Timer5Handler() {
    static uint8_t touch_status = 0xff;
    static uint8_t slideSW_status = 0xff;
    static uint8_t G1PPS_status = 0xff;

    //TouchStatus
    touch_status = (unsigned)(touch_status << 1) | T_IRQ_GetValue();
    if ((touch_status & 0x07) == 0) {
        //3回連続タッチ(0)と感知したら
        TouchCount++;
        if (TouchStatus <= 1) TouchStatus = 2;
        //短押しの処理をした後、1になって、カウントも0にしたら、連続処置可。

        if (TouchCount > 200) {
            TouchCount = 200; //2s以上長押し
            if (TouchStatus < 5) TouchStatus = 5;
            //長押し処理後、5以上に設定すれば、離すまで、連続処理しないようにできる
        }
    } else if ((touch_status & 0x07) == 0x07) {
        TouchCount = 0;
        //タッチしないとtouch_status=1になる
        if (TouchStatus > 1) TouchStatus = 0;
    }

    //アラーム用スライドSWの状態チェック
    // 0: Offに変化
    // 1: offの処理完了
    // 2: Onに変化
    // 3: Onの処理完了
    slideSW_status = (unsigned)(slideSW_status << 1) | ALMSW_GetValue();
    if ((slideSW_status & 0x07) == 0) {
        //それまでオン状態で、オフと判定されたので0にする
        if (SlideSWStatus != 1) SlideSWStatus = 0;
    } else if ((slideSW_status & 0x07) == 0x07) {
        //それまでオフ状態で、オンと判定されたので2にする
        if (SlideSWStatus != 3) SlideSWStatus = 2;
    }
    
    //GPSユニットが、GPS衛星追尾できると1PPSが発信される
    if (Count1PPS < STABLE1PPS) {
        G1PPS_status = (unsigned)(G1PPS_status << 1) | G1PPS_GetValue();
        if ((G1PPS_status & 0x0f) == 0x08) {
            //きちんとLowレベル出たら(3回連続してLになった時=HLLL)
            Count1PPS++;
            if (Count1PPS == STABLE1PPS) {
                GetGPS = GPS_Start;    //安定したら、GPSデータ取り込み指定
                BufferP = Buffer;   //初期値として、Bufferの先頭に設定
                return;
            }
        }
    }

}

/*
 * Year/Month/Day/WeekdayをEEPROMに格納しておき、電池入れ替え後の変更の手間を省く
 */
void WriteYMD() {
    int8_t jj;
    char tmp[4];

    //何回も書き換えをしないように、変化あった時だけにする
    for (jj = 0; jj < 4; jj++) {
        tmp[jj] = (unsigned)DATAEE_ReadByte(AddressYMD + jj);
        if (tmp[jj] != DateTime[jj+3]) {
            DATAEE_WriteByte(AddressYMD + jj, DateTime[jj+3]);
            UpdateFlag |= UpdateDate;
        }
    }
}


/*
 * INTAをINT0割り込みで
 * 1秒ごとに割り込みが入るので、時刻の更新することを知らせるフラグ立てる
 */
void RTC_handler() {
    UpdateFlag |= UpdateTime;

}

/*
 * アラームW(RTC_INTBのIOC)による割り込み
 */
void AlarmWHandler() {
    if (RTC_INTB_GetValue()==0) {
        //状態変化割り込みのため、0になった時だけフラグ立てる
//        UpdateFlag |= UpdateAlarmW;
        AlarmSoundOn(0);
        AlarmStatus = 1;
    }
}


/*
 * TMR1の100us毎の割り込みで、ロータリーエンコーダの状態をチェック
 * キーのチャタリング防止と同様のプログラム
 * クリックの安定点では、A端子は常にOFF=1の状態
 * RotCountを更新していく
 */
void RotaryHandler() {
    static uint8_t StartRot = 0;    //
    static int8_t Bdata = 0; // 時計回り=1、反時計回り=0
    static uint8_t RotA_status = 0xff;  //ROT_Aだけ変化点をチェック
    uint8_t newBdata;

    //A端子側のレベルをチェック。停止時=1、動かすと0となり、クリックポイントで1に戻る
    RotA_status = (unsigned)(RotA_status << 1) | ROT_A_GetValue();

    if ((StartRot == 0) && (RotA_status & 0x07) == 0) {
        //A端子が0になったら、回転開始と判断
        StartRot = 1;
        //回転開始と判断した時のB端子のレベルで回転方向を判定
        Bdata = ROT_B_GetValue();
    } else if ((StartRot == 1) && ((RotA_status & 0x07) == 0x07)) {
        //回転動作していて、A端子が1になった時の処理
        newBdata = ROT_B_GetValue();
        // B端子側が、1→0の時CW(インクリ)、0→1の時RCW(デクリ)
        if (Bdata < newBdata) {
            //A端子がHになった時、B端子がHなら反時計方向に回転
            RotCount--;
        } else if (Bdata > newBdata) {
            //A端子がHになった時、B端子がLなら時計方向に回転
            RotCount++;
        }
        //ちゃんと回転せず元に戻った場合、カウントさせず、終了するケースあり。
        StartRot = 0;
    }
    
}



/*
 * タッチした座標x,y(グラフィック座標に換算したもの)が、指定されたボタンの範囲に
 * 入っているかチェック
 * ボタンを押したと判定されると1を返す。それ以外は、0
 * ButtonObj3は、x1,y1, w, h
 * btn: 0=Year, 1=Month, 2=Day, 3=Time, 4=Cancel, 5=OK, 6=Calendar
 */
int8_t ButtonPush(uint16_t x, uint16_t y, uint8_t btn) {
    int16_t xx, yy;
    
    xx = x - ButtonObj3[btn].x;
    yy = y - ButtonObj3[btn].y;
    if ((xx >= 0) && (xx < ButtonObj3[btn].xw)) {
        if ((yy >= 0) && (yy < ButtonObj3[btn].yw)) {
            //ボタンの矩形領域に入っていた場合
            return 1;
        }
    }    
    return 0;
}

void resetPreDateTime() {
    int8_t jj;
    
    for (jj = 0; jj < 3; jj++) preDateTime[jj] = 0xff;
}


/*
 * 通常の処理
 * 時計、日付の表示更新
 */
void NormalProc() {
    char str[100];
    int16_t delta;
    int8_t mm, hh, jj;
    UINT actualLength;

    if (Mode == NormalInit) {
        lcd_fill(BLACK);
        //ボタンの位置座標をモードに合わせて変更
        ButtonObj3[BtnCalendar] = MonthCalendar[DisplayMode];

        //時刻表示は変化があった所だけ表示更新するので、BCDではありえない数値を設定しておく
        resetPreDateTime();

        get_tempHumidity(&Temp, &Humidity);
        drawTempHumidity(DisplayMode, Temp, Humidity);
    
        UpdateFlag |= UpdateTime | UpdateDate;
        Mode++; //To Normal
    }
    
    /*
     * シリアルデータ受信
     * これが呼び出されたら、LFまで構わずデータ取得
     * エラーあれば中断
     */
    if (GetGPS == GPS_Start) {
        //1行取得して、その処理が終わるまで、次のデータ取得させない
        uint8_t rxData;
        eusart1_status_t rxStatus;

        while (EUSART1_is_rx_ready()) {
            rxData = EUSART1_Read();
            rxStatus = EUSART1_get_last_status();
            if (rxStatus.ferr) return;
            *BufferP = rxData;
            BufferP++;
            if (BufferP - Buffer > 95) {
                BufferP = Buffer;
                return;    //バッファがあふれるようならデータ破棄し、中断する   
            }
            if (rxData == 0x0a) {
                //改行コード(0x0a=LF)が来たら、データ受信を終了し、フラグたてる
                *BufferP = '\0';
                GetGPS = GPS_GotData;
                return;
            }
        }
    }

    if (UpdateFlag & UpdateTime) {
        //日付、時刻の更新
        RTC_read(DateTime);
        drawDateTime(DisplayMode, DateTime);
        resetCTFG();
        EXT_INT0_InterruptFlagClear();
        WriteYMD(); //日付が変わったら、書き込んでおく
        UpdateFlag &= ~UpdateTime; //UpdateTimeをクリア
        
        if ((DateTime[0] & 0x0f) == 9) {
            //温湿度は、10秒毎に更新
            get_tempHumidity(&Temp, &Humidity);
            drawTempHumidity(DisplayMode, Temp, Humidity);
        }

        if ((DateTime[0] & 0x0f) == 7) {
            //照度センサで、明るさを10秒ごとに取得する
            //ADCの結果は、10bitの0-1023。明るいと大きな数値
            //ただし、暗いとADCの結果がかなりばらつく
            //昼間なら3.07V@3.29Vで、954-958、夜の蛍光灯下だと、50-150位だったか
            //ばらつきを低減するため、8回の合計値を取り、平均化
            //ADC_GetConversionでチャネル設定、ADC開始-取得まで実行
            Brightness = Brightness/8*7 + ADC_GetConversion(PhotoDiode);
            // Dutyを変更してバックライトの明るさを変更
            // Brightnessが一定数以上の時は、バックライトはほぼ常時点灯。
            BackLight = Brightness/8*3 + 10;
            // BackLightは、最低10-max999   0だと全く画面見えなくなるので、やめる
            if (BackLight >= 1000) BackLight = 999;

#ifdef DEBUG            
            //輝度のレベルを表示 (デバッグ用)
            //        sprintf(str, "B=%4d", Brightness);
            sprintf(str, "B=%d", BackLight);
            display_drawChars(170, 20, str, WHITE, BLACK, 1);
#endif
            //PWMは、10ビットの解像度 (TMR6の周期=PR6=0xffに設定した時)
            EPWM1_LoadDutyValue(BackLight);
        }

//#ifdef SDLOGWRITE
        // SDカードに温湿度を毎分記録
        if (DateTime[0] == 5) {
            //毎回、Mountすると、途中でSDカード抜き差ししても大丈夫なので、これで行く
            if (f_mount(&drive,"0:",1) == FR_OK) {
                if (f_open(&file, "TempLog.TXT", FA_WRITE | FA_OPEN_APPEND ) == FR_OK) { //Open or Create 追記
                    //日付時刻と気温を記録
                    sprintf(str, "%x/%x/%x %02x:%02x %d\r\n", DateTime[6], DateTime[5], DateTime[4], DateTime[2], DateTime[1], Temp);
                    f_write(&file, str, strlen(str), &actualLength);
                    f_close(&file);
                }
                f_mount(0,"0:",0);  //unmount disk
            } else {
                //オープンできなかった旨メッセージ出す
                sprintf(str, "No SD card");
                display_drawChars(15, 230, str, GREY, BLACK, 1);
            }
        }
//#endif
    }
    
    //タッチされた時の処理
    if (TouchStatus == 2) {
        TouchStatus++;
        while (GetTouchLocation(&TouchX, &TouchY) == -1);
        TransCoordination(TouchX, TouchY, &Test_x, &Test_y);
#ifdef DEBUG
        sprintf(str, "T=%3d,%3d", Test_x, Test_y);
        sprintf(str, "T=(%4d,%4d)(%3d,%3d)", TouchX, TouchY, Test_x, Test_y);
        display_drawChars(145, 25, str, WHITE, BLACK, 1);
#endif
        //アラーム中にタッチしたらスヌーズ
        if (AlarmStatus == 1) {
            AlarmSoundOff();
            AlarmStatus = 0;    //アラーム止めたら0に
            RTC_resetAlarm();
            SmoothCount++;
            if (SmoothCount <= 12) {
                //5分後にアラーム再設定　12回まで、1時間後まで
                mm = Bcd2Hex(AlarmTime[0]);
                hh = Bcd2Hex(AlarmTime[1]);
                IncDecTime(SmoothCount*5, &hh, &mm);    //5分*Smooth回数の時刻に設定
                SmoothAlarmTime[0] = Hex2Bcd(mm);
                SmoothAlarmTime[1] = Hex2Bcd(hh);

                //ここで、RTCにアラーム時刻再設定
                RTC_setAlarmTime(SmoothAlarmTime);
                
            } else {
                //スヌーズ終了
                SmoothCount = 0;
                //オリジナルのアラーム時刻をセットし直す(翌日用)
                RTC_setAlarmTime(AlarmTime);
            }
        }
        // カレンダー部をタッチしたら、表示モードを変更
        if (ButtonPush(Test_x, Test_y, BtnCalendar)) {
            DisplayMode = (DisplayMode +1) % 3;
            DATAEE_WriteByte(AddressDisplayMode, DisplayMode);  //変更したら書込む
            
            Mode = NormalInit;
            
        }

    } else if (TouchStatus == 5) {
        //タッチ長押し
//        display_drawChars(250, 140, "T- Long", WHITE, BLACK, 1);

        //タッチ長押しで設定モードへ移行
        Mode = Setting;
        TouchStatus++;
        AlarmSoundOn(0);
        __delay_ms(100);
        AlarmSoundOff();

    } else if (TouchStatus == 0) {
//        display_drawChars(250, 140, "TouchOFF", WHITE, BLACK, 1);
        drawAlarmTime(DisplayMode, AlarmTime, SlideSWStatus);
        TouchStatus++;
    }

    //ノーマル時のロータリーSWはアラーム時刻を変更
    if (RotCount != 0) {
        delta = RotCount;
        RotCount = 0;
        
        if (DisplayMode != DisplayMode3) {
            //早く回転させたら、倍速、4倍速で　　ただし、deltaは、int8ということに注意
            if ((delta > 2) || (delta < -2)) {
                //8倍加速まで
                if (Accel < 8) Accel = Accel * 2;
                delta = delta * Accel;
            }
            else Accel = 1;
        } else {
            delta = delta * 5;  //5分単位に
        }
        
        mm = Bcd2Hex(AlarmTime[0]);
        if (DisplayMode == DisplayMode3) mm = mm - (mm % 5);    //5分単位
        hh = Bcd2Hex(AlarmTime[1]);
        IncDecTime(delta, &hh, &mm);
        AlarmTime[0] = Hex2Bcd(mm);
        AlarmTime[1] = Hex2Bcd(hh);

        //アラームSWの状態にかかわらず、RTCにアラーム時刻の表示は変更
        //SWオンの時は、RTCも更新
        if (SlideSWStatus > 2) {
            RTC_setAlarmTime(AlarmTime);
        }
        drawAlarmTime(DisplayMode, AlarmTime, SlideSWStatus);

        //EEPROMに書き込むタイミングが問題。ロータリーをぐるぐる回している時は、
        //何度もEEPROMにアラーム時刻を書き込みたくない
        //変化が一定時間(1分とか)なかったら書き込む感じにする
        
    }
    
    //GPSの状態: 受信中は赤、完了すると緑(電源投入後で受信前でも)
    if (GetGPS >= GPS_Start) {
        display_fillCircle(7, 230, 3, RED);
    } else {
        display_fillCircle(7, 230, 3, GREEN);
    }
    
    if (UpdateFlag & UpdateDate) {
        //カレンダーを更新
        DrawCalendar(DisplayMode);
        UpdateFlag &= ~UpdateDate; //UpdateDateをクリア

        //日付が変わったら、再度GPS受信
        if (Count1PPS >= STABLE1PPS) Count1PPS = 0;
    }

//    sprintf(str, "%d", Count1PPS);
//    display_drawChars(15, 230, str, WHITE, BLACK, 1);
    
//    sprintf(str, "SD %d", SDcardStatus);
//    display_drawChars(15, 230, str, WHITE, BLACK, 1);

}


/*
 * 設定用画面描画
 * 枠で囲むことを考えたが、意外と面倒
 * 素直に対象の文字色を変更するだけにした。点滅とかの方が良いかもしれない
 * 
 * settingMode = SettingYear, SettingMonth, SettingDay, SettingTime
 */
void DrawSetBox(uint8_t settingMode) {
    char str[100];
    uint16_t color;
    uint8_t jj;

    //いったん普通の色で表示する
    drawDate(DispSetting, TmpTime);

    if (settingMode == SettingTime) {
        //設定している箇所の色を変える
        color = RED;
    } else {
        color = GREEN;
    }
    drawTime(DispSetting, TmpTime, color);

    if (settingMode == SettingYear) {
        sprintf(str, "20%02x", TmpTime[6]);
        display_drawChars(ButtonObj3[BtnYear].x, ButtonObj3[BtnYear].y, str, RED, BLACK, 2);
    }
    else if (settingMode == SettingMonth) {
        sprintf(str, "%02x", TmpTime[5]);
        display_drawChars(ButtonObj3[BtnMonth].x, ButtonObj3[BtnMonth].y, str, RED, BLACK, 2);
    } 
    else if (settingMode ==SettingDay) {
        sprintf(str, "%02x", TmpTime[4]);
        display_drawChars(ButtonObj3[BtnDay].x, ButtonObj3[BtnDay].y, str, RED, BLACK, 2);
    }
    
    //タッチでも変更できるように、三角ボタンを描画　全部で8つ
    uint16_t y1, y2;
    uint16_t xx[3];
    //アップ側
    xx[0] = ButtonObj3[BtnYearUp].x + 30;
    xx[1] = ButtonObj3[BtnMonthUp].x + 15;
    xx[2] = ButtonObj3[BtnDayUp].x + 15;

    y1 = ButtonObj3[BtnYearUp].y + 7;
    y2 = y1 + ButtonObj3[BtnYearUp].yw - 10;
    
    for (jj=0; jj<3; jj++) {
        display_fillTriangle(xx[jj], y1, xx[jj]-15, y2, xx[jj]+15, y2, GREY);
        display_drawTriangle(xx[jj], y1, xx[jj]-15, y2, xx[jj]+15, y2, WHITE);
    }

    //down側
    y2 = ButtonObj3[BtnYearDown].y + 3;
    y1 = y2 + ButtonObj3[BtnYearDown].yw - 10;
    for (jj=0; jj<3; jj++) {
        display_fillTriangle(xx[jj], y1, xx[jj]-15, y2, xx[jj]+15, y2, GREY);
        display_drawTriangle(xx[jj], y1, xx[jj]-15, y2, xx[jj]+15, y2, WHITE);
    }
    
    //時刻用
    xx[0] = ButtonObj3[BtnTimeUp].x + ButtonObj3[BtnTimeUp].xw/2;
    y1 = ButtonObj3[BtnTimeUp].y + 5;
    y2 = y1 + ButtonObj3[BtnTimeUp].yw - 10;
    display_fillTriangle(xx[0], y1, xx[0]-40, y2, xx[0]+40, y2, GREY);
    display_drawTriangle(xx[0], y1, xx[0]-40, y2, xx[0]+40, y2, WHITE);

    y2 = ButtonObj3[BtnTimeDown].y;
    y1 = y2 + ButtonObj3[BtnTimeDown].yw - 10;
    display_fillTriangle(xx[0], y1, xx[0]-40, y2, xx[0]+40, y2, GREY);
    display_drawTriangle(xx[0], y1, xx[0]-40, y2, xx[0]+40, y2, WHITE);

    AlarmSoundOff();
}

void YMDclac(uint8_t mode, int8_t delta) {
    int8_t yy, mo, dd;
    uint8_t y, m, d;

    yy = Bcd2Hex(TmpTime[6]);
    mo = Bcd2Hex(TmpTime[5]);
    dd = Bcd2Hex(TmpTime[4]);
            
    if (mode == SettingYear) {
        yy = (yy + delta -1 + 99) % 99 +1;  // 1-99年まで
    } else if (mode == SettingMonth) {
        mo = (mo + delta -1 + 12) % 12 +1;
    } else if (mode == SettingDay) {
        dd = (dd + delta -1 + 31) % 31 +1;
    }
    //曜日を計算
    y = yy;
    m = mo;
    d = dd;
    TmpTime[3] = getWeekdays(&y, &m, &d);
    TmpTime[6] = Hex2Bcd(y);
    TmpTime[5] = Hex2Bcd(m);
    TmpTime[4] = Hex2Bcd(d);
}

void Timecalc(uint8_t mode, int8_t delta) {
   int8_t mm, hh;
   
   mm = (int8_t)Bcd2Hex(TmpTime[1]);
   hh = (int8_t)Bcd2Hex(TmpTime[2]);
   IncDecTime(delta, &hh, &mm);
   TmpTime[1] = Hex2Bcd(mm);
   TmpTime[2] = Hex2Bcd(hh);
}

const char ButtonName[][8] = {
    "Cancel", "OK", "About", 
};

/*
 * タッチした座標をもらって、どのボタンが押されたか判定する
 */
void ButtonCheck(uint16_t x, uint16_t y, uint8_t changeTime) {
    uint8_t jj, kk;

    //どのボタンが押されたかチェック
    if (ButtonPush(x, y, BtnYearUp)) {
        YMDclac(SettingYear, 1);
    } else if (ButtonPush(x, y, BtnYearDown)) {
        YMDclac(SettingYear, -1);
    } else if (ButtonPush(x, y, BtnMonthUp)) {
        YMDclac(SettingMonth, 1);
    } else if (ButtonPush(x, y, BtnMonthDown)) {
        YMDclac(SettingMonth, -1);
    } else if (ButtonPush(x, y, BtnDayUp)) {
        YMDclac(SettingDay, 1);
    } else if (ButtonPush(x, y, BtnDayDown)) {
        YMDclac(SettingDay, -1);
    } else if (ButtonPush(x, y, BtnTimeUp)) {
        Timecalc(Mode, +1);
        //リピート利くよう
        TouchStatus = 2;
        TouchCount = 0;
    } else if (ButtonPush(x, y, BtnTimeDown)) {
        Timecalc(Mode, -1);
        //リピート利くよう
        TouchStatus = 2;
        TouchCount = 0;
    } else if (ButtonPush(x, y, BtnCancel)) {
        Mode = NormalInit;
    } else if (ButtonPush(x, y, BtnOK)) {
        for (jj = 0; jj < 7; jj++) DateTime[jj] = TmpTime[jj];
        DateTime[0] = 0x00;     //秒は00とする
        if (changeTime) RTC_setTime(DateTime);  //時刻変更時のみ
        RTC_setDate(DateTime);
        //日付を変更したら、EEPROMに書き込む
        WriteYMD();
        Mode = NormalInit;
    } else if (ButtonPush(x, y, BtnYear)) {
        //ロータリーの対象を選択(赤くする)
        Mode = SettingYear;
        resetPreDateTime();
        DrawSetBox(Mode);
    } else if (ButtonPush(x, y, BtnMonth)) {
        Mode = SettingMonth;
        resetPreDateTime();
        DrawSetBox(Mode);
        
    } else if (ButtonPush(x, y, BtnDay)) {
        Mode = SettingDay;
        resetPreDateTime();
        DrawSetBox(Mode);
        
    } else if (ButtonPush(x, y, BtnTime)) {
        Mode = SettingTime;
        resetPreDateTime();
        DrawSetBox(Mode);
        
    } else if (ButtonPush(x, y, BtnAbout)) {
        OpeningScreen(1);
        Mode = Setting;
    } else {
        return;
    }
    DrawSetBox(Mode);

}
        
/*
 * 設定の処理
 * 年月日、時分
 * アラーム時刻はここでは変更しない
 */
void SettingProc() {
    char str[100];
    int8_t delta, jj;
    int8_t yy, mo, dd, mm, hh;
    uint8_t y, m, d;
    static uint8_t changeTime;

    if (Mode == Setting) {
        //このモードに移行した時に初期化する
        lcd_fill(BLACK); //画面をクリア(黒)
        
        Mode = SettingTime;
        //設定用にテンポラリデータを現時刻からコピー
        for (jj = 0; jj < 7; jj++) TmpTime[jj] = DateTime[jj];
        changeTime = 0;
        resetPreDateTime();        //こうしておかないと描画されない
        //画面書き換え
        DrawSetBox(Mode);

        //ボタン描画  Cancel, OK, About
        for (jj=BtnCancel; jj<=BtnAbout; jj++) {
            display_drawRoundRect(ButtonObj3[jj].x, ButtonObj3[jj].y, ButtonObj3[jj].xw, ButtonObj3[jj].yw, 10, WHITE);
            //ボタンの中央にテキスト表示
            uint16_t xp = ButtonObj3[jj].x + ButtonObj3[jj].xw/2 - strlen(ButtonName[jj - BtnCancel])*4;
            uint16_t yp = ButtonObj3[jj].y + ButtonObj3[jj].yw/2 - 4;
            display_drawChars(xp, yp, (char *) ButtonName[jj - BtnCancel], WHITE, BLACK, 0x12);
        }
        return;
    }

    //タッチした時の処理
    if (TouchStatus == 2) {
        TouchStatus++;
//        display_drawChars(250, 160, "Touch ON", WHITE, BLACK, 1);
        while (GetTouchLocation(&TouchX, &TouchY) == -1);
        TransCoordination(TouchX, TouchY, &Test_x, &Test_y);
#ifdef DEBUG
        sprintf(str, "Touch=(%04d, %04d) (%03d, %03d)", TouchX, TouchY, Test_x, Test_y);
        display_drawChars(0, 150, str, WHITE, BLACK, 1);
        display_drawRoundRect(Test_x, Test_y, 3, 3, 3, WHITE);
#endif

        //タッチした場所にボタンがあるかチェックして、処理を実行
        ButtonCheck(Test_x, Test_y, changeTime);

    } else if (TouchStatus == 5) {
        TouchStatus++;
        //タッチ長押し
//        display_drawChars(250, 160, "T- Long", WHITE, BLACK, 1);
        //タッチ長押しでノーマルモードへ移行
//        DrawSetBox(Mode, 0xff);
        //ノーマルに抜ける時、キャンセルもできるようにと考えている
        Mode = NormalInit;
        
    } else if (TouchStatus == 0) {
        TouchStatus++;
//        display_drawChars(250, 160, "TouchOFF", WHITE, BLACK, 1);
    }

//    sprintf(str, "Rot=%4d", RotCount);
//    display_drawChars(250, 100, str, BLACK, WHITE, 1);

    //ロータリーを動かしたときの処理
    if (RotCount != 0) {
        delta = RotCount;
        RotCount = 0;
        //早く回転させたら、倍速、4倍速で　　ただし、deltaは、int8ということに注意
        if ((delta > 2) || (delta < -2)) {
            //8倍加速まで
            if (Accel < 8) Accel = Accel * 2;
            delta = delta * Accel;
        }
        else Accel = 1;

        if (Mode == SettingTime) {
            Timecalc(Mode, delta);
            drawTime(DispSetting, TmpTime, RED);

            changeTime = 1; //時刻を変更したら1
        } else if (Mode >= SettingYear && Mode <= SettingDay) {
            YMDclac(Mode, delta);
            DrawSetBox(Mode);
        }

    }

}

/*
 * スライドSWの処理ルーチン
 * SlideSWStatus
 * 　0: オフ判定直後
 * 　1: オフ処理後
 * 　2: オン判定直後
 * 　3: オン処理後
 */
void SlideSWProc() {
    int8_t jj;
    
    if (SlideSWStatus == 2) {
        // SWをオンにした直後の処理
        drawAlarmTime(DisplayMode, AlarmTime, SlideSWStatus);
        AlarmStatus = 0;
        RTC_setAlarmTime(AlarmTime);

        //スライドSWをOnにした時だけEEPROMにアラーム時刻を書き込む
        for (jj=0; jj<3; jj++) {
            DATAEE_WriteByte(AddressAlarm+jj, AlarmTime[jj]);
        }
        SlideSWStatus++; //スライドSWオンの処理完了したら3に
        
    } else if (SlideSWStatus == 0) {
        // SWをオフにした時の処理
        AlarmSoundOff();
        AlarmStatus = 0;
        RTC_resetAlarm();
        drawAlarmTime(DisplayMode, AlarmTime, SlideSWStatus);
        SlideSWStatus++; //スライドSWオフの処理完了したら1
        SmoothCount = 0;
    }

}


/*
 * チェックサムを計算して、一致しているかチェックする
 * Bufferに、$から?までのデータがある時に計算する
 * エラーあれば、1　　$や?がない場合、チェックサム計算が合っていない場合
 */
uint8_t CheckCheckSum(char *buf) {
    char *start;
    char *end;
    uint8_t checksum;
    
    start = strchr(buf, '$');
    if (start == NULL) return 1;    //見つからなければ、エラー
    end = strchr(buf, '*');
    if (end == NULL) return 1;    //見つからなければ、エラー
    
    checksum = 0;
    start++;    //$の次から計算対象とする
    while (start < end) {
        checksum = (uint8_t)(checksum ^ (*start)); //xor計算
        start++;
    }

    //計算したチェックサムとデータでもらったチェックサム(?の後ろの2文字)を比較
//    if (checksum == strtol(end+1, NULL, 16)) {    //こちらでもOKだが、サイズ大きくなる
    if (checksum == (uint8_t)((strchr(HEX, end[1]) - HEX) <<4) + (strchr(HEX, end[2])-HEX)) {
        return 0;
    } else {
        //チェックサムエラーの時
        return 1;
    }
    
}


/*
 * 取得したGPRMCのデータを処理する
 * GPRMC以外のデータの時は、何もしない
 * エラー時: 1
 */
uint8_t GPRMCdatacheck() {
    uint8_t jj;
    char *p;
    char str[100];

    //$GPRMCのデータの時だけ処理
    if (strncmp(Buffer, "$GPRMC", 6) != 0) return 1;

    //チェックサムのチェック
    if (CheckCheckSum(Buffer)) {
//        sprintf(str, "Chk Error");
//        sprintf(str, "x");
//        display_drawChars(0, 95, str, WHITE, BLACK, 1);
        return 1;
    }

    //データが正常なので、必要情報を取り出す
    //        sprintf(str, "-");
    //        display_drawChars(0, 120, str, WHITE, BLACK, 1);
    // hhmmss: 7-12文字　　1つ目のデータ=最初の,の後からデータがある
    DateTime[2] = ((Buffer[7]-'0')<<4) + (Buffer[8]-'0');   //hh
    DateTime[1] = ((Buffer[9]-'0')<<4) + (Buffer[10]-'0');  //mm
    DateTime[0] = ((Buffer[11]-'0')<<4) + (Buffer[12]-'0'); //ss
    
    // ddmmyy: 9つ目のデータ=9番目の,の後からデータがある
    p = strchr(Buffer, '$');
    for (jj = 0; jj < 9; jj++) {
        p = strchr(p+1, ',');
        if (p == NULL) return 1;    //見つからなかったら、やめる
    }
    DateTime[4] = ((p[1]-'0')<<4) + (p[2]-'0'); //dd
    DateTime[5] = ((p[3]-'0')<<4) + (p[4]-'0'); //mm
    DateTime[6] = ((p[5]-'0')<<4) + (p[6]-'0'); //yy
    
    //経度を取得: 5番目のデータ    yyyyy.yy
    //経度で、時差を補正しようと思ったけど、日本国内でも、地域によってはよろしくないので、中止
    int8_t TZ = +9; //現状JSTに固定
    /*
    p = strchr(Buffer, '$');
    for (jj = 0; jj < 5; jj++) {
        p = strchr(p+1, ',');
        if (p == NULL) return 1;
    }
    uint8_t len = (strchr(p, '.') - p)-3; //経度の桁数　必ずしも3桁ではないので
    for (jj = 0; jj < len; jj++) {
        TZ = TZ*10 + (p[1+jj]-'0');
    }
    //東経は+、西経は-にする
    p = strchr(p+1, ',');
    if (p[1] == 'W') TZ = -TZ;
    TZ = (TZ+8) / 15;   //1時間当たり経度で15度なので、経度から時差を計算
     */
    
    //標準時をJST (現在地の時刻)に
    int8_t hh;
    hh = Bcd2Hex(DateTime[2]) + TZ;    //JSTなら+9
    if (hh >= 24) {
        //深夜12時を過ぎたら日付を1日進める
        hh = hh - 24;
        DateTime[4] = DateTime[4] + 1;  //日付進める
    } else if (hh < 0) {
        //TZがマイナスの場合も一応プログラムしておく
        hh = hh + 24;
        DateTime[4] = DateTime[4] - 1;  //日付戻す 1日だったら、0日になるがたぶんOK
    }
    DateTime[2] = Hex2Bcd(hh);  //調整後の時間を戻す
    dateErrorCheck(DateTime);   //日付の調整
    
    //        sprintf(str, "%02x:%02x:%02x %02x/%02x/%02x", DateTime[2], DateTime[1], DateTime[0], DateTime[6], DateTime[5], DateTime[4]);
    //        display_drawChars(0, 110, str, WHITE, BLACK, 1);
    
    //日時が取得できたので、RTCに書き込む
    dateErrorCheck(DateTime);   //日付をチェックして、曜日を設定
    //RTCの日時を修正
    RTC_setDate(DateTime);
    RTC_setTime(DateTime);

#ifdef DEBUG
    デバッグ用にデータ表示
    Buffer[13] = '\0';
    display_drawChars(0, 95, Buffer, WHITE, BLACK, 1);
    Count1PPS = STABLE1PPS - 10; //すぐに次の受信をさせてみる
#endif

    return 0;

}


/*
 * タッチ座標調整用メッセージ
 * 対象のメッセージを白で、それ以外をグレーで
 */
void TouchAdjMsg(uint8_t num) {
    uint16_t color1, color2;
    char str[100];
    
    if (num == 1) {
        color1 = WHITE;
        color2 = GREY;
    } else {
        color1 = GREY;
        color2 = WHITE;
    }
    display_SetFont(NormalFont);
    sprintf(str, "1. Touch + at Up Left");
    display_drawChars(50, 100, str, color1, BLACK, 1);
    sprintf(str, "2. Touch + at Bottom Right");
    display_drawChars(50, 120, str, color2, BLACK, 1);
}

//#define DEBUG
// (20,20)、(300,220)の2点をタッチした時の座標を(T_x1, T_y1)、(T_x2, T_y2)と
// 取り込むためのルーチン
void TouchAdjust(void) {
    uint8_t count, kk;
    char str[100];

    //左上の十字
    display_drawLine(17, 20, 23, 20, WHITE);
    display_drawLine(20, 17, 20, 23, WHITE);

    //最初に左上をタッチして、次に右下をタッチしてもらう
    TouchAdjMsg(1);

    count = 0;
    while (1) {
        if (TouchStatus == 2) {
            if (count == 0) {
                // 1回目のタッチの位置
                while (GetTouchLocation(&T_x1, &T_y1) == -1);
                
                //タッチしたらピッという音を出す
                AlarmSoundOn(0);
                __delay_ms(100);
                AlarmSoundOff();
                
                //左上の十字
                display_drawLine(17, 20, 23, 20, GREY);
                display_drawLine(20, 17, 20, 23, GREY);

                //右下の十字
                display_drawLine(297, 220, 303, 220, WHITE);
                display_drawLine(300, 217, 300, 223, WHITE);
    
                TouchAdjMsg(2);

                count++;
            } else {
                int16_t dx, dy;
                // 2回目のタッチ
//                do {
                    while (GetTouchLocation(&T_x2, &T_y2) == -1);
                    //タッチしている位置がおかしいと思える場合を排除
                    if (T_x1 > T_x2) dx = T_x1 - T_x2;
                    else dx = T_x2 - T_x1;
                    if (T_y1 > T_y2) dy = T_y1 - T_y2;
                    else dy = T_y2 - T_y1;
//                } while ((dx < 1000) || (dy < 1000));
//                } while ((dx < 100) || (dy < 100));
#ifdef DEBUG
                sprintf(str, "P1=(%d, %d) P2=(%x, %x)", T_x1, T_y1, T_x2, T_y2);
                display_drawChars(0, 140, str, WHITE, BLACK, 1);
//                for (int8_t dd=0; dd<50; dd++) __delay_ms(1000);
#endif
                //タッチしたらピッという音を出す
                AlarmSoundOn(0);
                __delay_ms(100);
                AlarmSoundOff();
                return;
            }
            TouchStatus++;
        }
    }
    
}

/*
 * 16bit RGB=565のbmpファイルを読み込み表示する
 * 
 * 0x0000　(2)	bfType		ファイルタイプ　通常は'BM'
 * 0x0002　(4)	bfSize		ファイルサイズ (byte)
 * 0x0006　(2)	bfReserved1	予約領域　常に 0
 * 0x0008　(2)	bfReserved2	予約領域　常に 0
 * 0x000A　(4)	bfOffBits	ファイル先頭から画像データまでのオフセット (byte)
 * 0x000E　(4)	bcSize		ヘッダサイズ
 * 0x0012　(4)	bcWidth		画像の幅 (ピクセル)
 * 0x0016　(4)	bcHeight	画像の高さ (ピクセル) 正数なら，画像データは下から上へ
 *                                               負数なら，画像データは上から下へ
 * 0x001A　(2)	bcPlanes	プレーン数　常に 1
 * 0x001C　(2)	bcBitCount	1画素あたりのデータサイズ (bit)
 * 
 * ここでは、16bitカラーと決め打ちしている
 */
#define DATASIZE 320
uint16_t data[DATASIZE];    //ローカルでは入りきらなくても、Data spaceにはまだ入った
uint8_t ReadBmp16() {
    uint16_t x, y, jj, kk, ll;
    UINT actualLength;
    char str[100];
    uint8_t error = 0;
    
    if (f_mount(&drive,"0:",1) == FR_OK) {
        //ファイル名は8文字までサポート
        if (f_open(&file, "Open4.bmp", FA_READ ) == FR_OK) { //Open
            //ヘッダ読込
            f_read(&file, data, 32, &actualLength); //32バイト分読み込む
            x = data[0x12>>1];
            y = data[0x16>>1];
//            f_lseek(&file, data[0x0a>>1]); //データ先頭までシーク
            //シークの代わりにダミーリード
            for (jj=0; jj<(data[0x0a>>1]-32)/2; jj++) f_read(&file, data, 2, &actualLength);
            
            //bmpデータは、下の行から始まる
            for (kk=0; kk<y; kk++) {
                for (jj=0; jj<x/DATASIZE; jj++) {
                    f_read(&file, data, DATASIZE*2, &actualLength); //128バイト=64ワード=64ドット
//                    for (ll=0; ll<DATASIZE; ll++) lcd_draw_pixel_at(jj*DATASIZE+ll, 239-kk, data[ll]);
                    glcd_array(jj*DATASIZE, 239-kk, DATASIZE, 1, data);
                }
            }
            f_close(&file);
        } else {
            error = 1;
        }
        f_mount(0,"0:",0);  //unmount disk
    }
    else error = 1;
    
    return error;
    
}

void OpeningScreen(uint8_t waitTouch) {
    char str[100];
    int8_t jj;
    uint16_t x, y, w, h;
    //カラーバーと同じく、75%の白、黄、シアン、緑、マゼンタ、赤、青
    uint16_t color[] = {
        0xBDF7, //白
        0xBDE0, //黄
        0x05F7, //シアン
        0x05E0, //緑
        0xB817, //マゼンタ
        0xB800, //赤
        0x0017, //青
    };

    lcd_fill(BLACK); //画面をクリア(真っ黒)
    
    sprintf(str, "LCD Clock 2021 v1.0");
    y = 0;
    display_drawChars(0, y, str, WHITE, BLACK, 2);
    
    display_setCursor(0, 30);
    display_SetFontPitch(7, 10);
    sprintf(str, "PIC: 18F26K22 64KB\r\n");
    //        y += 30;
    display_puts(str);
    
    sprintf(str, "LCD: 2.8inch 320x240dot, 16bit color+Touch\r\n");
    display_puts(str);
    
    sprintf(str, "RTC: RX-8025NB\r\n");
    display_puts(str);
    
    sprintf(str, "Temp and Humidity Sensor: AM2320\r\n");
    display_puts(str);
    
    sprintf(str, "GPS unit: GYSFDMAXB\r\n");
    display_puts(str);
    display_SetFontPitch(8, 8);

    sprintf(str, "\nAccessing SD card...");
    display_puts(str);

    if (ReadBmp16()) {
        //    for (jj=0; jj<10; jj++) __delay_ms(1000);
    }    
    //カラーバー表示
    x = 0;
    y = 200;
    w = 40;
    h = 20;
    for (jj=0; jj<7; jj++) {
        fillRect(x, y, w, h, color[jj]);
        x += w;
    }

    if (waitTouch==1) {
        sprintf(str, "Touch anywhere on the screen");
        display_drawChars(20, 221, str, WHITE, BLACK, 0x12);
        while (TouchStatus!=2);
    }
    else {    
        for (jj=0; jj<4; jj++) {
            __delay_ms(500);
        }
    }
    lcd_fill(BLACK); //画面をクリア(真っ黒)

}
    

/*
                         Main application
 */
void main(void) {
    char str[100];
    uint8_t jj;
//    uint16_t x, y;

    // Initialize the device
    SYSTEM_Initialize();

    //アラームの初期化
    AlarmInitialize();  //アラーム音のTMR0の初期設定

    //原因は、よくわからないが、SYSTEM_Initialize時、IOCBが有効化され、空のハンドラの状態だと、なぜか
    //ハングする。ここInterruptEnableの前にしないとハングする。それがわからず相当悩んだ
    //IOCB4=0にするか、ハンドラ設定(中身がないとデフォルトと同じくダメ)をする必要あり
    IOCB4_SetInterruptHandler(AlarmWHandler); //アラーム Alarm Wの割り込み

    // ロータリー用TMR1の割り込み
    TMR1_SetInterruptHandler(RotaryHandler);

    //Touch/SlideSW/GPSは、TMR5の10ms割り込みで状態チェック
    TMR5_SetInterruptHandler(Timer5Handler);
    TMR5_StartTimer();
    TouchCount = 0;
    TouchStatus = 0;
    //GPSは、電源投入後、1PPS来たことを確認後、GPS受信動作を開始する
    Count1PPS = 0;
    GetGPS = GPS_Stop;

    // RTCの初期化
    init_RTC(DateTime);
    INT0_SetInterruptHandler(RTC_handler);  //INTA (1s毎)の割り込み処理

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

////    SDCard_CS_SetHigh(); //最初の確認用　LEDを光らせて、ハード、プログラム書き込み等の初期チェック
    
    //LCDの初期化
    glcd_init();

    OpeningScreen(0);
    
    //起動音
    AlarmSoundOn(0);
    __delay_ms(100);
    AlarmSoundOff();
    __delay_ms(200);
    AlarmSoundOn(0);
    __delay_ms(200);    //あまり大きい数だと適切などうさしない
//    __delay_ms(200);
    AlarmSoundOff();
    
    display_SetFont(NormalFont);  //初期フォント設定
    //初めて起動したときは、タッチの調整を実施し、そのデータをEEPROMに保持
    if (DATAEE_ReadByte(AddressInit) == 0xff) {
        TouchAdjust();
        lcd_fill(BLACK); //画面をクリア

        /*
        //タッチが正常にできているか動作確認用
        while (1) {
            if (TouchStatus != 0) {
                uint16_t x, y, dx, dy;
                //描画してみる
                if (GetTouchLocation(&x, &y) != -1) {
                    TransCoordination(x, y, &dx, &dy);
                    display_drawLine(dx, dy, dx, dy, WHITE);
                    sprintf(str, "P=(%4d, %4d) (%3d, %3d)", x, y, dx, dy);
                    display_drawChars(0, 0, str, WHITE, BLACK, 1);
                    sprintf(str, "(%4d, %4d) (%4d, %4d)", T_x1, T_y1, T_x2, T_y2);
                    display_drawChars(0, 10, str, WHITE, BLACK, 1);
                }
            }
        };
         */
        
        //タッチが機能しない時のデフォルト値
//        T_x1 = 0x0129;
//        T_y1 = 0x0117;
//        T_x2 = 0x0729;
//        T_y2 = 0x06c9;
        
        DATAEE_WriteByte(AddressInit, 0x55);
        //調整したタッチ座標を書き込む
        DATAEE_WriteByte(AddressTouch,   lo(T_x1));
        DATAEE_WriteByte(AddressTouch+1, hi(T_x1));
        DATAEE_WriteByte(AddressTouch+2, lo(T_y1));
        DATAEE_WriteByte(AddressTouch+3, hi(T_y1));
        DATAEE_WriteByte(AddressTouch+4, lo(T_x2));
        DATAEE_WriteByte(AddressTouch+5, hi(T_x2));
        DATAEE_WriteByte(AddressTouch+6, lo(T_y2));
        DATAEE_WriteByte(AddressTouch+7, hi(T_y2));
        //日付、アラーム設定のデフォルト値を書き込む　日付は書き込む意味がないので中止
//        for (jj=0; jj<4; jj++) {
//            DATAEE_WriteByte(AddressYMD+jj, DateTime[jj+3]);
//        }
        for (jj=0; jj<3; jj++) {
            DATAEE_WriteByte(AddressAlarm+jj, AlarmTime[jj]);
        }
        DisplayMode = DisplayMode1;
        DATAEE_WriteByte(AddressDisplayMode, DisplayMode);
        
    } else {
        //2回目以降の電源投入なら、タッチ座標補正のデータを読み出す
        T_x1 = DATAEE_ReadByte(AddressTouch) + (DATAEE_ReadByte(AddressTouch+1)<<8);
        T_y1 = DATAEE_ReadByte(AddressTouch+2) + (DATAEE_ReadByte(AddressTouch+3)<<8);
        T_x2 = DATAEE_ReadByte(AddressTouch+4) + (DATAEE_ReadByte(AddressTouch+5)<<8);
        T_y2 = DATAEE_ReadByte(AddressTouch+6) + (DATAEE_ReadByte(AddressTouch+7)<<8);
//        sprintf(str, "P1=(%d, %d) P2=(%d, %d)", T_x1, T_y1, T_x2, T_y2);
//        display_drawChars(0, 140, str, WHITE, BLACK, 1);
        
        //日付、アラーム設定を読み出す
        for (jj=0; jj<4; jj++) {
            DateTime[jj+3] = DATAEE_ReadByte(AddressYMD+jj);
        }
        for (jj=0; jj<3; jj++) {
            AlarmTime[jj] = DATAEE_ReadByte(AddressAlarm+jj);
        }
        //前回終了時の表示モードにする
        DisplayMode = DATAEE_ReadByte(AddressDisplayMode);        
    }
    
    Mode = NormalInit;  //初期状態はノーマル

    //SDカード
    //例えば、気温ログを作る場合
    /*
     * ファイルなければ、生成。あれば、追記
     * 定期的に気温を記録する
     * f_open  FA_CREATE_NEW だと、ファイルない時だけ生成。
     * 
     * 
     */
/*
    //以下は実質意味なし。常にtrue返すので
    if (SD_SPI_IsMediaPresent() == false) {
        return;
    }
 */
    
/*
    sprintf(str, "Initializing ...\r\n");
    display_puts(str);

    UINT actualLength;
    if (f_mount(&drive, "0:",1) == FR_OK)  //mount
    {
//        sprintf(str, "\r\nSD-card Mount OK...\r\n");
//        display_puts(str);
//        for (jj=0; jj<5; jj++) __delay_ms(200);
    
        const char time[] = __TIME__;   //コンパイルした時刻を取得
//        char data[] = "\r\nHello world! This is text message written to SD card\r\n";
        char data[] = "\r\nHello\r\n";
        //    char data[] = "PIC 18F26K22 AM2320 temperature log\r\n";
        //    char data2[] = "XC8 v2.20 (PIC18F-K_DFP 1.4.87) SPI=4MHz, C90\r\n";
////        if (f_open(&file, "GPS-SD2.TXT", FA_WRITE | FA_CREATE_NEW ) == FR_OK) { //Open or Create TEST.TXT file
        if (f_open(&file, "TempLog.TXT", FA_WRITE | FA_CREATE_ALWAYS ) == FR_OK) { //Open or Create TEST.TXT file上書きOK
//        if (f_open(&file, "TempLog.TXT", FA_WRITE | FA_OPEN_APPEND ) == FR_OK) { //Open or Create TEST.TXT file上書きOK
//        f_open(&file, "TempLog2.TXT", FA_WRITE | FA_CREATE_ALWAYS ); //Open or Create TEST.TXT file上書きOK
            
            //ヘッダ情報を書き込むZ
            f_write(&file, time, sizeof(time)-1, &actualLength);    //write the first line
            f_write(&file, data, sizeof(data)-1, &actualLength);    //write the first line
//            f_write(&file, data2, sizeof(data2)-1, &actualLength );  //write the second line
//            f_sync(&file);
            f_close(&file);
            //    AlarmSoundOn(0);
            //    __delay_ms(200);
            //    AlarmSoundOff();
            
        }
        f_mount(0,"0:",0);  //unmount disk
        //        SDCard_CS_SetHigh(); //最初の確認用　LEDを光らせて、書き込みOKを知らせる
    }
 */    

    while (1)
    {
        // Add your application code
#ifdef DEBUG
        sprintf(str, "Tch=%d", TouchCount);
        display_drawChars(250, 80, str, BLACK, WHITE, 1);
#endif
        if (Mode <= Normal) NormalProc();
        else if (Mode & Setting) SettingProc();

        //スライドSW
        SlideSWProc();

        //GPSからデータ1行取得できたら処理
        if (GetGPS == GPS_GotData) {
            //データをチェックして、RTCを設定
            if (GPRMCdatacheck()) {
                //エラーの時は、データ取得やり直し
                GetGPS = GPS_Start;
                BufferP = Buffer;   //初期値として、Bufferの先頭に設定
            }
            else {
                GetGPS = GPS_Stop; //データ取得できたら0に
            }
        }

    }
}
/**
 End of File
*/
