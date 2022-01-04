#pragma once
#ifndef __USER_5110_H_
#define __USER_5110_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include "spi.h"
#include "lcd_font.h"

/*
 * @brief LCD config
 */
struct LCD_conf {
    SPI_HandleTypeDef *spi;

    GPIO_TypeDef *RST_PORT;
    uint16_t RST_PIN;

    GPIO_TypeDef *CE_PORT;
    uint16_t CE_PIN;

    GPIO_TypeDef *DC_PORT;
    uint16_t DC_PIN;

    GPIO_TypeDef *LED_PORT;
    uint16_t LED_PIN;
};

/**
 * Set LCD spi port
 * @param spi
 */
void LCD_setSPI(SPI_HandleTypeDef *spi);

/**
 * set LCD reset pin.
 * @param PORT
 * @param PIN
 */
void LCD_setRST(GPIO_TypeDef *PORT, uint16_t PIN);

/**
 *  Set Turn on the display GPIO (Use hardware SPI NSS pin or define here)
 * @param PORT
 * @param PIN
 */
void LCD_setCE(GPIO_TypeDef *PORT, uint16_t PIN);

/**
 * Set Data/Command Flag GPIO (Data_Command select pin)
 * @param PORT
 * @param PIN
 */
void LCD_setDC(GPIO_TypeDef *PORT, uint16_t PIN);

/**
 * Set Led GPIO
 * @param PORT
 * @param PIN
 */
void LCD_setLED(GPIO_TypeDef *PORT, uint16_t PIN);

/**
 * LCD INIT function
 */
void LCD_Init(void);

/**
 * LCD RESET function
 */
void LCD_Reset(void);

/**
 * Write LCD command to SPI
 * @param cmd command to write.
 */
void LCD_Write_Command(uint8_t cmd);

/**
 * Write LCD data to SPI
 * @param data data to write.
 */
void LCD_Write_Data(uint8_t data);

/**
 * Set LCD cursor position.
 * @param PosX X Position
 * @param PosY Y Position
 */
void LCD_Set_Position(uint8_t PosX, uint8_t PosY);

/**
 * Clear all contents on LCD.
 */
void LCD_Clear(void);

/**
 * Write a single char to LCD.
 * @param ch char to write.
 */
void LCD_Write_Char(uint8_t ch);

/**
 * Write a string to LCD.
 * @param PosX X start point
 * @param PosY Y start point
 * @param str  string to write.
 */
void LCD_Write_String(uint8_t PosX, uint8_t PosY, char *str);

/**
 * LCD on/off display
 * @param state true - on, false - off
 */
void LCD_Led(bool state);

#ifdef __cplusplus
}
#endif

#endif
