#ifndef SRC_COMMON_HW_INCLUDE_LCD_H_
#define SRC_COMMON_HW_INCLUDE_LCD_H_

#include "hw_def.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifdef _USE_HW_LCD
#define 			 LCD_MAX_CH		HW_LCD_MAX_CH


#define LCD_COLOR_MONO		1
#define LCD_COLOR_RGB16		2

enum LcdColor
{
	BLACK = 0x00U,
	WHITE = 0x01U,

};






typedef struct
{
	bool 			(*init)(void);
	bool 			(*reset)(void);
	bool			(*setWindow)(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
	uint32_t 	(*getWidth)(void);
	uint32_t 	(*getHeight)(void);
	bool			(*setCallBack)(void (*func)(void));
	bool			(*sendbuffer)(uint8_t *buf);

} lcd_driver_t;





bool lcdInit(void);
bool lcdBegin(uint8_t ch, uint8_t *buffer, uint32_t buff_size);
bool lcdReset(uint8_t ch);
bool lcdUpdateFrame(uint8_t ch);
void lcdDrawRequest(uint8_t ch);
bool lcdDrawAvailable(uint8_t ch);
void lcdClearBuffer(uint8_t ch);
void lcdDrawPixel(uint8_t ch, uint32_t x, uint32_t y, uint8_t color);
void lcdFillScreen(uint8_t ch, uint8_t color);
void lcdPutchar(uint8_t ch, uint32_t x, uint32_t y, char character, uint8_t color);
void lcdPrintf(uint8_t ch, uint32_t x, uint32_t y, uint8_t color, const char *fmt, ...);
void lcdDrawLine(uint8_t ch, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint8_t color);
void lcdDrawTriangle(uint8_t ch, uint32_t x0, uint32_t y0,uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint8_t color);
void lcdDrawRectangle(uint8_t ch, uint32_t x, uint32_t y, uint32_t line_x, uint32_t line_y, uint8_t color);
void lcdDrawCircle(uint8_t ch, uint32_t par_x, uint32_t par_y, uint32_t par_r, uint8_t color);




#endif

#ifdef __cplusplus
}
#endif



#endif /* SRC_COMMON_HW_INCLUDE_LCD_H_ */
