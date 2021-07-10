/*
 * File:   TouchXT2046.c
 * Author: K.Ohno
 *
 *Color LCD 320x240�ɓ��ڂ̃^�b�`�C���^�t�F�[�X�p�ėp���[�`��
 * 
 * LCD�{�[�h���5�s���C���^�t�F�[�X
 * T_IRQ: �^�b�`�������ALow�ɂȂ�
 * T_DO: PIC��SDI�ɐڑ�
 * T_DIN: PIC��SDO�ɐڑ�
 * T_CS: �^�b�`�̃`�b�v�Z���N�g�@Low��Active
 * T_CLK: SPI�C���^�t�F�[�X�p�N���b�N
 * 
 * SPI�C���^�t�F�[�X��LCD�Ƌ��L����BCS�ŒʐM����Ώۂ�I��
 * 
 * 
 */

#include "mcc_generated_files/mcc.h"
#include "TouchXT2046.h" 

#define abs(a) ((a)<0 ? -(a):(a))

//(20,20)  (300,220)�̃^�b�`��̍��W��T_x1, T_y1, T_x2, T_y2

/*
 * ���C���v���O�����ňȉ��̂悤�Ȑݒ���s��
 * 
void TouchHandler() {
}

 * �ŐV�łł́AIOC���荞�݂��g�킸�A�^�C�}�ŕω����m�F���āA���u����悤�ɂ���
    IOCBF0_SetInterruptHandler(TouchHandler); //�^�b�`�������̊��荞��
    TouchOn = -1;
 */

/*
 * �^�b�`���W�̐��̒l���擾����
 * 12�r�b�g�̍��W���擾
 */
void GetTouchRawXY(uint16_t *tx, uint16_t *ty) {
    uint8_t data[2];

    //S A2 A1 A0 MODE SER/DFR PD1 PD0
    //1 1  0  1
    //  YP=+IN X-Position �𑪒�(�������ɂ��Ă���̂ŁAY������)
    SPI1_ExchangeByte(0b11010011);
    SPI1_ReadBlock(data, 2);    //12�r�b�g�A2�o�C�g������ǂݍ���
    *ty = data[0];
    *ty = (*ty << 4) + ((data[1] >> 4) & 0x0f);
    
    // Y-Position (���̎g�����ł�X���W)���擾
    SPI1_ExchangeByte(0b10010011);
    SPI1_ReadBlock(data, 2);    //
    *tx = data[0];
    *tx = (*tx << 4) + ((data[1] >> 4) & 0x0f);
    
}


/*
 * �^�b�`����X,Y���W��Ԃ�
 * �G���[�̎��A-1��Ԃ�
 * 
 * �^�b�`�������W���͂��̃��[�`���ł̂ݎ擾
 */
int8_t GetTouchLocation(uint16_t *tx, uint16_t *ty) {
    int8_t jj;
    uint8_t oversampling = 15;   //�^�b�`���W��7���荞�ݕ��ω�����
    uint16_t sample_sumX;   //12bit�f�[�^�Ȃ̂ŁA16��(=4bit)�܂ŉ��Z�ł���
    uint16_t sample_sumY;
    uint16_t samplesX[16], samplesY[16];    //oversampling�̐��ɍ��킹�邱��
    uint8_t num;
    uint16_t avg, diff, overrange;
    char str[100];

    if (T_IRQ_GetValue() != 0) return -1;
    
    if (SPI1_Open(Touch2M)) {
        //    SSP1ADD = 0x07; //2MHz�@�f�t�H���g�ݒ�8MHz�ɂ��Ă���̂ŁA�^�b�`�p�ɒᑬ�ɂ���
        T_CS_SetLow(); //�^�b�`CS��Low��
        
        // x�񑪒肵�A���ώ��
        sample_sumX = 0;
        sample_sumY = 0;
        for (jj = 0; jj < oversampling; jj++) {
            GetTouchRawXY(&samplesX[jj], &samplesY[jj]);
            sample_sumX += samplesX[jj];
            sample_sumY += samplesY[jj];
        }
        SPI1_ExchangeByte(0b10000000);  // PowerDown�ɂ��āAIRQ��Enabled�ɂ���
        
        T_CS_SetHigh(); //CS��High��
        //    SSP1ADD = 0x01; //8MHz�ɖ߂�
        SPI1_Close();
    }

    avg = sample_sumX / oversampling;
    num = oversampling;
    overrange = avg / 10;    //���ϒl��10%�@�@����80�ɂ��Ă���
    for (jj = 0; jj < oversampling; jj++) {
        if (avg > samplesX[jj]) diff = avg - samplesX[jj];
        else diff = samplesX[jj] - avg;
        if (diff > overrange) {
            //���ς��傫���O��Ă���f�[�^���폜
            sample_sumX -= samplesX[jj];
            num--;
        }
    }
    if (num > 0) *tx = sample_sumX / num;
    else return -1;  //�L���f�[�^�Ȃ��̂ŁA�G���[
    
    avg = sample_sumY / oversampling;
    num = oversampling;
    overrange = avg / 10;    //���ϒl��10%�@�@����80�ɂ��Ă���
    for (jj = 0; jj < oversampling; jj++) {
        if (avg > samplesY[jj]) diff = avg - samplesY[jj];
        else diff = samplesY[jj] - avg;
        if (diff > overrange) {
            //���ς��傫���O��Ă���f�[�^���폜
            sample_sumY -= samplesY[jj];
            num--;
        }
    }
    if (num > 0) *ty = sample_sumY / num;
    else return -1;  //�L���f�[�^�Ȃ��̂ŁA�G���[

#ifdef DEBUG
    sprintf(str, "T1=(%d, %d)", *tx, *ty);
    display_drawChars(0, 140, str, WHITE, BLACK, 1);
#endif

    return 0;
    
}

/*
 * �^�b�`�̍��W���O���t�B�b�N�̍��W�ɕϊ�
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

    //�ȉ��̌v�Z���x�����
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
