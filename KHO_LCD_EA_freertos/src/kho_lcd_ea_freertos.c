/*
 * kho_lcd_ea_freertos.c
 *
 *  Created on: 3 lis 2014
 *      Author: Krzysztof
 */

#include "kho_lcd_ea_freertos.h"
#include "KHO_LCD_ascii.h"
#include "task.h"

void initLCD_EA(void) {
	lcdCommand(LCD_CMD_SWRESET);

	vTaskDelay(configTICK_RATE_HZ / 2);
	lcdCommand(LCD_CMD_SLEEPOUT);
	lcdCommand(LCD_CMD_DISPON);
	lcdCommand(LCD_CMD_BSTRON);
	vTaskDelay(configTICK_RATE_HZ / 2);
	lcdCommand(LCD_CMD_MADCTL);   //Memory data acces control
	lcdData(MADCTL_HORIZ);    //X Mirror and BGR format
	lcdCommand(LCD_CMD_COLMOD);   //Colour mode
	lcdData(0x05);            //256 colour mode select
	lcdCommand(LCD_CMD_INVON);    //Non Invert mode
	lcdCommand(LCD_CMD_RGBSET);   //LUT write
	lcdData(0);               //Red
	lcdData(2);
	lcdData(4);
	lcdData(6);
	lcdData(9);
	lcdData(11);
	lcdData(13);
	lcdData(15);
	lcdData(0);               //Green
	lcdData(2);
	lcdData(4);
	lcdData(6);
	lcdData(9);
	lcdData(11);
	lcdData(13);
	lcdData(15);
	lcdData(0);               //Blue
	lcdData(6);
	lcdData(10);
	lcdData(15);

	lcd_x = 1;
	lcd_y = 1;
	textColor = 0xFFFF;
	bkgColor = 0x0000;
	fontSize = LCD_MEDIUM;
	//vTaskDelay(configTICK_RATE_HZ / 2);
	Chip_IOCON_PinMux(LPC_IOCON, GPIO_LCD_BACKLIGHT_PORT, GPIO_LCD_BACKLIGHT_PIN,0x10,IOCON_FUNC0);
	//Chip_IOCON_EnableOD(LPC_IOCON, GPIO_LCD_BACKLIGHT_PORT, GPIO_LCD_BACKLIGHT_PIN);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, GPIO_LCD_BACKLIGHT_PORT, GPIO_LCD_BACKLIGHT_PIN);
	//Chip_GPIO_WriteDirBit(LPC_GPIO, GPIO_LCD_BACKLIGHT_PORT, GPIO_LCD_BACKLIGHT_PIN, true);
}

void lcdBacklightOn(void) {
	Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_LCD_BACKLIGHT_PORT, GPIO_LCD_BACKLIGHT_PIN, true);
}

void lcdBacklightOff(void) {
	Chip_GPIO_WritePortBit(LPC_GPIO, GPIO_LCD_BACKLIGHT_PORT, GPIO_LCD_BACKLIGHT_PIN, false);
}

void lcdCommand(uint8_t data)
{
	spi_struct data_to_send;

	data_to_send.data_to_send[0] = 0x0000 | data;
	data_to_send.size_of_send = 1;
	data_to_send.size_of_received = 0;
	xQueueSendToBack(xWrite_SPIQueue, &data_to_send, 0);
}

void lcdData(uint8_t data)
{
	spi_struct data_to_send;

	data_to_send.data_to_send[0] = 0x0100 | data;
	data_to_send.size_of_send = 1;
	data_to_send.size_of_received = 0;
	xQueueSendToBack(xWrite_SPIQueue, &data_to_send, 0);
}
/*
void lcdByte(uint8_t data)
{
  if (data <= 127)
  {
	  uint32_t  mapOffset;
	  uint8_t  i,j,byteToShift;

	  data -= 30;
	  mapOffset = 14*data;

	  lcdCommand(LCD_CMD_CASET);
	  lcdData(lcd_x+2);
	  lcdData(lcd_x+9);
	  lcdCommand(LCD_CMD_PASET);
	  lcdData(lcd_y+2);
	  lcdData(lcd_y+15);
	  lcdCommand(LCD_CMD_RAMWR);

	  for(i=0; i<14; i++)
	  {
		  byteToShift = charMap[mapOffset++];
		  for(j=0; j<8; j++)
		  {
			  if (byteToShift & 0x80)
			  {
				  lcdData((0xFF00 & textColor)>>8);
				  lcdData(0x00FF & textColor);
			  }
			  else
			  {
				  lcdData((0xFF00 & bkgColor)>>8);
				  lcdData(0x00FF & bkgColor);
			  }
			  byteToShift <<= 1;
		  }
	  }
  }
  lcd_x += 8;
}
*/
void lcdNewline(void) {
	lcd_x  = 1;
	if(fontSize == LCD_LARGE)
		{
			if(lcd_y < 115) lcd_y	+= 16;
		}
	else
		{
			if(lcd_y < 123) lcd_y	+= 8;
		}

}
/*
void lcdPutChar(uint8_t data) {
	if (data == '\n')
		lcdNewline();
	else if (data != '\r')
	{
		if (setcolmark == 2)
		{
			textColor = data << 8;
			setcolmark--;
		}
		else if (setcolmark == 1)
		{
			textColor = textColor | data;
			setcolmark--;
		}
		else if (data == 0xff)
			setcolmark = 2;
		else if (lcd_x <= 124)
		{
			lcdByte(data);
		}
	}
}
*/
void lcdContrast(uint8_t cont) {
	lcdCommand(LCD_CMD_SETCON);
	lcdData(cont);
}

void lcdColor(uint16_t bkg, uint16_t text) {
  bkgColor  = bkg;
  textColor = text;
}

void lcdWindow(uint8_t xp, uint8_t yp, uint8_t xe, uint8_t ye) {
	lcdCommand(LCD_CMD_CASET);    //set X
	lcdData(xp);
	lcdData(xe);
	lcdCommand(LCD_CMD_PASET);    //set Y
	lcdData(yp);
	lcdData(ye);
}

void lcdClrscr(void) {
  uint32_t i;

  lcd_x = 1;
  lcd_y = 1;

  lcdWindow(0,0,131,131);
  lcdCommand(LCD_CMD_RAMWR);    //write memory

  for(i=0; i<17424; i++) {
	  lcdData((0xFF00 & bkgColor)>>8);
	  lcdData(0x00FF & bkgColor);
  }
}
/*
void lcdPuts(char *s) {
	  while(*s != '\0')
		  lcdPutChar(*s++);
}
*/
void lcdPutCh(uint8_t c, uint8_t x, uint8_t y, uint8_t size, uint16_t fColor, uint16_t bColor) {
	uint16_t i,j;
	uint8_t nCols;
	uint8_t nRows;
	uint8_t nBytes;
	uint8_t PixelRow;
	uint8_t Mask;
	uint8_t *pFont;
	uint8_t *pChar;
	uint8_t *FontTable[] = {(uint8_t *)FONT6x8, (uint8_t *)FONT8x8, (uint8_t *)FONT8x16};
	if (c == '\n')
		lcdNewline();
	else if (c != '\r')
	{
		textColor = fColor;
		bkgColor = bColor;
		fontSize = size;
		// get pointer to the beginning of the selected font table
		pFont = (uint8_t *)FontTable[fontSize];
		// get the nColumns, nRows and nBytes
		nCols = *pFont;
		nRows = *(pFont + 1);
		nBytes = *(pFont + 2);
		lcd_y = y;
		if(lcd_y > 131) { lcd_y = 1; }
		lcd_x = x + nCols;
		if(lcd_x > 131) { lcd_x = 1; }
		// get pointer to the last byte of the desired character
		pChar = pFont + (nBytes * (c - 0x1F));// + nBytes - 1;
		// Row address set (command 0x2B)
		lcdWindow(x,y,x + nCols - 1,y + nRows - 1);
		// WRITE MEMORY
		lcdCommand(LCD_CMD_RAMWR);
		// loop on each row, working backwards from the bottom to the top
		for (i = 0; i < nRows ; i++) {
			// copy pixel row from font table and then decrement row
			PixelRow = *pChar++;
			// loop on each pixel in the row (left to right)
			// Note: we do two pixels each loop
			Mask = 0x80;
			for (j = 0; j < nCols; j++) {
				// if pixel bit set, use foreground color; else use the background color
				// now get the pixel color for two successive pixels
				if ((PixelRow & Mask) == 0)
				{
					lcdData((0xFF00 & bkgColor)>>8);
					lcdData(0x00FF & bkgColor);
				}
				else
				{
					lcdData((0xFF00 & textColor)>>8);
					lcdData(0x00FF & textColor);
				}
				Mask = Mask >> 1;
			}
		}
	// terminate the Write Memory command
	//WriteSpiCommand(NOP);
	}
}

void lcdPutStr(char *pString) {
	// loop until null-terminator is seen
	while (*pString != 0x00) {
		// draw the character
		lcdPutCh(*pString++, lcd_x, lcd_y, fontSize, textColor, bkgColor);
	}
}

void lcdPutStrln(char *pString) {
	// loop until null-terminator is seen
	while (*pString != 0x00) {
		// draw the character
		lcdPutCh(*pString++, lcd_x, lcd_y, fontSize, textColor, bkgColor);
	}
	lcdNewline();
}

void lcdPutStrXY(uint8_t *pString, uint8_t x, uint8_t y, uint8_t Size, uint16_t fColor, uint16_t bColor) {
	// loop until null-terminator is seen
	//lcd_x = x;
	//lcd_y = y;
	//fontSize = Size;
	//textColor = fColor;
	//bkgColor = bColor;
	while (*pString != 0x00) {
		// draw the character
		lcdPutCh(*pString++, x, y, Size, fColor, bColor);
		// advance the y position
		if (Size == LCD_SMALL)
			x = x + 6;
		else if (Size == LCD_MEDIUM)
			x = x + 8;
		else
			x = x + 8;
		// bail out if y exceeds 131
		if (x > 131) break;
	}
}

void lcdFontSize(uint8_t size) {
	fontSize = size;
}
