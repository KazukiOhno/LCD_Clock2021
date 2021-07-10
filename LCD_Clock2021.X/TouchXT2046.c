/*
 * File:   TouchXT2046.c
 * Author: K.Ohno
 *
 *Color LCD 320x240に搭載のタッチインタフェース用汎用ルーチン
 * 
 * LCDボード上の5ピンインタフェース
 * T_IRQ: タッチした時、Lowになる
 * T_DO: PICのSDIに接続
 * T_DIN: PICのSDOに接続
 * T_CS: タッチのチップセレクト　LowでActive
 * T_CLK: SPIインタフェース用クロック
 * 
 * SPIインタフェースをLCDと共有する。CSで通信する対象を選択
 * 
 * 
 */

#include "mcc_generated_files/mcc.h"
#include "TouchXT2046.h" 

#define abs(a) ((a)<0 ? -(a):(a))

//(20,20)  (300,220)のタッチ上の座標をT_x1, T_y1, T_x2, T_y2

/*
 * メインプログラムで以下のような設定を行う
 * 
void TouchHandler() {
}

 * 最新版では、IOC割り込みを使わず、タイマで変化を確認して、処置するようにした
    IOCBF0_SetInterruptHandler(TouchHandler); //タッチした時の割り込み
    TouchOn = -1;
 */

/*
 * タッチ座標の生の値を取得する
 * 12ビットの座標を取得
 */
void GetTouchRawXY(uint16_t *tx, uint16_t *ty) {
    uint8_t data[2];

    //S A2 A1 A0 MODE SER/DFR PD1 PD0
    //1 1  0  1
    //  YP=+IN X-Position を測定(横向きにしているので、Y軸方向)
    SPI1_ExchangeByte(0b11010011);
    SPI1_ReadBlock(data, 2);    //12ビット、2バイト相当を読み込む
    *ty = data[0];
    *ty = (*ty << 4) + ((data[1] >> 4) & 0x0f);
    
    // Y-Position (今の使い方ではX座標)を取得
    SPI1_ExchangeByte(0b10010011);
    SPI1_ReadBlock(data, 2);    //
    *tx = data[0];
    *tx = (*tx << 4) + ((data[1] >> 4) & 0x0f);
    
}


/*
 * タッチしたX,Y座標を返す
 * エラーの時、-1を返す
 * 
 * タッチした座標情報はこのルーチンでのみ取得
 */
int8_t GetTouchLocation(uint16_t *tx, uint16_t *ty) {
    int8_t jj;
    uint8_t oversampling = 15;   //タッチ座標を7回取り込み平均化する
    uint16_t sample_sumX;   //12bitデータなので、16回(=4bit)まで加算できる
    uint16_t sample_sumY;
    uint16_t samplesX[16], samplesY[16];    //oversamplingの数に合わせること
    uint8_t num;
    uint16_t avg, diff, overrange;
    char str[100];

    if (T_IRQ_GetValue() != 0) return -1;
    
    if (SPI1_Open(Touch2M)) {
        //    SSP1ADD = 0x07; //2MHz　デフォルト設定8MHzにしているので、タッチ用に低速にする
        T_CS_SetLow(); //タッチCSをLowに
        
        // x回測定し、平均取る
        sample_sumX = 0;
        sample_sumY = 0;
        for (jj = 0; jj < oversampling; jj++) {
            GetTouchRawXY(&samplesX[jj], &samplesY[jj]);
            sample_sumX += samplesX[jj];
            sample_sumY += samplesY[jj];
        }
        SPI1_ExchangeByte(0b10000000);  // PowerDownにして、IRQをEnabledにする
        
        T_CS_SetHigh(); //CSをHighに
        //    SSP1ADD = 0x01; //8MHzに戻す
        SPI1_Close();
    }

    avg = sample_sumX / oversampling;
    num = oversampling;
    overrange = avg / 10;    //平均値の10%　　当初80にしていた
    for (jj = 0; jj < oversampling; jj++) {
        if (avg > samplesX[jj]) diff = avg - samplesX[jj];
        else diff = samplesX[jj] - avg;
        if (diff > overrange) {
            //平均より大きく外れているデータを削除
            sample_sumX -= samplesX[jj];
            num--;
        }
    }
    if (num > 0) *tx = sample_sumX / num;
    else return -1;  //有効データないので、エラー
    
    avg = sample_sumY / oversampling;
    num = oversampling;
    overrange = avg / 10;    //平均値の10%　　当初80にしていた
    for (jj = 0; jj < oversampling; jj++) {
        if (avg > samplesY[jj]) diff = avg - samplesY[jj];
        else diff = samplesY[jj] - avg;
        if (diff > overrange) {
            //平均より大きく外れているデータを削除
            sample_sumY -= samplesY[jj];
            num--;
        }
    }
    if (num > 0) *ty = sample_sumY / num;
    else return -1;  //有効データないので、エラー

#ifdef DEBUG
    sprintf(str, "T1=(%d, %d)", *tx, *ty);
    display_drawChars(0, 140, str, WHITE, BLACK, 1);
#endif

    return 0;
    
}

/*
 * タッチの座標をグラフィックの座標に変換
 */
void TransCoordination(uint16_t x, uint16_t y, uint16_t *xg, uint16_t *yg) {
    int32_t xx, yy;
    char str[100];
    
#ifdef DEBUG
    sprintf(str, "P1=(%d, %d) P2=(%d, %d)", T_x1, T_y1, T_x2, T_y2);
    display_drawChars(0, 140, str, WHITE, BLACK, 1);
#endif

    if ((T_x2 == T_x1) || (T_y2 == T_y1)) {
        *xg = x;
        *yg = y;
        return;
    }

    //以下の計算精度が問題
    xx =  280 * ((int32_t)x - T_x1) ;
    xx = xx/(T_x2 - T_x1) + 20;
    if (xx<0) *xg = 0;
    else if (xx>=320) *xg = 319;
    else *xg = (uint16_t) xx;
    
    yy = (int32_t)y - T_y1;
    yy = yy*200/(T_y2 - T_y1) + 20;
    if (yy < 0) *yg = 0;
    else if (yy>=240) *yg = 239;
    else *yg = (uint16_t) yy;

}
