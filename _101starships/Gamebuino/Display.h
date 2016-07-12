#ifndef DISPLAY_H
#define	DISPLAY_H

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <SPI.h>
#include "settings.c"

// colors
#define WHITE				0
#define BLACK				1
#define INVERT				2
#define GRAY				3

// for extended bitmap function
#define NOROT				0
#define ROTCCW				1
#define ROT180				2
#define ROTCW				3
#define NOFLIP				0
#define FLIPH				1
#define FLIPV				2
#define FLIPVH				3

// LCD size
#define LCDWIDTH			84
#define LCDHEIGHT			48

#define swap(a, b)			{ int8_t t = a; a = b; b = t; }


class Display : public Print
{
public:
	void     begin();

	void     command();
	void     data();

	uint8_t* getBuffer();
	void     setContrast(uint8_t val);
	void     clear(void);
	void     update();

	void     setColor(int8_t c);
	void     setColor(int8_t c, int8_t bg);
	void     drawPixel(int8_t x, int8_t y);
	uint8_t  getPixel(int8_t x, int8_t y);

	void     drawLine(int8_t x0, int8_t y0, int8_t x1, int8_t y1);
	void     drawFastVLine(int8_t x, int8_t y, int8_t h);
	void     drawFastHLine(int8_t x, int8_t y, int8_t w);
	void     drawRect(int8_t x, int8_t y, int8_t w, int8_t h);
	void     fillRect(int8_t x, int8_t y, int8_t w, int8_t h);
	void     fillScreen(uint8_t color);

	void     drawCircle(int8_t x0, int8_t y0, int8_t r);
	void     drawCircleHelper(int8_t x0, int8_t y0, int8_t r, uint8_t cornername);
	void     fillCircle(int8_t x0, int8_t y0, int8_t r);
	void     fillCircleHelper(int8_t x0, int8_t y0, int8_t r, uint8_t cornername, int8_t delta);

	void     drawTriangle(int8_t x0, int8_t y0, int8_t x1, int8_t y1, int8_t x2, int8_t y2);
	void     fillTriangle(int8_t x0, int8_t y0, int8_t x1, int8_t y1, int8_t x2, int8_t y2);
	void     drawRoundRect(int8_t x0, int8_t y0, int8_t w, int8_t h, int8_t radius);
	void     fillRoundRect(int8_t x0, int8_t y0, int8_t w, int8_t h, int8_t radius);

	void     drawBitmap(int8_t x, int8_t y, const uint8_t *bitmap);
	void     drawBitmap(int8_t x, int8_t y, const uint8_t *bitmap, uint8_t rotation, uint8_t flip);
	boolean  getBitmapPixel(const uint8_t* bitmap, uint8_t x, uint8_t y);

	void     setFont(const uint8_t* f);
	void     drawChar(int8_t x, int8_t y, unsigned char c, uint8_t size);

	virtual  size_t write(uint8_t);

	// disable clean() at each frame if true
	boolean  persistence;
	// If set, 'wrap' text at right edge of
	boolean  textWrap;
	uint8_t  fontSize;
	uint8_t  fontWidth, fontHeight;
	int8_t   cursorX, cursorY;
	byte     contrast;
	byte     frameCount;

private:
	uint8_t* font;
	uint8_t  color, bgcolor;

	volatile uint8_t* pCs;
	volatile uint8_t* pDc;
	uint8_t           csMask;
	uint8_t           dcMask;

};

#endif
