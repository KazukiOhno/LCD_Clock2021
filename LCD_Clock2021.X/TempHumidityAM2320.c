/*
 * File:   TempHumidityAM2320.c
 * Author: K.Ohno
 *
 * 温湿度センサ：AM2320, ±0.5℃, ±3%RH
 * 
 * I2C通信　　　100kHz動作
 * I2C2をRTCと共用。バスのpullupは、RTC側に任せる
 *          _____________
 * VDD  1--|  □ □ □   |
 * SDA  2--|  □ □ □ □|
 * GND  3--|  □ □ □ □|
 * SCL  4--|  □ □ □   |
 *          -------------
 */

#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/examples/i2c2_master_example.h"
#include "TempHumidityAM2320.h"

/*
 * CRCの計算
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
 * 温湿度情報を取得
 * 結果は、各10倍にした整数として。
 * 例：25.6C=256
 *****************************/
void get_tempHumidity(int16_t *temp, int16_t *humidity) {
    uint8_t work[8];
    uint8_t data[] = { 0x03, 0x00, 0x04 };
    uint16_t error;
    uint16_t CRC;

    //まずセンサを起こすため、ダミーリード
    I2C2_Read1ByteRegister(AM2320, 0);
    __delay_ms(1);
    
    I2C2_WriteNBytes(AM2320, data, 3);
    __delay_ms(2);
    
    I2C2_ReadNBytes(AM2320, work, 8);
    
    //エラーチェック
    CRC = work[6] + (work[7]<<8);
    error = crc16(work, 6);
    if (CRC != error) {
        //        lcd_puts("Er");
    } else {
        //　エラーでない時だけ更新
        *temp = (work[4]<<8) + work[5]; //実際の温度は、1/10
        *humidity = (work[2]<<8) + work[3]; //実際の湿度は、1/10
    }
    
}
