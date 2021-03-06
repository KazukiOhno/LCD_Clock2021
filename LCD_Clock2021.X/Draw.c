/*
 * File:   Draw.c
 * Author: K.Ohno
 *
 * 描画関係ルーチン
 */

#include "mcc_generated_files/mcc.h"
#include "main.h"
#include "Draw.h"
#include "RTC8025.h"
#include "LCD320x240color.h"

//char WeekDays[][4] = {
//    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat",
////    "0x80", "0x81", "0x82", "0x83", "0x84", "0x85", "0x86",
//};

uint16_t AlarmColor[2] = {0x4208, WHITE};   //0: SlideSW off時の色、1: on時の色
// 0x4208;    //灰色

// 絶対値を取得する関数マクロ定義
#define ABS(x) ((x) < 0 ? -(x) : (x))

//メモリ節約するため、アナログクロックを外す
//#define ANALOGCLOCK

typedef enum {
    segA,
    segB,
    segC,
    segD,
    segE,
    segF,
    segG,
    segG2,
};

/*
 * 7セグメントの1つを描画
 * seg a=0, b=1, c=2, d=3, e=4, f=5, g=6
 * (xorg, yorg): 基準座標
 * 基本的に三角形で描画する
 */
void draw1segment(uint16_t xorg, uint16_t yorg, uint8_t seg, uint8_t size, uint16_t color) {
    //seg gを除き、4角形の頂点の座標を定義
    //整数演算のため、データは10倍にして精度上げる
    uint16_t x[][4] ={
        { 50, 180, 160, 70 },   // seg a
        { 170, 190, 171, 157 }, // seg b
        { 150, 170, 151, 137 }, // seg c
        { 20, 120, 140, 0},    // seg d
        { 20, 40, 29, 1 },     // seg e
        { 40, 60, 49, 21 },     // seg f
        { 30, 50, 150, 40 },    // seg g
        { 140, 40, 140, 160 },   // seg g-2
    };
    uint16_t y[][4] ={
        { 0, 0, 20, 20, }, // seg a
        {  30,  10, 126, 112, }, // seg b
        { 150, 130, 246, 232, },   // seg c
        { 240, 240, 260, 260, },   // seg d
        { 130, 150, 232, 246 },     // seg e
        { 10,  30,  112, 126 },     // seg f
        { 130, 120, 120, 140 },   // seg g
        { 120, 140, 140, 130 },   // seg g-2
    };
        
    uint8_t jj;
    uint16_t xx[4], yy[4];
    
    for (jj=0; jj<4; jj++) {
        xx[jj] = xorg + (x[seg][jj]*size)/10;
        yy[jj] = yorg + (y[seg][jj]*size)/10;
    }

    display_fillTriangle(xx[0], yy[0], xx[1], yy[1], xx[2], yy[2], color);
    display_fillTriangle(xx[0], yy[0], xx[2], yy[2], xx[3], yy[3], color);

}

/*
 * num(0-9)を7セグスタイルで表示
 * 
 *     a
 *    ---
 *  f|   |b
 *    -g-
 *  e|   |c
 *    ---
 *     d
 * gfe dcbaの順でlsb=aで光る部分を表現
 * x, y座標に、sizeの整数倍で、colorの色で表示
 * 基本サイズ: 19x27　　周囲に空白なしのサイズ
 * 20x30と想定して描画するとちょうどよい見栄え
 * 
 */
void Dispseg(uint16_t x, uint16_t y, uint8_t num, uint8_t size, uint16_t color) {
    const uint8_t pattern[] = {
        0x3f,   //0
        0x06,   //1
        0x5b,   //2
        0x4f,   //3
        0x66,   //4
        0x6d,   //5
        0x7d,   //6
        0x07,   //7
        0x7f,   //8
        0x6f,   //9
    };
    uint8_t jj, loc, seg;
    
    seg = segA;
    loc = 0x01;
    for (jj=0; jj<6; jj++) {
        if (pattern[num] & loc) {
            draw1segment(x, y, seg, size, color);
        }
        seg++;
        loc = loc << 1;
    }
    
    // g セグメント
    if (pattern[num] & 0x40) {
        draw1segment(x, y, segG, size, color);
        draw1segment(x, y, segG2, size, color);
    }
    
}

/*
 * mode:
 * DisplayMode2: HHMMSS=0: HH:MM:SS
 * DisplayMode1: HHMMss=2: HH:MM + small SS
 * (x,y)座標を左上の起点として描画
 * 文字の外形より2ドット分大きめの領域を描画域と想定
 * 空白　2桁　コロン　2桁　コロン　2桁　　空白
 * 2    20x2   5     20x2  5     20x2　 2
 * 縦=27 x size　上下の空白　2x2
 * size=2を標準と想定　　空白以外は、size倍のサイズ
 * 
 */
void DispTime(uint8_t mode, uint8_t * datetime, uint16_t x, uint16_t y, uint16_t color) {
    int8_t jj;
    uint16_t xd, yd;
    uint8_t endpoint;
    uint8_t size;

    if (mode == DispSetting) {
        size = 2;
        endpoint = 1;
    }
    else {
        size = 2 - mode; //DisplayMode1の時: size=2, Mode2の時: size=1   
        endpoint = 0;
    }
    
//    fillRect(xd, yd, (20*6+5*2)*size, 30*size, BLACK);
    xd = x +2;
    yd = y +2;

    for (jj=2; jj>=endpoint; jj--) {
        if ((mode == DisplayMode1) && (jj == 0)) size--;  //秒の表示だけ小さく
        if (size < 1) size = 1;
        //書き換えが必要な桁だけ描画
        //上位桁
        if ((datetime[jj] >>4) != (preDateTime[jj] >>4)) {
            fillRect(xd, yd-2, 20*size, 27*size+4, BLACK);
            Dispseg(xd, yd, datetime[jj]>>4, size, color);
        }
        //下位桁
        xd += 20*size;        
        if ((datetime[jj] & 0x0f) != (preDateTime[jj] & 0x0f)) {
            fillRect(xd, yd-2, 20*size, 27*size+4, BLACK);
            Dispseg(xd, yd, datetime[jj] & 0x0f, size, color);
        }
        preDateTime[jj] = datetime[jj];
        
        //：を描画
        xd += 20*size;
        if ((jj == 2) || ((mode == DisplayMode2) && (jj==1))) {
            //時の表示、もしくは時分秒表示の時は分の表示の後ろに:を表示
            fillRect(xd, yd-2, 6*size+1, 27*size+4, BLACK);
            fillRect(xd+1, yd+7*size, 3*size, 3*size, color);
            fillRect(xd, yd+17*size, 3*size, 3*size, color);
            xd += 6*size ;
        }
    }
    
}

// 0~174度まで6度ごとの256倍にしたsinテーブル
//　アラーム針を5分単位にした場合、2.5度単位(30度で12分割)が欲しい。
//　　10分単位で5度と、6度毎のテーブルと若干合っていない。3度単位位は欲しい。
//sin(θ): θ=角度/6
//180度以降は、-sin(θ-180)という形で参照
//cos(θ)=sin(θ+90)
/*
const uint16_t sin_table6[] = {
      0,  26,  53,  79, 104, 128, 
    150, 171, 190, 207, 221, 233,
    243, 250, 254, 256, 254, 250,
    243, 233, 221, 207, 190, 171,
    150, 128, 104,  79,  53,  26,
};
*/
//1度単位のテーブル
const uint16_t sin_table[] = {
    //0    1    2    3    4    5    6    7    8    9
      0,   4,   8,  13,  17,  22,  26,  31,  35,  40,   //00-
     44,  48,  53,  57,  61,  66,  70,  74,  79,  83,   //10-
     87,  91,  95, 100, 104, 108, 112, 116, 120, 124,   //20-
    128, 131, 135, 139, 143, 146, 150, 154, 157, 161,   //30-
    164, 167, 171, 174, 177, 181, 184, 187, 190, 193,   //40-
    196, 198, 201, 204, 207, 209, 212, 214, 217, 219,   //50-
    221, 223, 226, 228, 230, 232, 233, 235, 237, 238,   //60-
    240, 242, 243, 244, 246, 247, 248, 249, 250, 251,   //70-
    252, 252, 253, 254, 254, 255, 255, 255, 255, 255,   //80-
    256, 255, 255, 255, 255, 255, 254, 254, 253, 252,   //90-
    252, 251, 250, 249, 248, 247, 246, 244, 243, 242,   //100-
    240, 238, 237, 235, 233, 232, 230, 228, 226, 223,   //110-
    221, 219, 217, 214, 212, 209, 207, 204, 201, 198,   //120-
    196, 193, 190, 187, 184, 181, 177, 174, 171, 167,   //130-
    164, 161, 157, 154, 150, 146, 143, 139, 135, 131,   //140-
    128, 124, 120, 116, 112, 108, 104, 100,  95,  91,   //150-
     87,  83,  79,  74,  70,  66,  61,  57,  53,  48,   //160-
     44,  40,  35,  31,  26,  22,  17,  13,   8,   4,   //170-
};

//degreeで指定された角度=thetaに対応したsinを返す(x256)
int16_t sind(uint16_t theta) {
    theta = theta % 360;

    if (theta >= 180) {
        //-sind(θ-180)
        return -sin_table[theta-180];          //度単位のテーブル用
//        return -sin_table6[(theta-180)/6];   //6度単位のテーブル用
    } else {
        return sin_table[theta];
//        return sin_table6[theta/6];
    }
}

//degreeで指定された角度=thetaに対応したcosを返す(x256)
int16_t cosd(uint16_t theta) {
    theta = theta+90;
    return sind(theta);
}

/*
 * アナログ時計表示
 * mode: 
 * datetime: 日付時間
 * xx, yy: 中心座標
 * size: 外形円の大きさ
 * color: 針の色指定
 */
#define SizeMin  3      //分針の太さ
#define SizeHour 4      //時針の太さ
//#define ColorHour   GREY
//#define ColorMin    GREY
#define ColorSec    RED
#define ColorAlarm  YELLOW


/*
 * アナログ時計のアラーム針を描画/消去
 */
void drawAlarmNeedle(uint16_t *x, uint16_t *y, uint16_t color) {
    int16_t xd, yd;
    
    //アラーム針描画
    display_drawLine(x[0], y[0], x[1], y[1], color);
    
    xd = x[1]-x[0];
    yd = y[1]-y[0];
    if ( ABS(yd) > ABS(xd)) {
        //角度により、移動する方向を変え、少し太くする
        display_drawLine(x[0]+1, y[0], x[1]+1, y[1], color);
        display_drawLine(x[0]-1, y[0], x[1]-1, y[1], color);
    } else {
        display_drawLine(x[0], y[0]-1, x[1], y[1]-1, color);
        display_drawLine(x[0], y[0]+1, x[1], y[1]+1, color);
    }
}

//時針、分針の描画/消去
void drawNeedle(uint16_t *x, uint16_t *y, uint16_t color, uint16_t color2) {
    display_fillTriangle(x[1], y[1], x[2], y[2], x[3], y[3], color);
    display_drawTriangle(x[1], y[1], x[2], y[2], x[3], y[3], color2);
    //反対側の出っ張り
    display_fillTriangle(x[4], y[4], x[2], y[2], x[3], y[3], color);
    display_drawTriangle(x[4], y[4], x[2], y[2], x[3], y[3], color2);

}

//秒針の描画/消去
void drawSecNeedle(uint16_t *x, uint16_t *y, uint16_t color) {
    display_drawLine(x[1], y[1], x[2], y[2], color);
    display_fillTriangle(x[0], y[0], x[3], y[3], x[4], y[4], color);
   
}

        
//アラーム針の長さは長針の75%
//長針：分針の長さは、目盛の線より3ドット内側
//短針
//秒針
//目盛の長さ=3
//ドットの大きさ=2
void DrawAnalogClock(uint8_t mode, uint8_t * datetime, uint16_t xx, uint16_t yy, uint8_t size, uint16_t color, uint8_t *alarmtime) {
    uint16_t xc, yc;    //時計の中心
    uint16_t rc, rc1, rcs, rcm, rch, rca;    //時計のサイズ、目盛、秒針、分針、時針、アラーム
    uint16_t rc2;
    uint16_t rcs2, rcm2, rcm3, rch2, rch3, rca2;    //時計のサイズ、目盛、秒針、分針、時針、アラーム
    uint16_t x[5], y[5];    //座標計算した結果を格納
    uint16_t temp, angle;
    int8_t jj, hh, mm, ss, kk;
    static int8_t almhh, almmm;
    //前回の座標
    static uint16_t phx[5], phy[5], pmx[5], pmy[5], psx[5], psy[5], pax[5], pay[5];
    //今回の座標
    uint16_t chx[5], chy[5], cmx[5], cmy[5], csx[5], csy[5], cax[5], cay[5];
    char str[3];
    int8_t minupdate = 0;
    uint16_t bcolor;
    
    if (color == GREEN) color = GREY;   //色指定が緑だったら灰色に変更してしまう。？？？の時
    
    //外形円の描画
    xc = xx;    //中心座標
    yc = yy;
    rc = size;  //外形の半径
    display_drawCircle(xc, yc, rc+3, color);    //一回り大きめの円
    display_drawCircle(xc, yc, rc, color);      //sizeで指定された円

    rc1 = rc -3;    // 目盛より内側　一番長い長針の長さ

    //アラーム針の描画準備----------------------------------------------------
    //最新のアラーム針の座標を計算
    rca = rc1 *3 /4;    //アラーム用の針の長さは、長針の75%

    if (alarmtime != NULL) {
        almmm = Bcd2Hex(alarmtime[0]);
        almhh = Bcd2Hex(alarmtime[1]);
    }
    almhh = almhh % 12;
    angle = almhh * 30 + almmm/2;   //角度に変換
    cax[0] = xc;    //中心座標
    cay[0] = yc;
    cax[1] = xc + rca * sind(angle)/256;
    cay[1] = yc - rca * cosd(angle)/256;
    
    //ちらつきをなくすため、表示に変化ある針だけを消去し、それ以外は上書きする
    //前の表示を消す
    if ((cax[1] == pax[1]) && (cay[1] == pay[1])) {
        //前と座標が変わっていない時は、消さない。最初の描画も同じ座標にしているので消去しない
    } else {
        drawAlarmNeedle(pax, pay, BLACK);   //元の秒針を消す
        //描画用の座標を保存
        for (jj=0; jj<2; jj++) {
            pax[jj] = cax[jj];
            pay[jj] = cay[jj];
        }
    }

    //分針の描画準備--------------------------------------------------------
    //分針の位置が変わったら、再描画の前に、元の時針、分針を消す
    if (preDateTime[1] != datetime[1]) {
        preDateTime[1] = datetime[1];
        mm = ((datetime[1]>>4)*10 + (datetime[1] & 0x0f));
        hh = (datetime[2]>>4)*10 + (datetime[2] & 0x0f);

        //分針の座標を計算
        rcm = rc1 -5;   //分針の長さは、目盛の線より3ドット内側
        angle = mm*6;
        cmx[1] = xc + rcm * sind(angle)/256;
        cmy[1] = yc - rcm * cosd(angle)/256;
    
        rcm2 = SizeMin;       //分針の幅
        angle = mm*6 +90;
        cmx[2] = xc + rcm2 * sind(angle)/256;
        cmy[2] = yc - rcm2 * cosd(angle)/256;
        cmx[3] = xc - (cmx[2]-xc);
        cmy[3] = yc - (cmy[2]-yc);

        //反対側の出っ張り
        rcm3 = 10;
        angle = mm*6 +180;
        cmx[4] = xc + rcm3 * sind(angle)/256;
        cmy[4] = yc - rcm3 * cosd(angle)/256;

        //時針の座標計算
        //分針が移動したら、時針も移動計算 (実際は2分毎)
        //　分のデータも取り込んで時針の角度決める
        hh = hh % 12;
        angle = hh * 30 + mm/2;   //角度に変換 8bit変数ではNG
        
        rch = rc1 *7/10;    //短針の長さは、70%
        chx[0] = xc;  //中心座標
        chy[0] = yc;
        chx[1] = xc + rch * sind(angle)/256;
        chy[1] = yc - rch * cosd(angle)/256;
        
        rch2 = SizeHour;       //時針の幅
        chx[2] = xc + rch2 * sind(angle +90)/256;
        chy[2] = yc - rch2 * cosd(angle +90)/256;
        chx[3] = xc - (chx[2]-xc);
        chy[3] = yc - (chy[2]-yc);
        
        //反対側の出っ張り
        rch3 = 10;
        chx[4] = xc + rch3 * sind(angle +180)/256;
        chy[4] = yc - rch3 * cosd(angle +180)/256;
        
        //分が変更になったら。時間が変わる時は、分の変更と同時に時針の消去
        drawNeedle(phx, phy, BLACK, BLACK);
        
        //分針の消去
        drawNeedle(pmx, pmy, BLACK, BLACK);
        minupdate = 1;  //分針updateする時

        //描画座標を保存
        for (jj=0; jj<5; jj++) {
            phx[jj] = chx[jj];
            phy[jj] = chy[jj];
            pmx[jj] = cmx[jj];
            pmy[jj] = cmy[jj];
        }
    }

    //最新の秒針の座標を計算
    ss = -1;
    if (preDateTime[0] != datetime[0]) {
        preDateTime[0] = datetime[0];
        ss = ((datetime[0]>>4)*10 + (datetime[0] & 0x0f));  //0-59の数値
        rcs = rc1 - 4;  //秒針の長さは、これ
        angle = ss*6;
        csx[0] = xc;  //中心座標
        csy[0] = yc;
        csx[1] = xc + rcs * sind(angle)/256;
        csy[1] = yc - rcs * cosd(angle)/256;
        
        rcs2 = 20;   //反対側に出っ張る量
        angle = ss*6+180;
        csx[2] = xc + rcs2 * sind(angle)/256;
        csy[2] = yc - rcs2 * cosd(angle)/256;
        csx[3] = xc + rcs2 * sind(angle+6)/256;
        csy[3] = yc - rcs2 * cosd(angle+6)/256;
        csx[4] = xc + rcs2 * sind(angle-6)/256;
        csy[4] = yc - rcs2 * cosd(angle-6)/256;
        
        //秒針を消去
        drawSecNeedle(psx, psy, BLACK);

        //描画座標を保存
        for (jj=0; jj<5; jj++) {
            psx[jj] = csx[jj];
            psy[jj] = csy[jj];
        }
    }
    
    //目盛の描画------------------------------------------------------------
    rc1 = rc -3;    //線の長さを3とする
    rc2 = rc1 -10;  //数字の位置
    for (jj=0; jj<60; jj++) {
        angle = jj*6;
        x[1] = xc + (rc * sind(angle))/256;
        y[1] = yc - (rc * cosd(angle))/256;
        x[2] = xc + (rc1 * sind(angle))/256;
        y[2] = yc - (rc1 * cosd(angle))/256;
        
        if (jj%5 == 0) {
            display_fillCircle(x[2], y[2], 2, color);   //時の所は、ドット、半径2
            //文字盤表示。ただし、minupdate=0で、mmが5の倍数の時は、描画しない(そうしないとチラつく)
            if ((minupdate == 0) && (jj==mm)) continue;
            x[3] = xc + (rc2 * sind(angle))/256;
            y[3] = yc - (rc2 * cosd(angle))/256;
            //表示する数字を設定
            if (jj==0) kk=12;   //0時の所は、12に設定
            else kk=jj/5;
            sprintf(str, "%d", kk);
            //背景色を同じにすると、背景色を塗らない=透明と同じ
            if (kk<10)
                display_drawChars(x[3]-4, y[3]-3, str, color, color, 1);
            else
                display_drawChars(x[3]-7, y[3]-3, str, color, color, 1);
        } else {
            display_drawLine(x[1], y[1], x[2], y[2], color);
        }
    }
    
    // AM/PMの表示
    //ちらつきをなくすため、背景部分は変化させない描画を行うが、
    //AM/PMの切替時だけ、背景描画実施
    if (preDateTime[2]/0x12 != datetime[2]/0x12) {
        bcolor = BLACK;
    }
    else {
        bcolor = color;
    }
    preDateTime[2] = datetime[2];
    if (datetime[2] < 0x12) sprintf(str, "AM");
    else sprintf(str, "PM");
    display_drawChars(xx-14, yy+rc/2, str, color, bcolor, 2);
    
    //ここから、針の描画を実行
    //アラーム針描画------------------------------------------------------
    drawAlarmNeedle(pax, pay, ColorAlarm);  //アラーム針の色

    //時針の描画------------------------------------------------------
    drawNeedle(phx, phy, color, WHITE);    //時針の色は、引数からもらう

    //分針の描画------------------------------------------------------
    drawNeedle(pmx, pmy, color, WHITE);

    //秒針の描画------------------------------------------------------
    display_fillCircle(psx[0], psy[0], 3, ColorSec);    //秒針の色
    drawSecNeedle(psx, psy, ColorSec);
    
}

/*
 * フォント指定して大きさに関係なく描画できるようにする
 * 左上の座標: xs, ys
 * フォントサイズ: fontsize, fontw, fonth
 * 表示ピッチ: xpitch(未使用), ypitch, space
 */

void basicDrawCalendar(uint8_t year, uint8_t month, uint16_t xs, uint16_t ys, uint8_t xpitch, uint8_t ypitch, 
        uint8_t fontsize, uint8_t fontw, uint8_t fonth, uint8_t space) {
    uint8_t jj, kk;
    uint16_t xx, yy;
    char str[5];
    int8_t startday;    //最初の日曜の日付、0以下は前月
    int8_t maxdays;
    uint8_t yr, mm, dd, wd;
    uint16_t color = WHITE;     //文字色
    uint16_t backColor = BLACK; //背景色
    uint8_t thismonth;
    //曜日ごとの色指定
    uint16_t dayColor[] = {RED, WHITE, WHITE, WHITE, WHITE, WHITE, BLUE, };

    yr = year;
    mm = month;
    dd = 1;
    wd = getWeekdays(&yr, &mm, &dd);  //1日の曜日を取得
    //1日が日曜なら1、月曜なら0、火曜なら-1、水曜なら-2になる
    startday = 1- wd;
    
    if (month == Bcd2Hex(DateTime[5])) thismonth = 1;
    else thismonth = 0;

    //当該月の最終日を取得
    dd = 31;
    getWeekdays(&yr, &mm, &dd);  //31日の曜日を取得。その日がない場合、自動調整される
    //mmが変更になったら、31日はなかったとわかる
    if (month != mm) {
        maxdays = 31 - dd;    //その月の最終日
    } else {
        maxdays = 31;
    }   
    
    //対象領域をクリア: fontwで14文字分?7日のスペース分、Y方向は8行分
    //xの開始を1ドット左にして、サイズもその分大きく。枠を付けた時はみ出しを防止
    lcd_fill_rect(xs-1, ys, xs+fontw*14+space*7+1, ys+ypitch*8, backColor);
    //月の描画
    yy = ys;
    xx = xs + 3*(fontw*2+space);
    if (month <10) xx = xx + fontw/2;    //2桁の時、半文字ずらす    
    sprintf(str, "%d", month);
    display_drawChars(xx, yy, str, color, backColor, fontsize);

    //曜日
    yy += ypitch;
    xx = xs+ fontw/2;   //フォント半文字分らす
    for (jj=0; jj<7; jj++) {
//        sprintf(str, "%c", WeekDays[jj][0]);  //SMTWTFSという表記方式
        sprintf(str, "%c", 0x80+jj);    //日月火水木金土という表示
        display_drawChars(xx, yy, str, dayColor[jj], backColor, fontsize);
//        display_drawChars(xx, yy, str, color, backColor, fontsize);   //こちらの方が8byte小さい
        xx += fontw * 2 +space;
    }
    
    //日付
    yy += ypitch;
    if (fontsize == 1) display_SetFont(tinyfont);

    for (kk = 0; kk<6; kk++) {
        xx = xs;
        for (jj = 0; jj < 7; jj++) {
            if (startday > 0 && startday <= maxdays) {
                sprintf(str, "%2d", startday);
                display_drawChars(xx, yy, str, dayColor[jj], backColor, fontsize);
                //今日の日付に印をつける
                if (thismonth && (startday == Bcd2Hex(DateTime[4]))) {
//                    display_drawChars2(xx, yy, str, backColor, color, 1);
                    //反転だと視認しにくいので枠を付ける
                    display_drawRect(xx-1, yy-2, fontw*2+2, fonth+2, WHITE);
                }
            }
            startday++;
            xx += fontw*2 + space;
        }
        yy += ypitch;
    }
    display_SetFont(NormalFont);  //元に戻す

}

/*
 * 大きなカレンダーを表示
 * xs,ysが描画する左上の座標
 */
void DrawBigCalendar(uint8_t year, uint8_t month, uint16_t xs, uint16_t ys) {
    uint8_t fontw, fonth, ypitch, space;

    //表示フォントの大きさ
    fontw = CurrentFont.xsize * 2;    //16
    space = 4;
    fonth = CurrentFont.ysize*2 +2;   //18;
    ypitch = fonth+4;
    
    basicDrawCalendar(year, month, xs, ys, 0, ypitch, 2, fontw, fonth, space);
        
}


/*
 * 小さい月カレンダーを表示
 * year: 下2桁
 * month: 1-12
 * xs, ys: 表示の基準位置　左上
 */
void DrawSmallCalendar(uint8_t year, uint8_t month, uint16_t xs, uint16_t ys) {
    uint8_t fontw, fonth, ypitch;

    //前月、次月の表示のため、0月、13月という設定を許す
    if (month == 0) {
        month = 12;
        year--;
    }
    if (month == 13) {
        month = 1;
        year++;
    }
    
    fontw = 6;
    fonth = 8;
    ypitch = fonth+2;
    
    basicDrawCalendar(year, month, xs, ys, 0, ypitch, 1, fontw, fonth, 1);
    
}


/*
 * 小さいカレンダーを3つ表示する
 * 　引数の年月は、表示中央の月を指す
 * 　BCDではない
 * mode: ディスプレイモードにより、座標を決定
 * startmonth: 表示する現在の年月を指定
 * 
 */
void Draw3month(uint8_t startyear, uint8_t startmonth, uint8_t mode) {
    uint8_t yy, mm;
    uint8_t jj;
    uint16_t xstep, ystep;
    
    if (mode == DisplayMode1) ystep = 0;
    else ystep = CalendarYw;
    
    if (mode == DisplayMode3) xstep = 0;
    else xstep = CalendarXstep;
    
    //3か月カレンダのX,Y座標は、リソースから取得
    //Mode3のアナログ時計表示の時は、2か月分しか表示しない→表示が欠けても良しとする
    for (jj=0; jj<3; jj++) {
        DrawSmallCalendar(startyear, startmonth+jj-1, MonthCalendar[mode].x +xstep*jj, MonthCalendar[mode].y +ystep*jj);
    }
}

/*
 * DisplayModeに応じてカレンダーを表示する
 */
void DrawCalendar(uint8_t mode) {
    if (mode == DisplayMode2)
        DrawBigCalendar(Bcd2Hex(DateTime[6]), Bcd2Hex(DateTime[5]), MonthCalendar[mode].x, MonthCalendar[mode].y);
    else
        Draw3month(Bcd2Hex(DateTime[6]), Bcd2Hex(DateTime[5]), mode);
    
}

/*
 * sw=SlideSWStatus: 0/1=Alarm off, 2/3=Alarm on
 */
void drawAlarmTime(uint8_t mode, uint8_t *alarmtime, uint8_t sw) {
    char str[100];
    char ampm[][3] = {"AM", "PM"};
    int8_t ap;
    char pm;    //オンオフで+-切替
    uint8_t idx = sw>>1;
    uint8_t size;
    
    //アラーム時刻の表示色は、AlarmColorで設定
    if (idx) pm = '+';
    else pm = '-';
    
    if (mode == DisplayMode3) {
        DrawAnalogClock(mode, DateTime, RTime[mode].x, RTime[mode].y, RTime[mode].xw, GREY, alarmtime);
        if (alarmtime[1] >= 0x12) ap= 1 ;   //BCDで12時以降なら午後
        else ap = 0;
        sprintf(str, "Alarm%c %s %02d:%02x", pm, ampm[ap], (alarmtime[1] & 0xf) + (alarmtime[1] >> 4)*10 -12*ap, alarmtime[0]);
        size = 0x13;
    } else {
        sprintf(str, "ALM%c %02x:%02x", pm, alarmtime[1], alarmtime[0]);
        size = 2;
    }
    display_drawChars(RAlarm[mode].x, RAlarm[mode].y, str, AlarmColor[idx], BLACK, size);

}

void drawTime(uint8_t mode, uint8_t * datetime, uint16_t color) {
    char str[100];

    if (mode == DisplayMode3) {
        //外形円の座標= (140, 130) 半径=90
        DrawAnalogClock(mode, datetime, RTime[mode].x, RTime[mode].y, RTime[mode].xw, color, NULL);
    } else {
        DispTime(mode, datetime, RTime[mode].x, RTime[mode].y, color);
    }
}

void drawDate(uint8_t mode, uint8_t *datetime) {
    char str[100];

//    sprintf(str, "20%02x/%02x/%02x(%s)", datetime[6], datetime[5], datetime[4], WeekDays[datetime[3]]);
    sprintf(str, "20%02x/%02x/%02x(%c)", datetime[6], datetime[5], datetime[4], 0x80+datetime[3]);
    display_drawChars(RYear[mode].x, RYear[mode].y, str, WHITE, BLACK, 2);
}

void drawDateTime(uint8_t mode, uint8_t *datetime) {

    drawDate(mode, datetime);
    drawTime(mode, datetime, GREEN);
}

void drawTempHumidity(uint8_t mode, int16_t temp, int16_t humidity) {
    char str[50];

    sprintf(str, "%2d.%1d\x87", temp / 10, temp % 10);  //\x87は、℃のコード
    display_drawChars(RTemp[mode].x, RTemp[mode].y, str, WHITE, BLACK, 2);
    sprintf(str, "%2d%%", humidity / 10);
    display_drawChars(RHumidity[mode].x, RHumidity[mode].y, str, WHITE, BLACK, 2);

}
