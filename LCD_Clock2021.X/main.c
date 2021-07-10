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
 * TMR0: �A���[�����̂Ȃ��Ă��鎞�Ԃ𑪒� 10ms
 * TMR1: ���[�^���[�G���R�[�_�̃T���v�����O�@100us
 * TMR2: ���d�X�s�[�J�p4KHz��ECCP2 PWM�𔭐������邽�߂̃^�C�}  250us
 * TMR3: ADC��CCP5���[�h�Ŏg�p���āATMR3=4ms�𗘗p
 * TMR5: �^�b�`�A�X���C�hSW�AGPS�̏�ԃ`�F�b�N�̊��荞�ݗp�^�C�}�@10ms
 * TMR6: �o�b�N���C�g��PWM����p�^�C�} 256us
 * 
 * SPI�C���^�t�F�[�X�FLCD�ƃ^�b�`��SD�J�[�h�ŋ��p
 * �@MSSP1=SPI (SCK1, SDI1, SDO1)+LCD_CS, T_CS, SDCard_CS
 *   �@LCD_RESET, LCD_DCRS�AT_IRQ
 *   LCD�Ƃ͍ő�X�s�[�h��8MHz�A�^�b�`��2MHz�ŒʐM�ASD�J�[�h�́A�ᑬ400kHz�A����2MHz
 * SPIMASTER�́A�J�������f�t�H���g�ŁASDFAT=8MHz�ɂȂ��Ă���̂ŁA�C�����Ă����Ȃ��ƁA
 * Generate�����㓮���Ȃ��Ƒ������ƂɂȂ�B
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
 * ���d�X�s�[�J: ECCP2=PWM (Timer2)�@P2A=RB3�@�@�@MCC�J�������ƁAP2A���ݒ肩������Ă��܂��̂ŁA����
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

// ���4�r�b�g�Ń��[�h�A����4�r�b�g�ł��̃��[�h�̒��Ŏg�����ڂȂ�
#define NormalInit       0x00
#define Normal           0x01
#define Setting          0x10
uint8_t Mode;

#define SettingYear      0x11
#define SettingMonth     0x12
#define SettingDay       0x13
#define SettingTime      0x14
#define SettingCancel    0x15
#define SettingOK        0x16

#define SettingAlarmTime 0x17

#define AdjustTouch      0x20

//�\�����[�h
//#define DisplayMode1    0x00    //���v�\���傫���A3�����J�����_�[�\��
//#define DisplayMode2    0x01    //�J�����_�[�傫���\��
//#define DisplayMode3    0x02    //�A�i���O�\��
uint8_t DisplayMode;    // = DisplayMode1;

uint8_t UpdateFlag = 0;
#define UpdateTime          0x01
#define UpdateTimeDisp      0x02
#define UpdateTempHumidity  0x04
#define UpdateBrightness    0x08
#define UpdateAlarmW        0x10
#define UpdateDate          0x20
#define UpdateMsg           0x80

uint8_t TouchCount;
int8_t RotCount = 0; //�񂵂ăN���b�N������
int8_t Accel = 1;   //������]���������ɁA�{����

uint16_t Brightness;
uint16_t BackLight = 100;

uint8_t AlarmStatus = 0;    //�A���[��������1�ɂ��āAsmooth�����Ȃǂ��s��
uint8_t SmoothCount = 0;    //����X�k�[�Y������

#define STABLE1PPS  20  // ����擾���������Ƃ݂Ȃ���
uint8_t Count1PPS;  //1PPS���荞�݂̔����񐔂��J�E���g�B�ő�STABLE1PPS�ɂȂ�
uint8_t GetGPS;     //�d���������A1��1�񂭂炢�̎擾�K�v�ɂȂ�����1�ɂ���
//0: GPS�擾��~�A1:GPS�擾���A2:1�s���̃f�[�^�擾����
char Buffer[100];   //GPS�Ƃ̃V���A���ʐM�p�o�b�t�@
char * BufferP;     //�o�b�t�@�̃f�[�^�ʒu�������|�C���^

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
uint8_t MountDone = 0;  //�}�E���g������A1

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
    //0: �^�b�`���Ă��Ȃ�
    //1: �^�b�`���Ȃ��Ȃ�������̏�������
    //2: �^�b�`�Z����
    //3-4: �^�b�`�Z�������������@�J�E���g��100�ɍs���܂ŁA1-3�̂ǂꂩ�̏�Ԃ͈ێ�
    //5: �^�b�`������
    //6-: �^�b�`��������������
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
                GetGPS = 1;    //���肵����AGPS�f�[�^��荞��
                BufferP = Buffer;   //�����l�Ƃ��āABuffer�̐擪�ɐݒ�
                return;
            }
        }
    }

    /*
     * �V���A���f�[�^��M
     * ���ꂪ�Ăяo���ꂽ��ALF�܂ō\�킸�f�[�^�擾
     * �G���[����Β��f
     */
    if (GetGPS == 1) {
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
                GetGPS = 2;
                return;
            }
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
        UpdateFlag |= UpdateAlarmW;
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


/*
 * �ʏ�̏���
 * ���v�A���t�̕\���X�V
 */
void NormalProc() {
    char str[100];
    int16_t delta;
    int8_t mm, hh, jj;
    UINT actualLength;
//    uint16_t newBL;

    if (UpdateFlag & UpdateAlarmW) {
        //�A���[�������ɂȂ���
        AlarmSoundOn(0);
        AlarmStatus = 1;
        RTC_resetAlarm();   //1���o�߂�����Ȃ��������~�܂��Ă��܂����̂ł����ǉ����Ă݂�
        
        UpdateFlag &= ~UpdateAlarmW; //UpdateAlarmW���N���A
    }

    if (Mode == NormalInit) {
        lcd_fill(BLACK);

        //�����\���͕ω����������������\���X�V����̂ŁABCD�ł͂��肦�Ȃ����l��ݒ肵�Ă���
        for (jj = 0; jj < 3; jj++) preDateTime[jj] = 0xff;

        UpdateFlag |= UpdateTime | UpdateDate;
        Mode++;
    }
    
    if (UpdateFlag & UpdateTime) {
        //���t�A�����̍X�V
        RTC_read(DateTime);

        drawDateTime(DisplayMode);

        resetCTFG();
        EXT_INT0_InterruptFlagClear();
        WriteYMD(); //���t���ς������A��������ł���
        UpdateFlag &= ~UpdateTime; //UpdateTime���N���A
        
        if ((DateTime[0] & 0x0f)==1) {
            //5�b���ƂɍX�V����悤�ɕύX
            //�Ɠx�Z���T�ŁA���邳��1�b���ƂɎ擾����
            //ADC�̌��ʂ́A10bit��0-1023�B���邢�Ƒ傫�Ȑ��l
            //�������A�Â���ADC�̌��ʂ����Ȃ�΂��
            //���ԂȂ�3.07V@3.29V�ŁA954-958�A��̌u���������ƁA50-150�ʂ�������
            //�΂����ጸ���邽�߁A8��̍��v�l�����A���ω�
            //ADC_GetConversion�Ń`���l���ݒ�AADC�J�n-�擾�܂Ŏ��s
            Brightness = Brightness/8*7 + ADC_GetConversion(PhotoDiode);
            // Duty��ύX���ăo�b�N���C�g�̖��邳��ύX
            // Brightness����萔�ȏ�̎��́A�o�b�N���C�g�͂قڏ펞�_���B
            if (Brightness/8 > 300) {
                //            newBL = 1000;
                BackLight = 1000;
            }
            else {
                // ������Brightness�������Ȃ�ƁA0��100�Ɍ����ď���������
                BackLight = 100 + Brightness/8*3;
                //            newBL = 100 + Brightness/8*3;
            }
            //�ω���50�ȏ�N����Ȃ��悤�ɒ���
            //        if (BackLight-newBL > 50) BackLight = BackLight -50;
            //        else if (BackLight-newBL < -50) BackLight = BackLight +50;
            //        else BackLight = newBL;
            
            //�P�x�̃��x����\�� (�f�o�b�O�p)
            //        sprintf(str, "B=%4d", Brightness);
            ////        sprintf(str, "B=%4d", BackLight);
            //        display_drawChars(180, 20, str, WHITE, BLACK, 1);
            //PWM�́A10�r�b�g (TMR6�̎���=PR6=0xff�ɐݒ肵����)
            EPWM1_LoadDutyValue(BackLight);
        }
        
        if ((DateTime[0] & 0x0f) == 2) {
            //�����x�́A10�b���ɍX�V
            get_tempHumidity(&Temp, &Humidity);
            drawTempHumidity(DisplayMode, Temp, Humidity);
        }

        /*
         */
        if (DateTime[0] == 8) {
            //�����A�L�^����
            //����AMount����ƁA�r����SD�J�[�h�����������Ă����v�Ȃ̂ŁA����ōs��
            //�}�E���g�ς݂ł��d�˂Ď��s���Ė��Ȃ�����
            if (f_mount(&drive,"0:",1) == FR_OK) {
                //mount
                if (f_open(&file, "TempLog.TXT", FA_WRITE | FA_OPEN_APPEND ) == FR_OK) { //Open or Create �ǋL
////                    f_lseek(&file, 0);
////                    f_read(&file, str, 80, &actualLength);
////                    str[actualLength] = '\0';
////                    display_drawChars(0, 180, str, WHITE, BLACK, 1);
//                    //���t�����ƋC�����L�^
                    sprintf(str, "%x/%x/%x %02x:%02x %d\r\n", DateTime[6], DateTime[5], DateTime[4], DateTime[2], DateTime[1], Temp);
                    f_write(&file, str, strlen(str), &actualLength);
////                    AlarmSoundOn(0);
////                    __delay_ms(200);
////                    AlarmSoundOff();
////                    f_sync(&file);
                    f_close(&file);
                }
                f_mount(0,"0:",0);  //unmount disk
                glcd_init2();
            }

        }
    }
    
    if (UpdateFlag & UpdateDate) {
        //�J�����_�[���X�V
        drawCalendar(DisplayMode);
        UpdateFlag &= ~UpdateDate; //UpdateDate���N���A

        //���t���ς������A�ēxGPS��M
        if (Count1PPS >= STABLE1PPS) Count1PPS = 0;
    }

    //�^�b�`���ꂽ���̏���
    if (TouchStatus == 2) {
        TouchStatus++;
        GetTouchLocation(&TouchX, &TouchY);
        TransCoordination(TouchX, TouchY, &Test_x, &Test_y);
#ifdef DEBUG
        sprintf(str, "Touch=(%4d, %4d) (%3d, %3d)", TouchX, TouchY, Test_x, Test_y);
        display_drawChars(0, 150, str, WHITE, BLACK, 1);
#endif
        //�A���[�����Ƀ^�b�`������X�k�[�Y
        if (AlarmStatus == 1) {
            AlarmSoundOff();
            AlarmStatus = 0;    //�A���[���~�߂���0��
//            RTC_resetAlarm();
            SmoothCount++;
            if (SmoothCount <= 12) {
                //5����ɃA���[���Đݒ�@12��܂ŁA1���Ԍ�܂�
                mm = Bcd2Hex(AlarmTime[0]);
                hh = Bcd2Hex(AlarmTime[1]);
                IncDecTime(SmoothCount*5, &hh, &mm);    //5��*Smooth�񐔂̎����ɐݒ�
//IncDecTime(SmoothCount*2, &hh, &mm);    //�f�o�b�O�p2��*Smooth�񐔂̎����ɐݒ�
                SmoothAlarmTime[0] = Hex2Bcd(mm);
                SmoothAlarmTime[1] = Hex2Bcd(hh);

                //�����ŁARTC�ɃA���[�������Đݒ�
                RTC_setAlarmTime(SmoothAlarmTime);
                
            } else {
                //�I���W�i���̃A���[���������Z�b�g������
                RTC_setAlarmTime(AlarmTime);
                AlarmStatus = 0;    //�X�k�[�Y�I��
            }
        }
        // �J�����_�[�����^�b�`������A�\�����[�h��ύX
        if (ButtonPush(Test_x, Test_y, BtnCalendar)) {
//        if (Test_y > RThisMonthCalendar[DisplayMode].y) {
            DisplayMode = (DisplayMode +1) % 3;
            DATAEE_WriteByte(AddressDisplayMode, DisplayMode);  //�ύX�����珑����
            FirstDraw = 1;
            
            Mode = NormalInit;
            //�{�^���̈ʒu���W�����[�h�ɍ��킹�ĕύX
            ButtonObj3[BtnYear] = RYear[DisplayMode];
            ButtonObj3[BtnMonth] = RMonth[DisplayMode];
            ButtonObj3[BtnDay] = RDay[DisplayMode];
            ButtonObj3[BtnTime] = RTime[DisplayMode];
            ButtonObj3[BtnCalendar] = RPrevMonthCalendar[DisplayMode];
            
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
        drawAlarmTime(DisplayMode, AlarmTime);
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
        drawAlarmTime(DisplayMode, AlarmTime);

        //EEPROM�ɏ������ރ^�C�~���O�����B���[�^���[�����邮��񂵂Ă��鎞�́A
        //���x��EEPROM�ɏ������݂����Ȃ�
        //�ω�����莞��(1���Ƃ�)�Ȃ������珑�����ފ����ɂ���
        
    }
    
    //GPS�̏��: ��M���͐ԁA��������Ɨ�(�d��������Ŏ�M�O�ł�)
    if (GetGPS >= 1) {
        display_fillCircle(7, 230, 3, RED);
    } else {
        display_fillCircle(7, 230, 3, GREEN);
    }
    
//    sprintf(str, "%d", Count1PPS);
//    display_drawChars(15, 230, str, WHITE, BLACK, 1);
    
//    sprintf(str, "SD %d", SDcardStatus);
//    display_drawChars(15, 230, str, WHITE, BLACK, 1);

}


/*
 * �g�ň͂ނ��Ƃ��l�������A�ӊO�Ɩʓ|
 * �f���ɑΏۂ̕����F��ύX���邾���ɂ����B�_�łƂ��̕����ǂ���������Ȃ�
 */
void DrawSetBox(uint8_t mode) {
    char str[100];
    uint16_t color;

    if (mode == SettingTime) {
        //�ݒ肵�Ă���ӏ��̐F��ς���
        color = RED;
    } else {
        color = GREEN;
    }
    drawTime(DisplayMode, TmpTime, color);

    //�������񕁒ʂ̐F�ŕ\������
//    sprintf(str, "20%02x/%02x/%02x(%s)", TmpTime[6], TmpTime[5], TmpTime[4], WeekDays[TmpTime[3]]);
    sprintf(str, "20%02x/%02x/%02x(%c)", TmpTime[6], TmpTime[5], TmpTime[4], 0x80+TmpTime[3]);
    display_drawChars(RDate[DisplayMode].x, RDate[DisplayMode].y, str, WHITE, BLACK, RDate[DisplayMode].font);

    if (mode == SettingYear) {
        sprintf(str, "20%02x", TmpTime[6]);
        display_drawChars(RYear[DisplayMode].x, RYear[DisplayMode].y, str, RED, BLACK, RDate[DisplayMode].font);
    }
    else if (mode == SettingMonth) {
        sprintf(str, "%02x", TmpTime[5]);
        display_drawChars(RMonth[DisplayMode].x, RMonth[DisplayMode].y, str, RED, BLACK, RDate[DisplayMode].font);
    } 
    else if (mode ==SettingDay) {
        sprintf(str, "%02x", TmpTime[4]);
        display_drawChars(RDay[DisplayMode].x, RDay[DisplayMode].y, str, RED, BLACK, RDate[DisplayMode].font);
    }
    AlarmSoundOff();
}

const char ButtonName[][8] = {
    "Y", "M", "D", "T", "Cancel", "OK", 
};

/*
 * �^�b�`�������W��������āA������i�߂�
 */
uint8_t ButtonCheck(uint16_t x, uint16_t y) {
    uint8_t jj, kk;
    uint8_t mode = Mode;

    //�ǂ̃{�^���������ꂽ���`�F�b�N    
    for (jj = 0; jj< 6; jj++) {
        if (ButtonPush(Test_x, Test_y, jj)) {
            //�ȉ��̏����Œ�`���Ă��邱�Ƃ�O��Ƀv���O����
            //#define SettingYear      0x11
            //#define SettingMonth     0x12
            //#define SettingDay       0x13
            //#define SettingTime      0x14
            //#define SettingCancel    0x15
            //#define SettingOK        0x16
            mode = SettingYear + jj;
            for (kk = 0; kk < 3; kk++) preDateTime[kk] = 0xff;
            DrawSetBox(mode);
            break;
        }
    }
    
    //�^�b�`���邲�ƂɕύX����Ώۂ�ς���  ���o�[�W����
/*
 *     Mode++;
    if (Mode > SettingTime) Mode = SettingYear;
    if ((Mode == SettingYear) || (Mode == SettingTime)) {
        for (jj = 0; jj < 3; jj++) preDateTime[jj] = 0xff;
    }
 */
    return mode;
}
        
/*
 * �ݒ�̏���
 * �N�����A����
 * �A���[�������͂����ł͕ύX���Ȃ�
 */
void SettingProc() {
    char str[100];
    int8_t delta, jj;
    uint8_t SetOK = 0;
    int8_t yy, mo, dd, mm, hh;
    uint8_t y, m, d;
    static uint8_t changeTime;

    if (Mode == Setting) {
        //���̃��[�h�Ɉڍs�������ɏ���������
        lcd_fill(BLACK); //��ʂ��N���A(�^����)

        Mode = SettingTime;
        for (jj = 0; jj < 7; jj++) TmpTime[jj] = DateTime[jj];
        changeTime = 0;
        //�������Ă����Ȃ��ƕ`�悳��Ȃ�
        for (jj = 0; jj < 3; jj++) preDateTime[jj] = 0xff;
        //��ʏ�������
        DrawSetBox(Mode);

        //�{�^���`��  Cancel��OK
        for (jj=BtnCancel; jj<=BtnOK; jj++) {
            display_drawRoundRect(ButtonObj3[jj].x, ButtonObj3[jj].y, ButtonObj3[jj].xw, ButtonObj3[jj].yw, 10, WHITE);
            //�{�^���̒����Ƀe�L�X�g�\��
            uint16_t xp = ButtonObj3[jj].x + ButtonObj3[jj].xw/2 - strlen(ButtonName[jj])*4*(jj-3);
            uint16_t yp = ButtonObj3[jj].y + ButtonObj3[jj].yw/2 - 4*(jj-3);
//            uint16_t xp = ButtonObj3[jj].x+10;//�������ߖ�̂��߁A�v�Z�ȗ���
//            uint16_t yp = ButtonObj3[jj].y+10;//
            display_drawChars(xp, yp, (char *) ButtonName[jj], WHITE, BLACK, jj-3);
        }
        
    }

    //�^�b�`�������̏���
    if (TouchStatus == 2) {
        TouchStatus++;
//        display_drawChars(250, 160, "Touch ON", WHITE, BLACK, 1);
        GetTouchLocation(&TouchX, &TouchY);
        TransCoordination(TouchX, TouchY, &Test_x, &Test_y);
#ifdef DEBUG
        sprintf(str, "Touch=(%d, %d) (%5d, %5d)", TouchX, TouchY, Test_x, Test_y);
        display_drawChars(0, 150, str, WHITE, BLACK, 1);
        display_drawRoundRect(Test_x, Test_y, 3, 3, 3, WHITE);
#endif

        //�^�b�`�����ꏊ�Ƀ{�^�������邩�`�F�b�N���āA���������s
        Mode = ButtonCheck(Test_x, Test_y);

        switch (Mode) {
            case SettingCancel:
                Mode = NormalInit;
                return;

            case SettingOK:
                for (jj = 0; jj < 7; jj++) DateTime[jj] = TmpTime[jj];
                DateTime[0] = 0x00;     //�b��00�Ƃ���
                if (changeTime) RTC_setTime(DateTime);  //�����ύX���̂�
                RTC_setDate(DateTime);
                //���t��ύX������AEEPROM�ɏ�������
                WriteYMD();
                Mode = NormalInit;
                break;
        }
        
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
            mm = (int8_t)Bcd2Hex(TmpTime[1]);
            hh = (int8_t)Bcd2Hex(TmpTime[2]);
            IncDecTime(delta, &hh, &mm);
            TmpTime[1] = Hex2Bcd(mm);
            TmpTime[2] = Hex2Bcd(hh);

            drawTime(DisplayMode, TmpTime, RED);
//            DispTime(HHMM, TmpTime, RTime[DisplayMode].x, RTime[DisplayMode].y, RTime[DisplayMode].font, RED);
            changeTime = 1; //������ύX������1
        } else if (Mode >= SettingYear && Mode <= SettingDay) {
            yy = Bcd2Hex(TmpTime[6]);
            mo = Bcd2Hex(TmpTime[5]);
            dd = Bcd2Hex(TmpTime[4]);
            
            if (Mode == SettingYear) {
                yy = (yy + delta -1 + 99) % 99 +1;  // 1-99�N�܂�
            } else if (Mode == SettingMonth) {
                mo = (mo + delta -1 + 12) % 12 +1;
            } else if (Mode == SettingDay) {
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
        drawAlarmTime(DisplayMode, AlarmTime);
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
        drawAlarmTime(DisplayMode, AlarmTime);
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
    display_SetFont(font);
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
    int8_t result;

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
                for (kk=0; kk<2; kk++) {
                    do {
                        result = GetTouchLocation(&T_x1, &T_y1);
                    } while (result == -1);
                }
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
                do {
                    for (kk=0; kk<2; kk++) {
                        do {
                            result = GetTouchLocation(&T_x2, &T_y2);
                        } while (result == -1);
                    }
                    //�^�b�`���Ă���ʒu�����������Ǝv����ꍇ��r��
                    if (T_x1 > T_x2) dx = T_x1 - T_x2;
                    else dx = T_x2 - T_x1;
                    if (T_y1 > T_y2) dy = T_y1 - T_y2;
                    else dy = T_y2 - T_y1;
                } while ((dx < 1000) || (dy < 1000));
#ifdef DEBUG
                sprintf(str, "P1=(%d, %d) P2=(%d, %d)", T_x1, T_y1, T_x2, T_y2);
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
                         Main application
 */
void main(void) {
    char str[100];
    uint8_t jj;
    uint16_t x, y;

    // Initialize the device
    SYSTEM_Initialize();

    //�A���[���̏�����
    AlarmInitialize();  //�A���[������TMR0�̏����ݒ�

    //�����́A�悭�킩��Ȃ����ASYSTEM_Initialize���AIOCB���L��������A��̃n���h���̏�Ԃ��ƁA�Ȃ���
    //�n���O����B����InterruptEnable�̑O�ɂ��Ȃ��ƃn���O����B���ꂪ�킩�炸�����Y��
    //IOCB4=0�ɂ��邩�A�n���h���ݒ�(���g���Ȃ��ƃf�t�H���g�Ɠ������_��)������K�v����
//    IOCBbits.IOCB4 = 0;
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
    GetGPS = 0;

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
    lcd_fill(BLACK); //��ʂ��N���A(�^����)

    //�N����
    AlarmSoundOn(0);
    __delay_ms(100);
    AlarmSoundOff();
    __delay_ms(200);
    AlarmSoundOn(0);
    __delay_ms(300);
    AlarmSoundOff();
    
    display_SetFont(font);  //�����t�H���g�ݒ�
    //���߂ċN�������Ƃ��́A�^�b�`�̒��������{���A���̃f�[�^��EEPROM�ɕێ�
    if (DATAEE_ReadByte(AddressInit) == 0xff) {
        TouchAdjust();
        lcd_fill(BLUE); //��ʂ��N���A
        /*
         //�^�b�`������ɂł��Ă��邩����m�F�p
            while (1) {
                uint16_t x, y, dx, dy;
                if (TouchStatus != 0) {
                    //�`�悵�Ă݂�
                    if (GetTouchLocation(&x, &y) != -1) {
                        TransCoordination(x, y, &dx, &dy);
                        lcd_draw_pixel_at(dx, dy, RED);
    sprintf(str, "P1=(%4d, %4d) P2=(%3d, %3d)", x, y, dx, dy);
    display_drawChars(0, 130, str, WHITE, BLACK, 1);
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

    //�A���[�������̕\���F
    AlarmColor[0] = 0x4208;
    AlarmColor[1] = WHITE;   //0: SlideSW off���̐F�A1: on���̐F

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
    //2021/6/19 ���̏�Ԃł́A���荞�݂��֎~����K�v���Ȃ����AIOCB4=0���s�v��Open�܂Ŗ��Ȃ�
    //f_write�����s����ƁA��ʂ��^�����ɂȂ�
    //f_write�����ɁAClose���Ă��A��ʂ��^������
    //���ǁASD�J�[�h�ւ̏������ݎ��͖̂��Ȃ��ł���̂ŁASD�J�[�h�֌W�̏����́A���̂܂�
    //LCD�֘A�̏����ɉ��炩�̖�肪���邩�ASD�J�[�h�ƃC���^�t�F�[�X���L���̉e�����o�Ă��܂��Ă�����̂Ɖ���
    //SD�J�[�h���̔z�����኱�����̂ŁA�ؑւ�������ł��A���炩�̃o�X�t�@�C�g�������Ƃ����z�肳���B
    
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
//    glcd_init2();   // Sleep Out�����ŁA�^�����ȉ�ʂ͌��ɖ߂���
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
        if (GetGPS == 2) {
            //�f�[�^���`�F�b�N���āARTC��ݒ�
            if (GPRMCdatacheck()) {
                GetGPS = 1;
                BufferP = Buffer;   //�����l�Ƃ��āABuffer�̐擪�ɐݒ�
            }
            else {
                GetGPS = 0; //�f�[�^�擾�ł�����0��
            }
        }

    }
}
/**
 End of File
*/