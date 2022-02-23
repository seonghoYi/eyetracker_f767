#ifndef SRC_COMMON_HW_INCLUDE_LCD_ILI9481_H_
#define SRC_COMMON_HW_INCLUDE_LCD_ILI9481_H_



#ifdef __cplusplus
extern "C" {
#endif

#include "hw_def.h"

#ifdef _USE_HW_ILI9481
#define 			 ILI9481_WIDTH						HW_ILI9481_WIDTH
#define				 ILI9481_HEIGHT						HW_ILI9481_HEIGHT


#include "lcd.h"




bool ili9481Init(void);
bool ili9481DriverInit(lcd_driver_t *lcd_driver);









#endif

#ifdef __cplusplus
}
#endif
#endif /* SRC_COMMON_HW_INCLUDE_LCD_ILI9481_H_ */
