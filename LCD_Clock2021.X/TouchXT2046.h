/* 
 * File:   TouchXT2046.h
 * Author: K.Ohno
 *
 * Created on December 6, 2020, 11:15 AM
 */

#ifndef TOUCHXT2046_H
#define	TOUCHXT2046_H

#ifdef	__cplusplus
extern "C" {
#endif


//座標補正用　(20,20)  (320,220)のタッチ上の座標をT_x1, T_y1, T_x2, T_y2
uint16_t T_x1, T_y1, T_x2, T_y2;

void GetTouchRawXY(uint16_t *tx, uint16_t *ty);
int8_t GetTouchLocation(uint16_t *tx, uint16_t *ty);
void TransCoordination(uint16_t x, uint16_t y, uint16_t *xg, uint16_t *yg);



#ifdef	__cplusplus
}
#endif

#endif	/* TOUCHXT2046_H */

