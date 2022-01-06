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

/**
 * @brief 0b00000000 no operation
 */
#define    PCD8544_NOP                              0x00

/**
 * @brief 00100 PD V H
 * PD -power down control; V -entry mode; H - extended instruction set control
 */
#define     PCD8544_FUNCTION_SET                    0x20
#define     PCD8544_PD_ACTIVE                       (PCD8544_FUNCTION_SET | 0x00)           // chip is active
#define     PCD8544_PD_POWER_DOWN                   (PCD8544_FUNCTION_SET | 0x04)           // chip is in Power-down mode
#define     PCD8544_HORIZONTAL_ADDRESSING           (PCD8544_FUNCTION_SET | 0x00)
#define     PCD8544_VERTICAL_ADDRESSING             (PCD8544_FUNCTION_SET | 0x02)
#define     PCD8544_NORMAL_MODE                     (PCD8544_FUNCTION_SET | 0x00)           // use basic instruction set
#define     PCD8544_ADVANCED_MODE                   (PCD8544_FUNCTION_SET | 0x01)           // use extended instruction set

/**
 * @brief sets display configuration
 */
#define     PCD8544_DISPLAY_CONTROL                 0x08                                    // 00001 D 0 E
#define     PCD8544_DISPLAY_BLANK                   (PCD8544_DISPLAY_CONTROL | 0x00)
#define     PCD8544_DISPLAY_NORMAL                  (PCD8544_DISPLAY_CONTROL | 0x04)
#define     PCD8544_DISPLAY_SEGMENTS                (PCD8544_DISPLAY_CONTROL | 0x01)
#define     PCD8544_DISPLAY_INVERSE                 (PCD8544_DISPLAY_CONTROL | 0x05)

/**
 * @brief sets Y-address of RAM; 0 ≤ Y ≤ 5
 */
#define     PCD8544_SET_Y                           0x40                                    // 01000 Y2 Y1 Y0

/**
 * @brief sets X-address part of RAM; 0 ≤ X ≤ 83
 */
#define     PCD8544_SET_X                           0x80                                    // 1 X6 X5 X4 X3 X2 X1 X0

/**
 * @brief set Temperature Coefficient (TCx)
 */
#define     PCD8544_TEMPERATURE_CONTROL             0x04                                    // 000000 TC1 TC0
#define     PCD8544_TEMPERATURE_COEFFICIENT0        (PCD8544_TEMPERATURE_CONTROL | 0x00)
#define     PCD8544_TEMPERATURE_COEFFICIENT1        (PCD8544_TEMPERATURE_CONTROL | 0x01)
#define     PCD8544_TEMPERATURE_COEFFICIENT2        (PCD8544_TEMPERATURE_CONTROL | 0x02)
#define     PCD8544_TEMPERATURE_COEFFICIENT3        (PCD8544_TEMPERATURE_CONTROL | 0x03)

#define     PCD8544_BIAS_SYSTEM                     0x10                                    // 00010 BS2 BS1 BS0
#define     PCD8544_BIAS7                           (PCD8544_BIAS_SYSTEM | 0x00)
#define     PCD8544_BIAS6                           (PCD8544_BIAS_SYSTEM | 0x01)
#define     PCD8544_BIAS5                           (PCD8544_BIAS_SYSTEM | 0x02)
#define     PCD8544_BIAS4                           (PCD8544_BIAS_SYSTEM | 0x03)
#define     PCD8544_BIAS3                           (PCD8544_BIAS_SYSTEM | 0x04)
#define     PCD8544_BIAS2                           (PCD8544_BIAS_SYSTEM | 0x05)
#define     PCD8544_BIAS1                           (PCD8544_BIAS_SYSTEM | 0x06)
#define     PCD8544_BIAS0                           (PCD8544_BIAS_SYSTEM | 0x07)

#define     PCD8544_VOP                             0x80                                    // 1 VOP6 VOP5 VOP4 VOP3 VOP2 VOP1 VOP0


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

struct LCD_cursor {
    uint8_t LCD_cursor_x;
    uint8_t LCD_cursor_y;
};

typedef enum {
    LCD_STATUS_ERROR = 0b0,
    LCD_STATUS_SUCCESS = 0b1,
} LCD_ErrorStatus;

typedef enum {
    LCD_LED_STATUS_OFF = 0b0,
    LCD_LED_STATUS_ON = 0b1,
} LCD_LedStatus;

/**
 * Set LCD spi port
 * @param spi
 */
void LCD_setSPI(SPI_HandleTypeDef *spi);

/**
 * Set LCD reset pin.
 * @param PORT
 * @param PIN
 */
void LCD_setRST(GPIO_TypeDef *PORT, uint16_t PIN);

/**
 * Set Turn on the display GPIO (Use hardware SPI NSS pin or define here)
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
 * @return LCD_ErrorStatus
 */
LCD_ErrorStatus LCD_Write_Command(uint8_t cmd);

/**
 * Write LCD data to SPI
 * @param data data to write.
 * @return LCD_ErrorStatus
 */
LCD_ErrorStatus LCD_Write_Data(uint8_t data);

/**
 * Set LCD X cursor position.
 * @param x Position 0 ≤ x ≤ 83
 */
void LCD_Set_X_Position(uint8_t x);

/**
 * Set LCD Y cursor position.
 * @param y Position 0 ≤ y ≤ 5
 */
void LCD_Set_Y_Position(uint8_t y);

/**
 * Set LCD cursor position.
 * @param x Position
 * @param y Position
 */
void LCD_Set_Position(uint8_t x, uint8_t y);

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
 * @param x start point
 * @param y start point
 * @param str string to write.
 */
void LCD_Write_String(uint8_t x, uint8_t y, char *str);

/**
 * LCD on/off display
 * @param status LCD_LED_STATUS_OFF/LCD_LED_STATUS_ON
 */
void LCD_Led(LCD_LedStatus status);

/**
 *  Get cursor x position
 * @return cursor x position
 */
uint8_t LCD_get_cursorX();

/**
 *  Get cursor y position
 * @return cursor y position
 */
uint8_t LCD_get_cursorY();

#ifdef __cplusplus
}
#endif

#endif
