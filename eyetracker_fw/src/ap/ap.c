#include "ap.h"



void apInit()
{

	cliOpen(_DEF_UART2, 38400);
}


void apMain()
{
	while(1)
	{
		cliMain();
	}
}
