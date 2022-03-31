#include "i2c.h"
#include "cli.h"

#ifdef _USE_HW_I2C


#ifdef _USE_HW_CLI
static void cliI2C(cli_args_t *args);
#endif


I2C_HandleTypeDef hi2c1;




typedef struct
{
	bool is_init;
	I2C_HandleTypeDef *p_hi2c;
} i2c_tbl_t;


i2c_tbl_t i2c_tbl[I2C_MAX_CH];



bool i2cInit()
{
	for(int i = 0; i < I2C_MAX_CH; i++)
	{
		i2c_tbl[i].is_init = false;
		i2c_tbl[i].p_hi2c = NULL;
	}

#ifdef _USE_HW_CLI
	cliAdd("i2c", cliI2C);
#endif

	return true;
}

bool i2cBegin(uint8_t ch, uint32_t freq_khz)
{
	UNUSED(freq_khz);

	bool ret = true;

	i2c_tbl_t *p_i2c = &i2c_tbl[ch];


	switch(ch)
	{
		case _DEF_I2C1:
			p_i2c->p_hi2c = &hi2c1;
			p_i2c->p_hi2c->Instance = I2C1;
			p_i2c->p_hi2c->Init.Timing = 0x10606AA0;
			p_i2c->p_hi2c->Init.OwnAddress1 = 0;
			p_i2c->p_hi2c->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
			p_i2c->p_hi2c->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
			p_i2c->p_hi2c->Init.OwnAddress2 = 0;
			p_i2c->p_hi2c->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
			p_i2c->p_hi2c->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

			HAL_I2C_DeInit(&hi2c1);
		  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
		  {
		  	ret = false;
		    Error_Handler();
		  }

		  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
		  {
		  	ret = false;
		    Error_Handler();
		  }

		  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
		  {
		  	ret = false;
		    Error_Handler();
		  }
		break;
		default:
		break;
	}


	return ret;
}

bool i2cIsDeviceReady(uint8_t ch, uint8_t dev_addr)
{
  bool ret = true;
  I2C_HandleTypeDef *p_handle = i2c_tbl[ch].p_hi2c;


	if (HAL_I2C_IsDeviceReady(p_handle, (uint16_t)(dev_addr << 1), 3, 100) != HAL_OK)
	{
		ret = false;
	}

	return ret;
}

uint32_t i2cWrite(uint8_t ch, uint16_t dev_addr, uint8_t *p_data, uint16_t size)
{
	uint32_t ret = 0;

	I2C_HandleTypeDef *p_handle = i2c_tbl[ch].p_hi2c;

	if(HAL_I2C_Master_Transmit(p_handle, dev_addr, p_data, size, 100) == HAL_OK)
	{
		ret = size;
	}
	return ret;
}

uint32_t i2cRead(uint8_t ch, uint16_t dev_addr, uint8_t *p_data, uint16_t size)
{
	uint32_t ret = 0;

	I2C_HandleTypeDef *p_handle = i2c_tbl[ch].p_hi2c;

	if(HAL_I2C_Master_Receive(p_handle, dev_addr, p_data, size, 100) == HAL_OK)
	{
		ret = size;
	}

	return ret;
}

uint32_t i2cMemWrite(uint8_t ch, uint16_t dev_addr, uint16_t mem_addr, uint16_t mem_addr_size, uint8_t *p_data, uint16_t size)
{
	uint32_t ret = 0;

	I2C_HandleTypeDef *p_handle = i2c_tbl[ch].p_hi2c;

	if(HAL_I2C_Mem_Write(p_handle, dev_addr, mem_addr, mem_addr_size, p_data, size, 100) == HAL_OK)
	{
		ret = size;
	}
	return ret;
}

uint32_t i2cMemRead(uint8_t ch, uint16_t dev_addr, uint16_t mem_addr, uint16_t mem_addr_size, uint8_t *p_data, uint16_t size)
{
	uint32_t ret = 0;

	I2C_HandleTypeDef *p_handle = i2c_tbl[ch].p_hi2c;

	if(HAL_I2C_Mem_Read(p_handle, dev_addr, mem_addr, mem_addr_size, p_data, size, 100) == HAL_OK)
	{
		ret = size;
	}
	return ret;
}



void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */

  /* USER CODE END I2C1_MspInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* I2C1 clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();
  /* USER CODE BEGIN I2C1_MspInit 1 */

  /* USER CODE END I2C1_MspInit 1 */
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspDeInit 0 */

  /* USER CODE END I2C1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C1_CLK_DISABLE();

    /**I2C1 GPIO Configuration
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_9);

  /* USER CODE BEGIN I2C1_MspDeInit 1 */

  /* USER CODE END I2C1_MspDeInit 1 */
  }
}


#ifdef _USE_HW_CLI
void cliI2C(cli_args_t *args)
{
  bool ret = true;
  bool i2c_ret;

  uint8_t print_ch;
  uint8_t ch;
  uint16_t dev_addr;
  uint16_t reg_addr;
  uint16_t length;

  uint32_t i;
  uint8_t i2c_data[128];
  uint32_t pre_time;



	//i2c scan


  if (args->argc == 2)
  {
    print_ch = (uint16_t) args->getData(1);

    print_ch = constrain(print_ch, 1, I2C_MAX_CH);
    print_ch -= 1;

    if(args->isStr(0, "scan") == true)
    {
      for (i=0x00; i<= 0x7F; i++)
      {
        if (i2cIsDeviceReady(print_ch, i) == true)
        {
          cliPrintf("I2C CH%d Addr 0x%X : OK\r\n", print_ch+1, i);
        }
      }
    }
    else if(args->isStr(0, "begin") == true)
    {
      i2c_ret = i2cBegin(print_ch, 400);
      if (i2c_ret == true)
      {
        cliPrintf("I2C CH%d Begin OK\r\n", print_ch + 1);
      }
      else
      {
        cliPrintf("I2C CH%d Begin Fail\r\n", print_ch + 1);
      }
    }
  }
  else if (args->argc == 5)
  {
    print_ch = (uint16_t) args->getData(1);
    print_ch = constrain(print_ch, 1, I2C_MAX_CH);

    dev_addr = (uint16_t) args->getData(2);
    reg_addr = (uint16_t) args->getData(3);
    length   = (uint16_t) args->getData(4);
    ch       = print_ch - 1;

    if(args->isStr(0, "read") == true)
    {
      for (i=0; i<length; i++)
      {
        i2c_ret = i2cMemRead(ch, dev_addr, reg_addr+i, 1, i2c_data, 1);

        if (i2c_ret == true)
        {
          cliPrintf("%d I2C - 0x%02X : 0x%02X\r\n", print_ch, reg_addr+i, i2c_data[0]);
        }
        else
        {
          cliPrintf("%d I2C - Fail \r\n", print_ch);
          break;
        }
      }
    }
    else if(args->isStr(0, "write") == true)
    {
      pre_time = millis();
      i2c_ret = i2cMemWrite(ch, dev_addr, reg_addr, 1, (uint8_t *)&length, 1);

      if (i2c_ret == true)
      {
        cliPrintf("%d I2C - 0x%02X : 0x%02X, %d ms\r\n", print_ch, reg_addr, length, millis()-pre_time);
      }
      else
      {
        cliPrintf("%d I2C - Fail \r\n", print_ch);
      }
    }
    else
    {
      ret = false;
    }
  }
  else
  {
    ret = false;
  }

  if (ret == false)
  {
    cliPrintf( "i2c begin channel[1~%d]\r\n", I2C_MAX_CH);
    cliPrintf( "i2c scan channel[1~%d]\r\n", I2C_MAX_CH);
    cliPrintf( "i2c read channel dev_addr reg_addr length\r\n");
    cliPrintf( "i2c write channel dev_addr reg_addr data\r\n");
  }
}

#endif




#endif
