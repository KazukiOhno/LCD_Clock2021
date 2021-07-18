/*
 * File:   Draw.c
 * Author: K.Ohno
 *
 * �`��֌W���[�`��
 */

#include "mcc_generated_files/mcc.h"
#include "main.h"
#include "Draw.h"
#include "RTC8025.h"
#include "LCD320x240color.h"

char WeekDays[][4] = {
    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat",
//    "0x80", "0x81", "0x82", "0x83", "0x84", "0x85", "0x86",
};

uint16_t AlarmColor[2] = {0x4208, WHITE};   //0: SlideSW off���̐F�A1: on���̐F
// 0x4208;    //�D�F

// ��Βl���擾����֐��}�N����`
#define ABS(x) ((x) < 0 ? -(x) : (x))

//�������ߖ񂷂邽�߁A�A�i���O�N���b�N���O��
//#define ANALOGCLOCK

/*
 * num(0-9)��7�Z�O�X�^�C���ŕ\��
 * 
 *     a
 *    ---
 *  f|   |b
 *    -g-
 *  e|   |c
 *    ---
 *     d
 * gfe dcba�̏���lsb=a�Ō��镔����\��
 * x, y���W�ɁAsize�̐����{�ŁAcolor�̐F�ŕ\��
 * ��{�T�C�Y: 19x27�@�@���͂ɋ󔒂Ȃ��̃T�C�Y
 * 20x30�Ƒz�肵�ĕ`�悷��Ƃ��傤�ǂ悢���h��
 * 
 */
void Dispseg(uint16_t x, uint16_t y, uint8_t num, uint8_t size, uint16_t color) {
    char str[100];
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
    uint16_t xs, ys, xt, yt;    //xs,ys�͋N�_�Axt,yt�͑Ίp��
    uint16_t x0, y0, x1, y1, x2, y2;    //��{�I�ɎO�p�`�ŕ`�悷��
    uint16_t jj;
    
    // a �Z�O�����g
    if (pattern[num] & 0x01) {
        x0 = x + 5*size;    //����
        y0 = y;
        x1 = x + 18*size; //�E��
        y1 = y;
        x2 = x + 16*size; //�E��
        y2 = y + 2*size;
        display_fillTriangle(x0, y0, x1, y1, x2, y2, color);
        x1 = x + 7*size;    //�E��
        y1 = y2;
        display_fillTriangle(x0, y0, x2, y2, x1, y1, color);
    }
    
    // b �Z�O�����g
    if (pattern[num] & 0x02) {
        x0 = x + 16*size;     //����
        y0 = y + 4*size - (size-1);
        x1 = x + 18*size;     //�E��
        y1 = y + 2*size - (size-1);
        x2 = x + 16*size + (size-1);     //�E��
        y2 = y + 13*size;
        display_fillTriangle(x1, y1, x0, y0, x2, y2, color);
//        display_drawTriangle(x0, y0, x1, y1, x2, y2, color);
//    sprintf(str, "%d, %d,  %d, %d,  %d, %d", x0, y0, x1, y1, x2, y2);
//    display_drawChars(0, 220, str, WHITE, BLACK, 1);
        x1 = x + 15*size;       //����
        y1 = y + 11*size;
        display_fillTriangle(x1, y1, x2, y2, x0, y0, color);
//        display_drawTriangle(x0, y0, x1, y1, x2, y2, color);
    }
    
    // c �Z�O�����g
    if (pattern[num] & 0x04) {
        x0 = x + 15*size - (size-1);     //����
        y0 = y + 15*size;
        x1 = x + 17*size - (size-1);     //�E��
        y1 = y + 14*size;
        x2 = x + 15*size;     //�E��
        y2 = y + 25*size + (size-1);
        display_fillTriangle(x1, y1, x0, y0, x2, y2, color);
        x1 = x + 13*size;   //����
        y1 = y + 23*size + (size-1);
        display_fillTriangle(x1, y1, x2, y2, x0, y0, color);
    }
    
    // d �Z�O�����g
    if (pattern[num] & 0x08) {
        x0 = x + 2*size;     //����
        y0 = y + 24*size;
        x1 = x + 12*size;     //�E��
        y1 = y + 24*size;
        x2 = x + 14*size;     //�E��
        y2 = y + 26*size;
        display_fillTriangle(x1, y1, x0, y0, x2, y2, color);
        x1 = x;   //����
        y1 = y + 26*size;
        display_fillTriangle(x1, y1, x2, y2, x0, y0, color);
    }
    
    // e �Z�O�����g
    if (pattern[num] & 0x10) {
        x0 = x + 2*size;     //����
        y0 = y + 14*size;
        x1 = x + 3*size + (size-1);     //�E��
        y1 = y + 15*size;
        x2 = x + 2*size + (size-1);     //�E��
        y2 = y + 22*size + (size-1);
        display_fillTriangle(x1, y1, x0, y0, x2, y2, color);
        x1 = x;   //����
        y1 = y + 24*size + (size-1);
        display_fillTriangle(x1, y1, x2, y2, x0, y0, color);
    }
    
    // f �Z�O�����g
    if (pattern[num] & 0x20) {
        x0 = x + 4*size;     //����
        y0 = y + 1*size - (size-1);
        x1 = x + 6*size;     //�E��
        y1 = y + 3*size - (size-1);
        x2 = x + 4*size + (size-1);     //�E��
        y2 = y + 11*size;
        display_fillTriangle(x1, y1, x0, y0, x2, y2, color);
        x1 = x + 2*size;   //����
        y1 = y + 12*size;
        display_fillTriangle(x1, y1, x2, y2, x0, y0, color);
    }
    
    // g �Z�O�����g
    if (pattern[num] & 0x40) {
        x0 = x + 3*size;     //���[
        y0 = y + 13*size;
        x1 = x + 5*size;     //����
        y1 = y + 12*size;
        x2 = x + 14*size;     //�E��
        y2 = y + 12*size;
        display_fillTriangle(x1, y1, x0, y0, x2, y2, color);
        x1 = x + 4*size;   //����
        y1 = y + 14*size;
        display_fillTriangle(x1, y1, x2, y2, x0, y0, color);
        x0 = x + 13*size;   //�E��
        y0 = y + 14*size;
        display_fillTriangle(x1, y1, x2, y2, x0, y0, color);
        x1 = x + 15*size;   //�E�[
        y1 = y + 13*size;
        display_fillTriangle(x1, y1, x2, y2, x0, y0, color);
    }
    
}

/*
 * mode:
 * HHMMSS=0: HH:MM:SS
 * HHMM  =1: HH:MM
 * HHMMss=2: HH:MM + small SS
 * (x,y)���W������̋N�_�Ƃ��ĕ`��
 * �����̊O�`���2�h�b�g���傫�߂̗̈��`���Ƒz��
 * �󔒁@2���@�R�����@2���@�R�����@2���@�@��
 * 2    20x2   5     20x2  5     20x2�@ 2
 * �c=27 x size�@�㉺�̋󔒁@2x2
 * size=2��W���Ƒz��@�@�󔒈ȊO�́Asize�{�̃T�C�Y
 * 
 */
void DispTime(uint8_t mode, uint8_t * datetime, uint16_t x, uint16_t y, uint8_t size, uint16_t color) {
    int8_t jj;
    uint16_t xd, yd;
    uint8_t endpoint;

//    fillRect(xd, yd, (20*6+5*2)*size, 30*size, BLACK);
    xd = x +2;
    yd = y +2;
    switch (mode) {
        case HHMM: endpoint = 1; break;
        default: endpoint = 0;
    }
    for (jj=2; jj>=endpoint; jj--) {
        if ((mode == HHMMss) && (jj == 0)) size--;  //�b�̕\������������
        if (size < 1) size = 1;
        //�����������K�v�Ȍ������`��
        //��ʌ�
        if ((datetime[jj] >>4) != (preDateTime[jj] >>4)) {
            fillRect(xd, yd-2, 20*size, 27*size+4, BLACK);
            Dispseg(xd, yd, datetime[jj]>>4, size, color);
        }
        //���ʌ�
        xd += 20*size;        
        if ((datetime[jj] & 0x0f) != (preDateTime[jj] & 0x0f)) {
            fillRect(xd, yd-2, 20*size, 27*size+4, BLACK);
            Dispseg(xd, yd, datetime[jj] & 0x0f, size, color);
        }
        preDateTime[jj] = datetime[jj];
        
        //�F��`��
        xd += 20*size;
        if ((jj == 2) || ((mode == HHMMSS) && (jj==1))) {
            //���̕\���A�������͎����b�\���̎��͕��̕\���̌���:��\��
            fillRect(xd, yd-2, 6*size+1, 27*size+4, BLACK);
            fillRect(xd+1, yd+7*size, 3*size, 3*size, color);
            fillRect(xd, yd+17*size, 3*size, 3*size, color);
            xd += 6*size ;
        }
    }
    
}

// 0~174�x�܂�6�x���Ƃ�256�{�ɂ���sin�e�[�u��
//�@�A���[���j��5���P�ʂɂ����ꍇ�A2.5�x�P��(30�x��12����)���~�����B
//�@�@10���P�ʂ�5�x�ƁA6�x���̃e�[�u���Ǝ኱�����Ă��Ȃ��B3�x�P�ʈʂ͗~�����B
//sin(��): ��=�p�x/6
//180�x�ȍ~�́A-sin(��-180)�Ƃ����`�ŎQ��
//cos(��)=sin(��+90)
/*
const uint16_t sin_table6[] = {
      0,  26,  53,  79, 104, 128, 
    150, 171, 190, 207, 221, 233,
    243, 250, 254, 256, 254, 250,
    243, 233, 221, 207, 190, 171,
    150, 128, 104,  79,  53,  26,
};
*/
//1�x�P�ʂ̃e�[�u��
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

//degree�Ŏw�肳�ꂽ�p�x=theta�ɑΉ�����sin��Ԃ�(x256)
int16_t sind(uint16_t theta) {
    theta = theta % 360;

    if (theta >= 180) {
        //-sind(��-180)
        return -sin_table[theta-180];          //�x�P�ʂ̃e�[�u���Ȃ̂�
//        return -sin_table6[(theta-180)/6];   //6�x�P�ʂ̃e�[�u���Ȃ̂�
    } else {
        return sin_table[theta];
//        return sin_table6[theta/6];
    }
}

//degree�Ŏw�肳�ꂽ�p�x=theta�ɑΉ�����cos��Ԃ�(x256)
int16_t cosd(uint16_t theta) {
    theta = theta+90;
    return sind(theta);
}

/*
 * �A�i���O���v�\��
 * mode: 
 * datetime: ���t����
 * xx, yy: ���S���W
 * size: �O�`�~�̑傫��
 * color: �j�̐F�w��
 */
#define SizeMin  3      //���j�̑���
#define SizeHour 4      //���j�̑���
#define ColorHour   GREY
#define ColorMin    GREY
#define ColorSec    RED
#define ColorAlarm  YELLOW


/*
 * �A�i���O���v�̃A���[���j��`��
 */
void drawAlarmNeedle(uint16_t *x, uint16_t *y, uint16_t color) {
    int16_t xd, yd;
    
    //�A���[���j�������@�������A�ŏ��̕`�掞�́A�������X�L�b�v
    display_drawLine(x[0], y[0], x[1], y[1], color);    //�A���[���j
    
    xd = x[1]-x[0];
    yd = y[1]-y[0];
    if ( ABS(yd) > ABS(xd)) {
        //�p�x�ɂ��A�ړ����������ς��A������������
        display_drawLine(x[0]+1, y[0], x[1]+1, y[1], color);
        display_drawLine(x[0]-1, y[0], x[1]-1, y[1], color);
    } else {
        display_drawLine(x[0], y[0]-1, x[1], y[1]-1, color);
        display_drawLine(x[0], y[0]+1, x[1], y[1]+1, color);
    }
}


//�A���[���j�̒����͒��j��75%
//���j�F���j�̒����́A�ڐ��̐����3�h�b�g����
//�Z�j
//�b�j
//�ڐ��̒���=3
//�h�b�g�̑傫��=2
void DrawAnalogClock(uint8_t mode, uint8_t * datetime, uint16_t xx, uint16_t yy, uint8_t size, uint16_t color, uint8_t *alarmtime) {
    uint16_t xc, yc;    //���v�̒��S
    uint16_t rc, rc1, rcs, rcm, rch, rca;    //���v�̃T�C�Y�A�ڐ��A�b�j�A���j�A���j�A�A���[��
    uint16_t rc2;
    uint16_t rcs2, rcm2, rcm3, rch2, rch3, rca2;    //���v�̃T�C�Y�A�ڐ��A�b�j�A���j�A���j�A�A���[��
    uint16_t x[5], y[5];    //���W�v�Z�������ʂ��i�[
    uint16_t temp, angle;
    int8_t jj, hh, mm, ss, kk;
    static int8_t almhh, almmm;
    //�O��̍��W
    static uint16_t phx[5], phy[5], pmx[5], pmy[5], psx[5], psy[5], pax[5], pay[5];
    //����̍��W
    uint16_t chx[5], chy[5], cmx[5], cmy[5], csx[5], csy[5], cax[5], cay[5];
    uint16_t hcolor, mcolor, scolor, acolor;
    char str[3];
    int8_t minupdate = 0;
    uint16_t bcolor;
    
    if (color == GREEN) color = GREY;   //�F�w�肪�΂�������D�F�ɕύX���Ă��܂��B�H�H�H�̎�
    hcolor = color;  //���j�̐F
    mcolor = color;  //���j�̐F
    scolor = ColorSec;    //�b�j�̐F
    acolor = ColorAlarm;  //�A���[���j�̐F
    
    //�O�`�~�̕`��
    xc = xx;    //���S���W
    yc = yy;
    rc = size;  //�O�`�̔��a
    display_drawCircle(xc, yc, rc+3, color);    //����傫�߂̉~
    display_drawCircle(xc, yc, rc, color);      //size�Ŏw�肳�ꂽ�~

    rc1 = rc -3;    // �ڐ��������@��Ԓ������j�̒���

    //�A���[���j�̕`�揀��----------------------------------------------------
    //�ŐV�̃A���[���j�̍��W���v�Z
    rca = rc1 *3 /4;    //�A���[���p�̐j�̒����́A���j��75%

    if (alarmtime != NULL) {
        almmm = Bcd2Hex(alarmtime[0]);
        almhh = Bcd2Hex(alarmtime[1]);
    }
    almhh = almhh % 12;
    angle = almhh * 30 + almmm/2;   //�p�x�ɕϊ�
    cax[0] = xc;    //���S���W
    cay[0] = yc;
    cax[1] = xc + rca * sind(angle)/256;
    cay[1] = yc - rca * cosd(angle)/256;
    
    //��������Ȃ������߁A�\���ɕω�����j�������������A����ȊO�͏㏑������
    //�O�̕\��������
    if ((cax[1] == pax[1]) && (cay[1] == pay[1])) {
        //�O�ƍ��W���ς���Ă��Ȃ����́A�����Ȃ��B�ŏ��̕`����������W�ɂ��Ă���̂ŏ������Ȃ�
    } else {
        drawAlarmNeedle(pax, pay, BLACK);   //���̕b�j������
        //�`��p�̍��W��ۑ�
        for (jj=0; jj<2; jj++) {
            pax[jj] = cax[jj];
            pay[jj] = cay[jj];
        }
    }

    //���j�̕`�揀��--------------------------------------------------------
    //���j�̈ʒu���ς������A�ĕ`��̑O�ɁA���̎��j�A���j������
    if (preDateTime[1] != datetime[1]) {
        preDateTime[1] = datetime[1];
        mm = ((datetime[1]>>4)*10 + (datetime[1] & 0x0f));
        hh = (datetime[2]>>4)*10 + (datetime[2] & 0x0f);

        //���j�̍��W���v�Z
        rcm = rc1 -5;   //���j�̒����́A�ڐ��̐����3�h�b�g����
        angle = mm*6;
        cmx[1] = xc + rcm * sind(angle)/256;
        cmy[1] = yc - rcm * cosd(angle)/256;
    
        rcm2 = SizeMin;       //���j�̕�
        angle = mm*6 +90;
        cmx[2] = xc + rcm2 * sind(angle)/256;
        cmy[2] = yc - rcm2 * cosd(angle)/256;
        cmx[3] = xc - (cmx[2]-xc);
        cmy[3] = yc - (cmy[2]-yc);

        //���Α��̏o������
        rcm3 = 10;
        angle = mm*6 +180;
        cmx[4] = xc + rcm3 * sind(angle)/256;
        cmy[4] = yc - rcm3 * cosd(angle)/256;

        //���j�̍��W�v�Z
        //���j���ړ�������A���j���ړ��v�Z (���ۂ�2����)
        //�@���̃f�[�^����荞��Ŏ��j�̊p�x���߂�
        hh = hh % 12;
        angle = hh * 30 + mm/2;   //�p�x�ɕϊ� 8bit�ϐ��ł�NG
        
        rch = rc1 *7/10;    //�Z�j�̒����́A70%
        chx[0] = xc;  //���S���W
        chy[0] = yc;
        chx[1] = xc + rch * sind(angle)/256;
        chy[1] = yc - rch * cosd(angle)/256;
        
        rch2 = SizeHour;       //���j�̕�
        chx[2] = xc + rch2 * sind(angle +90)/256;
        chy[2] = yc - rch2 * cosd(angle +90)/256;
        chx[3] = xc - (chx[2]-xc);
        chy[3] = yc - (chy[2]-yc);
        
        //���Α��̏o������
        rch3 = 10;
        chx[4] = xc + rch3 * sind(angle +180)/256;
        chy[4] = yc - rch3 * cosd(angle +180)/256;
        
        //�����ύX�ɂȂ�����B���Ԃ��ς�鎞�́A���̕ύX�Ɠ���
        display_fillTriangle(phx[1], phy[1], phx[2], phy[2], phx[3], phy[3], BLACK);
        display_drawTriangle(phx[1], phy[1], phx[2], phy[2], phx[3], phy[3], BLACK);
        display_fillTriangle(phx[4], phy[4], phx[2], phy[2], phx[3], phy[3], BLACK);
        display_drawTriangle(phx[4], phy[4], phx[2], phy[2], phx[3], phy[3], BLACK);
        
        //�`����W��ۑ�
        for (jj=0; jj<5; jj++) {
            phx[jj] = chx[jj];
            phy[jj] = chy[jj];
        }

        //���j�̏���
        display_fillTriangle(pmx[1], pmy[1], pmx[2], pmy[2], pmx[3], pmy[3], BLACK);
        display_drawTriangle(pmx[1], pmy[1], pmx[2], pmy[2], pmx[3], pmy[3], BLACK);
        display_fillTriangle(pmx[4], pmy[4], pmx[2], pmy[2], pmx[3], pmy[3], BLACK);
        display_drawTriangle(pmx[4], pmy[4], pmx[2], pmy[2], pmx[3], pmy[3], BLACK);
        minupdate = 1;  //���jupdate���鎞
        //�`����W��ۑ�
        for (jj=0; jj<5; jj++) {
            pmx[jj] = cmx[jj];
            pmy[jj] = cmy[jj];
        }
    }

    //�ŐV�̕b�j�̍��W���v�Z
    ss = -1;
    if (preDateTime[0] != datetime[0]) {
        preDateTime[0] = datetime[0];
        ss = ((datetime[0]>>4)*10 + (datetime[0] & 0x0f));  //0-59�̐��l
        rcs = rc1 - 4;  //�b�j�̒����́A����
        angle = ss*6;
        csx[0] = xc;  //���S���W
        csy[0] = yc;
        csx[1] = xc + rcs * sind(angle)/256;
        csy[1] = yc - rcs * cosd(angle)/256;
        
        rcs2 = 20;   //���Α��ɏo�������
        angle = ss*6+180;
        csx[2] = xc + rcs2 * sind(angle)/256;
        csy[2] = yc - rcs2 * cosd(angle)/256;
        csx[3] = xc + rcs2 * sind(angle+6)/256;
        csy[3] = yc - rcs2 * cosd(angle+6)/256;
        csx[4] = xc + rcs2 * sind(angle-6)/256;
        csy[4] = yc - rcs2 * cosd(angle-6)/256;
        
        //�b�j������
        display_drawLine(psx[1], psy[1], psx[2], psy[2], BLACK);
//        display_fillTriangle(psx[0], psy[0], psx[3], psy[3], psx[4], psy[4], BLACK);
        display_drawLine(psx[0], psy[0], psx[3], psy[3], BLACK);
        display_drawLine(psx[0], psy[0], psx[4], psy[4], BLACK);

        //�`����W��ۑ�
        for (jj=0; jj<5; jj++) {
            psx[jj] = csx[jj];
            psy[jj] = csy[jj];
        }
    }
    
    //�ڐ��̕`��------------------------------------------------------------
    rc1 = rc -3;    //���̒�����3�Ƃ���
    rc2 = rc1 -10;  //�����̈ʒu
    for (jj=0; jj<60; jj++) {
        angle = jj*6;
        x[1] = xc + (rc * sind(angle))/256;
        y[1] = yc - (rc * cosd(angle))/256;
        x[2] = xc + (rc1 * sind(angle))/256;
        y[2] = yc - (rc1 * cosd(angle))/256;
        
        if (jj%5 == 0) {
            display_fillCircle(x[2], y[2], 2, color);   //���̏��́A�h�b�g�A���a2
            //�����Օ\���B�������Aminupdate=0�ŁAmm��5�̔{���̎��́A�`�悵�Ȃ�(�������Ȃ��ƃ`����)
            if ((minupdate == 0) && (jj==mm)) continue;
            x[3] = xc + (rc2 * sind(angle))/256;
            y[3] = yc - (rc2 * cosd(angle))/256;
            //�\�����鐔����ݒ�
            if (jj==0) kk=12;   //0���̏��́A12�ɐݒ�
            else kk=jj/5;
            sprintf(str, "%d", kk);
            //�w�i�F�𓯂��ɂ���ƁA�w�i�F��h��Ȃ�=�����Ɠ���
            if (kk<10)
                display_drawChars(x[3]-4, y[3]-3, str, color, color, 1);
            else
                display_drawChars(x[3]-7, y[3]-3, str, color, color, 1);
        } else {
            display_drawLine(x[1], y[1], x[2], y[2], color);
        }
    }
    
    // AM/PM�̕\��
    //��������Ȃ������߁A�w�i�����͕ω������Ȃ��`����s�����A
    if (preDateTime[2]/12 != datetime[2]/12) {
        //AM/PM�̐ؑ֎������A�w�i�`����{
        bcolor = BLACK;
        preDateTime[2] = datetime[2];
    }
    else {
        bcolor = color;
    }
    if (datetime[2] < 0x12) sprintf(str, "AM");
    else sprintf(str, "PM");
    display_drawChars(xx-14, yy+rc/2, str, color, bcolor, 2);
    
    //��������A�j�̕`������s
    //�A���[���j�`��------------------------------------------------------
    drawAlarmNeedle(pax, pay, acolor);

    //���j�̕`��------------------------------------------------------
    display_fillTriangle(phx[1], phy[1], phx[2], phy[2], phx[3], phy[3], hcolor);
//    display_drawTriangle(phx[1], phy[1], phx[2], phy[2], phx[3], phy[3], WHITE);
    //���Α��̏o������
    display_fillTriangle(phx[4], phy[4], phx[2], phy[2], phx[3], phy[3], hcolor);
//    display_drawTriangle(phx[4], phy[4], phx[2], phy[2], phx[3], phy[3], WHITE);

    display_drawLine(phx[1], phy[1], phx[2], phy[2], WHITE);
    display_drawLine(phx[1], phy[1], phx[3], phy[3], WHITE);
    display_drawLine(phx[4], phy[4], phx[2], phy[2], WHITE);
    display_drawLine(phx[4], phy[4], phx[3], phy[3], WHITE);

    //���j�̕`��------------------------------------------------------
    display_fillTriangle(pmx[1], pmy[1], pmx[2], pmy[2], pmx[3], pmy[3], mcolor);
    display_drawTriangle(pmx[1], pmy[1], pmx[2], pmy[2], pmx[3], pmy[3], WHITE);
    //���Α��̏o������
    display_fillTriangle(pmx[4], pmy[4], pmx[2], pmy[2], pmx[3], pmy[3], mcolor);
    display_drawTriangle(pmx[4], pmy[4], pmx[2], pmy[2], pmx[3], pmy[3], WHITE);
    
    display_drawLine(pmx[1], pmy[1], pmx[2], pmy[2], WHITE);
    display_drawLine(pmx[1], pmy[1], pmx[3], pmy[3], WHITE);
    display_drawLine(pmx[4], pmy[4], pmx[2], phy[2], WHITE);
    display_drawLine(phx[4], phy[4], phx[3], phy[3], WHITE);

    //�b�j�̕`��------------------------------------------------------
    display_fillCircle(psx[0], psy[0], 3, scolor);
    display_drawLine(psx[1], psy[1], psx[2], psy[2], scolor);
//    display_fillTriangle(psx[0], psy[0], psx[3], psy[3], psx[4], psy[4], scolor);
    display_drawLine(psx[0], psy[0], psx[3], psy[3], scolor);
    display_drawLine(psx[0], psy[0], psx[4], psy[4], scolor);
    
}

/*
 * �t�H���g�w�肵�đ傫���Ɋ֌W�Ȃ��`��ł���悤�ɂ���
 * ����̍��W: xs, ys
 * �t�H���g�T�C�Y: fontsize, fontw, fonth
 * �\���s�b�`: xpitch(���g�p), ypitch, space
 */

void DrawCalendar(uint8_t year, uint8_t month, uint16_t xs, uint16_t ys, uint8_t xpitch, uint8_t ypitch, 
        uint8_t fontsize, uint8_t fontw, uint8_t fonth, uint8_t space) {
    uint8_t jj, kk;
    uint16_t xx, yy;
    char str[5];
    int8_t startday;    //�ŏ��̓��j�̓��t�A0�ȉ��͑O��
    int8_t maxdays;
    uint8_t yr, mm, dd, wd;
    uint16_t color = WHITE;
    uint16_t backColor = BLACK;
    uint8_t thismonth;

    yr = year;
    mm = month;
    dd = 1;
    wd = getWeekdays(&yr, &mm, &dd);  //1���̗j�����擾
    //1�������j�Ȃ�1�A���j�Ȃ�0�A�Ηj�Ȃ�-1�A���j�Ȃ�-2�ɂȂ�
    startday = 1- wd;
    
    if (month == Bcd2Hex(DateTime[5])) thismonth = 1;
    else thismonth = 0;

    //���Y���̍ŏI�����擾
    dd = 31;
    getWeekdays(&yr, &mm, &dd);  //31���̗j�����擾�B���̓����Ȃ��ꍇ�A�������������
    //mm���ύX�ɂȂ�����A31���͂Ȃ������Ƃ킩��
    if (month != mm) {
        maxdays = 31 - dd;    //���̌��̍ŏI��
    } else {
        maxdays = 31;
    }   
    
    //�Ώۗ̈���N���A: fontw��14������?7���̃X�y�[�X���AY������8�s��
    //x�̊J�n��1�h�b�g���ɂ��āA�T�C�Y�����̕��傫���B�g��t�������͂ݏo����h�~
    lcd_fill_rect(xs-1, ys, xs+fontw*14+space*7+1, ys+ypitch*8, backColor);
    //���̕`��
    yy = ys;
    xx = xs + 3*(fontw*2+space);
    if (month <10) xx = xx + fontw/2;    //2���̎��A���������炷    
    sprintf(str, "%d", month);
    display_drawChars(xx, yy, str, color, backColor, fontsize);

    //�j��
    yy += ypitch;
    xx = xs+ fontw/2;   //�t�H���g���������炷
    for (jj=0; jj<7; jj++) {
//        sprintf(str, "%c", WeekDays[jj][0]);  //SMTWTFS�Ƃ����\�L����
        sprintf(str, "%c", 0x80+jj);    //�����ΐ��؋��y�Ƃ����\��
        display_drawChars(xx, yy, str, color, backColor, fontsize);
        xx += fontw * 2 +space;
    }
    
    //���t
    yy += ypitch;
    if (fontsize == 1) {
        display_SetFont(tinyfont);
    }

    for (kk = 0; kk<6; kk++) {
        xx = xs;
        for (jj = 0; jj < 7; jj++) {
            if (startday > 0 && startday <= maxdays) {
                sprintf(str, "%2d", startday);
                if (jj==0) color = RED;         //���j
                else if (jj==6) color = BLUE;   //�y�j
                else color = WHITE;             //����
                display_drawChars(xx, yy, str, color, backColor, fontsize);
                //�����̓��t�Ɉ������
                if (thismonth && (startday == Bcd2Hex(DateTime[4]))) {
//                    display_drawChars2(xx, yy, str, backColor, color, 1);
                    //���]���Ǝ��F���ɂ����̂Řg��t����
                    display_drawRect(xx-1, yy-2, fontw*2+2, fonth+2, WHITE);
                }
            }
            startday++;
            xx += fontw*2 + space;
        }
        yy += ypitch;
    }
    display_SetFont(NormalFont);  //���ɖ߂�

}

/*
 * �傫�ȃJ�����_�[��\��
 * xs,ys���`�悷�鍶��̍��W
 */
void DrawBigCalendar(uint8_t year, uint8_t month, uint16_t xs, uint16_t ys) {
    uint8_t fontw, fonth, ypitch, space;

    //�\���t�H���g�̑傫��
    fontw = CurrentFont.xsize * 2;    //16
    space = 4;
    fonth = CurrentFont.ysize*2 +2;   //18;
    ypitch = fonth+4;
    
    DrawCalendar(year, month, xs, ys, 0, ypitch, 2, fontw, fonth, space);
        
}


/*
 * ���������J�����_�[��\��
 * year: ��2��
 * month: 1-12
 * xs, ys: �\���̊�ʒu�@����
 */
void DrawSmallCalendar(uint8_t year, uint8_t month, uint16_t xs, uint16_t ys) {
    uint8_t fontw, fonth, ypitch;

    //�O���A�����̕\���̂��߁A0���A13���Ƃ����ݒ������
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
    
    DrawCalendar(year, month, xs, ys, 0, ypitch, 1, fontw, fonth, 1);
    
}


/*
 * �������J�����_�[��3�\������
 * �@�����̔N���́A�\�������̌����w��
 * �@BCD�ł͂Ȃ�
 * mode: �f�B�X�v���C���[�h�ɂ��A���W������
 * startmonth: �\�����錻�݂̔N�����w��
 * 
 */
void Draw3month(uint8_t startyear, uint8_t startmonth, uint8_t mode) {
    uint8_t yy, mm;
    uint8_t jj;
    uint16_t xstep = CalendarXstep;  //mode=DisplayMode1
    uint16_t ystep = CalendarYw; //mode=DisplayMode3
    
    if (mode == DisplayMode1) ystep = 0;
    if (mode == DisplayMode3) xstep = 0;
    //3�����J�����_��X,Y���W�́A���\�[�X����擾
    //Mode3�̃A�i���O���v�\���̎��́A2�����������\�����Ȃ����\���������Ă��ǂ��Ƃ���
    for (jj=0; jj<3; jj++) {
        DrawSmallCalendar(startyear, startmonth+jj-1, MonthCalendar[mode].x +xstep*jj, MonthCalendar[mode].y +ystep*jj);
    }
}

/*
 * sw=SlideSWStatus: 0/1=Alarm off, 2/3=Alarm on
 */
void drawAlarmTime(uint8_t mode, uint8_t *alarmtime, uint8_t sw) {
    char str[100];
    char ampm[][3] = {"AM", "PM"};
    int8_t ap;
    char pm;    //�I���I�t��+-�ؑ�
    uint8_t idx = sw>>1;
    
    //�A���[�������̕\���F�́AAlarmColor�Őݒ�
    if (idx) pm = '+';
    else pm = '-';
    
    if (mode == DisplayMode3) {
        DrawAnalogClock(mode, DateTime, RTime[mode].x, RTime[mode].y, RTime[mode].xw, GREY, alarmtime);
        if (alarmtime[1] >= 0x12) ap= 1 ;   //BCD��12���ȍ~�Ȃ�ߌ�
        else ap = 0;
        sprintf(str, "Alarm%c %s %02d:%02x", pm, ampm[ap], (alarmtime[1] & 0xf) + (alarmtime[1] >> 4)*10 -12*ap, alarmtime[0]);
    } else {
        sprintf(str, "ALM%c %02x:%02x", pm, alarmtime[1], alarmtime[0]);
    }
    display_drawChars(RAlarm[mode].x, RAlarm[mode].y, str, AlarmColor[idx], BLACK, RAlarm[mode].font);

}

void drawTime(uint8_t mode, uint8_t * datetime, uint16_t color) {
    char str[100];

    if (mode == DisplayMode3) {
        //�O�`�~�̍��W= (140, 130) ���a=90
        DrawAnalogClock(mode, datetime, RTime[mode].x, RTime[mode].y, RTime[mode].xw, color, NULL);
    } else {
        DispTime(RTime[mode].mode, datetime, RTime[mode].x, RTime[mode].y, RTime[mode].font, color);
    }
}

void drawDateTime(uint8_t mode, uint8_t *datetime) {
    char str[100];

//    sprintf(str, "20%02x/%02x/%02x(%s)", datetime[6], datetime[5], datetime[4], WeekDays[datetime[3]]);
    sprintf(str, "20%02x/%02x/%02x(%c)", datetime[6], datetime[5], datetime[4], 0x80+datetime[3]);
    display_drawChars(RDate[mode].x, RDate[mode].y, str, WHITE, BLACK, RDate[mode].font);
    drawTime(mode, datetime, GREEN);
}

void drawTempHumidity(uint8_t mode, int16_t temp, int16_t humidity) {
    char str[50];

    sprintf(str, "%2d.%1d\x87", temp / 10, temp % 10);
    display_drawChars(RTemp[mode].x, RTemp[mode].y, str, WHITE, BLACK, RTemp[mode].font);
    sprintf(str, "%2d%%", humidity / 10);
    display_drawChars(RHumidity[mode].x, RHumidity[mode].y, str, WHITE, BLACK, RHumidity[mode].font);

}

/*
 * DisplayMode�ɉ����ăJ�����_�[��\������
 */
void drawCalendar(uint8_t mode) {
    switch (mode) {
        case DisplayMode2:
            DrawBigCalendar(Bcd2Hex(DateTime[6]), Bcd2Hex(DateTime[5]), MonthCalendar[mode].x, MonthCalendar[mode].y);
            break;
        default:
            Draw3month(Bcd2Hex(DateTime[6]), Bcd2Hex(DateTime[5]), mode);
    }
}
