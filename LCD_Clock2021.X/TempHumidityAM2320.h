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

    
#define AM2320  0x5c    //7bit長のLSBにあるR/Wビットを除いたアドレス

// センサから温湿度読み出し、その値を返す
void get_tempHumidity(int16_t *temp, int16_t *humidity);


#ifdef	__cplusplus
}
#endif

#endif	/* TEMPHUMIDITYAM2320_H */

