/*
 * File:   LCD320x240color.c
 * Author: K.Ohno
 *
 * 「PICのブログ」http://picceri.blogspot.com/2019/01/320240-lcd.html
 * からライブラリをいただき、それをベースにMCC用、かつ8bit PIC用に書き換えました。
 * 2020/11/2
 * 
 */
///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                            GFX_Library.c                          ////
////                                                                   ////
////                 Graphics library for mikroC compiler.             ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////               This is a free software with NO WARRANTY.           ////
////                     https://simple-circuit.com/                   ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
//xc.hから以下に変更
#include "mcc_generated_files/mcc.h"
#include "LCD320x240color.h"
#include "mcc_generated_files/drivers/spi_master.h"

#ifndef _swap_uint16_t
#define _swap_uint16_t(a, b) { uint16_t t = a; a = b; b = t; }
#endif

// 絶対値を取得する関数マクロ定義
#define ABS(x) ((x) < 0 ? -(x) : (x))

uint16_t global_bg_color = BLACK;

int16_t
  Cursor_x = 0,          ///< x location to start print()ing text
  Cursor_y = 0;          ///< y location to start print()ing text
uint16_t
  Textcolor = 0xFFFF,    ///< 16-bit background color for print()
  Textbgcolor = 0xFFFF;  ///< 16-bit text color for print()
uint8_t
  Textsize = 1;          ///< Desired magnification of text to print()
bool
  wrap = true;           ///< If set, 'wrap' text at right edge of display


// Prototype
void display_drawCircleHelper(uint16_t x0, uint16_t y0, uint16_t r, uint8_t cornername, uint16_t color);
void display_fillCircleHelper(uint16_t x0, uint16_t y0, uint16_t r, 
        uint8_t corners, uint16_t delta, uint16_t color);

//MCCのSPIのルーチンを呼び出すように変更
/**
 * This function send data or register-information via SPI to the LC-Display
 * @param value Value to send to the LCD-Controller
 */
void write_command(uint8_t cmd) {
    LCD_DCRS_SetLow();  //コマンドモード
//    if (AccessSD) return;
//    LCD_CS_SetHigh();    //CSをHighに
//    SPI1_ExchangeByte(0x00);
//    SPI1_ExchangeByte(0x00);
//    LCD_CS_SetLow();    //CSをLowに
    SPI1_ExchangeByte(cmd);
}
 void write_data(uint8_t data) {
    LCD_DCRS_SetHigh();  //データモード
//    if (AccessSD) return;
    SPI1_ExchangeByte(data);
}

// RGB=5,6,5bit
void glcd_SetFrontColor(uint16_t color) {
    FrontColor.color16 = color;
}

void glcd_SetBackColor(uint16_t color) {
    BackColor.color16 = color;
}

/**************************************************************************
    @brief   Set text font color with custom background color
    @param   c   16-bit 5-6-5 Color to draw text with
    @param   b   16-bit 5-6-5 Color to draw background/fill with
 **************************************************************************/
void display_setTextColor(uint16_t c, uint16_t b) {
    Textcolor   = c;
    Textbgcolor = b;
}

/**************************************************************************
    @brief   Set text 'magnification' size. Each increase in s makes 1 pixel that much bigger.
    @param  s  Desired text size. 1 is default 6x8, 2 is 12x16, 3 is 18x24, etc
 **************************************************************************/
void display_setTextSize(uint8_t s) {
    Textsize = (s > 0) ? s : 1;
}


// データを書き込むエリアの指定
// 4つの引数はすべて画面上の座標
void addset(uint16_t x, uint16_t y, uint16_t xx, uint16_t yy) {
    
    if (spi_master_open(LCD8M)) {
        //ちゃんと開けたら
        LCD_CS_SetLow();    //CSをLowに
        
        write_command(0x2A);
        write_data(x >> 8);
        write_data(x & 0xff);
        write_data(xx >> 8);
        write_data(xx & 0xff);
        write_command(0x2B);
        write_data(y >> 8);
        write_data(y & 0xff);
        write_data(yy >> 8);
        write_data(yy & 0xff);
        write_command(0x2C);
 
        LCD_CS_SetHigh();    //CSをHighに
        SPI1_Close();
    }
    
}


/*******************************************************************************
 * Set the cursor to the coordinate (x,_)
 */
uint8_t lcd_set_cursor_x(uint16_t x) {
    
    if ( x >= LCD_WIDTH ) {
        return EXIT_FAILURE;
    }
    uint8_t hi = x >>8;
    uint8_t lo = x & 0xff;

    if (spi_master_open(LCD8M)) {
        //ちゃんと開けたら
        LCD_CS_SetLow();    //CSをLowに
        write_command(0x2A);    //Column Address Set
        write_data(hi);
        write_data(lo);
        write_data(hi);
        write_data(lo);
        write_command(0x2C);
        LCD_CS_SetHigh();    //CSをHighに
        SPI1_Close();
    }

    return EXIT_SUCCESS;
}

/*******************************************************************************
 * Set the cursor to the coordinate (_,y)
 */
uint8_t lcd_set_cursor_y(uint16_t y) {
    
    if( y >= LCD_HEIGHT ) {
        return EXIT_FAILURE;
    }
    uint8_t hi = y >>8;
    uint8_t lo = y & 0xff;
    
    if (spi_master_open(LCD8M)) {
        //ちゃんと開けたら
        LCD_CS_SetLow();    //CSをLowに
        write_command(0x2B);    // Page Address Set
        write_data(hi);
        write_data(lo);
        write_data(hi);
        write_data(lo);
        write_command(0x2C);
        LCD_CS_SetHigh();    //CSをHighに
        SPI1_Close();
    }

    return EXIT_SUCCESS;
}

/*******************************************************************************
 * Set the cursor at position (x,y)
 */
uint8_t lcd_set_cursor(uint16_t x, uint16_t y) {
    if( lcd_set_cursor_x(x) || lcd_set_cursor_y(y) ) {
        return EXIT_FAILURE;
    }	
    return EXIT_SUCCESS;
}

/**************************************************************************
    @brief  Set text cursor location
    @param  x    X coordinate in pixels
    @param  y    Y coordinate in pixels
 **************************************************************************/
void display_setCursor(uint16_t x, uint16_t y) {
    Cursor_x = x;
    Cursor_y = y;
}

/**************************************************************************
    @brief  Get text cursor X location
    @returns    X coordinate in pixels
 **************************************************************************/
uint16_t display_getCursorX(void) {
    return Cursor_x;
}

/**************************************************************************
    @brief      Get text cursor Y location
    @returns    Y coordinate in pixels
 **************************************************************************/
uint16_t display_getCursorY(void) {
    return Cursor_y;
}

/*******************************************************************************
 * Draw a Pixel at the actual position
 */
uint8_t draw_pixel(uint16_t color) {

    if (spi_master_open(LCD8M)) {
        //ちゃんと開けたら
        LCD_CS_SetLow();    //CSをLowに
        write_data(color >>8);
        write_data(color & 0xff);
        LCD_CS_SetHigh();    //CSをHighに
        SPI1_Close();
    }

    return EXIT_SUCCESS;
}

/*******************************************************************************
 * This function will draw a Pixel at (x,y) with color
 */
void lcd_draw_pixel_at(uint16_t x, uint16_t y, uint16_t color) {
    if( !lcd_set_cursor(x, y) ) {
        draw_pixel(color);
    }
}


/*
 * フォント設定
 */
void display_SetFont(const uint8_t *font) {
    Font.xsize = font[Fxsize];
    Font.ysize = font[Fysize];
    Font.xpitch = Font.xsize;   //ピッチは、デフォルトでsizeと同じ
    Font.ypitch = Font.ysize;
    Font.font = font;
    Font.offset = font[Foffset];    
}

/*
 * フォントのピッチを変更したい時に使う
 */
void display_SetFontPitch(uint8_t xp, uint8_t yp) {
    Font.xpitch = xp;
    Font.ypitch = yp;
    
}

/**************************************************************************
    @brief  Print one byte/character of data
    @param  c  The 8-bit ascii character to write
 * フォントを変更できるように、FontStruct変数で対応
 * \rで、左端に戻る
 * \nで、次の行に行くけど、X座標は変わらない
 * wrap=trueの時、行の先頭に空白が来た時は、それは描画しない
 * Font指定により、fontXsize, fontYsize, pitchX, pitchYデータを使って描画
 * Textsizeを拡張して、16進2桁xyで指定。X方向倍率、Y方向倍率を独立化。
 * 1桁指定の場合は、XY同倍率とみなす
 * 　例:12hの場合、横1倍、縦2倍の表示ができるように
 **************************************************************************/
void display_putc(char c) {
    uint8_t i, j;
    uint8_t textsizeX, textsizeY, pitchX, pitchY;

    uint8_t fontXsize = Font.xsize;
    uint8_t fontYsize = Font.ysize;

    if (Textsize < 0x10) {  //2桁でない時は、X,Y同倍率
        textsizeX = Textsize;
        textsizeY = Textsize;
    } else {
        textsizeX = Textsize >> 4;
        textsizeY = Textsize & 0x0f;
    }
    
    pitchX = Font.xpitch * textsizeX;
    pitchY = Font.ypitch * textsizeY;

    if (c == ' ' && Cursor_x == 0 && wrap)  //行の先頭の空白は削除
        return;
    if (c == '\r') {    //改行=Carriage return　左端にカーソル戻す
        Cursor_x = 0;
        return;
    }
    if (c == '\n') {    //改行=Line Feed　イメージ的には次の行へ行く
        Cursor_y += pitchY;
        return;
    }

    for(i = 0; i < fontXsize; i++ ) {
        uint8_t line = Font.font[8+ fontXsize * (c - Font.offset) + i];  //8は、フォントデータの前の8バイト分の情報分
        if (c == ' ') line = 0;
        for(j = 0; j < fontYsize; j++, line >>= 1) {
            if (line & 1) {
                if (Textsize == 1)
                    lcd_draw_pixel_at(Cursor_x + i, Cursor_y + j, Textcolor);
                else
                    fillRect(Cursor_x + i * textsizeX, Cursor_y + j * textsizeY, textsizeX, textsizeY, Textcolor);
            }
            else if (Textbgcolor != Textcolor) {
                // Backカラーがfrontカラーと違う時は、その色を背景として塗る
                if (Textsize == 1)
                    lcd_draw_pixel_at(Cursor_x + i, Cursor_y + j, Textbgcolor);
                else
                    fillRect(Cursor_x + i * textsizeX, Cursor_y + j * textsizeY, textsizeX, textsizeY, Textbgcolor);
            }
        }
    }

    Cursor_x += pitchX;
    if ( Cursor_x > (LCD_WIDTH + pitchX) ) Cursor_x = LCD_WIDTH;
    if (wrap && (Cursor_x + pitchX) > LCD_WIDTH) {
        Cursor_x = 0;
        Cursor_y += textsizeY * fontYsize;
        if ( Cursor_y > (LCD_HEIGHT + pitchY) )
            Cursor_y = LCD_HEIGHT;
    }

}


// print string
void display_puts(char *s) {
    while(*s)
        display_putc(*s++);
}

/**************************************************************************
   @brief   Draw a single character
    @param    x   Bottom left corner x coordinate
    @param    y   Bottom left corner y coordinate
    @param    c   The 8-bit font-indexed character (likely ascii)
    @param    color 16-bit 5-6-5 Color to draw chraracter with
    @param    bg 16-bit 5-6-5 Color to fill background with (if same as color, no background)
    @param    size  Font magnification level, 1 is 'original' size
 カーソル位置、色、サイズは、別関数で意識的に設定したものが、ここで勝手に変わってしまわないように元に戻している
 **************************************************************************/
void display_drawChars(uint16_t x, uint16_t y, char *s, uint16_t color, uint16_t bg,
     uint8_t size) {
  uint16_t prev_x     = Cursor_x,
           prev_y     = Cursor_y,
           prev_color = Textcolor,
           prev_bg    = Textbgcolor;
  uint8_t  prev_size  = Textsize;

  display_setCursor(x, y);
  display_setTextSize(size);
  display_setTextColor(color, bg);
  
//  display_SetFont(NormalFont);
  display_puts(s);  //ここを変更
  
  Cursor_x    = prev_x;
  Cursor_y    = prev_y;
  Textcolor   = prev_color;
  Textbgcolor = prev_bg;
  Textsize    = prev_size;    
}


/*******************************************************************************
 * This function fill the hole TFT with a user defined Color.
 */
// 画面クリア
void lcd_fill(uint16_t bg_color) {   
    uint16_t width = LCD_WIDTH, height = LCD_HEIGHT;
    uint8_t hi = bg_color >> 8, lo = bg_color;

    addset(0, 0, width, height); //これで描画範囲を制限する

    if (spi_master_open(LCD8M)) {
        //ちゃんと開けたら
        LCD_CS_SetLow();    //CSをLowに
        while (height--) {
            while (width--) {
                write_data(hi);
                write_data(lo);
            }
            width = LCD_WIDTH;
        }
        LCD_CS_SetHigh();    //CSをHighに
        SPI1_Close();
    }
}


/**************************************************************************
   @brief    Draw a perfectly horizontal line
   @param    x   Left-most x coordinate
   @param    y   Left-most y coordinate
   @param    w   Width in pixels
   @param    color 16-bit 5-6-5 Color to fill with
 **************************************************************************/
void drawHLine(uint16_t x, uint16_t y, uint16_t w, uint16_t color) {
    if ( (x < LCD_WIDTH) && (y < LCD_WIDTH) && w) {
        uint8_t hi = color >>8;
        uint8_t lo = color & 0xff;

        if ((x + w - 1) >= LCD_WIDTH) w = LCD_WIDTH  - x;
        addset(x, y,  x + w-1, y);
        if (spi_master_open(LCD8M)) {
            //ちゃんと開けたら
            LCD_CS_SetLow();    //CSをLowに
            while (w--) {
                write_data(hi);
                write_data(lo);
            }
            LCD_CS_SetHigh();    //CSをHighに
            SPI1_Close();
        }        
    }
}

/**************************************************************************
   @brief    Draw a perfectly vertical line
   @param    x   Top-most x coordinate
   @param    y   Top-most y coordinate
   @param    h   Height in pixels
   @param    color 16-bit 5-6-5 Color to fill with
 **************************************************************************/
void drawVLine(uint16_t x, uint16_t y, uint16_t h, uint16_t color) {
    if ( (x < LCD_WIDTH) && (y < LCD_WIDTH) && h) {  
        uint8_t hi = color >>8;
        uint8_t lo = color & 0xff;

        if ((y + h - 1) >= LCD_WIDTH) h = LCD_WIDTH - y;
        addset(x, y, x, y+h-1);
        if (spi_master_open(LCD8M)) {
            //ちゃんと開けたら
            LCD_CS_SetLow();    //CSをLowに
            while (h--) {
                write_data(hi);
                write_data(lo);
            }
            LCD_CS_SetHigh();    //CSをHighに
            SPI1_Close();
        }        

    }
}

/**************************************************************************
   @brief    Write a line.  Bresenham's algorithm - thx wikpedia
   @param    x0  Start point x coordinate
   @param    y0  Start point y coordinate
   @param    x1  End point x coordinate
   @param    y1  End point y coordinate
   @param    color 16-bit 5-6-5 Color to draw with
 **************************************************************************/
void writeLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
    bool steep = ABS((int16_t)(y1 - y0)) > ABS((int16_t)(x1 - x0));
    int16_t dx, dy, err, ystep;
    if (steep) {
        _swap_uint16_t(x0, y0);
        _swap_uint16_t(x1, y1);
    }

    if (x0 > x1) {
        _swap_uint16_t(x0, x1);
        _swap_uint16_t(y0, y1);
    }

    dx = x1 - x0;
    dy = ABS((int16_t)(y1 - y0));

    err = dx / 2;

    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }

    for (; x0<=x1; x0++) {
        if (steep) {
            lcd_draw_pixel_at(y0, x0, color);
        } else {
            lcd_draw_pixel_at(x0, y0, color);
        }
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

/**************************************************************************
   @brief    Draw a line
   @param    x0  Start point x coordinate
   @param    y0  Start point y coordinate
   @param    x1  End point x coordinate
   @param    y1  End point y coordinate
   @param    color 16-bit 5-6-5 Color to draw with
 **************************************************************************/
void display_drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {

    if (x0 == x1) {
        if (y0 > y1) _swap_uint16_t(y0, y1);
        drawVLine(x0, y0, y1 - y0 + 1, color);
    } else if (y0 == y1) {
        if (x0 > x1) _swap_uint16_t(x0, x1);
        drawHLine(x0, y0, x1 - x0 + 1, color);
    } else {
        writeLine(x0, y0, x1, y1, color);
    }
}


/**************************************************************************
   @brief   Draw a triangle with no fill color
    @param    x0  Vertex #0 x coordinate
    @param    y0  Vertex #0 y coordinate
    @param    x1  Vertex #1 x coordinate
    @param    y1  Vertex #1 y coordinate
    @param    x2  Vertex #2 x coordinate
    @param    y2  Vertex #2 y coordinate
    @param    color 16-bit 5-6-5 Color to draw with
 **************************************************************************/
void display_drawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
        uint16_t x2, uint16_t y2, uint16_t color) {
    display_drawLine(x0, y0, x1, y1, color);
    display_drawLine(x1, y1, x2, y2, color);
    display_drawLine(x2, y2, x0, y0, color);
}

/*******************************************************************************
 * This function will draw a filled rect from (x0,y0) to (x1,y1) with color
 */
void lcd_fill_rect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
    uint16_t ii, jj;
    uint8_t hi = color >> 8, lo = color;

    if( (y0 >= LCD_HEIGHT) || (x0 >= LCD_WIDTH) ) {
        return;
    }
    
    if( x1 >= LCD_WIDTH ) x1 = LCD_WIDTH;
    if( y1 >= LCD_HEIGHT ) y1 = LCD_HEIGHT;

    addset(x0, y0, x1, y1); //これで描画範囲を制限する

    if (spi_master_open(LCD8M)) {
        //ちゃんと開けたら
        LCD_CS_SetLow();    //CSをLowに
        jj=0;
        while (jj++ < y1 - y0 +1) {
            ii=0; 
            while (ii++ < x1 - x0 +1) {
                write_data(hi); //制限された矩形領域をアドレス指定せず描画
                write_data(lo);
            }
        }
        LCD_CS_SetHigh();    //CSをHighに
        SPI1_Close();
    }        
} 
/**************************************************************************
   @brief    Fill a rectangle completely with one color. Update in subclasses if desired!
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    w   Width in pixels
    @param    h   Height in pixels
    @param    color 16-bit 5-6-5 Color to fill with
 **************************************************************************/
void fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {

    if( w && h) {                            // Nonzero width and height?
        lcd_fill_rect(x, y, x+w-1, y+h-1, color);
    }
}


/**************************************************************************
   @brief   Draw a rectangle with no fill color
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    w   Width in pixels
    @param    h   Height in pixels
    @param    color 16-bit 5-6-5 Color to draw with
 **************************************************************************/
void display_drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {

    drawHLine(x, y, w, color);
    drawHLine(x, y+h-1, w, color);
    drawVLine(x, y, h, color);
    drawVLine(x+w-1, y, h, color);

}

/**************************************************************************
   @brief   Draw a rounded rectangle with no fill color
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    w   Width in pixels
    @param    h   Height in pixels
    @param    r   Radius of corner rounding
    @param    color 16-bit 5-6-5 Color to draw with
 **************************************************************************/
void display_drawRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t r, uint16_t color) {
    // smarter version
    int16_t max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
    if (r > max_radius) r = max_radius;
    
    drawHLine(x+r  , y    , w-2*r, color); // Top
    drawHLine(x+r  , y+h-1, w-2*r, color); // Bottom
    drawVLine(x    , y+r  , h-2*r, color); // Left
    drawVLine(x+w-1, y+r  , h-2*r, color); // Right
    // draw four corners
    display_drawCircleHelper(x+r    , y+r    , r, 1, color);
    display_drawCircleHelper(x+w-r-1, y+r    , r, 2, color);
    display_drawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color);
    display_drawCircleHelper(x+r    , y+h-r-1, r, 8, color);
    
}

/**************************************************************************
   @brief   Draw a rounded rectangle with fill color
   @param    x   Top left corner x coordinate
   @param    y   Top left corner y coordinate
   @param    w   Width in pixels
   @param    h   Height in pixels
   @param    r   Radius of corner rounding
   @param    color 16-bit 5-6-5 Color to draw/fill with
 **************************************************************************/
void display_fillRoundRect(uint16_t x, uint16_t y, uint16_t w,
        uint16_t h, uint16_t r, uint16_t color) {
    int16_t max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis

    if (r > max_radius) r = max_radius;
    
    // smarter version
    lcd_fill_rect(x+r, y, x+r+w-2*r-1, y+h-1, color);
    // draw four corners
    display_fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
    display_fillCircleHelper(x+r    , y+r, r, 2, h-2*r-1, color);
    
}

/**************************************************************************
   @brief    Quarter-circle drawer, used to do circles and roundrects
    @param    x0   Center-point x coordinate
    @param    y0   Center-point y coordinate
    @param    r   Radius of circle
    @param    cornername  Mask bit #1 or bit #2 to indicate which quarters of the circle we're doing
    @param    color 16-bit 5-6-5 Color to draw with
 **************************************************************************/
void display_drawCircleHelper(uint16_t x0, uint16_t y0, uint16_t r, uint8_t cornername, uint16_t color) {
    int16_t f     = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x     = 0;
    int16_t y     = r;

    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f     += ddF_y;
        }
        x++;
        ddF_x += 2;
        f     += ddF_x;
        if (cornername & 0x4) {
            lcd_draw_pixel_at(x0 + x, y0 + y, color);
            lcd_draw_pixel_at(x0 + y, y0 + x, color);
        }
        if (cornername & 0x2) {
            lcd_draw_pixel_at(x0 + x, y0 - y, color);
            lcd_draw_pixel_at(x0 + y, y0 - x, color);
        }
        if (cornername & 0x8) {
            lcd_draw_pixel_at(x0 - y, y0 + x, color);
            lcd_draw_pixel_at(x0 - x, y0 + y, color);
        }
        if (cornername & 0x1) {
            lcd_draw_pixel_at(x0 - y, y0 - x, color);
            lcd_draw_pixel_at(x0 - x, y0 - y, color);
        }
    }
}

/**************************************************************************
   @brief    Draw a circle outline
   @param    x0   Center-point x coordinate
   @param    y0   Center-point y coordinate
   @param    r   Radius of circle
   @param    color 16-bit 5-6-5 Color to draw with
 **************************************************************************/
void display_drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    lcd_draw_pixel_at(x0  , y0+r, color);
    lcd_draw_pixel_at(x0  , y0-r, color);
    lcd_draw_pixel_at(x0+r, y0  , color);
    lcd_draw_pixel_at(x0-r, y0  , color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        lcd_draw_pixel_at(x0 + x, y0 + y, color);
        lcd_draw_pixel_at(x0 - x, y0 + y, color);
        lcd_draw_pixel_at(x0 + x, y0 - y, color);
        lcd_draw_pixel_at(x0 - x, y0 - y, color);
        lcd_draw_pixel_at(x0 + y, y0 + x, color);
        lcd_draw_pixel_at(x0 - y, y0 + x, color);
        lcd_draw_pixel_at(x0 + y, y0 - x, color);
        lcd_draw_pixel_at(x0 - y, y0 - x, color);
    }
}


/**************************************************************************
    @brief  Quarter-circle drawer with fill, used for circles and roundrects
    @param  x0       Center-point x coordinate
    @param  y0       Center-point y coordinate
    @param  r        Radius of circle
    @param  corners  Mask bits indicating which quarters we're doing
    @param  delta    Offset from center-point, used for round-rects
    @param  color    16-bit 5-6-5 Color to fill with
 **************************************************************************/
void display_fillCircleHelper(uint16_t x0, uint16_t y0, uint16_t r, 
        uint8_t corners, uint16_t delta, uint16_t color) {
    
    int16_t f     = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x     = 0;
    int16_t y     = r;
    int16_t px    = x;
    int16_t py    = y;

    delta++; // Avoid some +1's in the loop

    while(x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f     += ddF_y;
        }
        x++;
        ddF_x += 2;
        f     += ddF_x;
        // These checks avoid double-drawing certain lines, important
        // for the SSD1306 library which has an INVERT drawing mode.
        if(x < (y + 1)) {
            if(corners & 1) drawVLine(x0+x, y0-y, 2*y+delta, color);
            if(corners & 2) drawVLine(x0-x, y0-y, 2*y+delta, color);
        }
        if(y != py) {
            if(corners & 1) drawVLine(x0+py, y0-px, 2*px+delta, color);
            if(corners & 2) drawVLine(x0-py, y0-px, 2*px+delta, color);
            py = y;
        }
        px = x;
    }
}

/**************************************************************************
   @brief    Draw a circle with filled color
   @param    x0   Center-point x coordinate
   @param    y0   Center-point y coordinate
   @param    r   Radius of circle
   @param    color 16-bit 5-6-5 Color to fill with
 **************************************************************************/
void display_fillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color) {
    
    drawVLine(x0, y0-r, 2*r+1, color);
    display_fillCircleHelper(x0, y0, r, 3, 0, color);
}


/*******************************************************************************
 * This function will draw a chatacter at (x,y)
 */
//void lcd_draw_char (uint16_t x, uint16_t y, uint16_t fIndex, uint16_t fg_color, uint16_t bg_color)
//{
//    uint8_t j, k, i;
//    
//    for(j=0; j < NormalFont[fIndex]; j++) {   // variable character width
//        lcd_set_cursor(x + NormalFont[fIndex] - j, y); // print from right to left
//        
//        for(k=0; k<FONT_HEIGHT; k++) {  // _ bytes per character (height)
//            for(i=0; i<8; i++) {
//                if( NormalFont[ (fIndex + ((NormalFont[fIndex]) << 1)) - (j<<1) - k ] & (0x80 >> i) ) {
//                    draw_pixel(fg_color);
//                }
//                else {
//                    draw_pixel(bg_color);
//                }
//            }
//        }
//    }
//}

/*******************************************************************************
 * This function will draw a string at (x,y)
 */
//void lcd_draw_string (uint16_t x, uint16_t y, const char *pS, uint16_t fg_color, uint16_t bg_color) {    
//    uint16_t lIndex, k;
//    
//    while (*pS) {
//        /* index the width information of character <c> */
//        lIndex = 0;
//        for (k=0; k<(*pS - ' '); k++) {
//            lIndex += ((NormalFont[lIndex]) << 1) + 1;
//        }
//        
//        /* draw character */
//        lcd_draw_char(x, y, lIndex, fg_color, bg_color);
//        
//        /* move the cursor forward for the next character */
//        x += NormalFont[lIndex];
//        
//        /* next character */
//        pS++;
//    }
//}


//ここから下は、別のソース。　今後、良さそうなルーチンを取り込む予定として、コメント化
/*
This is the core graphics library for all our displays, providing a common
set of graphics primitives (points, lines, circles, etc.).  It needs to be
paired with a hardware-specific library for each display device we carry
(to handle the lower-level functions).

Adafruit invests time and resources providing this open source code, please
support Adafruit & open-source hardware by purchasing products from Adafruit!

Copyright (c) 2013 Adafruit Industries.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
 */
///////////////////////////////////////////////////////////////////////////////

//uint8_t  display_getRotation();
//uint16_t getCursorX(void);
//uint16_t getCursorY(void);
//uint16_t display_getWidth();
//uint16_t display_getHeight();
//uint16_t display_color565(uint8_t red, uint8_t green, uint8_t blue);
//
//void display_drawBitmapV1   (uint16_t x, uint16_t y, const uint8_t *bitmap, uint16_t w, uint16_t h, uint16_t color);
//void display_drawBitmapV1_bg(uint16_t x, uint16_t y, const uint8_t *bitmap, uint16_t w, uint16_t h, uint16_t color, uint16_t bg);
//void display_drawBitmapV2   (uint16_t x, uint16_t y, const uint8_t *bitmap, uint16_t w, uint16_t h, uint16_t color);
//void display_drawBitmapV2_bg(uint16_t x, uint16_t y, const uint8_t *bitmap, uint16_t w, uint16_t h, uint16_t color, uint16_t bg);
//
////************************* Non User Functions *************************//
//void writeLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
//uint8_t printNumber(uint32_t n, int8_t n_width, uint8_t _flags);
//void printFloat(float float_n, int8_t f_width, int8_t decimal, uint8_t _flags);
//void v_printf(const char *fmt, va_list arp);
////////////////////////////////////////////////////////////////////////////
//
//#ifndef min
//#define min(a,b) (((a) < (b)) ? (a) : (b))
//#endif



/**************************************************************************
   @brief     Draw a triangle with color-fill
    @param    x0  Vertex #0 x coordinate
    @param    y0  Vertex #0 y coordinate
    @param    x1  Vertex #1 x coordinate
    @param    y1  Vertex #1 y coordinate
    @param    x2  Vertex #2 x coordinate
    @param    y2  Vertex #2 y coordinate
    @param    color 16-bit 5-6-5 Color to fill/draw with
 **************************************************************************/
void display_fillTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
  uint16_t x2, uint16_t y2, uint16_t color) {
    int16_t a, b, y, last, dx01, dy01, dx02, dy02, dx12, dy12;
    //以下は、int32_tだったが、結果がおかしい時あり
    //以下のようにキャストしたら動いたようが、たぶん32bit変数不要と判断
    //        b   = (int16_t)x0 +(int16_t) sb / dy02;
    int16_t sa   = 0;
    int16_t sb   = 0;

    // Sort coordinates by Y order (y2 >= y1 >= y0)
    if (y0 > y1) {
        _swap_uint16_t(y0, y1); _swap_uint16_t(x0, x1);
    }
    if (y1 > y2) {
        _swap_uint16_t(y2, y1); _swap_uint16_t(x2, x1);
    }
    if (y0 > y1) {
        _swap_uint16_t(y0, y1); _swap_uint16_t(x0, x1);
    }
    
    if(y0 == y2) { // Handle awkward all-on-same-line case as its own thing
        a = b = x0;
        if(x1 < a)      a = x1;
        else if(x1 > b) b = x1;
        if(x2 < a)      a = x2;
        else if(x2 > b) b = x2;
        drawHLine(a, y0, b-a+1, color);
        return;
    }
    
    dx01 = x1 - x0;
    dy01 = y1 - y0;
    dx02 = x2 - x0;
    dy02 = y2 - y0;
    dx12 = x2 - x1;
    dy12 = y2 - y1;
    
    // For upper part of triangle, find scanline crossings for segments
    // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
    // is included here (and second loop will be skipped, avoiding a /0
    // error there), otherwise scanline y1 is skipped here and handled
    // in the second loop...which also avoids a /0 error here if y0=y1
    // (flat-topped triangle).
    if(y1 == y2) last = y1;   // Include y1 scanline
    else         last = y1-1; // Skip it
    
    for(y=y0; y<=last; y++) {
        a   = x0 + sa / dy01;
        b   = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;
        /* longhand:
         a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
         b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
         */
        if(a > b) _swap_uint16_t(a,b);
        drawHLine(a, y, b-a+1, color);
    }
    // For lower part of triangle, find scanline crossings for segments
    // 0-2 and 1-2.  This loop is skipped if y1=y2.
    sa = dx12 * (y - y1);
    sb = dx02 * (y - y0);
    
    // sprintfの書式に2つ以上の変数の設定を入れると、2つ目以降のデータがおかしく表示される場合あり
    //    sprintf(str, "%d, y=%d", sa, sb, y);
    
    for(; y<=y2; y++) {
        a   = x1 + sa / dy12;
        b   = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;
        /* longhand:
         a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
         b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
         */
        if(a > b) _swap_uint16_t(a,b);
        drawHLine(a, y, b-a+1, color);
    }

}

///**************************************************************************/
///*!
//    @brief      Whether text that is too long should 'wrap' around to the next line.
//    @param  w Set true for wrapping, false for clipping
//*/
///**************************************************************************/
//void display_setTextWrap(bool w) {
//    wrap = w;
//}

/**************************************************************************
    @brief      Get rotation setting for display
    @returns    0 thru 3 corresponding to 4 cardinal rotations
 **************************************************************************/
//uint8_t display_getRotation(void) {
//    return rotation;
//}

/**************************************************************************
    @brief      Get width of the display, accounting for the current rotation
    @returns    Width in pixels
 **************************************************************************/
//uint16_t display_getWidth(void) {
//    return display_width;
//}

/**************************************************************************
    @brief      Get height of the display, accounting for the current rotation
    @returns    Height in pixels
 **************************************************************************/
//uint16_t display_getHeight(void) {
//    return display_height;
//}

/**************************************************************************
    @brief   Given 8-bit red, green and blue values, return a 'packed'
             16-bit color value in '565' RGB format (5 bits red, 6 bits
             green, 5 bits blue). This is just a mathematical operation,
             no hardware is touched.
    @param   red    8-bit red brightnesss (0 = off, 255 = max).
    @param   green  8-bit green brightnesss (0 = off, 255 = max).
    @param   blue   8-bit blue brightnesss (0 = off, 255 = max).
    @return  'Packed' 16-bit color value (565 format).
 **************************************************************************/
//uint16_t display_color565(uint8_t red, uint8_t green, uint8_t blue) {
//    return ((uint16_t)(red & 0xF8) << 8) | ((uint16_t)(green & 0xFC) << 3) | (blue >> 3);
//}

/**************************************************************************
   @brief     Draw a ROM-resident 1-bit image at the specified (x,y) position,
               using the specified foreground color (unset bits are transparent).
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    bitmap  byte array with monochrome bitmap
    @param    w   Width of bitmap in pixels
    @param    h   Hieght of bitmap in pixels
    @param    color 16-bit 5-6-5 Color to draw with
 **************************************************************************/
//void display_drawBitmapV1(uint16_t x, uint16_t y, const uint8_t *bitmap, uint16_t w, uint16_t h,
//     uint16_t color) {
//  uint16_t i, j;
//  for( i = 0; i < h/8; i++)
//  {    
//    for( j = 0; j < w * 8; j++)
//    {      
//      if( bitmap[j/8 + i*w] & (1 << (j % 8)) )
//        display_drawPixel(x + j/8, y + i*8 + (j % 8), color);
//    }
//  }
//}

/**************************************************************************
   @brief     Draw a ROM-resident 1-bit image at the specified (x,y) position,
              using the specified foreground (for set bits) and background (unset bits) colors.
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    bitmap  byte array with monochrome bitmap
    @param    w   Width of bitmap in pixels
    @param    h   Hieght of bitmap in pixels
    @param    color 16-bit 5-6-5 Color to draw pixels with
    @param    bg 16-bit 5-6-5 Color to draw background with
 **************************************************************************/
//void display_drawBitmapV1_bg(uint16_t x, uint16_t y, const uint8_t *bitmap, uint16_t w, uint16_t h,
//     uint16_t color, uint16_t bg) {
//  uint16_t i, j;
//  for( i = 0; i < h/8; i++)
//  {    
//    for( j = 0; j < w * 8; j++)
//    {      
//      if( bitmap[j/8 + i*w] & (1 << (j % 8)) )
//        display_drawPixel(x + j/8, y + i*8 + (j % 8), color);
//      else
//        display_drawPixel(x + j/8, y + i*8 + (j % 8), bg);
//    }
//  }
//}

/**************************************************************************
   @brief     Draw a ROM-resident 1-bit image at the specified (x,y) position,
              using the specified foreground color (unset bits are transparent).
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    bitmap  byte array with monochrome bitmap
    @param    w   Width of bitmap in pixels
    @param    h   Hieght of bitmap in pixels
    @param    color 16-bit 5-6-5 Color to draw pixels with
 **************************************************************************/
//void display_drawBitmapV2(uint16_t x, uint16_t y, const uint8_t *bitmap, uint16_t w, uint16_t h,
//  uint16_t color) {
//
//    uint16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
//    uint8_t _byte = 0;
//    uint16_t i, j;
//
//    for(j = 0; j < h; j++, y++) {
//        for( i = 0; i < w; i++ ) {
//            if(i & 7) _byte <<= 1;
//            else      _byte   = bitmap[j * byteWidth + i / 8];
//            if(_byte & 0x80)
//              display_drawPixel(x+i, y, color);
//        }
//    }
//}

/**************************************************************************
   @brief     Draw a ROM-resident 1-bit image at the specified (x,y) position,
              using the specified foreground (for set bits) and background (unset bits) colors.
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    bitmap  byte array with monochrome bitmap
    @param    w   Width of bitmap in pixels
    @param    h   Hieght of bitmap in pixels
    @param    color 16-bit 5-6-5 Color to draw pixels with
    @param    bg 16-bit 5-6-5 Color to draw background with
 **************************************************************************/
//void display_drawBitmapV2_bg(uint16_t x, uint16_t y, const uint8_t *bitmap, uint16_t w, uint16_t h,
//  uint16_t color, uint16_t bg) {
//
//    uint16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
//    uint8_t _byte = 0;
//    uint16_t i, j;
//    for(j = 0; j < h; j++, y++) {
//        for(i = 0; i < w; i++ ) {
//            if(i & 7) _byte <<= 1;
//            else      _byte   = bitmap[j * byteWidth + i / 8];
//            if(_byte & 0x80)
//              display_drawPixel(x+i, y, color);
//            else
//              display_drawPixel(x+i, y, bg);
//        }
//    }
//}

//uint8_t printNumber(uint32_t n, int8_t n_width, uint8_t _flags) {
//  uint8_t i=0, j, buff[10];
//  do {
//      buff[i] = (uint8_t)( n % (_flags & 0x1F) );
//      if (buff[i] > 9)
//        buff[i] += (_flags & 0x80) ? 0x07 : 0x27;
//      buff[i++] += '0';
//      n /= (_flags & 0x1F);
//      } while (n);
//  j = i;
//  if(_flags & 0x40) {
//    n_width--;
//    j++;
//    if(_flags & 0x20) {    // put '-' before the zeros
//      display_putc('-');
//      _flags &= ~0x40;
//    }
//  }
//  while (i < n_width--) {
//    if (_flags & 0x20)  display_putc('0');
//    else                display_putc(' ');
//  }
//  if (_flags & 0x40)
//    display_putc('-');
//  do {
//      display_putc(buff[--i]);
//      } while(i);
//  return j;
//}
//
//void printFloat(float float_n, int8_t f_width, int8_t decimal, uint8_t _flags) {
//  int32_t int_part = float_n;
//  float rem_part;
//  if(decimal == 0)  decimal = 1;
//  if(float_n < 0) {
//    _flags |= 0x40;
//    rem_part = (float)int_part - float_n;
//    int_part = ~int_part + 1;
//  }
//  else
//    rem_part = float_n - (float)int_part;
//  _flags |= 10;
//  f_width -= printNumber(int_part, f_width - decimal - 1, _flags);
//  display_putc('.');
//  f_width--;
//  if(f_width < 1)  f_width = 1;
//  if(decimal > f_width)  decimal = f_width;
//  while( decimal > 0 && (rem_part != 0 || decimal > 0) ) {
//    decimal--;
//    rem_part *= 10;
//    display_putc( (uint8_t)rem_part + '0' );
//    rem_part -= (uint8_t)rem_part;
//  }
//}
//
//void v_printf(const char *fmt, va_list arp) {
//  uint8_t _flags, c, d=0, w=0;
//  uint32_t nbr;
//  while (1) {
//    c = *fmt++;
//
//    if (!c)
//      break;
//
//    if (c != '%') {
//      display_putc(c);
//      continue;
//    }
//
//    _flags = 0;
//    c = *fmt++;
//
//    if (c == '0') {
//      _flags |= 0x20;  // zero flag
//      c = *fmt++;
//    }
//
//    for (w = 0; c >= '0' && c <= '9'; c = *fmt++)   // width
//      w = w * 10 + c - '0';
//
//    if (c == '.') {
//      c = *fmt++;
//      for (d = 0; c >= '0' && c <= '9'; c = *fmt++)   // decimals width
//        d = d * 10 + c - '0';
//    }
//    
//    if(c == 'f' || c == 'F') {    // if float number
//      printFloat(va_arg(arp, float), w, d, _flags);
//      continue;
//    }
//
//    if (c == 'l' || c == 'L') {   // long number (4 bytes)
//      _flags |= 0x40;    // long number flag
//      c = *fmt++;
//    }
//
//    if (!c)   // end of format?
//      break;
//
//    if(c == 'X') {
//      _flags |= 0x80;   // upper case hex flag
//    }
//    
//    if (c >= 'a')   // if lower case, switch to upper
//      c -= 0x20;
//    switch (c) {
//      case 'C' :        // character
//        display_putc( (uint8_t)va_arg(arp, uint8_t) ); continue;
//      case 'B' :        // binary
//        _flags |= 2;  break;
//      case 'O' :        // octal
//        _flags |= 8;  break;
//      case 'D' :        // signed decimal
//      case 'U' :        // unsigned decimal
//        _flags |= 10; break;
//      case 'X' :        // hexadecimal
//        _flags |= 16; break;
//      default:          // other
//        display_putc(c); continue;
//    }
//
//    if(_flags & 0x40)  // if long number
//      nbr = (c == 'D') ? va_arg(arp, int32_t) : va_arg(arp, uint32_t);
//    else
//      nbr = (c == 'D') ? (int32_t)va_arg(arp, int16_t) : (uint32_t)va_arg(arp, uint16_t);
//    if ( (c == 'D') &&  (nbr & 0x80000000) ) {
//      _flags |= 0x40;     // negative number flag
//      nbr = ~nbr + 1;     // change to positive form
//    }
//    else
//      _flags &= ~0x40;    // number is positive
//    printNumber(nbr, w, _flags);
//  }
//}
//
//void display_printf(const char *fmt, ...) {
//  va_list arg;
//  va_start(arg, fmt);
//  v_printf(fmt, arg);
//}

/*
 * LCDディスプレイの初期化
 */
void glcd_init(void)
{
    //LCD Reset
    LCD_RESET_SetLow();
    __delay_ms(1);  //Reset pulse duration > 10us
    LCD_RESET_SetHigh();
    __delay_ms(15); // >5ms
    
    if (spi_master_open(LCD8M)) {
        //ちゃんと開けたら
        LCD_CS_SetLow();    //CSをLowに
        
        // Power control A
        write_command(0xCB);
        write_data(0x39);
        write_data(0x2C);
        write_data(0x00);
        write_data(0x34);
        write_data(0x02);
        
        // Power control B
        write_command(0xCF);
        write_data(0x00);
        write_data(0xC1);   //A2h
        write_data(0x30);
        
        // Driver timing control A
        write_command(0xE8);
        write_data(0x85);   //84h
        write_data(0x00);   //11h
        write_data(0x78);   //7Ah
        
        // Driver timing control B
        write_command(0xEA);
        write_data(0x00);   //66h
        write_data(0x00);
        
        // Power on sequence control
        write_command(0xED);
        write_data(0x64);   //55h
        write_data(0x03);   //01h
        write_data(0x12);   //23h
        write_data(0x81);   //01h
        
        // Pump ratio control
        write_command(0xF7);
        write_data(0x20);   //10h
        
        // Power Control 1
        write_command(0xC0); //Power control
        write_data(0x23); //VRH[5:0]   0x23=4.60V
        
        // Power Control 2
        write_command(0xC1); //Power control
        write_data(0x10); //SAP[2:0];BT[3:0]
        
        // VCOM Control 1
        write_command(0xC5); //VCM control
        write_data(0x3E);   // 4.25V
        write_data(0x28);   // -1.5V
        
        // VCOM Control 2
        write_command(0xC7); //VCM control2
        write_data(0x86); // VCOM offset voltage: VMH ? 58 VML ? 58
        
        // Memory Access Control
        write_command(0x36); // Memory Access Control
        write_data(0xe8); //
        
        // COLMOD: Pixel Format Set
        write_command(0x3A);
        write_data(0x55);   //16 bits/pixel
        
        // Frame Rate Control (In Normal Mode/Full Colors)
        write_command(0xB1);
        write_data(0x00);
        write_data(0x18);   // 79Hz,  0x1B=70Hz (default)
        
        // Display Function Control
        write_command(0xB6); // Display Function Control
        write_data(0x08);
        write_data(0x82);
        write_data(0x27);
        
        // Enable 3G
        write_command(0xF2); // 3Gamma Function Disable
        write_data(0x00);   // Disable 3 gamma control
        
        // Gamma Set
        write_command(0x26); //Gamma curve selected
        write_data(0x01);   // Gammana curve 1 (G2.2))
        
        // Positive Gamma Correction
        write_command(0xE0); //Set Gamma
        write_data(0x0F);
        write_data(0x31);
        write_data(0x2B);
        write_data(0x0C);
        write_data(0x0E);
        write_data(0x08);
        write_data(0x4E);
        write_data(0xF1);
        write_data(0x37);
        write_data(0x07);
        write_data(0x10);
        write_data(0x03);
        write_data(0x0E);
        write_data(0x09);
        write_data(0x00);
        
        // Negative Gamma Correction
        write_command(0xE1); //Set Gamma
        write_data(0x00);
        write_data(0x0E);
        write_data(0x14);
        write_data(0x03);
        write_data(0x11);
        write_data(0x07);
        write_data(0x31);
        write_data(0xC1);
        write_data(0x48);
        write_data(0x08);
        write_data(0x0F);
        write_data(0x0C);
        write_data(0x31);
        write_data(0x36);
        write_data(0x0F);
        
        write_command(0x11); //Sleep Out
        __delay_ms(10); // It will be necessary to wait 5msec before sending next command
        write_command(0x29); //Display ON
        
        //    //バックライトの実験
        //    write_command(0xBE); //Backlight Control2
        //    write_data(0xFF); //94%
        //    addset(0, 0, LCD_WIDTH-1, LCD_HEIGHT-1);
        
        write_command(0x2c);
        
        LCD_CS_SetHigh();    //CSをHighに
        SPI1_Close();
    }

}


/*
 * LCDディスプレイの初期化
 */
void glcd_init2(void)
{
  
//    SPI1_ExchangeByte(0xff);
    if (spi_master_open(LCD8M)) {
        //ちゃんと開けたら
        LCD_CS_SetHigh();    //CSをHighに
        SPI1_ExchangeByte(0);
        LCD_CS_SetLow();    //CSをLowに
        write_command(0x11); //Sleep Out
        __delay_ms(10); // It will be necessary to wait 5msec before sending next command
        write_command(0x2c);
        LCD_CS_SetHigh();    //CSをHighに
        SPI1_Close();
    }
}


/*
void glcd_array(uint16_t x, uint16_t y, uint16_t xx, uint16_t yy,uint8_t *data)
{
    uint16_t a, b;
    b = 0;

    a = xx * yy * 2;
    LCD_CS_SetLow();    //CSをLowに
    addset(x, y, x+xx-1, y+yy-1);
    while(b<a){
        write_data(data[b+1]);
        write_data(data[b]);
        b += 2;
    }
    LCD_CS_SetHigh();    //CSをHighに
    
}


void glcd_all_p(uint8_t *data)
{
    unsigned long i;
    uint16_t j, k;
    i=0; j=0; k=0;
    
    LCD_CS_SetLow();    //CSをLowに
    addset(0, 0, LCD_WIDTH-1, LCD_HEIGHT-1);
    while(i < 76800){ //240*320
        while(j < data[k]){
            write_data(data[k+2]);
            write_data(data[k+1]);
            j++;
        }      
        j=0;
        i += data[k];  
        k += 3;
    }
    LCD_CS_SetHigh();    //CSをHighに

}
void glcd_all(uint8_t *data)
{
    uint16_t i, j;
    
    LCD_CS_SetLow();    //CSをLowに
    addset(0, 0, LCD_WIDTH-1, LCD_HEIGHT-1);
    j = 0;
    while (j<320) {
        i = 0;
        while(i < 2*240){ //2*240*320
            write_data(data[i]);
            write_data(data[i+1]);
            i += 2; 
        }
        j++;
    }
    LCD_CS_SetHigh();    //CSをHighに

}
*/

// end of library code.

