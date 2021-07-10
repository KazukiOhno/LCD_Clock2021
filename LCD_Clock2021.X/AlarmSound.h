/* 
 * File:   AlarmSound.h
 * Author: K.Ohno
 *
 * Created on December 13, 2020, 2:29 PM
 */

#ifndef ALARMSOUND_H
#define	ALARMSOUND_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    
void AlarmSoundOn(uint8_t sn);
void AlarmSoundOff(void);
void AlarmInitialize(void);


#ifdef	__cplusplus
}
#endif

#endif	/* ALARMSOUND_H */

