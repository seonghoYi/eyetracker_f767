#include <lcd/ssd1306.h>
#include "i2c.h"





#ifdef _USE_HW_SSD1306


#define _DEF_SSD1306						_DEF_I2C1
#define _DEF_SSD1306_I2C_ADDR		(0x3C << 1)






static bool is_init = false;
static void (*frameCallBack)(void) = NULL;

bool 			ssd1306Reset(void);
bool 			ssd1306SetWindow(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
uint32_t 	ssd1306GetWidth(void);
uint32_t 	ssd1306GetHeight(void);
bool			ssd1306SendBuffer(uint8_t *buf);
bool 			ssd1306SetCallBack(void (*func)(void));


bool ssd1306WriteCommand(uint8_t cmd)
{
	bool ret = false;
	if(i2cMemWrite(_DEF_SSD1306, _DEF_SSD1306_I2C_ADDR, 0x00, 1, &cmd, 1) != 0)
	{
		ret = true;
	}

	return ret;
}

bool ssd1306WriteData(uint8_t *buf)
{
	bool ret = true;
	for (int i = 0; i < SSD1306_NUM_PAGE; i++)
	{
		ret &= ssd1306WriteCommand(0xB0 + i);
		ret &= ssd1306WriteCommand(0x00);
		ret &= ssd1306WriteCommand(0x10);

		if(i2cMemWrite(_DEF_SSD1306, _DEF_SSD1306_I2C_ADDR, 0x40, 1, &buf[SSD1306_WIDTH * i], SSD1306_WIDTH) == 0)
		{
			ret = false;
		}
	}
	return ret;
}




bool ssd1306Init(void)
{
	ssd1306Reset();
	is_init = true;
	return true;
}


bool ssd1306DriverInit(lcd_driver_t *lcd_driver)
{
	lcd_driver->init 				= ssd1306Init;
	lcd_driver->reset 			= ssd1306Reset;
	lcd_driver->setWindow		= ssd1306SetWindow;
	lcd_driver->getWidth 		= ssd1306GetWidth;
	lcd_driver->getHeight 	= ssd1306GetHeight;
	lcd_driver->setCallBack = ssd1306SetCallBack;
	lcd_driver->sendbuffer 	= ssd1306SendBuffer;

	return true;
}


bool ssd1306Reset(void)
{
	bool ret = true;

	ret &= i2cBegin(_DEF_I2C1, 100);


	ret &= ssd1306WriteCommand(0xAE); //Off display

	ret &= ssd1306WriteCommand(0x20); //Set memory addressing mode
	ret &= ssd1306WriteCommand(0x02); //00b: Horizontal addressing mode 01b: Vertical addressing mode 10b: Page addressing mode

	ret &= ssd1306WriteCommand(0xB0); //Set page start address

	ret &= ssd1306WriteCommand(0xC8); //Set COM output direction

	ret &= ssd1306WriteCommand(0x00); //Set low column address
	ret &= ssd1306WriteCommand(0x10); //Set high column address

	ret &= ssd1306WriteCommand(0x40); //Set start line address

	ret &= ssd1306WriteCommand(0x81);
	ret &= ssd1306WriteCommand(0x7F); //Set contrast control

	ret &= ssd1306WriteCommand(0xA1); //Set segment re-map

	ret &= ssd1306WriteCommand(0xA6); //Set normal color

	ret &= ssd1306WriteCommand(0xA8); //Set multiplex ratio
	ret &= ssd1306WriteCommand(0x3F);

	ret &= ssd1306WriteCommand(0xA4); //Set display RAM content

	ret &= ssd1306WriteCommand(0xD3); //Set display offset
	ret &= ssd1306WriteCommand(0x00); //Offset

	ret &= ssd1306WriteCommand(0xD5); //Set display clock divide ratio
	ret &= ssd1306WriteCommand(0xF0); //Set divide ratio

	ret &= ssd1306WriteCommand(0xD9); //Set precharge period
	ret &= ssd1306WriteCommand(0x22);

	ret &= ssd1306WriteCommand(0xDA);
	ret &= ssd1306WriteCommand(0x12); //Set COM pins hardware config

	ret &= ssd1306WriteCommand(0xDB); //Set vcomh
	ret &= ssd1306WriteCommand(0x20); //0.77vcc

	ret &= ssd1306WriteCommand(0x8D); //Set DC-DC enable
	ret &= ssd1306WriteCommand(0x14);

	ret &= ssd1306WriteCommand(0xAF); //On display




	return ret;
}

bool ssd1306SetWindow(uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
	return false;
}

uint32_t ssd1306GetWidth()
{
	uint32_t ret = 0;

	return ret;
}

uint32_t ssd1306GetHeight()
{
	uint32_t ret = 0;

	return ret;
}

bool ssd1306SendBuffer(uint8_t *buf)
{
	bool ret = true;

	ret &= ssd1306WriteData(buf);

	if (frameCallBack != NULL)
	{
		frameCallBack();
	}
	return ret;
}


bool ssd1306SetCallBack(void (*func)(void))
{
	bool ret = true;
	if (func == NULL)
	{
		frameCallBack = func;
	}

	return ret;
}




#endif
