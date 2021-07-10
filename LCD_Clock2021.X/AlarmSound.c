/* 
 * File:   AlarmSound.c
 * Author: K.Ohno
 *
 * ���d�X�s�[�J�[
 * TMR2�ň��d�X�s�[�J��炷
 * TMR0�ŉ��̒����𑪒�
 *  
 * ECCP2��PWM���[�h�Ŏg���ATMR2�Ŏ����ݒ�(1us-256us)�AP2A��RB3�ɏo��
 * �@�@64MHz/4/16 = 1MHz = 1us�@TMR2�̍ŏ�����
 * �@�@FOSC=64MHz, Prescaler=1:16
 * �@�f�t�H���g250us=4kHz���r�[�v���Ƃ��Đݒ�
 * TMR0: ���Ă��鎞�Ԃ𐧌�A10ms���Ɋ��荞�݂��|���A�������ԁA�������Ԃ𐧌�
 */

#include "mcc_generated_files/mcc.h"
#include "AlarmSound.h"

#define SoundDataMax 3     //����3�̃A���[���p�^�[��

//�A���[���̃p�^�[�����`�@10ms��P�ʂɖ��Ă��鎞�ԂƏ����̎��Ԃ��`
uint8_t SoundData[][7] = {
    {4, 10, 20, 10, 60},
    {6, 10, 10, 10, 10, 10, 50},
    {4, 10, 10, 10, 10},
};
uint8_t SoundDataPos;   //�ǂ̃f�[�^���g���Ă��邩
uint8_t SoundCount;     //��`����Ă钷�����J�E���g�_�E��
uint8_t SoundNum;       //�A���[���̎�ނ����A�o�ߎ��ԑ���ɂ��g�p
uint16_t SoundTime;   //�A���[�������Ă��鎞�Ԃ𑪒� (max 3000=30s�܂�)

/*
 * TMR0�̊��荞�݂̃n���h�� 10ms���ɂ����ɂ���
 * 
 * SoundCount�����Z���āA0�ɂȂ�����A���̃f�[�^���擾
 * �������A�������ŁATMR2�̃I���I�t����
 */
void SoundHandler() {

    //�A���[���Ȃ��Ă��鎞�Ԃ��v�����A30�b�A60�b�o�߂�����A���[������ύX����
    SoundTime++;    //�����o���Ă���̌o�ߎ���
    if (SoundTime >= 3000) {
        //�O�̏��u����30s�o�߂�����
        SoundTime = 0;
        SoundNum++;
        if (SoundNum < SoundDataMax) {
            SoundDataPos = SoundData[SoundNum][0];
            SoundCount = 1; //���̏����ŁA�f�N����A�ŏ�����T�E���h�J�n
        } else if (SoundNum >= 6) {
            //�A���[����5���ȏ����ςȂ��̎��́A�~�߂�
            //30�b���Ƃ�SoundNum��1������=10��5��
            AlarmSoundOff();
            return;
        }
    }

    SoundCount--;
    if (SoundCount == 0) {
        uint8_t num;
        
        //SoundCount��10ms���荞�ݖ��J�E���g�_�E�����āA0�ɂȂ�����A
        SoundDataPos++;
        //SoundNum���T�E���h�o�ߎ��ԗp�Ɏg�����߁A�f�[�^���Ȃ��̈���������Ƃɂ�������
        if (SoundNum >= SoundDataMax) num = SoundDataMax-1;
        else num = SoundNum;
        if (SoundDataPos > SoundData[num][0]) SoundDataPos = 1;
        SoundCount = SoundData[num][SoundDataPos];
        //SoundDataPos����̎��A�T�E���hOn�A�����̎��̓I�t
        if (SoundDataPos % 2) TMR2_StartTimer();
        else TMR2_StopTimer();
    }

}

/*
 * �A���[���T�E���h�J�n
 * �A���[����炷���߂̐ݒ�����{
 * 
 */
void AlarmSoundOn(uint8_t sn) {
    //���ł�On�̎��́A�������Ȃ�
    if (SoundDataPos != 0) return;
    //�T�E���h�f�[�^�Z�b�g
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
