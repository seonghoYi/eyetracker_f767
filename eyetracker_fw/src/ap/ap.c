#include "ap.h"


static uint8_t image[_DEF_QVGA *_DEF_IMAGE_DEPTH2];
static frame_t captured_frame;

extern uint32_t camera_fps;

void apInit()
{

	cliOpen(_DEF_UART2, 38400);
	//cliOpenLog(_DEF_UART1, 38400);
	uartOpen(_DEF_UART1, 38400);
	//uartOpen(_DEF_UART2, 38400);
	//ov7670Init();

	captured_frame.is_latest_data = false;
	captured_frame.p_frame = image;

	camOpen(_DEF_QVGA, _DEF_IMAGE_DEPTH2, 30, &captured_frame);
}


void apMain()
{
	uint32_t pre_time = millis();
	while(1)
	{
		if (millis() - pre_time >= 500)
		{
			//uartPrintf(_DEF_UART1, "fps: %d\r\n", camera_fps);
			gpioPinToggle(_DEF_GPIO1);
			pre_time = millis();
		}

		if (captured_frame.is_latest_data == true)
		{
			uartWrite(_DEF_UART1, captured_frame.p_frame, sizeof(image));
		}



		cliMain();
	}
}


