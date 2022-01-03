#include "lcd_5110.h"

/**
 * @brief lcd config
 */
struct LCD_conf conf;

/**
 * Set LCD spi port
 * @param spi
 */
void LCD_setSPI(SPI_HandleTypeDef *spi) {
    conf.spi = spi;
}

/**
 * set LCD reset pin.
 * @param PORT
 * @param PIN
 */
void LCD_setRST(GPIO_TypeDef *PORT, uint16_t PIN) {
    conf.RST_PORT = PORT;
    conf.RST_PIN = PIN;
}

/**
 *  Set Turn on the display GPIO (Use hardware SPI NSS pin or define here)
 * @param PORT
 * @param PIN
 */
void LCD_setCE(GPIO_TypeDef *PORT, uint16_t PIN) {
    conf.CE_PORT = PORT;
    conf.CE_PIN = PIN;
}

/**
 * Set Data/Command Flag GPIO (Data_Command select pin)
 * @param PORT
 * @param PIN
 */
void LCD_setDC(GPIO_TypeDef *PORT, uint16_t PIN) {
    conf.DC_PORT = PORT;
    conf.DC_PIN = PIN;
}

/**
 * Set Led GPIO
 * @param PORT
 * @param PIN
 */
void LCD_setLED(GPIO_TypeDef *PORT, uint16_t PIN) {
    conf.LED_PORT = PORT;
    conf.LED_PIN = PIN;
}

void LCD_Config() {
    LCD_Write_Command(0x21);      // switch to advanced mode
    LCD_Write_Command(0xD0);      // D0 Bias - works like contrast
    LCD_Write_Command(0x20);      // switch normal mode

    /**
     * 0b1100 - normal mode
     * 0b1101 - invert mode
     * 0b1001 - fully lit screen
     * 0b1000 - clear screen
     */
    LCD_Write_Command(0x0C);
}

void LCD_Config_advanced() {
    LCD_Write_Command(0x21);      // switch to advanced mode
    LCD_Write_Command(0xC1);
    LCD_Write_Command(0x00);      // D0 Bias 0b0001 0xxx - works like contrast
    LCD_Write_Command(0x20);      // switch normal mode

    /**
     * 0b1100 - normal mode
     * 0b1101 - invert mode
     * 0b1001 - fully lit screen
     * 0b1000 - clear screen
     */
    LCD_Write_Command(0x0C);
}

/**
 * LCD INIT function
 */
void LCD_Init() {
    LCD_Reset();
    //LCD_Config();
    LCD_Config_advanced();
    LCD_Clear();
}

/**
 * LCD RESET function
 */
void LCD_Reset(void) {
    HAL_GPIO_WritePin(conf.RST_PORT, conf.RST_PIN, GPIO_PIN_RESET);
    HAL_Delay(50);
    HAL_GPIO_WritePin(conf.RST_PORT, conf.RST_PIN, GPIO_PIN_SET);
}

/**
 * Set LCD cursor position.
 * @param PosX X Position
 * @param PosY Y Position
 */
void LCD_Set_Position(uint8_t PosX, uint8_t PosY) {
    LCD_Write_Command(0x40 | PosY);
    LCD_Write_Command(0x80 | PosX);
}

/**
 * Clear all contents on LCD.
 */
void LCD_Clear() {
    uint8_t t;
    uint8_t k;

    LCD_Set_Position(0, 0);

    for (t = 0; t < 6; t++) {
        for (k = 0; k < 84; k++) {
            LCD_Write_Data(0x00);
        }
    }
}

/**
 * Write a single char to LCD.
 * @param ch char to write.
 */
void LCD_Write_Char(uint8_t ch) {
    uint8_t line;

    if (ch == 208 || ch == 209) {
        return;
    }

    for (line = 0; line < 6; line++) {
        LCD_Write_Data(LCD_getChar(ch, line));
    }
}

/**
 * Write a string to LCD.
 * @param PosX X start point
 * @param PosY Y start point
 * @param str  string to write.
 */
void LCD_Write_String(uint8_t PosX, uint8_t PosY, char *str) {
    LCD_Set_Position(PosX, PosY);
    while (*str) {
        LCD_Write_Char(*str);
        str++;
    }
}

/**
 * Write LCD command to SPI
 * @param cmd command to write.
 */
void LCD_Write_Command(uint8_t cmd) {
    HAL_GPIO_WritePin(conf.CE_PORT, conf.CE_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(conf.DC_PORT, conf.DC_PIN, GPIO_PIN_RESET);
    HAL_SPI_Transmit(conf.spi, &cmd, 0x01, 50);
    HAL_GPIO_WritePin(conf.CE_PORT, conf.CE_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(conf.DC_PORT, conf.DC_PIN, GPIO_PIN_SET);
}

/**
 * Write LCD data to SPI
 * @param data data to write.
 */
void LCD_Write_Data(uint8_t data) {
    HAL_GPIO_WritePin(conf.CE_PORT, conf.CE_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(conf.DC_PORT, conf.DC_PIN, GPIO_PIN_SET);
    HAL_SPI_Transmit(conf.spi, &data, 0x01, 50);
    HAL_GPIO_WritePin(conf.CE_PORT, conf.CE_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(conf.CE_PORT, conf.CE_PIN, GPIO_PIN_RESET);
}

/**
 * LCD on/off display
 * @param state true - on, false - off
 */
void LCD_Led(bool state) {
    GPIO_PinState pinState = GPIO_PIN_RESET;

    if (state == true) {
        pinState = GPIO_PIN_SET;
    }

    HAL_GPIO_WritePin(conf.LED_PORT, conf.LED_PIN, pinState);
}