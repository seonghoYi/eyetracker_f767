#include "lcd.h"
#include "lcd/lcd_fonts.h"


#ifdef _USE_HW_LCD



#ifdef _USE_HW_SSD1306
#include "lcd/ssd1306.h"
static lcd_driver_t ssd1306_driver;
uint8_t ssd1306_buffer[SSD1306_WIDTH * SSD1306_HEIGHT / SSD1306_NUM_PAGE];
#endif


#ifdef _USE_HW_ILI9481
#include "lcd/ili9481.h"
static lcd_driver_t ili9481_driver;
//uint8_t ili9481_buffer[ILI9481_WIDTH * ILI9481_HEIGHT * LCD_COLOR_RGB16];
#endif


typedef struct
{
	uint32_t pos_x;
	uint32_t pos_y;
} lcd_cursor_t;

typedef struct
{
	bool is_init;

	bool request_draw;
	lcd_driver_t *driver;
	lcd_font_t *font;

	uint8_t *buffer;
	uint32_t buff_len;


	uint32_t fps_time;
	uint32_t fps_pre_time;
	uint32_t fps_count;

} lcd_tbl_t;

lcd_tbl_t lcd_tbl[LCD_MAX_CH] = {
		//{false, false, &ssd1306_driver, &font_07x10, &ssd1306_buffer[0], SSD1306_WIDTH * SSD1306_HEIGHT / SSD1306_NUM_PAGE, 0, 0, 0},
		{false, false, &ili9481_driver, &font_07x10, NULL, 0, 0, 0, 0},
};


#ifdef _USE_HW_SSD1306
void ssd1306DoneISR(void)
{
	lcd_tbl[_DEF_LCD1].fps_time = millis() - lcd_tbl[_DEF_LCD1].fps_pre_time;
	lcd_tbl[_DEF_LCD1].fps_pre_time = millis();

	if (lcd_tbl[_DEF_LCD1].fps_time > 0)
	{
		lcd_tbl[_DEF_LCD1].fps_count = 1000 / lcd_tbl[_DEF_LCD1].fps_time;
	}

	lcd_tbl[_DEF_LCD1].request_draw = false;
	lcdClearBuffer(_DEF_LCD1);
}
#endif




bool lcdInit()
{
	bool ret = true;

#ifdef _USE_HW_SSD1306
	ret &= ssd1306DriverInit(lcd_tbl[_DEF_LCD1].driver);
	ret &= lcd_tbl[_DEF_LCD1].driver->setCallBack(ssd1306DoneISR);
	ret &= lcdReset(_DEF_LCD1);
#endif


#ifdef _USE_HW_ILI9481
	ret &= ili9481DriverInit(lcd_tbl[_DEF_LCD1].driver);
	ret &= lcdReset(_DEF_LCD1);

#endif

	if (ret)
	{
		for (int i = 0; i < LCD_MAX_CH; i++)
		{
			lcd_tbl[i].is_init = true;
		}
	}

	return ret;
}


bool lcdBegin(uint8_t ch, uint8_t *buffer, uint32_t buff_size)
{
	if (ch < 0 || ch > LCD_MAX_CH) return false;

	lcd_tbl[ch].buffer = &buffer[0];
	lcd_tbl[ch].buff_len = buff_size;

	return true;
}

bool lcdReset(uint8_t ch)
{
	bool ret = true;

	switch(ch)
	{
	case _DEF_LCD1:
#ifdef _USE_HW_SSD1306
	ret &= lcd_tbl[ch].driver->init();
	lcdFillScreen(ch, BLACK);
	ret &= lcdUpdateFrame(ch);



	break;
	case _DEF_LCD2:
#endif
	ret &= lcd_tbl[ch].driver->init();

	break;
	}



	return ret;
}



bool lcdUpdateFrame(uint8_t ch)
{
	bool ret = true;

	switch(ch)
	{
	case _DEF_LCD1:
#ifdef _USE_HW_SSD1306
	lcdDrawRequest(ch);
	ret &= lcd_tbl[ch].driver->sendbuffer((uint8_t *)&lcd_tbl[ch].buffer[0]);

	break;
	case _DEF_LCD2:
#endif

	break;

	return ret;
	}


	return ret;
}


void lcdDrawRequest(uint8_t ch)
{
	switch(ch)
	{
	case _DEF_LCD1:
#ifdef _USE_HW_SSD1306
	lcd_tbl[ch].request_draw = true;;
	break;
	case _DEF_LCD2:
#endif

	break;
	}

}


bool lcdDrawAvailable(uint8_t ch)
{
	bool ret = false;

	switch(ch)
	{
	case _DEF_LCD1:
#ifdef _USE_HW_SSD1306
	ret = lcd_tbl[ch].request_draw;
	break;
	case _DEF_LCD2:
#endif

	break;
	}
	return ret;
}


void lcdClearBuffer(uint8_t ch)
{
	switch(ch)
	{
	case _DEF_LCD1:
#ifdef _USE_HW_SSD1306
	memset(&lcd_tbl[ch].buffer[0], 0, SSD1306_WIDTH * SSD1306_HEIGHT / SSD1306_NUM_PAGE);
	break;
	case _DEF_LCD2:
#endif

	break;
	}
}

void lcdDrawPixel(uint8_t ch, uint32_t x, uint32_t y, uint8_t color)
{
#ifdef _USE_HW_SSD1306
	if(x > SSD1306_WIDTH || y > SSD1306_HEIGHT) return;


	if (color)
	{
		ssd1306_buffer[x + SSD1306_WIDTH * (y / SSD1306_NUM_PAGE)] |= 1<<(y%8);
	}
	else
	{
		ssd1306_buffer[x + SSD1306_WIDTH * (y / SSD1306_NUM_PAGE)] &= ~(1<<(y%8));
	}
#endif

}

void lcdFillScreen(uint8_t ch, uint8_t color)
{
#ifdef _USE_HW_SSD1306
	for(int i = 0; i < SSD1306_WIDTH; i++)
	{
		for(int j = 0; j < SSD1306_HEIGHT; j++)
		{
			lcdDrawPixel(ch, i, j, color);
		}
	}
#endif
}



void lcdPutchar(uint8_t ch, uint32_t x, uint32_t y, char character, uint8_t color)
{
	if(character < 0x20 || character > 0x7E) return;

	lcd_font_t *font = lcd_tbl[ch].font;

#ifdef _USE_HW_SSD1306
	uint32_t b;


	for(int i = 0; i < font->height; i++)
	{
		b = font->data[(character-0x20) * font->height + i];
		for(int j = 0; j < font->width; j++)
		{

			if((b << j) & 0x8000)
			{
				lcdDrawPixel(ch, x + j, y + i, color);
			}
		}
	}
#endif
}


void lcdPrintf(uint8_t ch, uint32_t x, uint32_t y, uint8_t color, const char *fmt, ...)
{
	va_list arg;
	va_start (arg, fmt);
	int32_t len;
	lcd_font_t *font = lcd_tbl[ch].font;

	char print_buffer[256];

	uint32_t pre_x = x;

	len = vsnprintf(print_buffer, 255, fmt, arg);
	va_end (arg);

#ifdef _USE_HW_SSD1306
	for(int i = 0; i < len; i++)
	{
		lcdPutchar(ch, x, y, print_buffer[i], color);
		x+=font->width;

		if(x + font->width > SSD1306_WIDTH)
		{
			x = pre_x;
			y += font->height;
		}
	}
#endif
}



void lcdDrawLine(uint8_t ch, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint8_t color)
{

	int32_t delta_x = abs(x0 - x1);
	int32_t delta_y = abs(y0 - y1);

	int32_t sign_x = ((x0 < x1) ? 1 : -1);
	int32_t sign_y = ((y0 < y1) ? 1 : -1);

	int32_t error = delta_x - delta_y;
	int32_t error2;

#ifdef _USE_HW_SSD1306
	while((x0!=x1) || (y0!=y1))
	{
		lcdDrawPixel(ch, x0, y0, color);
		error2 = error * 2;
		if(error2 > -delta_y)
		{
			error -= delta_y;
			x0 += sign_x;
		}

		if(error2 < delta_x)
		{
			error += delta_x;
			y0 += sign_y;
		}
	}
#endif
}


void lcdDrawTriangle(uint8_t ch, uint32_t x0, uint32_t y0,uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint8_t color)
{

	lcdDrawLine(ch, x0, y0, x1, y1, color);
	lcdDrawLine(ch, x1, y1, x2, y2, color);
	lcdDrawLine(ch, x2, y2, x0, y0, color);
}


void lcdDrawRectangle(uint8_t ch, uint32_t x, uint32_t y, uint32_t line_x, uint32_t line_y, uint8_t color)
{
	lcdDrawLine(ch, x, y, x + line_x, y, color);
	lcdDrawLine(ch, x + line_x, y, x + line_x, y + line_y, color);
	lcdDrawLine(ch, x + line_x, y + line_y, x, y + line_y, color);
	lcdDrawLine(ch, x, y + line_y, x, y, color);
}


void lcdDrawCircle(uint8_t ch, uint32_t par_x, uint32_t par_y, uint32_t par_r, uint8_t color)
{
	int32_t x = -par_r;
	int32_t y = 0;
	int32_t err = 2 - 2 * par_r;
	int32_t e2;
#ifdef _USE_HW_SSD1306
	if (par_x >= SSD1306_WIDTH || par_y >= SSD1306_HEIGHT) {
		return;
	}

		do {
			lcdDrawPixel(ch, par_x - x, par_y + y, color);
			lcdDrawPixel(ch, par_x + x, par_y + y, color);
			lcdDrawPixel(ch, par_x + x, par_y - y, color);
			lcdDrawPixel(ch, par_x - x, par_y - y, color);
			e2 = err;
			if (e2 <= y)
			{
				y++;
				err = err + (y * 2 + 1);
				if(-x == y && e2 <= x)
				{
					e2 = 0;
				}
			}
			if(e2 > x)
			{
				x++;
				err = err + (x * 2 + 1);
			}
		} while(x <= 0);
#endif
}




#endif
