#include "Display.h"

//---------------------------------------------------------------------------
const uint8_t bootProgram[] PROGMEM = {
	0xD5,	// Set Display Clock Divisor 0xF0
	0xF0,
	0x8D,	// Charge Pump Setting 0x14
	0x14,
	0xA1,	// Set Segment Re-map (A0) | (b0001)
	0xC8,	// Set COM Output Scan Direction
	0x81,	// Set Contrast 0xCF
	0xCF,
	0xD9,	// Set Precharge 0xF1
	0xF1,
	0xAF,	// Display On
	0x20,	// Set display mode
	0x00,	// Horizontal addressing mode
};

extern const uint8_t font3x5[] PROGMEM;
uint8_t _displayBuffer[512];

//---------------------------------------------------------------------------
void Display::begin()
{
	color    = BLACK;
	bgcolor  = WHITE;
	textWrap = true;
	fontSize = 1;

	setFont(font3x5);


	SPI.begin();
	SPI.setClockDivider(SPI_CLOCK_DIV2);

	pinMode(SCR_DC,  OUTPUT);
	pinMode(SCR_CS,  OUTPUT);
	pinMode(SCR_RST, OUTPUT);

	// VDD (3.3V) goes high at start, lets just chill for a ms
	digitalWrite(SCR_RST, HIGH);
	delay(1);

	// bring reset low. wait 10ms
	digitalWrite(SCR_RST, LOW);
	delay(10);

	// bring out of reset
	digitalWrite(SCR_RST, HIGH);

	// setup the ports we need to talk to the OLED
	pCs    = portOutputRegister(digitalPinToPort(SCR_CS));
	pDc    = portOutputRegister(digitalPinToPort(SCR_DC));
	csMask = digitalPinToBitMask(SCR_CS);
	dcMask = digitalPinToBitMask(SCR_DC);

	// send boot Program
	command();

	for(uint8_t i=0; i<sizeof(bootProgram); i++)
	{
		SPI.transfer(__LPM(bootProgram + i));
	}

	// screen clear
	data();

	for(uint16_t i=0; i<128*64/8; i++)
	{
		SPI.transfer(0x00);
	}
}
//---------------------------------------------------------------------------
void Display::command()
{
	// command Mode
	*pCs |=  csMask;
	*pDc &= ~dcMask;
	*pCs &= ~csMask;
}
//---------------------------------------------------------------------------
void Display::data()
{
	// data Mode
	*pDc |=  dcMask;
	*pCs &= ~csMask;
}
//---------------------------------------------------------------------------
uint8_t* Display::getBuffer()
{
	return _displayBuffer;
}
//---------------------------------------------------------------------------
void Display::setContrast(uint8_t val)
{
	// EMPTY
}
//---------------------------------------------------------------------------
void Display::clear(void)
{
	memset(_displayBuffer, 0, LCDWIDTH * LCDHEIGHT / 8);
	cursorY = cursorX = 0;
}
//---------------------------------------------------------------------------
void Display::update(void)
{
	frameCount++;

	for(uint8_t page=0; page<6; page++)
	{
		command();

		SPI.transfer(0x22);
		SPI.transfer(1 + page);
		SPI.transfer(7);

		SPI.transfer(0x21);
		SPI.transfer(((128 - 84) / 2) - 1);
		SPI.transfer(127);

		data();

		for(uint8_t col=0; col<=LCDWIDTH-1; col++)
		{
			SPI.transfer(_displayBuffer[(LCDWIDTH * page) + col]);
		}
	}
}
//---------------------------------------------------------------------------
void Display::setColor(int8_t c)
{
	color   = c;
	bgcolor = c;
}
//---------------------------------------------------------------------------
void Display::setColor(int8_t c, int8_t bg)
{
	color   = c;
	bgcolor = bg;
}
//---------------------------------------------------------------------------
void Display::drawPixel(int8_t x, int8_t y)
{
	if((x < 0) || (x >= LCDWIDTH) || (y < 0) || (y >= LCDHEIGHT))
	{
		return;
	}

	byte c = color;

	if(color == INVERT)
	{
		c = !getPixel(x, y);
	}
	else if(color == GRAY)
	{
		// alternative checkers pattern
		if(((frameCount & 0x01) ^ ((x & 0x01) ^ (y & 0x01)) == 0))
		{
			c = WHITE;
		}
		else
		{
			c= BLACK;
		}
	}

	// white or black
	if(c == WHITE)
	{
		_displayBuffer[x + (y / 8) * LCDWIDTH] &= ~_BV(y % 8);
	}
	else
	{
		_displayBuffer[x + (y / 8) * LCDWIDTH] |=  _BV(y % 8);
	}
}
//---------------------------------------------------------------------------
uint8_t Display::getPixel(int8_t x, int8_t y)
{
	if((x < 0) || (x >= LCDWIDTH) || (y < 0) || (y >= LCDHEIGHT))
	{
		return 0;
	}

	return (_displayBuffer[x + (y / 8) * LCDWIDTH] >> (y % 8)) & 0x1;
}
//---------------------------------------------------------------------------
void Display::drawFastVLine(int8_t x, int8_t y, int8_t h)
{
	for(int8_t i=0; i<h; i++)
	{
		drawPixel(x,y+i);
	}
}
//---------------------------------------------------------------------------
void Display::drawFastHLine(int8_t x, int8_t y, int8_t w)
{
	for(int8_t i=0; i<w; i++)
	{
		drawPixel(x+i,y);
	}
}
//---------------------------------------------------------------------------
void Display::drawRect(int8_t x, int8_t y, int8_t w, int8_t h)
{
	drawFastHLine(x, y, w);
	drawFastHLine(x, y + h - 1, w);
	drawFastVLine(x, y, h);
	drawFastVLine(x + w - 1, y, h);
}
//---------------------------------------------------------------------------
void Display::fillRect(int8_t x, int8_t y, int8_t w, int8_t h)
{
	// stupidest version - update in subclasses if desired!
	for(int8_t i = y; i < y + h; i++)
	{
		drawFastHLine(x, i, w);
	}
}
//---------------------------------------------------------------------------
void Display::fillScreen(uint8_t color)
{
	memset(_displayBuffer, 0xFFFF, LCDWIDTH * LCDHEIGHT / 8);
}
//---------------------------------------------------------------------------
void Display::drawLine(int8_t x0, int8_t y0, int8_t x1, int8_t y1)
{
	int8_t steep = abs(y1 - y0) > abs(x1 - x0);

	if(steep)
	{
		swap(x0, y0);
		swap(x1, y1);
	}

	if(x0 > x1)
	{
		swap(x0, x1);
		swap(y0, y1);
	}

	int8_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int8_t err = dx / 2;
	int8_t ystep;

	if(y0 < y1)
	{
		ystep = 1;
	}
	else
	{
		ystep = -1;
	}

	for(; x0<=x1; x0++)
	{
		if(steep)
		{
			drawPixel(y0, x0);
		}
		else
		{
			drawPixel(x0, y0);
		}
		
		err -= dy;

		if(err < 0)
		{
			y0 += ystep;
			err += dx;
		}
	}
}
//---------------------------------------------------------------------------
void Display::drawCircle(int8_t x0, int8_t y0, int8_t r)
{
	int8_t f = 1 - r;
	int8_t ddF_x = 1;
	int8_t ddF_y = -2 * r;
	int8_t x = 0;
	int8_t y = r;

	drawPixel(x0, y0 + r);
	drawPixel(x0, y0 - r);
	drawPixel(x0 + r, y0);
	drawPixel(x0 - r, y0);

	while(x < y)
	{
		if(f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		drawPixel(x0 + x, y0 + y);
		drawPixel(x0 - x, y0 + y);
		drawPixel(x0 + x, y0 - y);
		drawPixel(x0 - x, y0 - y);
		drawPixel(x0 + y, y0 + x);
		drawPixel(x0 - y, y0 + x);
		drawPixel(x0 + y, y0 - x);
		drawPixel(x0 - y, y0 - x);
	}
}
//---------------------------------------------------------------------------
void Display::drawCircleHelper(int8_t x0, int8_t y0, int8_t r, uint8_t cornername)
{
	int8_t f = 1 - r;
	int8_t ddF_x = 1;
	int8_t ddF_y = -2 * r;
	int8_t x = 0;
	int8_t y = r;

	while(x < y)
	{
		if(f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		if(cornername & 0x4)
		{
			drawPixel(x0 + x, y0 + y);
			drawPixel(x0 + y, y0 + x);
		}
		if(cornername & 0x2)
		{
			drawPixel(x0 + x, y0 - y);
			drawPixel(x0 + y, y0 - x);
		}
		if(cornername & 0x8)
		{
			drawPixel(x0 - y, y0 + x);
			drawPixel(x0 - x, y0 + y);
		}
		if(cornername & 0x1)
		{
			drawPixel(x0 - y, y0 - x);
			drawPixel(x0 - x, y0 - y);
		}
	}
}
//---------------------------------------------------------------------------
void Display::fillCircle(int8_t x0, int8_t y0, int8_t r)
{
	drawFastVLine(x0, y0 - r, 2 * r + 1);
	fillCircleHelper(x0, y0, r, 3, 0);
}
//---------------------------------------------------------------------------
// used to do circles and roundrects!
void Display::fillCircleHelper(int8_t x0, int8_t y0, int8_t r, uint8_t cornername, int8_t delta)
{
	int8_t f = 1 - r;
	int8_t ddF_x = 1;
	int8_t ddF_y = -2 * r;
	int8_t x = 0;
	int8_t y = r;

	while (x < y)
	{
		if(f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		if(cornername & 0x1)
		{
			drawFastVLine(x0 + x, y0 - y, 2 * y + 1 + delta);
			drawFastVLine(x0 + y, y0 - x, 2 * x + 1 + delta);
		}

		if(cornername & 0x2)
		{
			drawFastVLine(x0 - x, y0 - y, 2 * y + 1 + delta);
			drawFastVLine(x0 - y, y0 - x, 2 * x + 1 + delta);
		}
	}
}
//---------------------------------------------------------------------------
void Display::drawRoundRect(int8_t x, int8_t y, int8_t w,int8_t h, int8_t r)
{
	// smarter version
	drawFastHLine(x + r, y, w - 2 * r); // Top
	drawFastHLine(x + r, y + h - 1, w - 2 * r); // Bottom
	drawFastVLine(x, y + r, h - 2 * r); // Left
	drawFastVLine(x + w - 1, y + r, h - 2 * r); // Right

	// draw four corners
	drawCircleHelper(x + r, y + r, r, 1);
	drawCircleHelper(x + w - r - 1, y + r, r, 2);
	drawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4);
	drawCircleHelper(x + r, y + h - r - 1, r, 8);
}
//---------------------------------------------------------------------------
void Display::fillRoundRect(int8_t x, int8_t y, int8_t w,int8_t h, int8_t r)
{
	fillRect(x + r, y, w - 2 * r, h);

	// draw four corners
	fillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1);
	fillCircleHelper(x + r, y + r, r, 2, h - 2 * r - 1);
}
//---------------------------------------------------------------------------
void Display::drawTriangle(int8_t x0, int8_t y0, int8_t x1, int8_t y1, int8_t x2, int8_t y2)
{
	drawLine(x0, y0, x1, y1);
	drawLine(x1, y1, x2, y2);
	drawLine(x2, y2, x0, y0);
}
//---------------------------------------------------------------------------
void Display::fillTriangle(int8_t x0, int8_t y0, int8_t x1, int8_t y1, int8_t x2, int8_t y2)
{
	int8_t a, b, y, last;

	// Sort coordinates by Y order (y2 >= y1 >= y0)
	if(y0 > y1)
	{
		swap(y0, y1);
		swap(x0, x1);
	}

	if(y1 > y2)
	{
		swap(y2, y1);
		swap(x2, x1);
	}

	if(y0 > y1)
	{
		swap(y0, y1);
		swap(x0, x1);
	}

	// Handle awkward all-on-same-line case as its own thing
	if(y0 == y2)
	{
		a = b = x0;

		if(x1 < a)
		{
			a = x1;
		}
		else if(x1 > b)
		{
			b = x1;
		}

		if(x2 < a)
		{
			a = x2;
		}
		else if(x2 > b)
		{
			b = x2;
		}

		drawFastHLine(a, y0, b - a + 1);
		return;
	}

	int16_t dx01 = x1 - x0;
	int16_t dy01 = y1 - y0;
	int16_t dx02 = x2 - x0;
	int16_t dy02 = y2 - y0;
	int16_t dx12 = x2 - x1;
	int16_t dy12 = y2 - y1;
	int16_t sa   = 0;
	int16_t sb   = 0;

	// For upper part of triangle, find scanline crossings for segments
	// 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
	// is included here (and second loop will be skipped, avoiding a /0
	// error there), otherwise scanline y1 is skipped here and handled
	// in the second loop...which also avoids a /0 error here if y0=y1
	// (flat-topped triangle).
	if(y1 == y2)
	{
		// Include y1 scanline
		last = y1;
	}
	else
	{
		// Skip it
		last = y1 - 1;
	}

	for(y=y0; y<=last; y++)
	{
		a   = x0 + sa / dy01;
		b   = x0 + sb / dy02;
		sa += dx01;
		sb += dx02;

		if(a > b)
		{
			swap(a, b);
		}

		drawFastHLine(a, y, b - a + 1);
	}

	// For lower part of triangle, find scanline crossings for segments
	// 0-2 and 1-2.  This loop is skipped if y1=y2.
	sa = dx12 * (y - y1);
	sb = dx02 * (y - y0);

	for(; y<=y2; y++)
	{
		a   = x1 + sa / dy12;
		b   = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;

		if(a > b)
		{
			swap(a, b);
		}

		drawFastHLine(a, y, b - a + 1);
	}
}
//---------------------------------------------------------------------------
void Display::drawBitmap(int8_t x, int8_t y, const uint8_t *bitmap)
{
	int8_t w = pgm_read_byte(bitmap);
	int8_t h = pgm_read_byte(bitmap + 1);

	bitmap = bitmap + 2;


	uint8_t * buffer  = getBuffer();
	const uint8_t col = color;
	const uint8_t bw  = (w+7) / 8;

	// clip
	if(x >= LCDWIDTH)  return;
	if(y >= LCDHEIGHT) return;
	if(x + w <= 0)     return;
	if(y + h <= 0)     return;

	if(y < 0)
	{
		h += y;
		bitmap -= bw * y;
		y = 0;
	}

	if(y + h > LCDHEIGHT)
	{
		h = LCDHEIGHT - y;
	}

	uint8_t x1 = max(0, x);
	uint8_t x2 = min(LCDWIDTH, x + w);
	uint8_t g  = y ^ frameCount;

	// draw
	uint8_t first_bitmap_mask = 0x80 >> ((x1 - x) & 7);
	const uint8_t * bitmap_line = bitmap + (x1 - x) / 8;
	uint8_t screen_mask = 0x01 << (y % 8);
	uint8_t * screen_row = buffer + (y / 8) * LCDWIDTH + x1;

	for(uint8_t dy=0; dy<h; dy++, bitmap_line+=bw)
	{
		const uint8_t * bitmap_ptr = bitmap_line;
		uint8_t bitmap_mask = first_bitmap_mask;
		uint8_t pixels = pgm_read_byte(bitmap_ptr);
		uint8_t * dst = screen_row;

		if(col == BLACK)
		{
			for(uint8_t sx=x1; sx<x2; sx++, dst++)
			{
				if(pixels & bitmap_mask)
				{
					*dst |= screen_mask;
				}
				bitmap_mask >>= 1;

				if(!bitmap_mask)
				{
					bitmap_mask = 0x80;
					pixels = pgm_read_byte(++bitmap_ptr);
				}
			}
		}
		else if(col == WHITE)
		{
			uint8_t inv_screen_mask = ~screen_mask;

			for(uint8_t sx=x1; sx<x2; sx++, dst++)
			{
				if(pixels & bitmap_mask)
				{
					*dst &= inv_screen_mask;
				}
				bitmap_mask >>= 1;

				if(!bitmap_mask)
				{
					bitmap_mask = 0x80;
					pixels = pgm_read_byte(++bitmap_ptr);
				}
			}
		}
		else if(col == GRAY)
		{
			uint8_t inv_screen_mask = ~screen_mask;

			for(uint8_t sx=x1; sx<x2; sx++, dst++)
			{
				if(pixels & bitmap_mask)
				{
					if((sx^g) & 1) *dst |= screen_mask;
					else           *dst &= inv_screen_mask;
				}
				bitmap_mask >>= 1;

				if(!bitmap_mask)
				{
					bitmap_mask = 0x80;
					pixels = pgm_read_byte(++bitmap_ptr);
				}
			}

			g ^= 1;
		}
		else // invert
		{
			for(uint8_t sx=x1; sx<x2; sx++, dst++)
			{
				if(pixels & bitmap_mask)
				{
					*dst ^= screen_mask;
				}
				bitmap_mask >>= 1;

				if(!bitmap_mask)
				{
					bitmap_mask = 0x80;
					pixels = pgm_read_byte(++bitmap_ptr);
				}
			}
		}

		screen_mask <<= 1;

		if(!screen_mask)
		{
			screen_mask = 1;
			screen_row += LCDWIDTH;
		}
	}
}
//---------------------------------------------------------------------------
boolean Display::getBitmapPixel(const uint8_t* bitmap, uint8_t x, uint8_t y)
{
	return pgm_read_byte(bitmap+2 + y * ((pgm_read_byte(bitmap)+7)/8) + (x >> 3)) & (B10000000 >> (x % 8));
}
//---------------------------------------------------------------------------
void Display::drawBitmap(int8_t x, int8_t y, const uint8_t *bitmap, uint8_t rotation, uint8_t flip)
{
	if((rotation == NOROT) && (flip == NOFLIP))
	{
		// use the faster algorithm
		drawBitmap(x,y,bitmap);
		return;
	}

	uint8_t w = pgm_read_byte(bitmap);
	uint8_t h = pgm_read_byte(bitmap + 1);

	bitmap = bitmap + 2;


	int8_t i, j; //coordinates in the raw bitmap
	int8_t k, l; //coordinates in the rotated/flipped bitmap
	int8_t byteNum, bitNum, byteWidth = (w + 7) >> 3;

	rotation %= 4;

	for(i=0; i<w; i++)
	{
		byteNum = i / 8;
		bitNum  = i % 8;

		for(j=0; j<h; j++)
		{
			if(pgm_read_byte(bitmap + j * byteWidth + byteNum) & (B10000000 >> bitNum))
			{
				switch(rotation)
				{
				// no rotation
				case NOROT:
					k = i;
					l = j;
					break;

				// 90 counter-clockwise
				case ROTCCW:
					k = j;
					l = w - i - 1;
					break;

				// 180
				case ROT180:
					k = w - i - 1;
					l = h - j - 1;
					break;

				// 90 clockwise
				case ROTCW:
					k = h - j - 1;
					l = i;
					break;
				}

				if(flip)
				{
					flip %= 4;

					 // horizontal flip
					if(flip & B00000001)
					{
						k = w - k - 1;
					}

					// vertical flip
					if(flip & B00000010)
					{
						l = h - l;
					}
				}

				// place the bitmap on the screen
				k += x;
				l += y;
				drawPixel(k, l);
			}
		}
	}
}
//---------------------------------------------------------------------------
size_t Display::write(uint8_t c)
{
	if(c == '\n')
	{
		cursorY += fontSize * fontHeight;
		cursorX = 0;
	}
	else if(c == '\r')
	{
		// skip em
	}
	else
	{
		drawChar(cursorX, cursorY, c, fontSize);
		cursorX += fontSize * fontWidth;

		if(textWrap && (cursorX > (LCDWIDTH - fontSize * fontWidth)))
		{
			cursorY += fontSize * fontHeight;
			cursorX = 0;
		}
	}

	return 1;
}
//---------------------------------------------------------------------------
void Display::setFont(const uint8_t *f)
{
	font = (uint8_t*)f;
	fontWidth = pgm_read_byte(font) + 1;
	fontHeight = pgm_read_byte(font+1) + 1;

	font += 2; //offset the pointer to star after the width and height bytes
}
//---------------------------------------------------------------------------
void Display::drawChar(int8_t x, int8_t y, unsigned char c, uint8_t size)
{
	int8_t tempcolor = color;

	// Clip
	if((x >= LCDWIDTH) || (y >= LCDHEIGHT) || ((x + (fontWidth-1) * size - 1) < 0) || ((y + fontHeight * size - 1) < 0))
	{
		return;
	}

	for(int8_t i=0; i<fontWidth; i++)
	{
		uint8_t line;

		if(i == (fontWidth-1))
		{
			line = 0x0;
		}
		else
		{
			line = pgm_read_byte(font + (c * (fontWidth-1)) + i);
		}

		for(int8_t j=0; j<fontHeight; j++)
		{
			if(line & 0x1)
			{
				if(size == 1)
				{
					// default size
					drawPixel(x + i, y + j);
				}
				else
				{
					// big size
					fillRect(x + (i * size), y + (j * size), size, size);
				}
			}
			else if(bgcolor != color)
			{
				color = bgcolor;
				if(size == 1)
				{
					// default size
					drawPixel(x + i, y + j);
				}
				else
				{
					// big size
					fillRect(x + i*size, y + j*size, size, size);
				}
				//restore color to its initial value
				color = tempcolor;
			}
			line >>= 1;
		}
	}

	//restore color to what it was previously
	color = tempcolor;
}
