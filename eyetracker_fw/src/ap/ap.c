#include "ap.h"



void apInit()
{


}


void apMain()
{
	while(1)
	{
		gpioPinToggle(_DEF_GPIO1);
		delay(1000);
	}
}
