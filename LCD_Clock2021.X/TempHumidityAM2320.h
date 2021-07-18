/* 
 * File:   TempHumidityAM2320.h
 * Author: K.Ohno
 *
 * Created on December 27, 2020, 2:17 PM
 */

#ifndef TEMPHUMIDITYAM2320_H
#define	TEMPHUMIDITYAM2320_H

#ifdef	__cplusplus
extern "C" {
#endif

    
#define AM2320  0x5c    //7bit����LSB�ɂ���R/W�r�b�g���������A�h���X(7bit��)
    //���ۂɎg�p���鎞�́ALSB��R/W���t�������̂ŁAWrite=0xB8�ARead=0xB9

// �Z���T���牷���x�ǂݏo���A���̒l��Ԃ�
void get_tempHumidity(int16_t *temp, int16_t *humidity);


#ifdef	__cplusplus
}
#endif

#endif	/* TEMPHUMIDITYAM2320_H */

