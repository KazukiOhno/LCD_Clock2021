/* 
 * File:   font.h
 * Author: K.Ohno
 *
 * Created on December 19, 2020, 5:19 PM
 */

#ifndef FONT_H
#define	FONT_H

#ifdef	__cplusplus
extern "C" {
#endif

/****************************************************************************/
/* 対象マイコン R8C/M12A                                                    */
/* ﾌｧｲﾙ内容     グラフィック液晶制御ライブラリ用フォントデータ              */
/* バージョン   Ver.1.00                                                    */
/* Date         2012.04.27                                                  */
/* Copyright    ルネサスマイコンカーラリー事務局                            */
/****************************************************************************/

/*
本キャラクタデータは、
8×8 ドット日本語フォント「美咲フォント」
http://www.geocities.jp/littlimi/font.htm
を16進数データに変換して使用しています。
フリーで提供されている門真なむ様にはお礼申し上げます。
→自作のフォントに変更
*/

    
// グラフィックLCD用 1バイトキャラクタデータ　8x8ドット
// $20-$7Fまで定義
// bmpファイルから、perl bmp2txt.pl [bitmap.bmp] で生成する
// そこから手修正。絵のデータは、部品在庫.xlsxに。
// 2021/3現在有効なフォントは、Font.xlsxのPIC clock2020のシートからコピーしたもの
const uint8_t NormalFont[] = {
    8, 8, 8, 10, ' ', 0, 0, 0,   //先頭の8バイトはx,ysize, x,ypitch, データ先頭の文字を指定
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // '　' 0x20
    0x00, 0x00, 0x6F, 0x6F, 0x03, 0x00, 0x00, 0x00, // '!'
    0x00, 0x06, 0x03, 0x01, 0x06, 0x03, 0x01, 0x00, // '"'
    0x10, 0x74, 0x1C, 0x77, 0x1C, 0x17, 0x04, 0x00, // '#'
    0x04, 0x2E, 0x2A, 0x7F, 0x2A, 0x3A, 0x10, 0x00, // '$' 0x24
    0x47, 0x65, 0x37, 0x08, 0x76, 0x53, 0x71, 0x00, // '%'
    0x30, 0x7A, 0x4F, 0x55, 0x66, 0x38, 0x48, 0x00, // '&'
    0x00, 0x06, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, // '''
    0x00, 0x00, 0x00, 0x3E, 0x7F, 0x41, 0x00, 0x00, // '(' 0x28
    0x00, 0x00, 0x41, 0x7F, 0x3E, 0x00, 0x00, 0x00, // ')'
//    0x2A, 0x2A, 0x1C, 0x1C, 0x2A, 0x2A, 0x00, 0x00, // '*'
    0x00, 0x22, 0x14, 0x3E, 0x14, 0x22, 0x00, 0x00, // '*'
    0x08, 0x08, 0x3E, 0x3E, 0x08, 0x08, 0x00, 0x00, // '+'
    0x00, 0xB0, 0xF0, 0x70, 0x00, 0x00, 0x00, 0x00, // ',' 0x2c
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, // '-'
    0x00, 0x00, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, // '.'
    0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x00, 0x00, // '/'

//    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ' ' 0x20
//    0x00, 0x00, 0x2f, 0x2f, 0x03, 0x00, 0x00, 0x00, // '!'
//    0x00, 0x07, 0x03, 0x00, 0x07, 0x03, 0x00, 0x00, // '"'
//    0x08, 0x3a, 0x3f, 0x0b, 0x3a, 0x3f, 0x0b, 0x02, // '#'
//    0x04, 0x2e, 0x2a, 0x7f, 0x2a, 0x3a, 0x10, 0x00, // '$' 0x24
//    0x42, 0x25, 0x12, 0x08, 0x24, 0x52, 0x21, 0x00, // '%'
//    0x18, 0x3a, 0x27, 0x2d, 0x16, 0x18, 0x24, 0x00, // '&'
//    0x00, 0x06, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, // '''
//    0x00, 0x00, 0x00, 0x3e, 0x7f, 0x41, 0x00, 0x00, // '(' 0x28
//    0x00, 0x00, 0x41, 0x7f, 0x3e, 0x00, 0x00, 0x00, // ')'
//    0x15, 0x15, 0x0e, 0x0e, 0x15, 0x15, 0x00, 0x00, // '*'
//    0x04, 0x04, 0x1f, 0x1f, 0x04, 0x04, 0x00, 0x00, // '+'
//    0x00, 0x58, 0x78, 0x38, 0x00, 0x00, 0x00, 0x00, // ',' 0x2c
//    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x00, // '-'
//    0x00, 0x00, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, // '.'
//    0x00, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x00, // '/'

    0x00, 0x3E, 0x7F, 0x41, 0x41, 0x7F, 0x3E, 0x00, // '0' 0x30
    0x00, 0x00, 0x02, 0x7F, 0x7F, 0x00, 0x00, 0x00, // '1'
    0x00, 0x62, 0x73, 0x59, 0x49, 0x4F, 0x46, 0x00, // '2'
    0x00, 0x41, 0x49, 0x49, 0x49, 0x7F, 0x36, 0x00, // '3'
    0x00, 0x1F, 0x10, 0x10, 0x7F, 0x7F, 0x10, 0x00, // '4' 0x34
    0x00, 0x2F, 0x6F, 0x45, 0x45, 0x7D, 0x39, 0x00, // '5'
    0x00, 0x3C, 0x7E, 0x4B, 0x49, 0x79, 0x30, 0x00, // '6'
    0x00, 0x01, 0x01, 0x71, 0x7D, 0x0F, 0x03, 0x00, // '7'
    0x00, 0x36, 0x7F, 0x49, 0x49, 0x7F, 0x36, 0x00, // '8' 0x38
    0x00, 0x06, 0x4F, 0x49, 0x69, 0x3F, 0x1E, 0x00, // '9'
    0x00, 0x00, 0x00, 0x36, 0x36, 0x00, 0x00, 0x00, // ':'
    0x00, 0x00, 0x00, 0xB6, 0x76, 0x00, 0x00, 0x00, // ';'
    0x00, 0x08, 0x1C, 0x36, 0x63, 0x41, 0x00, 0x00, // '<' 0x3c
    0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x00, // '='
    0x41, 0x63, 0x36, 0x1C, 0x08, 0x00, 0x00, 0x00, // '>'
    0x06, 0x07, 0x51, 0x59, 0x09, 0x0F, 0x06, 0x00, // '?'
    
    0x3E, 0x63, 0x59, 0x65, 0x45, 0x7F, 0x3E, 0x00, // '@' 0x40
    0x7C, 0x7E, 0x13, 0x11, 0x13, 0x7E, 0x7C, 0x00, // 'A'
    0x7F, 0x7F, 0x49, 0x49, 0x49, 0x7F, 0x36, 0x00, // 'B'
    0x3E, 0x7F, 0x63, 0x41, 0x41, 0x41, 0x41, 0x00, // 'C'
    0x7F, 0x7F, 0x41, 0x41, 0x41, 0x7F, 0x3E, 0x00, // 'D' 0x44
    0x7F, 0x7F, 0x49, 0x49, 0x49, 0x49, 0x41, 0x00, // 'E'
    0x7F, 0x7F, 0x09, 0x09, 0x09, 0x09, 0x01, 0x00, // 'F'
    0x3E, 0x7F, 0x41, 0x49, 0x49, 0x79, 0x7A, 0x00, // 'G'
    0x7F, 0x7F, 0x08, 0x08, 0x08, 0x7F, 0x7F, 0x00, // 'H' 0x48
    0x00, 0x00, 0x41, 0x7F, 0x7F, 0x41, 0x00, 0x00, // 'I'
    0x20, 0x40, 0x41, 0x7F, 0x3F, 0x01, 0x00, 0x00, // 'J'
    0x7F, 0x7F, 0x08, 0x1C, 0x36, 0x63, 0x41, 0x00, // 'K'
    0x7F, 0x7F, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, // 'L' 0x4c
    0x7F, 0x7F, 0x06, 0x1C, 0x06, 0x7F, 0x7F, 0x00, // 'M'
    0x7F, 0x7F, 0x06, 0x08, 0x30, 0x7F, 0x7F, 0x00, // 'N'
    0x3E, 0x7F, 0x41, 0x41, 0x41, 0x7F, 0x3E, 0x00, // 'O'
    0x7F, 0x7F, 0x09, 0x09, 0x09, 0x0F, 0x06, 0x00, // 'P' 0x50
    0x3E, 0x7F, 0x41, 0x51, 0x21, 0x7F, 0x5E, 0x00, // 'Q'
    0x7F, 0x7F, 0x09, 0x19, 0x39, 0x6F, 0x46, 0x00, // 'R'
    0x46, 0x4F, 0x49, 0x49, 0x49, 0x79, 0x31, 0x00, // 'S'
    0x00, 0x01, 0x01, 0x7F, 0x7F, 0x01, 0x01, 0x00, // 'T' 0x54
    0x3F, 0x7F, 0x40, 0x40, 0x40, 0x7F, 0x3F, 0x00, // 'U'
    0x03, 0x0E, 0x38, 0x60, 0x38, 0x0E, 0x03, 0x00, // 'V'
    0x1F, 0x78, 0x70, 0x1E, 0x70, 0x78, 0x1F, 0x00, // 'W'
    0x63, 0x77, 0x1C, 0x08, 0x1C, 0x77, 0x63, 0x00, // 'X' 0x58
    0x03, 0x06, 0x0C, 0x78, 0x0C, 0x06, 0x03, 0x00, // 'Y'
    0x61, 0x71, 0x79, 0x5D, 0x4F, 0x47, 0x43, 0x00, // 'Z'
    0x7F, 0x7F, 0x41, 0x41, 0x41, 0x00, 0x00, 0x00, // '['
    0x15, 0x16, 0x7C, 0x7C, 0x16, 0x15, 0x00, 0x00, // '\' 0x5c
    0x00, 0x41, 0x41, 0x41, 0x7F, 0x7F, 0x00, 0x00, // ']'
    0x04, 0x06, 0x03, 0x03, 0x06, 0x04, 0x00, 0x00, // '^'
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, // '_'

//    0x1e, 0x33, 0x29, 0x35, 0x35, 0x3f, 0x1e, 0x00, // '@' 0x40
//    0x3c, 0x3e, 0x0b, 0x09, 0x0b, 0x3e, 0x3c, 0x00, // 'A'
//    0x3f, 0x3f, 0x25, 0x25, 0x3f, 0x3f, 0x1a, 0x00, // 'B'
//    0x1e, 0x3f, 0x33, 0x21, 0x21, 0x21, 0x21, 0x00, // 'C'
//    0x3f, 0x3f, 0x21, 0x21, 0x21, 0x3f, 0x1e, 0x00, // 'D' 0x44
//    0x3f, 0x3f, 0x25, 0x25, 0x25, 0x21, 0x21, 0x00, // 'E'
//    0x3f, 0x3f, 0x05, 0x05, 0x05, 0x01, 0x01, 0x00, // 'F'
//    0x1e, 0x3f, 0x21, 0x29, 0x29, 0x3b, 0x3a, 0x00, // 'G'
//    0x3f, 0x3f, 0x04, 0x04, 0x04, 0x3f, 0x3f, 0x00, // 'H' 0x48
//    0x00, 0x00, 0x21, 0x3f, 0x3f, 0x21, 0x00, 0x00, // 'I'
//    0x18, 0x38, 0x20, 0x21, 0x3f, 0x1f, 0x01, 0x00, // 'J'
//    0x3f, 0x3f, 0x04, 0x0e, 0x1a, 0x33, 0x21, 0x00, // 'K'
//    0x3f, 0x3f, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, // 'L' 0x4c
//    0x3f, 0x3f, 0x06, 0x0c, 0x06, 0x3f, 0x3f, 0x00, // 'M'
//    0x3f, 0x3f, 0x06, 0x0c, 0x18, 0x3f, 0x3f, 0x00, // 'N'
//    0x1e, 0x3f, 0x21, 0x21, 0x21, 0x3f, 0x1e, 0x00, // 'O'
//    0x3f, 0x3f, 0x09, 0x09, 0x09, 0x0f, 0x06, 0x00, // 'P' 0x50
//    0x1e, 0x3f, 0x29, 0x29, 0x11, 0x3f, 0x2e, 0x00, // 'Q'
//    0x3f, 0x3f, 0x05, 0x0d, 0x1f, 0x37, 0x22, 0x00, // 'R'
//    0x12, 0x37, 0x27, 0x25, 0x3d, 0x3b, 0x1a, 0x00, // 'S'
//    0x01, 0x01, 0x3f, 0x3f, 0x01, 0x01, 0x00, 0x00, // 'T' 0x54
//    0x1f, 0x3f, 0x20, 0x20, 0x20, 0x3f, 0x1f, 0x00, // 'U'
//    0x03, 0x07, 0x1c, 0x30, 0x1c, 0x07, 0x03, 0x00, // 'V'
//    0x07, 0x3f, 0x18, 0x0e, 0x18, 0x3f, 0x07, 0x00, // 'W'
//    0x21, 0x33, 0x1e, 0x0c, 0x1e, 0x33, 0x21, 0x00, // 'X' 0x58
//    0x03, 0x07, 0x3c, 0x3c, 0x07, 0x03, 0x00, 0x00, // 'Y'
//    0x21, 0x31, 0x39, 0x2d, 0x27, 0x23, 0x21, 0x00, // 'Z'
//    0x3f, 0x3f, 0x21, 0x21, 0x21, 0x00, 0x00, 0x00, // '['
//    0x0d, 0x0f, 0x3c, 0x3c, 0x0f, 0x0d, 0x00, 0x00, // '\' 0x5c
//    0x00, 0x21, 0x21, 0x21, 0x3f, 0x3f, 0x00, 0x00, // ']'
//    0x04, 0x06, 0x03, 0x03, 0x06, 0x04, 0x00, 0x00, // '^'
//    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, // '_'

    0x00, 0x01, 0x03, 0x06, 0x00, 0x00, 0x00, 0x00, // '`' 0x60
    0x38, 0x7C, 0x44, 0x24, 0x7C, 0x7C, 0x00, 0x00, // 'a'
    0x7F, 0x7F, 0x48, 0x48, 0x78, 0x30, 0x00, 0x00, // 'b'
    0x38, 0x7C, 0x44, 0x44, 0x44, 0x44, 0x00, 0x00, // 'c'
    0x30, 0x78, 0x48, 0x48, 0x7F, 0x7F, 0x00, 0x00, // 'd' 0x64
    0x38, 0x7C, 0x54, 0x54, 0x54, 0x58, 0x00, 0x00, // 'e'
    0x08, 0x08, 0x7E, 0x7F, 0x09, 0x09, 0x00, 0x00, // 'f'
    0x18, 0xBC, 0xA4, 0xA4, 0xFC, 0x7C, 0x00, 0x00, // 'g'
    0x7F, 0x7F, 0x08, 0x08, 0x78, 0x70, 0x00, 0x00, // 'h' 0x68
    0x00, 0x00, 0x7A, 0x7A, 0x00, 0x00, 0x00, 0x00, // 'i'
    0x80, 0x80, 0xFA, 0x7A, 0x00, 0x00, 0x00, 0x00, // 'j'
    0x7F, 0x7F, 0x10, 0x38, 0x6C, 0x44, 0x00, 0x00, // 'k'
    0x00, 0x00, 0x7F, 0x7F, 0x00, 0x00, 0x00, 0x00, // 'l' 0x6c
    0x7C, 0x7C, 0x0C, 0x78, 0x0C, 0x7C, 0x78, 0x00, // 'm'
    0x7C, 0x7C, 0x04, 0x04, 0x7C, 0x78, 0x00, 0x00, // 'n'
    0x38, 0x7C, 0x44, 0x44, 0x7C, 0x38, 0x00, 0x00, // 'o'
    0xFC, 0xFC, 0x24, 0x24, 0x3C, 0x18, 0x00, 0x00, // 'p' 0x70
    0x18, 0x3C, 0x24, 0x24, 0xFC, 0xFC, 0x00, 0x00, // 'q'
    0x7C, 0x7C, 0x18, 0x0C, 0x0C, 0x0C, 0x00, 0x00, // 'r'
    0x48, 0x5C, 0x54, 0x54, 0x74, 0x24, 0x00, 0x00, // 's'
    0x00, 0x08, 0x3C, 0x7C, 0x48, 0x00, 0x00, 0x00, // 't' 0x74
    0x3C, 0x7C, 0x40, 0x40, 0x7C, 0x7C, 0x00, 0x00, // 'u'
    0x0C, 0x38, 0x60, 0x30, 0x0C, 0x00, 0x00, 0x00, // 'v'
    0x3C, 0x70, 0x3C, 0x70, 0x3C, 0x00, 0x00, 0x00, // 'w'
    0x44, 0x6C, 0x38, 0x38, 0x6C, 0x44, 0x00, 0x00, // 'x' 0x78
    0x0C, 0x18, 0x70, 0x18, 0x0C, 0x00, 0x00, 0x00, // 'y'
    0x44, 0x64, 0x74, 0x5C, 0x4C, 0x44, 0x00, 0x00, // 'z'
    0x08, 0x1C, 0x77, 0x41, 0x41, 0x00, 0x00, 0x00, // '{'
    0x00, 0x00, 0x7F, 0x7F, 0x00, 0x00, 0x00, 0x00, // '|' 0x7c
    0x00, 0x41, 0x41, 0x77, 0x1C, 0x08, 0x00, 0x00, // '}'
    0x0C, 0x06, 0x06, 0x04, 0x0C, 0x0C, 0x06, 0x00, // '~'
    0x7F, 0x41, 0x41, 0x41, 0x41, 0x41, 0x7F, 0x00, // '□'

//    0x00, 0x01, 0x03, 0x06, 0x00, 0x00, 0x00, 0x00, // '`' 0x60
//    0x18, 0x3c, 0x24, 0x14, 0x3c, 0x3c, 0x00, 0x00, // 'a'
//    0x3f, 0x3f, 0x24, 0x24, 0x3c, 0x18, 0x00, 0x00, // 'b'
//    0x18, 0x3c, 0x3c, 0x24, 0x24, 0x24, 0x00, 0x00, // 'c'
//    0x18, 0x3c, 0x24, 0x24, 0x3f, 0x3f, 0x00, 0x00, // 'd' 0x64
//    0x1c, 0x3e, 0x3e, 0x2a, 0x2a, 0x2e, 0x04, 0x00, // 'e'
//    0x04, 0x04, 0x3e, 0x3f, 0x05, 0x05, 0x00, 0x00, // 'f'
//    0x18, 0xbc, 0xa4, 0xa4, 0xfc, 0x7c, 0x00, 0x00, // 'g'
//    0x3f, 0x3f, 0x04, 0x04, 0x3c, 0x38, 0x00, 0x00, // 'h' 0x68
//    0x00, 0x00, 0x3d, 0x3d, 0x00, 0x00, 0x00, 0x00, // 'i'
//    0x00, 0x80, 0x80, 0xfd, 0x7d, 0x00, 0x00, 0x00, // 'j'
//    0x3f, 0x3f, 0x08, 0x1c, 0x36, 0x22, 0x00, 0x00, // 'k'
//    0x00, 0x00, 0x3f, 0x3f, 0x00, 0x00, 0x00, 0x00, // 'l' 0x6c
//    0x3c, 0x3c, 0x0c, 0x38, 0x0c, 0x3c, 0x38, 0x00, // 'm'
//    0x3c, 0x3c, 0x04, 0x04, 0x3c, 0x38, 0x00, 0x00, // 'n'
//    0x18, 0x3c, 0x24, 0x24, 0x3c, 0x18, 0x00, 0x00, // 'o'
//    0xfc, 0xfc, 0x24, 0x24, 0x3c, 0x18, 0x00, 0x00, // 'p' 0x70
//    0x18, 0x3c, 0x24, 0x24, 0xfc, 0xfc, 0x00, 0x00, // 'q'
//    0x3c, 0x3c, 0x08, 0x04, 0x04, 0x04, 0x00, 0x00, // 'r'
//    0x24, 0x2e, 0x2e, 0x3a, 0x3a, 0x12, 0x00, 0x00, // 's'
//    0x04, 0x04, 0x1e, 0x3e, 0x24, 0x24, 0x00, 0x00, // 't' 0x74
//    0x1c, 0x3c, 0x20, 0x10, 0x3c, 0x3c, 0x00, 0x00, // 'u'
//    0x04, 0x0c, 0x18, 0x30, 0x18, 0x0c, 0x04, 0x00, // 'v'
//    0x0c, 0x3c, 0x30, 0x0c, 0x30, 0x3c, 0x0c, 0x00, // 'w'
//    0x24, 0x2c, 0x3c, 0x18, 0x3c, 0x34, 0x24, 0x00, // 'x' 0x78
//    0x84, 0x8c, 0xd8, 0x70, 0x38, 0x1c, 0x0c, 0x00, // 'y'
//    0x24, 0x34, 0x34, 0x2c, 0x2c, 0x24, 0x00, 0x00, // 'z'
//    0x0c, 0x0c, 0x3f, 0x33, 0x21, 0x21, 0x00, 0x00, // '{'
//    0x00, 0x00, 0x3f, 0x3f, 0x00, 0x00, 0x00, 0x00, // '|' 0x7c
//    0x21, 0x21, 0x33, 0x3f, 0x0c, 0x0c, 0x00, 0x00, // '}'
//    0x0c, 0x06, 0x06, 0x04, 0x0c, 0x0c, 0x06, 0x00, // '~'
//    0x3f, 0x21, 0x21, 0x21, 0x21, 0x21, 0x3f, 0x00, // '□'

    0x00, 0x7F, 0x49, 0x49, 0x49, 0x7F, 0x00, 0x00, // '日' 0x80
    0x40, 0x3F, 0x15, 0x15, 0x55, 0x7F, 0x00, 0x00, // '月'
    0x00, 0x46, 0x30, 0x0F, 0x30, 0x46, 0x00, 0x00, // '火'
    0x24, 0x14, 0x0C, 0x7F, 0x0C, 0x12, 0x21, 0x00, // '水'
    0x22, 0x12, 0x0A, 0x7F, 0x0A, 0x12, 0x22, 0x00, // '木'
    0x54, 0x72, 0x55, 0x7D, 0x55, 0x72, 0x54, 0x00, // '金'
    0x40, 0x44, 0x44, 0x7F, 0x44, 0x44, 0x40, 0x00, // '土'
    0x02, 0x05, 0x02, 0x3C, 0x42, 0x42, 0x42, 0x00, // '℃'
    
};


//カレンダー用　6x8ドットフォント
const uint8_t tinyfont[] = {
    6, 8, 6, 8, '0', 0, 0, 0,   //先頭の8バイトはx,ysize, データ先頭の文字を指定
    0x3e,0x7f,0x41,0x7f,0x3e,0x00,    // 0
    0x00,0x02,0x7f,0x7f,0x00,0x00,    // 1
    0x66,0x73,0x59,0x4f,0x46,0x00,    // 2
    0x22,0x6b,0x49,0x7f,0x36,0x00,    // 3
    0x1c,0x13,0x7f,0x7f,0x10,0x00,    // 4
    0x2f,0x47,0x45,0x7d,0x39,0x00,    // 5
    0x38,0x7e,0x4b,0x79,0x38,0x00,    // 6
    0x01,0x61,0x79,0x1f,0x07,0x00,    // 7
    0x36,0x7f,0x49,0x7f,0x36,0x00,    // 8
    0x06,0x4f,0x69,0x7f,0x1e,0x00,    // 9
    0x00,0x00,0x00,0x00,0x00,0x00,    // ' '
};

/* 各種フォントデータで文字表示させるために必要な情報
 * フォントデータのX,Y ドットサイズ
 * 表示する際のX,Yピッチ
 * フォントデータ:縦方向に上側LSB、下側MSBで、1バイトのデータ
 * 　　　　　　　横方向は、配列の要素として持っている
 * X方向のデータは柔軟に持てるが、プロポーショナルフォントにするような構成にはなっていない
 * Y方向のデータは1バイトとして定義しているので、8ドットmaxが現状
 */
//配列内の位置
#define Fxsize 0
#define Fysize 1
#define Fxpitch 2
#define Fypitch 3
#define Foffset 4

typedef struct {
    uint8_t xsize;     //フォントのXサイズ
    uint8_t ysize;     //フォントのYサイズ
    uint8_t xpitch;    //フォントのXピッチ
    uint8_t ypitch;    //フォントのYピッチ
    const uint8_t *font;    //フォントデータ配列へのポインタ
    char offset;        //フォント配列の最初の文字コード
} FontStruct;



#ifdef	__cplusplus
}
#endif

#endif	/* FONT_H */

