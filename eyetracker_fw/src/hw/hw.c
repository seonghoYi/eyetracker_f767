#include "hw.h"




void hwInit()
{
	bspInit();
	usbInit();
	uartInit();
	gpioInit();
	i2cInit();

	dcmiInit();


}
