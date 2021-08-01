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
 * TMR0: �A���[�����̖��Ă��鎞�Ԃ𑪒� 10ms���̊�����
 * TMR1: ���[�^���[�G���R�[�_�̃T���v�����O�@100us���̊�����
 * TMR2: ���d�X�s�[�J�p��4KHz��ECCP2 PWM�𔭐������邽�߂̃^�C�}  250us
 * TMR3: ADC��CCP5���[�h�Ŏg�p���āATMR3=4ms�𗘗p
 * TMR5: �^�b�`�A�X���C�hSW�AGPS�̏�ԃ`�F�b�N�̊��荞�ݗp�^�C�}�@10ms
 * TMR6: �o�b�N���C�g��PWM����p�^�C�} 256us
 * 
 * SPI�C���^�t�F�[�X�FLCD�A�^�b�`�ASD�J�[�h��3�ŋ��p
 * �@MSSP1=SPI (SCK1, SDI1, SDO1)+LCD_CS, T_CS, SDCard_CS
 *   �@LCD_RESET, LCD_DCRS�AT_IRQ
 *   LCD�Ƃ͍ő�X�s�[�h��8MHz�A�^�b�`��2MHz�ŒʐM�ASD�J�[�h�́A�ᑬ400kHz�A����8MHz
 * SPIMASTER�́A�J�������f�t�H���g�ŁAMODE3�ɂȂ�̂ŁAMODE0�ɕύX�v
 * �C�����Ă����Ȃ��ƁAGenerate�����㓮���Ȃ��Ƒ������ƂɂȂ�
 * 
 * �Ɠx�Z���T(PhotoDiode)�́A�A�i���O�œd����荞�� RA3=AN3=CCP5=PhotoDiode
 * LCD�̃o�b�N���C�g��PWM������g���Ė��邳����
 * �@�@ECCP1: �o�b�N���C�g��PWM (Timer6) P1A=RC2
 * �@�@�Ɠx�Z���T�̌��ʂɊ�Â��AEPWM1_LoadDutyValue()�Œ���
 * 
 * I2C�C���^�t�F�[�X:RTC�Ɖ��x�Z���T�ŋ��p
 * �@MSSP2=I2C (SCL2�ASDA2) 100KHz
 * �@RTC�́A/INTA, /INTB ���g�p
 *     INTA=RB0=INT0�́ARTC����pullup�A1�b���ƂɊ��荞�ݓ���BEXT_INT�ŏ���
 *     INTB=RB4�́APIC��WPU�A��ԕω����荞�݂ŃA���[��������
 * 
 * ���[�^���[�G���R�[�_:RC0/RC1��2�s���̃f�W�^��Input���g���Ĕ��f
 * �@�@RC0=ROT_B, RC1=ROT_A
 * 
 * �A���[��On/Off�pSW�@�@RA7=ALMSW
 * ���d�X�s�[�J: ECCP2=PWM (Timer2)�@P2A=RB3�@�@�@MCC�J�������ƁAP2A���ݒ肩������Ă��܂��̂Œ���
 * GPS�Ƃ̃V���A���ʐM: RX1�݂̂Ŕ񓯊��ʐM(EUSART1)�@9600bps, 8bit, parity�Ȃ��AStop1bit
 * GPS��1PPS: SW�Ɠ��l��10ms���Ƀ`�F�b�N�̂��鏈���ŁARB5�����荞��
 * 
 * ����m�F�pLED: RA6=LED
 * �s�����s�������̂ŁARA6��SDCard_CS�Ƌ��p
 * 
 * Revision:
 * Rev4: ���\�[�X�����āA��ʃf�U�C�������R��
 * GPSv2: GPS���j�b�g�̒ʐM�����荞�݂ŏ���
 * GPSv3: SD�J�[�h�Ή�
 * GPSv21: �T�|�[�g���[�`���̒�`�𐮗�(����Global�ϐ��ɂ��āA�ǂ̊֐������p�����邩)
 * GPSv22: �{�^���I�u�W�F�N�g�̒�`���g���₷���悤�ɍĒ�`
 * GPS4: SD�J�[�h�������ݎ��̖������BLCD��Sleep_Out�����ĉ���
 * LCD_Clock2021�ɖ��̕ύX: GitHub��upload�B���̃t�@�C���͂���clone
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

uint8_t Mode;   //�����Ԃ�����
//�\�����[�h
uint8_t DisplayMode;    // �\���p���[�h

uint8_t UpdateFlag = 0;
#define UpdateTime          0x01
#define UpdateTimeDisp      0x02
#define UpdateTempHumidity  0x04
#define UpdateBrightness    0x08
#define UpdateAlarmW        0x10
#define UpdateDate          0x20
#define UpdateMsg           0x80

#define AdjustTouch      0x20

//�A���[�������Amm,hh, �A���[�����L���ȗj���@RTC�Ɠ�������
uint8_t AlarmTime[3] = {0, 0, 0x7f};    //mm, hh, wday
uint8_t SmoothAlarmTime[3] = {0, 0, 0x7f};
uint8_t TmpTime[7]; //�ݒ�r���̎�����ێ�

//�����x�Z���T
int16_t Temp, Humidity;

//�^�b�`�����p
uint8_t TouchStatus = 0;
//0: �^�b�`���Ă��Ȃ�
//1: �^�b�`���Ȃ��Ȃ�������̏�������
//2: �^�b�`�Z����
//3-4: �^�b�`�Z�������������@�J�E���g��100�ɍs���܂ŁA1-3�̂ǂꂩ�̏�Ԃ͈ێ�
//5: �^�b�`������
//6-: �^�b�`��������������
//�^�b�`�����0�ɂȂ�

uint8_t TouchCount;
uint16_t TouchX, TouchY;
uint16_t Test_x, Test_y;

int8_t RotCount = 0; //�񂵂ăN���b�N������
int8_t Accel = 1;   //������]���������ɁA�{����

uint16_t Brightness = 1023;
uint16_t BackLight = 100;

uint8_t SlideSWStatus = 0x0f;   //�ǂ̏�Ԃł��Ȃ��l��ݒ肵�Ă���

uint8_t AlarmStatus = 0;    //�A���[��������1�ɂ��āAsmooth�����Ȃǂ��s��
uint8_t SmoothCount = 0;    //����X�k�[�Y������

#define STABLE1PPS  20  // ����擾���������Ƃ݂Ȃ���
uint8_t Count1PPS;  //1PPS���荞�݂̔����񐔂��J�E���g�B�ő�STABLE1PPS�ɂȂ�
uint8_t GetGPS;     //�d���������A1��1�񂭂炢�̎擾�K�v�ɂȂ�����1�ɂ���
//0: GPS�擾��~�A1:GPS�擾���A2:1�s���̃f�[�^�擾����
char Buffer[100];   //GPS�Ƃ̃V���A���ʐM�p�o�b�t�@
char * BufferP;     //�o�b�t�@�̃f�[�^�ʒu�������|�C���^

#define GPS_Stop    0
#define GPS_Start   1
#define GPS_GotData 2

//EEPROM���f�[�^�z�u�@�@�d����������A��������ݒ���ǂݏo��
#define AddressInit         0x00    //1�o�C�g�@�@�����lFF�A�������񂾂�55
#define AddressDisplayMode  0x07    //1�o�C�g
//���t
#define AddressYMD          0x08    //4�o�C�g�@�j���A���A���A�N�̏�
#define AddressAlarm        0x0c    //3�o�C�g�@���A���A�j���̏�
//#define AddressAlarmNo      0x0f     //1�o�C�g�@�@�A���[���p�^�[���̕ύX�p
#define AddressTouch        0x18    //8�o�C�g�@�@�^�b�`�␳�p��2�����̍��W

//SD�J�[�h�֌W
uint8_t SDcardStatus = 0;
FATFS drive;
FIL file;

//�v���g�^�C�v�錾
void OpeningScreen(uint8_t waitTouch);

/*
 * 10ms���Ɋ��荞��
 * Touch�ƁA�X���C�hSW�̏�ԃ`�F�b�N�AGPS���j�b�g�̋N���`�F�b�N
 * �`���^�����O�h�~
 */
void Timer5Handler() {
    static uint8_t touch_status = 0xff;
    static uint8_t slideSW_status = 0xff;
    static uint8_t G1PPS_status = 0xff;

    //TouchStatus
    touch_status = (unsigned)(touch_status << 1) | T_IRQ_GetValue();
    if ((touch_status & 0x07) == 0) {
        //3��A���^�b�`(0)�Ɗ��m������
        TouchCount++;
        if (TouchStatus <= 1) TouchStatus = 2;
        //�Z�����̏�����������A1�ɂȂ��āA�J�E���g��0�ɂ�����A�A�����u�B

        if (TouchCount > 200) {
            TouchCount = 200; //2s�ȏ㒷����
            if (TouchStatus < 5) TouchStatus = 5;
            //������������A5�ȏ�ɐݒ肷��΁A�����܂ŁA�A���������Ȃ��悤�ɂł���
        }
    } else if ((touch_status & 0x07) == 0x07) {
        TouchCount = 0;
        //�^�b�`���Ȃ���touch_status=1�ɂȂ�
        if (TouchStatus > 1) TouchStatus = 0;
    }

    //�A���[���p�X���C�hSW�̏�ԃ`�F�b�N
    // 0: Off�ɕω�
    // 1: off�̏�������
    // 2: On�ɕω�
    // 3: On�̏�������
    slideSW_status = (unsigned)(slideSW_status << 1) | ALMSW_GetValue();
    if ((slideSW_status & 0x07) == 0) {
        //����܂ŃI����ԂŁA�I�t�Ɣ��肳�ꂽ�̂�0�ɂ���
        if (SlideSWStatus != 1) SlideSWStatus = 0;
    } else if ((slideSW_status & 0x07) == 0x07) {
        //����܂ŃI�t��ԂŁA�I���Ɣ��肳�ꂽ�̂�2�ɂ���
        if (SlideSWStatus != 3) SlideSWStatus = 2;
    }
    
    //GPS���j�b�g���AGPS�q���ǔ��ł����1PPS�����M�����
    if (Count1PPS < STABLE1PPS) {
        G1PPS_status = (unsigned)(G1PPS_status << 1) | G1PPS_GetValue();
        if ((G1PPS_status & 0x0f) == 0x08) {
            //�������Low���x���o����(3��A������L�ɂȂ�����=HLLL)
            Count1PPS++;
            if (Count1PPS == STABLE1PPS) {
                GetGPS = GPS_Start;    //���肵����AGPS�f�[�^��荞�ݎw��
                BufferP = Buffer;   //�����l�Ƃ��āABuffer�̐擪�ɐݒ�
                return;
            }
        }
    }

}

/*
 * Year/Month/Day/Weekday��EEPROM�Ɋi�[���Ă����A�d�r����ւ���̕ύX�̎�Ԃ��Ȃ�
 */
void WriteYMD() {
    int8_t jj;
    char tmp[4];

    //������������������Ȃ��悤�ɁA�ω��������������ɂ���
    for (jj = 0; jj < 4; jj++) {
        tmp[jj] = (unsigned)DATAEE_ReadByte(AddressYMD + jj);
        if (tmp[jj] != DateTime[jj+3]) {
            DATAEE_WriteByte(AddressYMD + jj, DateTime[jj+3]);
            UpdateFlag |= UpdateDate;
        }
    }
}


/*
 * INTA��INT0���荞�݂�
 * 1�b���ƂɊ��荞�݂�����̂ŁA�����̍X�V���邱�Ƃ�m�点��t���O���Ă�
 */
void RTC_handler() {
    UpdateFlag |= UpdateTime;

}

/*
 * �A���[��W(RTC_INTB��IOC)�ɂ�銄�荞��
 */
void AlarmWHandler() {
    if (RTC_INTB_GetValue()==0) {
        //��ԕω����荞�݂̂��߁A0�ɂȂ����������t���O���Ă�
//        UpdateFlag |= UpdateAlarmW;
        AlarmSoundOn(0);
        AlarmStatus = 1;
    }
}


/*
 * TMR1��100us���̊��荞�݂ŁA���[�^���[�G���R�[�_�̏�Ԃ��`�F�b�N
 * �L�[�̃`���^�����O�h�~�Ɠ��l�̃v���O����
 * �N���b�N�̈���_�ł́AA�[�q�͏��OFF=1�̏��
 * RotCount���X�V���Ă���
 */
void RotaryHandler() {
    static uint8_t StartRot = 0;    //
    static int8_t Bdata = 0; // ���v���=1�A�����v���=0
    static uint8_t RotA_status = 0xff;  //ROT_A�����ω��_���`�F�b�N
    uint8_t newBdata;

    //A�[�q���̃��x�����`�F�b�N�B��~��=1�A��������0�ƂȂ�A�N���b�N�|�C���g��1�ɖ߂�
    RotA_status = (unsigned)(RotA_status << 1) | ROT_A_GetValue();

    if ((StartRot == 0) && (RotA_status & 0x07) == 0) {
        //A�[�q��0�ɂȂ�����A��]�J�n�Ɣ��f
        StartRot = 1;
        //��]�J�n�Ɣ��f��������B�[�q�̃��x���ŉ�]�����𔻒�
        Bdata = ROT_B_GetValue();
    } else if ((StartRot == 1) && ((RotA_status & 0x07) == 0x07)) {
        //��]���삵�Ă��āAA�[�q��1�ɂȂ������̏���
        newBdata = ROT_B_GetValue();
        // B�[�q�����A1��0�̎�CW(�C���N��)�A0��1�̎�RCW(�f�N��)
        if (Bdata < newBdata) {
            //A�[�q��H�ɂȂ������AB�[�q��H�Ȃ甽���v�����ɉ�]
            RotCount--;
        } else if (Bdata > newBdata) {
            //A�[�q��H�ɂȂ������AB�[�q��L�Ȃ玞�v�����ɉ�]
            RotCount++;
        }
        //�����Ɖ�]�������ɖ߂����ꍇ�A�J�E���g�������A�I������P�[�X����B
        StartRot = 0;
    }
    
}



/*
 * �^�b�`�������Wx,y(�O���t�B�b�N���W�Ɋ��Z��������)���A�w�肳�ꂽ�{�^���͈̔͂�
 * �����Ă��邩�`�F�b�N
 * �{�^�����������Ɣ��肳����1��Ԃ��B����ȊO�́A0
 * ButtonObj3�́Ax1,y1, w, h
 * btn: 0=Year, 1=Month, 2=Day, 3=Time, 4=Cancel, 5=OK, 6=Calendar
 */
int8_t ButtonPush(uint16_t x, uint16_t y, uint8_t btn) {
    int16_t xx, yy;
    
    xx = x - ButtonObj3[btn].x;
    yy = y - ButtonObj3[btn].y;
    if ((xx >= 0) && (xx < ButtonObj3[btn].xw)) {
        if ((yy >= 0) && (yy < ButtonObj3[btn].yw)) {
            //�{�^���̋�`�̈�ɓ����Ă����ꍇ
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
 * �ʏ�̏���
 * ���v�A���t�̕\���X�V
 */
void NormalProc() {
    char str[100];
    int16_t delta;
    int8_t mm, hh, jj;
    UINT actualLength;

    if (Mode == NormalInit) {
        lcd_fill(BLACK);
        //�{�^���̈ʒu���W�����[�h�ɍ��킹�ĕύX
        ButtonObj3[BtnCalendar] = MonthCalendar[DisplayMode];

        //�����\���͕ω����������������\���X�V����̂ŁABCD�ł͂��肦�Ȃ����l��ݒ肵�Ă���
        resetPreDateTime();

        get_tempHumidity(&Temp, &Humidity);
        drawTempHumidity(DisplayMode, Temp, Humidity);
    
        UpdateFlag |= UpdateTime | UpdateDate;
        Mode++; //To Normal
    }
    
    /*
     * �V���A���f�[�^��M
     * ���ꂪ�Ăяo���ꂽ��ALF�܂ō\�킸�f�[�^�擾
     * �G���[����Β��f
     */
    if (GetGPS == GPS_Start) {
        //1�s�擾���āA���̏������I���܂ŁA���̃f�[�^�擾�����Ȃ�
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
                return;    //�o�b�t�@�����ӂ��悤�Ȃ�f�[�^�j�����A���f����   
            }
            if (rxData == 0x0a) {
                //���s�R�[�h(0x0a=LF)��������A�f�[�^��M���I�����A�t���O���Ă�
                *BufferP = '\0';
                GetGPS = GPS_GotData;
                return;
            }
        }
    }

    if (UpdateFlag & UpdateTime) {
        //���t�A�����̍X�V
        RTC_read(DateTime);
        drawDateTime(DisplayMode, DateTime);
        resetCTFG();
        EXT_INT0_InterruptFlagClear();
        WriteYMD(); //���t���ς������A��������ł���
        UpdateFlag &= ~UpdateTime; //UpdateTime���N���A
        
        if ((DateTime[0] & 0x0f) == 9) {
            //�����x�́A10�b���ɍX�V
            get_tempHumidity(&Temp, &Humidity);
            drawTempHumidity(DisplayMode, Temp, Humidity);
        }

        if ((DateTime[0] & 0x0f) == 7) {
            //�Ɠx�Z���T�ŁA���邳��10�b���ƂɎ擾����
            //ADC�̌��ʂ́A10bit��0-1023�B���邢�Ƒ傫�Ȑ��l
            //�������A�Â���ADC�̌��ʂ����Ȃ�΂��
            //���ԂȂ�3.07V@3.29V�ŁA954-958�A��̌u���������ƁA50-150�ʂ�������
            //�΂����ጸ���邽�߁A8��̍��v�l�����A���ω�
            //ADC_GetConversion�Ń`���l���ݒ�AADC�J�n-�擾�܂Ŏ��s
            Brightness = Brightness/8*7 + ADC_GetConversion(PhotoDiode);
            // Duty��ύX���ăo�b�N���C�g�̖��邳��ύX
            // Brightness����萔�ȏ�̎��́A�o�b�N���C�g�͂قڏ펞�_���B
            BackLight = Brightness/8*3 + 10;
            // BackLight�́A�Œ�10-max999   0���ƑS����ʌ����Ȃ��Ȃ�̂ŁA��߂�
            if (BackLight >= 1000) BackLight = 999;

#ifdef DEBUG            
            //�P�x�̃��x����\�� (�f�o�b�O�p)
            //        sprintf(str, "B=%4d", Brightness);
            sprintf(str, "B=%d", BackLight);
            display_drawChars(170, 20, str, WHITE, BLACK, 1);
#endif
            //PWM�́A10�r�b�g�̉𑜓x (TMR6�̎���=PR6=0xff�ɐݒ肵����)
            EPWM1_LoadDutyValue(BackLight);
        }

//#ifdef SDLOGWRITE
        // SD�J�[�h�ɉ����x�𖈕��L�^
        if (DateTime[0] == 5) {
            //����AMount����ƁA�r����SD�J�[�h�����������Ă����v�Ȃ̂ŁA����ōs��
            if (f_mount(&drive,"0:",1) == FR_OK) {
                if (f_open(&file, "TempLog.TXT", FA_WRITE | FA_OPEN_APPEND ) == FR_OK) { //Open or Create �ǋL
                    //���t�����ƋC�����L�^
                    sprintf(str, "%x/%x/%x %02x:%02x %d\r\n", DateTime[6], DateTime[5], DateTime[4], DateTime[2], DateTime[1], Temp);
                    f_write(&file, str, strlen(str), &actualLength);
                    f_close(&file);
                }
                f_mount(0,"0:",0);  //unmount disk
            } else {
                //�I�[�v���ł��Ȃ������|���b�Z�[�W�o��
                sprintf(str, "No SD card");
                display_drawChars(15, 230, str, GREY, BLACK, 1);
            }
        }
//#endif
    }
    
    //�^�b�`���ꂽ���̏���
    if (TouchStatus == 2) {
        TouchStatus++;
        while (GetTouchLocation(&TouchX, &TouchY) == -1);
        TransCoordination(TouchX, TouchY, &Test_x, &Test_y);
#ifdef DEBUG
        sprintf(str, "T=%3d,%3d", Test_x, Test_y);
        sprintf(str, "T=(%4d,%4d)(%3d,%3d)", TouchX, TouchY, Test_x, Test_y);
        display_drawChars(145, 25, str, WHITE, BLACK, 1);
#endif
        //�A���[�����Ƀ^�b�`������X�k�[�Y
        if (AlarmStatus == 1) {
            AlarmSoundOff();
            AlarmStatus = 0;    //�A���[���~�߂���0��
            RTC_resetAlarm();
            SmoothCount++;
            if (SmoothCount <= 12) {
                //5����ɃA���[���Đݒ�@12��܂ŁA1���Ԍ�܂�
                mm = Bcd2Hex(AlarmTime[0]);
                hh = Bcd2Hex(AlarmTime[1]);
                IncDecTime(SmoothCount*5, &hh, &mm);    //5��*Smooth�񐔂̎����ɐݒ�
                SmoothAlarmTime[0] = Hex2Bcd(mm);
                SmoothAlarmTime[1] = Hex2Bcd(hh);

                //�����ŁARTC�ɃA���[�������Đݒ�
                RTC_setAlarmTime(SmoothAlarmTime);
                
            } else {
                //�X�k�[�Y�I��
                SmoothCount = 0;
                //�I���W�i���̃A���[���������Z�b�g������(�����p)
                RTC_setAlarmTime(AlarmTime);
            }
        }
        // �J�����_�[�����^�b�`������A�\�����[�h��ύX
        if (ButtonPush(Test_x, Test_y, BtnCalendar)) {
            DisplayMode = (DisplayMode +1) % 3;
            DATAEE_WriteByte(AddressDisplayMode, DisplayMode);  //�ύX�����珑����
            
            Mode = NormalInit;
            
        }

    } else if (TouchStatus == 5) {
        //�^�b�`������
//        display_drawChars(250, 140, "T- Long", WHITE, BLACK, 1);

        //�^�b�`�������Őݒ胂�[�h�ֈڍs
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

    //�m�[�}�����̃��[�^���[SW�̓A���[��������ύX
    if (RotCount != 0) {
        delta = RotCount;
        RotCount = 0;
        
        if (DisplayMode != DisplayMode3) {
            //������]��������A�{���A4�{���Ł@�@�������Adelta�́Aint8�Ƃ������Ƃɒ���
            if ((delta > 2) || (delta < -2)) {
                //8�{�����܂�
                if (Accel < 8) Accel = Accel * 2;
                delta = delta * Accel;
            }
            else Accel = 1;
        } else {
            delta = delta * 5;  //5���P�ʂ�
        }
        
        mm = Bcd2Hex(AlarmTime[0]);
        if (DisplayMode == DisplayMode3) mm = mm - (mm % 5);    //5���P��
        hh = Bcd2Hex(AlarmTime[1]);
        IncDecTime(delta, &hh, &mm);
        AlarmTime[0] = Hex2Bcd(mm);
        AlarmTime[1] = Hex2Bcd(hh);

        //�A���[��SW�̏�Ԃɂ�����炸�ARTC�ɃA���[�������̕\���͕ύX
        //SW�I���̎��́ARTC���X�V
        if (SlideSWStatus > 2) {
            RTC_setAlarmTime(AlarmTime);
        }
        drawAlarmTime(DisplayMode, AlarmTime, SlideSWStatus);

        //EEPROM�ɏ������ރ^�C�~���O�����B���[�^���[�����邮��񂵂Ă��鎞�́A
        //���x��EEPROM�ɃA���[���������������݂����Ȃ�
        //�ω�����莞��(1���Ƃ�)�Ȃ������珑�����ފ����ɂ���
        
    }
    
    //GPS�̏��: ��M���͐ԁA��������Ɨ�(�d��������Ŏ�M�O�ł�)
    if (GetGPS >= GPS_Start) {
        display_fillCircle(7, 230, 3, RED);
    } else {
        display_fillCircle(7, 230, 3, GREEN);
    }
    
    if (UpdateFlag & UpdateDate) {
        //�J�����_�[���X�V
        DrawCalendar(DisplayMode);
        UpdateFlag &= ~UpdateDate; //UpdateDate���N���A

        //���t���ς������A�ēxGPS��M
        if (Count1PPS >= STABLE1PPS) Count1PPS = 0;
    }

//    sprintf(str, "%d", Count1PPS);
//    display_drawChars(15, 230, str, WHITE, BLACK, 1);
    
//    sprintf(str, "SD %d", SDcardStatus);
//    display_drawChars(15, 230, str, WHITE, BLACK, 1);

}


/*
 * �ݒ�p��ʕ`��
 * �g�ň͂ނ��Ƃ��l�������A�ӊO�Ɩʓ|
 * �f���ɑΏۂ̕����F��ύX���邾���ɂ����B�_�łƂ��̕����ǂ���������Ȃ�
 * 
 * settingMode = SettingYear, SettingMonth, SettingDay, SettingTime
 */
void DrawSetBox(uint8_t settingMode) {
    char str[100];
    uint16_t color;
    uint8_t jj;

    //�������񕁒ʂ̐F�ŕ\������
    drawDate(DispSetting, TmpTime);

    if (settingMode == SettingTime) {
        //�ݒ肵�Ă���ӏ��̐F��ς���
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
    
    //�^�b�`�ł��ύX�ł���悤�ɁA�O�p�{�^����`��@�S����8��
    uint16_t y1, y2;
    uint16_t xx[3];
    //�A�b�v��
    xx[0] = ButtonObj3[BtnYearUp].x + 30;
    xx[1] = ButtonObj3[BtnMonthUp].x + 15;
    xx[2] = ButtonObj3[BtnDayUp].x + 15;

    y1 = ButtonObj3[BtnYearUp].y + 7;
    y2 = y1 + ButtonObj3[BtnYearUp].yw - 10;
    
    for (jj=0; jj<3; jj++) {
        display_fillTriangle(xx[jj], y1, xx[jj]-15, y2, xx[jj]+15, y2, GREY);
        display_drawTriangle(xx[jj], y1, xx[jj]-15, y2, xx[jj]+15, y2, WHITE);
    }

    //down��
    y2 = ButtonObj3[BtnYearDown].y + 3;
    y1 = y2 + ButtonObj3[BtnYearDown].yw - 10;
    for (jj=0; jj<3; jj++) {
        display_fillTriangle(xx[jj], y1, xx[jj]-15, y2, xx[jj]+15, y2, GREY);
        display_drawTriangle(xx[jj], y1, xx[jj]-15, y2, xx[jj]+15, y2, WHITE);
    }
    
    //�����p
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
        yy = (yy + delta -1 + 99) % 99 +1;  // 1-99�N�܂�
    } else if (mode == SettingMonth) {
        mo = (mo + delta -1 + 12) % 12 +1;
    } else if (mode == SettingDay) {
        dd = (dd + delta -1 + 31) % 31 +1;
    }
    //�j�����v�Z
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
 * �^�b�`�������W��������āA�ǂ̃{�^���������ꂽ�����肷��
 */
void ButtonCheck(uint16_t x, uint16_t y, uint8_t changeTime) {
    uint8_t jj, kk;

    //�ǂ̃{�^���������ꂽ���`�F�b�N
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
        //���s�[�g�����悤
        TouchStatus = 2;
        TouchCount = 0;
    } else if (ButtonPush(x, y, BtnTimeDown)) {
        Timecalc(Mode, -1);
        //���s�[�g�����悤
        TouchStatus = 2;
        TouchCount = 0;
    } else if (ButtonPush(x, y, BtnCancel)) {
        Mode = NormalInit;
    } else if (ButtonPush(x, y, BtnOK)) {
        for (jj = 0; jj < 7; jj++) DateTime[jj] = TmpTime[jj];
        DateTime[0] = 0x00;     //�b��00�Ƃ���
        if (changeTime) RTC_setTime(DateTime);  //�����ύX���̂�
        RTC_setDate(DateTime);
        //���t��ύX������AEEPROM�ɏ�������
        WriteYMD();
        Mode = NormalInit;
    } else if (ButtonPush(x, y, BtnYear)) {
        //���[�^���[�̑Ώۂ�I��(�Ԃ�����)
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
 * �ݒ�̏���
 * �N�����A����
 * �A���[�������͂����ł͕ύX���Ȃ�
 */
void SettingProc() {
    char str[100];
    int8_t delta, jj;
    int8_t yy, mo, dd, mm, hh;
    uint8_t y, m, d;
    static uint8_t changeTime;

    if (Mode == Setting) {
        //���̃��[�h�Ɉڍs�������ɏ���������
        lcd_fill(BLACK); //��ʂ��N���A(��)
        
        Mode = SettingTime;
        //�ݒ�p�Ƀe���|�����f�[�^������������R�s�[
        for (jj = 0; jj < 7; jj++) TmpTime[jj] = DateTime[jj];
        changeTime = 0;
        resetPreDateTime();        //�������Ă����Ȃ��ƕ`�悳��Ȃ�
        //��ʏ�������
        DrawSetBox(Mode);

        //�{�^���`��  Cancel, OK, About
        for (jj=BtnCancel; jj<=BtnAbout; jj++) {
            display_drawRoundRect(ButtonObj3[jj].x, ButtonObj3[jj].y, ButtonObj3[jj].xw, ButtonObj3[jj].yw, 10, WHITE);
            //�{�^���̒����Ƀe�L�X�g�\��
            uint16_t xp = ButtonObj3[jj].x + ButtonObj3[jj].xw/2 - strlen(ButtonName[jj - BtnCancel])*4;
            uint16_t yp = ButtonObj3[jj].y + ButtonObj3[jj].yw/2 - 4;
            display_drawChars(xp, yp, (char *) ButtonName[jj - BtnCancel], WHITE, BLACK, 0x12);
        }
        return;
    }

    //�^�b�`�������̏���
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

        //�^�b�`�����ꏊ�Ƀ{�^�������邩�`�F�b�N���āA���������s
        ButtonCheck(Test_x, Test_y, changeTime);

    } else if (TouchStatus == 5) {
        TouchStatus++;
        //�^�b�`������
//        display_drawChars(250, 160, "T- Long", WHITE, BLACK, 1);
        //�^�b�`�������Ńm�[�}�����[�h�ֈڍs
//        DrawSetBox(Mode, 0xff);
        //�m�[�}���ɔ����鎞�A�L�����Z�����ł���悤�ɂƍl���Ă���
        Mode = NormalInit;
        
    } else if (TouchStatus == 0) {
        TouchStatus++;
//        display_drawChars(250, 160, "TouchOFF", WHITE, BLACK, 1);
    }

//    sprintf(str, "Rot=%4d", RotCount);
//    display_drawChars(250, 100, str, BLACK, WHITE, 1);

    //���[�^���[�𓮂������Ƃ��̏���
    if (RotCount != 0) {
        delta = RotCount;
        RotCount = 0;
        //������]��������A�{���A4�{���Ł@�@�������Adelta�́Aint8�Ƃ������Ƃɒ���
        if ((delta > 2) || (delta < -2)) {
            //8�{�����܂�
            if (Accel < 8) Accel = Accel * 2;
            delta = delta * Accel;
        }
        else Accel = 1;

        if (Mode == SettingTime) {
            Timecalc(Mode, delta);
            drawTime(DispSetting, TmpTime, RED);

            changeTime = 1; //������ύX������1
        } else if (Mode >= SettingYear && Mode <= SettingDay) {
            YMDclac(Mode, delta);
            DrawSetBox(Mode);
        }

    }

}

/*
 * �X���C�hSW�̏������[�`��
 * SlideSWStatus
 * �@0: �I�t���蒼��
 * �@1: �I�t������
 * �@2: �I�����蒼��
 * �@3: �I��������
 */
void SlideSWProc() {
    int8_t jj;
    
    if (SlideSWStatus == 2) {
        // SW���I���ɂ�������̏���
        drawAlarmTime(DisplayMode, AlarmTime, SlideSWStatus);
        AlarmStatus = 0;
        RTC_setAlarmTime(AlarmTime);

        //�X���C�hSW��On�ɂ���������EEPROM�ɃA���[����������������
        for (jj=0; jj<3; jj++) {
            DATAEE_WriteByte(AddressAlarm+jj, AlarmTime[jj]);
        }
        SlideSWStatus++; //�X���C�hSW�I���̏�������������3��
        
    } else if (SlideSWStatus == 0) {
        // SW���I�t�ɂ������̏���
        AlarmSoundOff();
        AlarmStatus = 0;
        RTC_resetAlarm();
        drawAlarmTime(DisplayMode, AlarmTime, SlideSWStatus);
        SlideSWStatus++; //�X���C�hSW�I�t�̏�������������1
        SmoothCount = 0;
    }

}


/*
 * �`�F�b�N�T�����v�Z���āA��v���Ă��邩�`�F�b�N����
 * Buffer�ɁA$����?�܂ł̃f�[�^�����鎞�Ɍv�Z����
 * �G���[����΁A1�@�@$��?���Ȃ��ꍇ�A�`�F�b�N�T���v�Z�������Ă��Ȃ��ꍇ
 */
uint8_t CheckCheckSum(char *buf) {
    char *start;
    char *end;
    uint8_t checksum;
    
    start = strchr(buf, '$');
    if (start == NULL) return 1;    //������Ȃ���΁A�G���[
    end = strchr(buf, '*');
    if (end == NULL) return 1;    //������Ȃ���΁A�G���[
    
    checksum = 0;
    start++;    //$�̎�����v�Z�ΏۂƂ���
    while (start < end) {
        checksum = (uint8_t)(checksum ^ (*start)); //xor�v�Z
        start++;
    }

    //�v�Z�����`�F�b�N�T���ƃf�[�^�ł�������`�F�b�N�T��(?�̌���2����)���r
//    if (checksum == strtol(end+1, NULL, 16)) {    //������ł�OK�����A�T�C�Y�傫���Ȃ�
    if (checksum == (uint8_t)((strchr(HEX, end[1]) - HEX) <<4) + (strchr(HEX, end[2])-HEX)) {
        return 0;
    } else {
        //�`�F�b�N�T���G���[�̎�
        return 1;
    }
    
}


/*
 * �擾����GPRMC�̃f�[�^����������
 * GPRMC�ȊO�̃f�[�^�̎��́A�������Ȃ�
 * �G���[��: 1
 */
uint8_t GPRMCdatacheck() {
    uint8_t jj;
    char *p;
    char str[100];

    //$GPRMC�̃f�[�^�̎���������
    if (strncmp(Buffer, "$GPRMC", 6) != 0) return 1;

    //�`�F�b�N�T���̃`�F�b�N
    if (CheckCheckSum(Buffer)) {
//        sprintf(str, "Chk Error");
//        sprintf(str, "x");
//        display_drawChars(0, 95, str, WHITE, BLACK, 1);
        return 1;
    }

    //�f�[�^������Ȃ̂ŁA�K�v�������o��
    //        sprintf(str, "-");
    //        display_drawChars(0, 120, str, WHITE, BLACK, 1);
    // hhmmss: 7-12�����@�@1�ڂ̃f�[�^=�ŏ���,�̌ォ��f�[�^������
    DateTime[2] = ((Buffer[7]-'0')<<4) + (Buffer[8]-'0');   //hh
    DateTime[1] = ((Buffer[9]-'0')<<4) + (Buffer[10]-'0');  //mm
    DateTime[0] = ((Buffer[11]-'0')<<4) + (Buffer[12]-'0'); //ss
    
    // ddmmyy: 9�ڂ̃f�[�^=9�Ԗڂ�,�̌ォ��f�[�^������
    p = strchr(Buffer, '$');
    for (jj = 0; jj < 9; jj++) {
        p = strchr(p+1, ',');
        if (p == NULL) return 1;    //������Ȃ�������A��߂�
    }
    DateTime[4] = ((p[1]-'0')<<4) + (p[2]-'0'); //dd
    DateTime[5] = ((p[3]-'0')<<4) + (p[4]-'0'); //mm
    DateTime[6] = ((p[5]-'0')<<4) + (p[6]-'0'); //yy
    
    //�o�x���擾: 5�Ԗڂ̃f�[�^    yyyyy.yy
    //�o�x�ŁA������␳���悤�Ǝv�������ǁA���{�����ł��A�n��ɂ���Ă͂�낵���Ȃ��̂ŁA���~
    int8_t TZ = +9; //����JST�ɌŒ�
    /*
    p = strchr(Buffer, '$');
    for (jj = 0; jj < 5; jj++) {
        p = strchr(p+1, ',');
        if (p == NULL) return 1;
    }
    uint8_t len = (strchr(p, '.') - p)-3; //�o�x�̌����@�K������3���ł͂Ȃ��̂�
    for (jj = 0; jj < len; jj++) {
        TZ = TZ*10 + (p[1+jj]-'0');
    }
    //���o��+�A���o��-�ɂ���
    p = strchr(p+1, ',');
    if (p[1] == 'W') TZ = -TZ;
    TZ = (TZ+8) / 15;   //1���ԓ�����o�x��15�x�Ȃ̂ŁA�o�x���玞�����v�Z
     */
    
    //�W������JST (���ݒn�̎���)��
    int8_t hh;
    hh = Bcd2Hex(DateTime[2]) + TZ;    //JST�Ȃ�+9
    if (hh >= 24) {
        //�[��12�����߂�������t��1���i�߂�
        hh = hh - 24;
        DateTime[4] = DateTime[4] + 1;  //���t�i�߂�
    } else if (hh < 0) {
        //TZ���}�C�i�X�̏ꍇ���ꉞ�v���O�������Ă���
        hh = hh + 24;
        DateTime[4] = DateTime[4] - 1;  //���t�߂� 1����������A0���ɂȂ邪���Ԃ�OK
    }
    DateTime[2] = Hex2Bcd(hh);  //������̎��Ԃ�߂�
    dateErrorCheck(DateTime);   //���t�̒���
    
    //        sprintf(str, "%02x:%02x:%02x %02x/%02x/%02x", DateTime[2], DateTime[1], DateTime[0], DateTime[6], DateTime[5], DateTime[4]);
    //        display_drawChars(0, 110, str, WHITE, BLACK, 1);
    
    //�������擾�ł����̂ŁARTC�ɏ�������
    dateErrorCheck(DateTime);   //���t���`�F�b�N���āA�j����ݒ�
    //RTC�̓������C��
    RTC_setDate(DateTime);
    RTC_setTime(DateTime);

#ifdef DEBUG
    �f�o�b�O�p�Ƀf�[�^�\��
    Buffer[13] = '\0';
    display_drawChars(0, 95, Buffer, WHITE, BLACK, 1);
    Count1PPS = STABLE1PPS - 10; //�����Ɏ��̎�M�������Ă݂�
#endif

    return 0;

}


/*
 * �^�b�`���W�����p���b�Z�[�W
 * �Ώۂ̃��b�Z�[�W�𔒂ŁA����ȊO���O���[��
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
// (20,20)�A(300,220)��2�_���^�b�`�������̍��W��(T_x1, T_y1)�A(T_x2, T_y2)��
// ��荞�ނ��߂̃��[�`��
void TouchAdjust(void) {
    uint8_t count, kk;
    char str[100];

    //����̏\��
    display_drawLine(17, 20, 23, 20, WHITE);
    display_drawLine(20, 17, 20, 23, WHITE);

    //�ŏ��ɍ�����^�b�`���āA���ɉE�����^�b�`���Ă��炤
    TouchAdjMsg(1);

    count = 0;
    while (1) {
        if (TouchStatus == 2) {
            if (count == 0) {
                // 1��ڂ̃^�b�`�̈ʒu
                while (GetTouchLocation(&T_x1, &T_y1) == -1);
                
                //�^�b�`������s�b�Ƃ��������o��
                AlarmSoundOn(0);
                __delay_ms(100);
                AlarmSoundOff();
                
                //����̏\��
                display_drawLine(17, 20, 23, 20, GREY);
                display_drawLine(20, 17, 20, 23, GREY);

                //�E���̏\��
                display_drawLine(297, 220, 303, 220, WHITE);
                display_drawLine(300, 217, 300, 223, WHITE);
    
                TouchAdjMsg(2);

                count++;
            } else {
                int16_t dx, dy;
                // 2��ڂ̃^�b�`
//                do {
                    while (GetTouchLocation(&T_x2, &T_y2) == -1);
                    //�^�b�`���Ă���ʒu�����������Ǝv����ꍇ��r��
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
                //�^�b�`������s�b�Ƃ��������o��
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
 * 16bit RGB=565��bmp�t�@�C����ǂݍ��ݕ\������
 * 
 * 0x0000�@(2)	bfType		�t�@�C���^�C�v�@�ʏ��'BM'
 * 0x0002�@(4)	bfSize		�t�@�C���T�C�Y (byte)
 * 0x0006�@(2)	bfReserved1	�\��̈�@��� 0
 * 0x0008�@(2)	bfReserved2	�\��̈�@��� 0
 * 0x000A�@(4)	bfOffBits	�t�@�C���擪����摜�f�[�^�܂ł̃I�t�Z�b�g (byte)
 * 0x000E�@(4)	bcSize		�w�b�_�T�C�Y
 * 0x0012�@(4)	bcWidth		�摜�̕� (�s�N�Z��)
 * 0x0016�@(4)	bcHeight	�摜�̍��� (�s�N�Z��) �����Ȃ�C�摜�f�[�^�͉�������
 *                                               �����Ȃ�C�摜�f�[�^�͏ォ�牺��
 * 0x001A�@(2)	bcPlanes	�v���[�����@��� 1
 * 0x001C�@(2)	bcBitCount	1��f������̃f�[�^�T�C�Y (bit)
 * 
 * �����ł́A16bit�J���[�ƌ��ߑł����Ă���
 */
#define DATASIZE 320
uint16_t data[DATASIZE];    //���[�J���ł͓��肫��Ȃ��Ă��AData space�ɂ͂܂�������
uint8_t ReadBmp16() {
    uint16_t x, y, jj, kk, ll;
    UINT actualLength;
    char str[100];
    uint8_t error = 0;
    
    if (f_mount(&drive,"0:",1) == FR_OK) {
        //�t�@�C������8�����܂ŃT�|�[�g
        if (f_open(&file, "Open4.bmp", FA_READ ) == FR_OK) { //Open
            //�w�b�_�Ǎ�
            f_read(&file, data, 32, &actualLength); //32�o�C�g���ǂݍ���
            x = data[0x12>>1];
            y = data[0x16>>1];
//            f_lseek(&file, data[0x0a>>1]); //�f�[�^�擪�܂ŃV�[�N
            //�V�[�N�̑���Ƀ_�~�[���[�h
            for (jj=0; jj<(data[0x0a>>1]-32)/2; jj++) f_read(&file, data, 2, &actualLength);
            
            //bmp�f�[�^�́A���̍s����n�܂�
            for (kk=0; kk<y; kk++) {
                for (jj=0; jj<x/DATASIZE; jj++) {
                    f_read(&file, data, DATASIZE*2, &actualLength); //128�o�C�g=64���[�h=64�h�b�g
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
    //�J���[�o�[�Ɠ������A75%�̔��A���A�V�A���A�΁A�}�[���^�A�ԁA��
    uint16_t color[] = {
        0xBDF7, //��
        0xBDE0, //��
        0x05F7, //�V�A��
        0x05E0, //��
        0xB817, //�}�[���^
        0xB800, //��
        0x0017, //��
    };

    lcd_fill(BLACK); //��ʂ��N���A(�^����)
    
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
    //�J���[�o�[�\��
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
    lcd_fill(BLACK); //��ʂ��N���A(�^����)

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

    //�A���[���̏�����
    AlarmInitialize();  //�A���[������TMR0�̏����ݒ�

    //�����́A�悭�킩��Ȃ����ASYSTEM_Initialize���AIOCB���L��������A��̃n���h���̏�Ԃ��ƁA�Ȃ���
    //�n���O����B����InterruptEnable�̑O�ɂ��Ȃ��ƃn���O����B���ꂪ�킩�炸�����Y��
    //IOCB4=0�ɂ��邩�A�n���h���ݒ�(���g���Ȃ��ƃf�t�H���g�Ɠ������_��)������K�v����
    IOCB4_SetInterruptHandler(AlarmWHandler); //�A���[�� Alarm W�̊��荞��

    // ���[�^���[�pTMR1�̊��荞��
    TMR1_SetInterruptHandler(RotaryHandler);

    //Touch/SlideSW/GPS�́ATMR5��10ms���荞�݂ŏ�ԃ`�F�b�N
    TMR5_SetInterruptHandler(Timer5Handler);
    TMR5_StartTimer();
    TouchCount = 0;
    TouchStatus = 0;
    //GPS�́A�d��������A1PPS�������Ƃ��m�F��AGPS��M������J�n����
    Count1PPS = 0;
    GetGPS = GPS_Stop;

    // RTC�̏�����
    init_RTC(DateTime);
    INT0_SetInterruptHandler(RTC_handler);  //INTA (1s��)�̊��荞�ݏ���

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

////    SDCard_CS_SetHigh(); //�ŏ��̊m�F�p�@LED�����点�āA�n�[�h�A�v���O�����������ݓ��̏����`�F�b�N
    
    //LCD�̏�����
    glcd_init();

    OpeningScreen(0);
    
    //�N����
    AlarmSoundOn(0);
    __delay_ms(100);
    AlarmSoundOff();
    __delay_ms(200);
    AlarmSoundOn(0);
    __delay_ms(200);    //���܂�傫�������ƓK�؂Ȃǂ������Ȃ�
//    __delay_ms(200);
    AlarmSoundOff();
    
    display_SetFont(NormalFont);  //�����t�H���g�ݒ�
    //���߂ċN�������Ƃ��́A�^�b�`�̒��������{���A���̃f�[�^��EEPROM�ɕێ�
    if (DATAEE_ReadByte(AddressInit) == 0xff) {
        TouchAdjust();
        lcd_fill(BLACK); //��ʂ��N���A

        /*
        //�^�b�`������ɂł��Ă��邩����m�F�p
        while (1) {
            if (TouchStatus != 0) {
                uint16_t x, y, dx, dy;
                //�`�悵�Ă݂�
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
        
        //�^�b�`���@�\���Ȃ����̃f�t�H���g�l
//        T_x1 = 0x0129;
//        T_y1 = 0x0117;
//        T_x2 = 0x0729;
//        T_y2 = 0x06c9;
        
        DATAEE_WriteByte(AddressInit, 0x55);
        //���������^�b�`���W����������
        DATAEE_WriteByte(AddressTouch,   lo(T_x1));
        DATAEE_WriteByte(AddressTouch+1, hi(T_x1));
        DATAEE_WriteByte(AddressTouch+2, lo(T_y1));
        DATAEE_WriteByte(AddressTouch+3, hi(T_y1));
        DATAEE_WriteByte(AddressTouch+4, lo(T_x2));
        DATAEE_WriteByte(AddressTouch+5, hi(T_x2));
        DATAEE_WriteByte(AddressTouch+6, lo(T_y2));
        DATAEE_WriteByte(AddressTouch+7, hi(T_y2));
        //���t�A�A���[���ݒ�̃f�t�H���g�l���������ށ@���t�͏������ވӖ����Ȃ��̂Œ��~
//        for (jj=0; jj<4; jj++) {
//            DATAEE_WriteByte(AddressYMD+jj, DateTime[jj+3]);
//        }
        for (jj=0; jj<3; jj++) {
            DATAEE_WriteByte(AddressAlarm+jj, AlarmTime[jj]);
        }
        DisplayMode = DisplayMode1;
        DATAEE_WriteByte(AddressDisplayMode, DisplayMode);
        
    } else {
        //2��ڈȍ~�̓d�������Ȃ�A�^�b�`���W�␳�̃f�[�^��ǂݏo��
        T_x1 = DATAEE_ReadByte(AddressTouch) + (DATAEE_ReadByte(AddressTouch+1)<<8);
        T_y1 = DATAEE_ReadByte(AddressTouch+2) + (DATAEE_ReadByte(AddressTouch+3)<<8);
        T_x2 = DATAEE_ReadByte(AddressTouch+4) + (DATAEE_ReadByte(AddressTouch+5)<<8);
        T_y2 = DATAEE_ReadByte(AddressTouch+6) + (DATAEE_ReadByte(AddressTouch+7)<<8);
//        sprintf(str, "P1=(%d, %d) P2=(%d, %d)", T_x1, T_y1, T_x2, T_y2);
//        display_drawChars(0, 140, str, WHITE, BLACK, 1);
        
        //���t�A�A���[���ݒ��ǂݏo��
        for (jj=0; jj<4; jj++) {
            DateTime[jj+3] = DATAEE_ReadByte(AddressYMD+jj);
        }
        for (jj=0; jj<3; jj++) {
            AlarmTime[jj] = DATAEE_ReadByte(AddressAlarm+jj);
        }
        //�O��I�����̕\�����[�h�ɂ���
        DisplayMode = DATAEE_ReadByte(AddressDisplayMode);        
    }
    
    Mode = NormalInit;  //������Ԃ̓m�[�}��

    //SD�J�[�h
    //�Ⴆ�΁A�C�����O�����ꍇ
    /*
     * �t�@�C���Ȃ���΁A�����B����΁A�ǋL
     * ����I�ɋC�����L�^����
     * f_open  FA_CREATE_NEW ���ƁA�t�@�C���Ȃ������������B
     * 
     * 
     */
/*
    //�ȉ��͎����Ӗ��Ȃ��B���true�Ԃ��̂�
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
    
        const char time[] = __TIME__;   //�R���p�C�������������擾
//        char data[] = "\r\nHello world! This is text message written to SD card\r\n";
        char data[] = "\r\nHello\r\n";
        //    char data[] = "PIC 18F26K22 AM2320 temperature log\r\n";
        //    char data2[] = "XC8 v2.20 (PIC18F-K_DFP 1.4.87) SPI=4MHz, C90\r\n";
////        if (f_open(&file, "GPS-SD2.TXT", FA_WRITE | FA_CREATE_NEW ) == FR_OK) { //Open or Create TEST.TXT file
        if (f_open(&file, "TempLog.TXT", FA_WRITE | FA_CREATE_ALWAYS ) == FR_OK) { //Open or Create TEST.TXT file�㏑��OK
//        if (f_open(&file, "TempLog.TXT", FA_WRITE | FA_OPEN_APPEND ) == FR_OK) { //Open or Create TEST.TXT file�㏑��OK
//        f_open(&file, "TempLog2.TXT", FA_WRITE | FA_CREATE_ALWAYS ); //Open or Create TEST.TXT file�㏑��OK
            
            //�w�b�_������������Z
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
        //        SDCard_CS_SetHigh(); //�ŏ��̊m�F�p�@LED�����点�āA��������OK��m�点��
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

        //�X���C�hSW
        SlideSWProc();

        //GPS����f�[�^1�s�擾�ł����珈��
        if (GetGPS == GPS_GotData) {
            //�f�[�^���`�F�b�N���āARTC��ݒ�
            if (GPRMCdatacheck()) {
                //�G���[�̎��́A�f�[�^�擾��蒼��
                GetGPS = GPS_Start;
                BufferP = Buffer;   //�����l�Ƃ��āABuffer�̐擪�ɐݒ�
            }
            else {
                GetGPS = GPS_Stop; //�f�[�^�擾�ł�����0��
            }
        }

    }
}
/**
 End of File
*/
