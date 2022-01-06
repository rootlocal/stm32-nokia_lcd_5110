#include "lcd_5110.h"

/**
 * @brief lcd config
 */
struct LCD_conf conf;
struct LCD_cursor cursor = {0};

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
    LCD_Write_Command(PCD8544_ADVANCED_MODE);
    LCD_Write_Command(PCD8544_VOP | 0x10);
    LCD_Write_Command(PCD8544_PD_ACTIVE);
    LCD_Write_Command(PCD8544_DISPLAY_NORMAL);
    LCD_Write_Command(PCD8544_NORMAL_MODE);
    LCD_Write_Command(PCD8544_DISPLAY_NORMAL);
}

/**
 * LCD INIT function
 */
void LCD_Init() {
    LCD_Reset();
    LCD_Config();
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
 * Set LCD X cursor position.
 * @param x Position 0 ≤ x ≤ 83
 */
void LCD_Set_X_Position(uint8_t x) {
    if (x >= 0 && x <= 83) {
        cursor.LCD_cursor_x = x;
        LCD_Write_Command(PCD8544_SET_X | x);
    }
}

/**
 * Set LCD Y cursor position.
 * @param y Position 0 ≤ y ≤ 5
 */
void LCD_Set_Y_Position(uint8_t y) {
    if (y >= 0 && y <= 5) {
        cursor.LCD_cursor_y = y;
        LCD_Write_Command(PCD8544_SET_Y | y);
    }
}

/**
 * Set LCD cursor position.
 * @param x Position
 * @param y Position
 */
void LCD_Set_Position(uint8_t x, uint8_t y) {
    LCD_Set_Y_Position(y);
    LCD_Set_X_Position(x);
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
            LCD_Write_Data(PCD8544_NOP);
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

    if (ch == '\n') {
        LCD_Set_Position(0, cursor.LCD_cursor_y + 1);
        return;
    }

    if (ch == '\t') {
        ch = ' ';
    }

    for (line = 0; line < 6; line++) {
        LCD_Write_Data(LCD_getChar(ch, line));
    }

}

/**
 * Write a string to LCD.
 * @param x start point
 * @param y start point
 * @param str string to write.
 */
void LCD_Write_String(uint8_t x, uint8_t y, char *str) {

    LCD_Set_Position(x, y);

    while (*str) {
        LCD_Write_Char(*str);
        str++;
    }
}

/**
 * Write LCD command to SPI
 * @param cmd command to write.
 */
LCD_ErrorStatus LCD_Write_Command(uint8_t cmd) {
    HAL_StatusTypeDef HAL_Status;

    HAL_GPIO_WritePin(conf.CE_PORT, conf.CE_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(conf.DC_PORT, conf.DC_PIN, GPIO_PIN_RESET);
    HAL_Status = HAL_SPI_Transmit(conf.spi, &cmd, 0x01, 50);
    HAL_GPIO_WritePin(conf.CE_PORT, conf.CE_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(conf.DC_PORT, conf.DC_PIN, GPIO_PIN_SET);

    if (HAL_Status != HAL_OK) {
        return LCD_STATUS_ERROR;
    }

    return LCD_STATUS_SUCCESS;
}

/**
 * Write LCD data to SPI
 * @param data data to write.
 */
LCD_ErrorStatus LCD_Write_Data(uint8_t data) {
    HAL_StatusTypeDef HAL_Status;

    HAL_GPIO_WritePin(conf.CE_PORT, conf.CE_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(conf.DC_PORT, conf.DC_PIN, GPIO_PIN_SET);
    HAL_Status = HAL_SPI_Transmit(conf.spi, &data, 0x01, 50);
    HAL_GPIO_WritePin(conf.CE_PORT, conf.CE_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(conf.CE_PORT, conf.CE_PIN, GPIO_PIN_RESET);

    if (HAL_Status != HAL_OK) {
        return LCD_STATUS_ERROR;
    }

    return LCD_STATUS_SUCCESS;
}

/**
 * LCD on/off display
 * @param status LCD_LED_STATUS_OFF/LCD_LED_STATUS_ON
 */
void LCD_Led(LCD_LedStatus status) {
    GPIO_PinState pinState = GPIO_PIN_RESET;

    if (status == LCD_LED_STATUS_ON) {
        pinState = GPIO_PIN_SET;
    }

    HAL_GPIO_WritePin(conf.LED_PORT, conf.LED_PIN, pinState);
}

/**
 *  Get cursor x position
 * @return cursor x position
 */
uint8_t LCD_get_cursorX() {
    return cursor.LCD_cursor_x;
}

/**
 * Get cursor y position
 * @return cursor y position
 */
uint8_t LCD_get_cursorY() {
    return cursor.LCD_cursor_y;
}

