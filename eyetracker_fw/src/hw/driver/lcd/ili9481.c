#include "lcd/ili9481.h"
#include "lcd/ili9481_regs.h"
#include "fmc.h"
#include "gpio.h"



#ifdef _USE_HW_ILI9481

#define NOP		asm volatile("NOP")


void delay_us(uint32_t us)
{
	if (us <= 0)
	{
		return;
	}

	uint32_t microsec = us;
	microsec *= SystemCoreClock / 1000000;

	while(microsec--);
}




uint8_t buffer[ILI9481_WIDTH * ILI9481_HEIGHT * LCD_COLOR_RGB16];


void ili9481WriteData(uint8_t data);
void ili9481WriteCommand(uint8_t com);
uint8_t ili9481ReadData(void);
bool ili9481Reset();
bool ili9481SetAddrWindow(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
bool ili9481SetRotation(uint8_t r);

void ili9481SetPixel(uint32_t x, uint32_t y);




void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c)
{
  unsigned int i,j;

/*
  l=l+x;
  ili9481SetAddrWindow(x, y, l, y);
  j=l*2;
  for(i=1;i<=j;i++)
  {
  	ili9481WriteData(c >> 8);
  	ili9481WriteData(c & 0xFF);
  }
*/
  for(int i = x; i < x+l; i++)
	  ili9481SetPixel(i, y);
}

void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c)
{
  unsigned int i,j;

  /*
  l=l+y;
  ili9481SetAddrWindow(x,y,x,l);
  j=l*2;
  for(i=1;i<=j;i++)
  {
	ili9481WriteData(c >> 8);
	ili9481WriteData(c & 0xFF);
  }
  */
  for(int i = y; i < y+l; i++)
	  ili9481SetPixel(x, i);
}


void Rect(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int c)
{
  H_line(x  , y  , w, c);
  H_line(x  , y+h, w, c);
  V_line(x  , y  , h, c);
  V_line(x+w, y  , h, c);
}



bool ili9481Init()
{
	bool ret = true;

	ret &= fmcInit();
	ret &= ili9481Reset();

	ili9481SetAddrWindow(0, 0, ILI9481_WIDTH, ILI9481_HEIGHT);

	for(int i = 0; i < ILI9481_WIDTH * ILI9481_HEIGHT * 2; i++)
	{
		buffer[i] = i%2 ? 0x00 : 0xF8;
	}

	for(int i = 0; i < ILI9481_WIDTH * ILI9481_HEIGHT * 2; i++)
	{
		ili9481WriteData(buffer[i]);
	}

	Rect(50, 50, 50, 50, 0xF800);

	return ret;
}



bool ili9481DriverInit(lcd_driver_t *lcd_driver)
{
	bool ret = true;
	lcd_driver->init 					= ili9481Init;
	lcd_driver->reset 				= ili9481Reset;
	lcd_driver->setWindow 		= ili9481SetAddrWindow;
	lcd_driver->getWidth;
	lcd_driver->getHeight;
	lcd_driver->setCallBack;
	lcd_driver->sendbuffer;

	return ret;
}



bool ili9481Reset()
{
	bool ret = true;

	gpioPinWrite(_DEF_GPIO_LCD_RST, _DEF_HIGH);
	delay(10);
	gpioPinWrite(_DEF_GPIO_LCD_RST, _DEF_LOW);
	delay(100);

	ili9481WriteCommand(ILI9481_COM_EXIT_SLEEP_MODE);
	delay(20);


	ili9481WriteCommand(ILI9481_COM_POWER_SETTING);
	delay(5);
	ili9481WriteData(0x07);
	delay(5);
	ili9481WriteData(0x42);
	delay(5);
	ili9481WriteData(0x18);
	delay(5);

	ili9481WriteCommand(ILI9481_COM_VCOM_CONTROL);
	delay(5);
	ili9481WriteData(0x00);
	delay(5);
	ili9481WriteData(0x07);
	delay(5);
	ili9481WriteData(0x10);
	delay(5);


	ili9481WriteCommand(ILI9481_COM_POWER_SET_FOR_NORMAL_MODE);
	delay(5);
	ili9481WriteData(0x01);
	delay(5);
	ili9481WriteData(0x02);
	delay(5);


	ili9481WriteCommand(ILI9481_COM_PANEL_DRIVING_SETTING);
	delay(5);
	ili9481WriteData(0x10); //important!
	delay(5);
	ili9481WriteData(0x3B);
	delay(5);
	ili9481WriteData(0x00);
	delay(5);
	ili9481WriteData(0x02);
	delay(5);
	ili9481WriteData(0x11);
	delay(5);


	ili9481WriteCommand(ILI9481_COM_FRAME_RATE_INVERSION_CONTROL);
	delay(5);
	ili9481WriteData(0x03);
	delay(5);


	ili9481WriteCommand(ILI9481_COM_SET_ADDRESS_MODE);
	delay(5);
	ili9481WriteData(0x0A);
	delay(5);


	ili9481SetRotation(1);
	delay(5);

	ili9481WriteCommand(ILI9481_COM_SET_PIXEL_FORMAT);
	delay(5);
	ili9481WriteData(0x55);
	delay(5);

	ili9481WriteCommand(ILI9481_COM_SET_COLUMN_ADDRESS);
	delay(5);
	ili9481WriteData(0x00);
	delay(5);
	ili9481WriteData(0x00);
	delay(5);
	ili9481WriteData(0x01);
	delay(5);
	ili9481WriteData(0x3F);
	delay(5);

	ili9481WriteCommand(ILI9481_COM_SET_PAGE_ADDRESS);
	delay(5);
	ili9481WriteData(0x00);
	delay(5);
	ili9481WriteData(0x00);
	delay(5);
	ili9481WriteData(0x01);
	delay(5);
	ili9481WriteData(0xE0);
	delay(5);

	delay(50);


	ili9481WriteCommand(ILI9481_COM_GAMMA_SETTING);
	delay(5);
	ili9481WriteData(0x00);
	delay(5);
	ili9481WriteData(0x32);
	delay(5);
	ili9481WriteData(0x36);
	delay(5);
	ili9481WriteData(0x45);
	delay(5);
	ili9481WriteData(0x06);
	delay(5);
	ili9481WriteData(0x16);
	delay(5);
	ili9481WriteData(0x37);
	delay(5);
	ili9481WriteData(0x75);
	delay(5);
	ili9481WriteData(0x77);
	delay(5);
	ili9481WriteData(0x54);
	delay(5);
	ili9481WriteData(0x0C);
	delay(5);
	ili9481WriteData(0x00);



	delay(120);


	ili9481WriteCommand(ILI9481_COM_SET_DISPLAY_ON);


	return ret;
}


void ili9481WriteData(uint8_t data)
{
	//fmcWriteByte(0x60010000, data);
	*((uint8_t *)0x60010000) = data;
}


void ili9481WriteCommand(uint8_t com)
{
	//fmcWriteByte(0x60000000	, com);
	*((uint8_t *)0x60000000) = com;
}


bool ili9481SetAddrWindow(uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
	ili9481WriteCommand(ILI9481_COM_SET_COLUMN_ADDRESS);
	ili9481WriteData(x >> 8);
	ili9481WriteData(x & 0xFF);
	ili9481WriteData((x + w - 1) >> 8);
	ili9481WriteData((x + w - 1) & 0xFF);
	ili9481WriteCommand(ILI9481_COM_SET_PAGE_ADDRESS);
	ili9481WriteData(y >> 8);
	ili9481WriteData(y & 0xFF);
	ili9481WriteData((y + h - 1) >> 8);
	ili9481WriteData((y + h - 1) & 0xFF);
	ili9481WriteCommand(ILI9481_COM_WRITE_MEMORY_START);
	return true;
}


bool ili9481SetRotation(uint8_t r)
{
	bool ret = true;


	uint8_t com = 0x00;

	//0x20: page/column order
	//0x40: column address order
	//0x80: page address order
	//0x08: RGB order

	switch(r)
	{
	case 0:
		com = 0x80;
		break;
	case 1:
		com = 0x80 | 0x40 | 0x20;
		break;
	case 2:
		com = 0x40;
		break;
	case 3:
		com = 0x20;
		break;
	}

	ili9481WriteCommand(ILI9481_COM_SET_ADDRESS_MODE);
	delay(5);
	ili9481WriteData(com | 0x08);

	return ret;
}


void ili9481SetPixel(uint32_t x, uint32_t y)
{
	ili9481SetAddrWindow(x, y, 1, 1);
	ili9481WriteData(0xF8);
	ili9481WriteData(0x00);

}












#endif
