#include "hw.h"




void hwInit()
{
	bspInit();
	usbInit();
	uartInit();
	cliInit();
	gpioInit();
	i2cInit();

	camInit();


}
