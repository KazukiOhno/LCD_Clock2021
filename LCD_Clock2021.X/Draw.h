/* 
 * File:   Draw.h
 * Author: K.Ohno
 *
 * Created on December 20, 2020, 3:50 PM
 */

#ifndef DRAW_H
#define	DRAW_H

#ifdef	__cplusplus
extern "C" {
#endif


void Dispseg(uint16_t x, uint16_t y, uint8_t num, uint8_t size, uint16_t color);
void DispTime(uint8_t mode, uint8_t * datetime, uint16_t x, uint16_t y, uint8_t size, uint16_t color);
void DrawAnalogClock(uint8_t mode, uint8_t * datetime, uint16_t xx, uint16_t yy, uint8_t size, uint16_t color, uint8_t *alarmtime);
void DrawBigCalendar(uint8_t year, uint8_t month, uint16_t xs, uint16_t ys);
void DrawSmallCalendar(uint8_t year, uint8_t month, uint16_t xs, uint16_t ys);
void Draw3month(uint8_t startyear, uint8_t startmonth, uint8_t mode);

void drawAlarmTime(uint8_t mode, uint8_t *alarmtime, uint8_t sw);
void drawTime(uint8_t mode, uint8_t * datetime, uint16_t color);
void drawDateTime(uint8_t mode, uint8_t *datetime);
void drawTempHumidity(uint8_t mode, int16_t temp, int16_t humidity);
void drawCalendar(uint8_t mode);

#ifdef	__cplusplus
}
#endif

#endif	/* DRAW_H */

