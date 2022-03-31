#ifndef SRC_COMMON_DEF_H_
#define SRC_COMMON_DEF_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>


#define _DEF_GPIO1							0


#define _DEF_UART1							0
#define _DEF_UART2							1
#define _DEF_UART3							2
#define _DEF_UART4							3

#define _DEF_I2C1								0
#define _DEF_I2C2								1

#define _DEF_LCD1								0
#define _DEF_LCD2								1



#define _DEF_OUTPUT							0
#define _DEF_OUTPUT_PULLUP			1
#define _DEF_OUTPUT_PULLDOWN		2
#define _DEF_INPUT							3
#define _DEF_INPUT_PULLUP				4
#define _DEF_INPUT_PULLDOWN			5

#define _DEF_LOW								0
#define _DEF_HIGH								1



#define _DEF_VGA								(640*480)
#define _DEF_QVGA								(320*240)
#define _DEF_QQVGA							(160*120)

#define _DEF_IMAGE_DEPTH1				1
#define _DEF_IMAGE_DEPTH2				2
#define _DEF_IMAGE_DEPTH3				3


#define constrain(in,low,high) ((in)<(low)?(low):((in)>(high)?(high):(in)))

#ifndef map
#define map(input, in_min, in_max, out_min, out_max) ((input - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)
#endif

#endif /* SRC_COMMON_DEF_H_ */
