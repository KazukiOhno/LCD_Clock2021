/* 
 * File:   LCD320x240color.h
 * Author: K.Ohno
 *
 * Created on December 19, 2020, 5:18 PM
 */

#ifndef LCD320X240COLOR_H
#define	LCD320X240COLOR_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "mcc_generated_files/mcc.h"
#include "font.h"

/*******************************************************************************
 * Usefull Makros for the LCD-Functions
 * ��������́APIC-Projekte (https://pic-projekte.de/blog/ili9341-bibliothek/)
 * ���񋟂��Ă���Ă���Github���玝���Ă�������
 * 
 * ILI9341�f�[�^�V�[�g�ɂ��ASCL=100ns min�Ƃ������ƂȂ̂ŁASPI��10MHz�܂ł���
 * �Ή��ł��Ȃ��B16MHz�̐ݒ�œ��삵�Ȃ��āA1���Y��
 */

// LCD�f�B�X�v���C�̃T�C�Y
#define LCD_WIDTH   320
#define LCD_HEIGHT  240	

#define WHITE       0xFFFF
#define SILVER      0xC618
#define BLACK       0x0000
#define GREY        0x8410
#define RED         0xF800
#define MAROON      0x8000
#define FUCHSIA     0xF81F		
#define PURPLE      0x8010
#define LIME        0x07E0
#define GREEN       0x0400
#define YELLOW      0xFFE0
#define OLIVE       0x8400
#define BLUE        0x001F
#define NAVY        0x0010
#define AQUA        0x07FF
#define TEAL        0x0410
#define DARK_BLUE   0x0002

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

// PIC18�́A���g���G���f�B�A��
//typedef union {
//    uint16_t w;
//    uint8_t b[2];
//    struct {
//        uint8_t lo;
//        uint8_t hi;
//    };
//} WORDBYTE;
/* ��L�̂悤��union���g���ƋL�q���Z���ł���̂ŁA�ǂ��Ǝv�������A
 * �v���O�����T�C�Y�͋t�ɑ傫���Ȃ����B
 * uint16�Ŏ󂯂āA>>8�Ƃ��A& 0xff�Ɖ��Z�����āA�g�������������v���O�����ɂȂ����̂ŁA
 * ���̕��@�͋p���Ƃ���@�@2020/11/21
 */
    
typedef union {
    uint16_t color16;
    struct {
        uint8_t lo;
        uint8_t hi;
    };
} COLOR;

FontStruct CurrentFont = {
  8,8,      //8x8 dot, normal font
  8,8,      //8x8 dot pitch
  NormalFont,      //�m�[�}���t�H���g
  0x20      //�ŏ��̕����R�[�h
};


//SPI���L�������邽�߁A�g�p���𖾊m��
/*
 * �Ŋ���write_command��write_data�̏���Open/Close�����̂͌���������
 * ���̏�̃��x���ŏ������邱�Ƃ���{�Ƃ������B
 * addset, lcd_set_cursor_x, y, draw_pixel, lcd_fill, 
 * drawHLine, drawVLine, lcd_fill_rect, glcd_init
 */

//������p�́Alcd_�Ƃ��̃w�b�_�Ȃ��@�@CS�̐ݒ�����Ȃ��Ɠ��삵�Ȃ�
//void glcd_SetFrontColor(uint16_t color);
//void glcd_SetBackColor(uint16_t color);
///* Set the cursor to the coordinate (x,_)
// * @param x
// */
//uint8_t lcd_set_cursor_x(uint16_t x);
//
///* Set the cursor to the coordinate (_,y)
// * @param y
// */
//uint8_t lcd_set_cursor_y(uint16_t y);
//
///**
// * Set the cursor at position (x,y)
// * @param x X-Position [0..319] depend on LCD_WIDTH
// * @param y Y-Position [0..239] depend on LCD_HEIGHT
// */
//uint8_t lcd_set_cursor(uint16_t x, uint16_t y);
//
///**
// * Draw a Pixel at the current position
// * @param color The foreground color of the pixel
// */
////uint8_t draw_pixel(uint16_t color);

////void glcd_chr(uint16_t x, uint16_t y, char moji);
////void glcd_chr_color(uint16_t x, uint16_t y, char moji, uint16_t fg_color, uint16_t bg_color);
////void glcd_str(uint16_t x , uint16_t y , char *str) ;

void glcd_init(void) ; //LCD������

//***************************************************************************
//void drawHLine(uint16_t x, uint16_t y, uint16_t w, uint16_t color);
//void drawVLine(uint16_t x, uint16_t y, uint16_t h, uint16_t color);
//void writeLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color):
/**************************************************************************
   @brief    Draw a line
   @param    x0  Start point x coordinate
   @param    y0  Start point y coordinate
   @param    x1  End point x coordinate
   @param    y1  End point y coordinate
   @param    color 16-bit 5-6-5 Color to draw with
 **************************************************************************/
//�`�F�b�N�ς�
void display_drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

/**
 * Draw a circle with the center at (xm,ym) and the radius (r)
 * @param x0
 * @param y0
 * @param r
 * @param color
 */
void display_drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);

/**
 * Draw a filled circle with the center at (xm,ym) and the radius (r)
 * @param x0
 * @param y0
 * @param r
 * @param color
 */
void display_fillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);

///**
// * This function will draw a chatacter at (x,y)
// * @param x X-Coordinate
// * @param y Y-Coordinate
// * @param c Character to draw
// * @param fIndex Index to the character (first entry is the width of the character)
// * @param fg_color Foregroundcolor
// * @param bg_color Backgroundcolor
// */
////void lcd_draw_char (uint16_t x, uint16_t y, uint16_t fIndex, uint16_t fg_color, uint16_t bg_color);


/**
 * This function will draw a Pixel at (x,y)
 * @param x X-Coordinate
 * @param y Y-Coordinate
 * @param color Foreground color of the Pixel
 */
//void lcd_draw_pixel_at(uint16_t x, uint16_t y, uint16_t color);

/**
 * This function fill the hole TFT with a user defined Color.
 * @param bg_color Backgroundcolor
 */
void lcd_fill(uint16_t bg_color);

/**
 * This function will draw a filled rect from (x0,y0) to (x1,y1)
 * @param x0 Start-X-Coordinate
 * @param y0 Start-Y-Coordinate
 * @param x1 End-X-Coordinate
 * @param y1 End-Y-Coordinate
 * @param color Foregroundcolor of the rect
 */
void lcd_fill_rect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

////void display_putc(char c);
void display_puts(char *s); //display_drawChars�ƈႢ��������ǂ�ǂ񏑂���B���s���Ή�
void display_drawChars(uint16_t x, uint16_t y, char *s, uint16_t color, uint16_t bg,
     uint8_t size);
//
///**
// * This function will draw a string at (x,y)
// * @param x X-Coordinate
// * @param y Y-Coordinate
// * @param pS Pointer to string
// * @param fg_color Foregroundcolor
// * @param bg_color Backgroundcolor
// */
////void lcd_draw_string (uint16_t x, uint16_t y, const char *pS, uint16_t fg_color, uint16_t bg_color);


//*************************** User Functions ***************************//
void display_drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void display_drawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void display_fillTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void display_drawRoundRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t radius, uint16_t color);
//void display_fillRoundRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t radius, uint16_t color);
//

//void display_setCursor(uint16_t x, uint16_t y);
//void display_setTextColor(uint16_t c, uint16_t bg);
//void display_setTextSize(uint8_t s);
////void display_setTextWrap(bool w);
////void display_printf(const char *fmt, ...);
void display_SetFont(const uint8_t *font);
//void display_SetFontPitch(uint8_t xp, uint8_t yp);



#ifdef	__cplusplus
}
#endif

#endif	/* LCD320X240COLOR_H */

