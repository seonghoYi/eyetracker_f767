#ifndef SRC_HW_HW_DEF_H_
#define SRC_HW_HW_DEF_H_

#include "def.h"
#include "bsp.h"



#define _USE_HW_USB
#define	_USE_HW_CDC

#define _USE_HW_UART
#define 		 HW_UART_MAX_CH					2

#define _USE_HW_I2C
#define 		 HW_I2C_MAX_CH					1


//#define _USE_HW_FMC


#define _USE_HW_GPIO
#define 		 HW_GPIO_MAX_CH					1


//#define _USE_HW_SSD1306
#define 		 HW_SSD1306_WIDTH				128
#define			 HW_SSD1306_HEIGHT				64


//#define _USE_HW_ILI9481
#define			 HW_ILI9481_WIDTH				480
#define 		 HW_ILI9481_HEIGHT				320


//#define _USE_HW_LCD
#define 		 HW_LCD_MAX_CH					1

#define _USE_HW_DCMI

#define _USE_HW_OV7670

#define _USE_HW_CAM




#endif /* SRC_HW_HW_DEF_H_ */
