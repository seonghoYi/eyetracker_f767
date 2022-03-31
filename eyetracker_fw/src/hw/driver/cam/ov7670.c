#include "cam/ov7670.h"
#include "cam/ov7670_regs.h"
#include "i2c.h"
#include "dcmi.h"


#ifdef _USE_HW_OV7670


#define OV7670_ADDRESS			0x42 //0x21


typedef struct regval_list
{
	uint8_t reg;
	uint8_t val;
} regval_list_t;



const regval_list_t ov7670_default_regs[] = {

		{OV7670_REG_DBLV, 0x8A},

		{OV7670_REG_COM10, 0x18}, //reverse PCLK, reverse HREF
		{OV7670_REG_COM7, 0x14}, //QVGA, RGB
		{OV7670_REG_RGB444, 0x00}, //RGB444 disable
		{OV7670_REG_COM15, 0xD0}, //value range 0x00~0xFF, RGB565
		{OV7670_REG_TSLB, 0x0C}, //VYUYOV7670_REG_MVFP
		{OV7670_REG_COM13, 0x88}, //gamma enable, UV swap YUYV

		{OV7670_REG_COM3, 0x04}, //DWC enable
		{OV7670_REG_COM14, 0x19}, //scaling manually PCLK / 2
		{OV7670_REG_SCAILXSC, 0x3A},
		{OV7670_REG_SCAILYSC, 0x35},
		{OV7670_REG_SCAILDCWCTR, 0x11},
		{OV7670_REG_SCAILPCLKDIV, 0xF1},

		{OV7670_REG_HSTART, 0x16},
		{OV7670_REG_HSTOP, 0x04},
		{OV7670_REG_HREF, 0x80},
		{OV7670_REG_VSTART, 0x03},
		{OV7670_REG_VSTOP, 0x7B},
		{OV7670_REG_VREF, 0x0A},


		{OV7670_REG_MTX1, 0xB3},
		{OV7670_REG_MTX2, 0xB3},
		{OV7670_REG_MTX3, 0x00},
		{OV7670_REG_MTX4, 0x3D},
		{OV7670_REG_MTX5, 0xE4},
		{OV7670_REG_MTX6, 0x9E},

		{OV7670_REG_COM16, 0x38},

		{OV7670_REG_GAM1, 16},
		{OV7670_REG_GAM2, 30},
		{OV7670_REG_GAM3, 53},
		{OV7670_REG_GAM4, 90},
		{OV7670_REG_GAM5, 105},
		{OV7670_REG_GAM6, 118},
		{OV7670_REG_GAM7, 130},
		{OV7670_REG_GAM8, 140},
		{OV7670_REG_GAM9, 150},
		{OV7670_REG_GAM10, 160},
		{OV7670_REG_GAM11, 180},
		{OV7670_REG_GAM12, 195},
		{OV7670_REG_GAM13, 215},
		{OV7670_REG_GAM14, 230},
		{OV7670_REG_GAM15, 244},
		{OV7670_REG_SLOP, 16},

		{OV7670_REG_BD50MAX, 0x05},
		{OV7670_REG_BD60MAX, 0x07},
		{OV7670_REG_AEW, 0x95},
		{OV7670_REG_AEB, 0x33},
		{OV7670_REG_VPT, 0xE3},
		{OV7670_REG_HAECC1, 0x78},
		{OV7670_REG_HAECC2, 0x68},
		{OV7670_REG_HAECC3, 0xD8},
		{OV7670_REG_HAECC4, 0xD8},
		{OV7670_REG_HAECC5, 0xF0},
		{OV7670_REG_HAECC6, 0x90},
		{OV7670_REG_HAECC7, 0x94},
		{OV7670_REG_AECH, 0x00},

		{OV7670_REG_AWBC1, 0x0A},
		{OV7670_REG_AWBC2, 0xF0},
		{OV7670_REG_AWBC3, 0x34},
		{OV7670_REG_AWBC4, 0x58},
		{OV7670_REG_AWBC5, 0x28},
		{OV7670_REG_AWBC6, 0x3A},

		{0x59, 0x88},
		{0x5A, 0x88},
		{0x5B, 0x44},
		{0x5C, 0x67},
		{0x5D, 0x49},
		{0x5E, 0x0E},

		{OV7670_REG_AWBCTR3, 0x0A},
		{OV7670_REG_AWBCTR2, 0x55},
		{OV7670_REG_AWBCTR1, 0x11},
		{OV7670_REG_AWBCTR0, 0x9F},
		{OV7670_REG_GGAIN, 0x40},
		{OV7670_REG_BLUE, 0x40},
		{OV7670_REG_RED, 0x60},
		{OV7670_REG_COM8, 0xE7},

		{OV7670_REG_MVFP, 0x03},

		{0xFF, 0xFF},	/* END MARKER */
};

bool sccbRegWrite(uint8_t reg_addr, uint8_t data)
{
	bool ret = false;
	uint8_t tx_data[2];
	tx_data[0] = reg_addr;
	tx_data[1] = data;
	if (i2cWrite(_DEF_I2C1, OV7670_ADDRESS, tx_data, 2) != 0)
	{
		ret = true;
	}
	return ret;
}


bool sccbInit(void)
{
	bool ret = false;
	ret = i2cBegin(_DEF_I2C1, 100);

	return ret;
}


typedef struct
{
	uint32_t res;
	uint8_t depth;
	uint8_t fps;
} ov7670_t;


static ov7670_t ov7670;
static void (*callback)(void) = NULL;

bool ov7670Open(uint8_t *frame_buffer);
bool ov7670Release(void);
bool ov7670SetCallBack(void (*func)(void));

bool ov7670Init(uint32_t resolution, uint8_t depth, uint8_t fps)
{
	bool ret = true;

	ret &= sccbInit();

	for (int i = 0; i < sizeof(ov7670_default_regs)/sizeof(regval_list_t); i++)
	{
		ret &= sccbRegWrite(ov7670_default_regs[0].reg, ov7670_default_regs[1].val);
	}

	ret &= dcmiInit();


	ov7670.res = resolution;
	ov7670.depth = depth;
	ov7670.fps = fps;

	return ret;
}

bool ov7670DriverInit(cam_driver_t *driver)
{
	bool ret = true;

	driver->init 					= ov7670Init;
	driver->open 					= ov7670Open;
	driver->release 			= ov7670Release;
	driver->setCallBack 	= ov7670SetCallBack;

	return ret;
}


bool ov7670Open(uint8_t *frame_buffer)
{
	bool ret = false;

	uint32_t buff_size = ov7670.res * ov7670.depth;

	do
	{
		buff_size /= 2;
	} while(buff_size > 0xFFFF);

	ret = dcmiContinuousStart((uint32_t *)frame_buffer, buff_size);
	return ret;
}


bool ov7670Release(void)
{
	bool ret = false;

	ret = dcmiStop();

	return ret;
}

bool ov7670SetCallBack(void (*func)(void))
{
	bool ret = false;


	callback = func;
	if (callback != NULL)
	{
		ret = true;
	}

	return ret;
}


void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi)
{
	if (callback != NULL)
	{
		callback();
	}
}

#endif
