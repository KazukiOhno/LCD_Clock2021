/*
 * File:   TempHumidityAM2320.c
 * Author: K.Ohno
 *
 * �����x�Z���T�FAM2320, �}0.5��, �}3%RH
 * 
 * I2C�ʐM�@�@�@100kHz����
 * I2C2��RTC�Ƌ��p�B�o�X��pullup�́ARTC���ɔC����
 *          _____________
 * VDD  1--|  �� �� ��   |
 * SDA  2--|  �� �� �� ��|
 * GND  3--|  �� �� �� ��|
 * SCL  4--|  �� �� ��   |
 *          -------------
 */

#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/examples/i2c2_master_example.h"
#include "TempHumidityAM2320.h"

/*
 * CRC�̌v�Z
 */
uint16_t crc16(uint8_t *ptr, int8_t len) {
    uint16_t crc = 0xFFFF;
    int8_t ii;

    while (len--) {
        crc ^= *ptr++;

        for (ii = 0; ii < 8; ii++) {
            if (crc & 0x01) {
                crc >>= 1;
                crc ^= 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}

/*****************************
 * �����x�����擾
 * ���ʂ́A�e10�{�ɂ��������Ƃ��āB
 * ��F25.6C=256
 *****************************/
void get_tempHumidity(int16_t *temp, int16_t *humidity) {
    uint8_t work[8];
    uint8_t data[] = { 0x03, 0x00, 0x04 };
    uint16_t error;
    uint16_t CRC;

    //�܂��Z���T���N�������߁A�_�~�[���[�h
    I2C2_Read1ByteRegister(AM2320, 0);
    __delay_ms(1);
    
    I2C2_WriteNBytes(AM2320, data, 3);
    __delay_ms(2);
    
    I2C2_ReadNBytes(AM2320, work, 8);
    
    //�G���[�`�F�b�N
    CRC = work[6] + (work[7]<<8);
    error = crc16(work, 6);
    if (CRC != error) {
        //        lcd_puts("Er");
    } else {
        //�@�G���[�łȂ��������X�V
        *temp = (work[4]<<8) + work[5]; //���ۂ̉��x�́A1/10
        *humidity = (work[2]<<8) + work[3]; //���ۂ̎��x�́A1/10
    }
    
}
