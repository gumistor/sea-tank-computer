/*
 * kho_lcd_ea_freertos.h
 *
 *  Created on: 3 lis 2014
 *      Author: Krzysztof
 */

#ifndef KHO_LCD_EA_FREERTOS_H_
#define KHO_LCD_EA_FREERTOS_H_

#include "FreeRTOS.h"
#include "queue.h"
#include "kho_spi_freertos.h"

#define GPIO_LCD_BACKLIGHT_PIN		2
#define GPIO_LCD_BACKLIGHT_PORT		0

#define LCD_CMD_SWRESET   0x01
#define LCD_CMD_BSTRON    0x03
#define LCD_CMD_SLEEPIN   0x10
#define LCD_CMD_SLEEPOUT  0x11
#define LCD_CMD_INVON     0x21
#define LCD_CMD_SETCON    0x25
#define LCD_CMD_DISPON    0x29
#define LCD_CMD_CASET     0x2A
#define LCD_CMD_PASET     0x2B
#define LCD_CMD_RAMWR     0x2C
#define LCD_CMD_RGBSET    0x2D
#define LCD_CMD_MADCTL    0x36
#define LCD_CMD_COLMOD    0x3A

#define LCD_BLACK		0b0000000000000000
#define LCD_WHITE		0b1111111111111111
#define LCD_RED			0b1111100000000000
#define LCD_BLUE		0b0000000000011111
#define LCD_GREEN		0b0000011111100000
#define LCD_YELLOW		0b1111111111100000
#define LCD_MAGENTA		0b1111100000011111
#define LCD_CYJAN		0b0000011111111111
#define LCD_DARKCYJAN	0b0000000111101111

#define LCD_SMALL 0
#define LCD_MEDIUM 1
#define LCD_LARGE 2

#define MADCTL_HORIZ      0x48
#define MADCTL_VERT       0x68

uint16_t lcd_x, lcd_y;
uint16_t textColor,bkgColor;
uint8_t setcolmark;
uint8_t fontSize;

void initLCD_EA(void);
void lcdBacklightOn(void);
void lcdBacklightOff(void);
void lcdCommand(uint8_t);
void lcdData(uint8_t);
void lcdByte(uint8_t);
void lcdNewline(void);
void lcdPutChar(uint8_t);
void lcdContrast(uint8_t);
void lcdColor(uint16_t, uint16_t);
void lcdWindow(uint8_t, uint8_t, uint8_t, uint8_t);
void lcdClrscr(void);
void lcdPuts(char *);
void lcdPutStr(char *pString);
void lcdPutStrln(char *pString);
void lcdPutStrXY(uint8_t *pString, uint8_t x, uint8_t y, uint8_t Size, uint16_t fColor, uint16_t bColor);
void lcdPutCh(uint8_t c, uint8_t x, uint8_t y, uint8_t size, uint16_t fColor, uint16_t bColor);
void lcdFontSize(uint8_t);

#endif /* KHO_LCD_EA_FREERTOS_H_ */
