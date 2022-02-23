#ifndef SRC_COMMON_HW_INCLUDE_LCD_SSD1306_H_
#define SRC_COMMON_HW_INCLUDE_LCD_SSD1306_H_


#include "hw_def.h"


#ifdef __cplusplus
extern "C" {
#endif


#ifdef _USE_HW_SSD1306
#define 			 SSD1306_WIDTH			HW_SSD1306_WIDTH
#define 			 SSD1306_HEIGHT			HW_SSD1306_HEIGHT
#define				 SSD1306_NUM_PAGE		8


#include "lcd.h"




bool ssd1306Init(void);
bool ssd1306DriverInit(lcd_driver_t *lcd_driver);






#endif


#ifdef __cplusplus
}
#endif
#endif /* SRC_COMMON_HW_INCLUDE_LCD_SSD1306_H_ */
