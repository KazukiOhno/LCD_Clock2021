/* 
 * File:   AlarmSound.c
 * Author: K.Ohno
 *
 * 圧電スピーカー
 * TMR2で圧電スピーカを鳴らす
 * TMR0で音の長さを測定
 *  
 * ECCP2をPWMモードで使い、TMR2で周期設定(1us-256us)、P2A＝RB3に出力
 * 　　64MHz/4/16 = 1MHz = 1us　TMR2の最小周期
 * 　　FOSC=64MHz, Prescaler=1:16
 * 　デフォルト250us=4kHzをビープ音として設定
 * TMR0: 鳴っている時間を制御、10ms毎に割り込みを掛け、発音時間、消音時間を制御
 */

#include "mcc_generated_files/mcc.h"
#include "AlarmSound.h"

#define SoundDataMax 3     //現状3つのアラームパターン

//アラームのパターンを定義　10msを単位に鳴っている時間と消音の時間を定義
uint8_t SoundData[][7] = {
    {4, 10, 20, 10, 60},
    {6, 10, 10, 10, 10, 10, 50},
    {4, 10, 10, 10, 10},
};
uint8_t SoundDataPos;   //どのデータを使っているか
uint8_t SoundCount;     //定義されてる長さをカウントダウン
uint8_t SoundNum;       //アラームの種類を特定、経過時間測定にも使用
uint16_t SoundTime;   //アラームが鳴っている時間を測定 (max 3000=30sまで)

/*
 * TMR0の割り込みのハンドラ 10ms毎にここにくる
 * 
 * SoundCountを減算して、0になったら、次のデータを取得
 * 発音か、消音かで、TMR2のオンオフする
 */
void SoundHandler() {

    //アラームなっている時間を計測し、30秒、60秒経過したらアラーム音を変更する
    SoundTime++;    //音を出してからの経過時間
    if (SoundTime >= 3000) {
        //前の処置から30s経過したら
        SoundTime = 0;
        SoundNum++;
        if (SoundNum < SoundDataMax) {
            SoundDataPos = SoundData[SoundNum][0];
            SoundCount = 1; //下の処理で、デクリ後、最初からサウンド開始
        } else if (SoundNum >= 6) {
            //アラームが5分以上鳴りっぱなしの時は、止める
            //30秒ごとにSoundNumが1つ増える=10で5分
            AlarmSoundOff();
            return;
        }
    }

    SoundCount--;
    if (SoundCount == 0) {
        uint8_t num;
        
        //SoundCountを10ms割り込み毎カウントダウンして、0になったら、
        SoundDataPos++;
        //SoundNumをサウンド経過時間用に使うため、データがない領域も示すことにしたため
        if (SoundNum >= SoundDataMax) num = SoundDataMax-1;
        else num = SoundNum;
        if (SoundDataPos > SoundData[num][0]) SoundDataPos = 1;
        SoundCount = SoundData[num][SoundDataPos];
        //SoundDataPosが奇数の時、サウンドOn、偶数の時はオフ
        if (SoundDataPos % 2) TMR2_StartTimer();
        else TMR2_StopTimer();
    }

}

/*
 * アラームサウンド開始
 * アラームを鳴らすための設定を実施
 * 
 */
void AlarmSoundOn(uint8_t sn) {
    //すでにOnの時は、何もしない
    if (SoundDataPos != 0) return;
    //サウンドデータセット
    SoundNum = sn;
    SoundDataPos = 1;
    SoundCount = SoundData[SoundNum][SoundDataPos];
    SoundTime = 0;

    RB3_SetDigitalOutput();
    TMR2_StartTimer();
    TMR0_Reload();
    TMR0_StartTimer();
    
}

void AlarmSoundOff(void) {
    TMR2_StopTimer();
    TMR0_StopTimer();
    SoundDataPos = 0;
    RB3_SetDigitalInput();
}

void AlarmInitialize(void) {
    AlarmSoundOff();
    TMR0_SetInterruptHandler(SoundHandler);
}
