
#include "lcd_font.h"
#include "font6x8_bold.h"
#include "font_6x8_rus.h"

LCD_fonts lcd_font;

void LCD_setFONT(LCD_fonts font) {
    lcd_font = font;
}

LCD_fonts LCD_getFont(void) {

    if (lcd_font == 0) {
        lcd_font = font_6x8_rus;
    }

    return lcd_font;
}

uint8_t LCD_getChar(uint8_t cr, uint8_t line) {
    uint8_t chars;

    switch (LCD_getFont()) {
        case font_6x8_bold:
            chars = font_6X8_bold[cr][line];
            break;
        case font_6x8_rus:
            chars = font_6X8_rus[cr][line];
            break;
    }

    return chars;
}