#pragma once
#ifndef FONT_H
#define FONT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    font_6x8_rus,
    font_6x8_bold,
} LCD_fonts;

void LCD_setFONT(LCD_fonts font);

LCD_fonts LCD_getFont(void);

uint8_t LCD_getChar(uint8_t cr, uint8_t line);

#ifdef __cplusplus
}
#endif

#endif //FONT_H
