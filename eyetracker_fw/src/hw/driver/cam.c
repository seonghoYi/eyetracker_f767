#include "cam.h"

#ifdef _USE_HW_OV7670
#include "cam/ov7670.h"
#endif

#ifdef _USE_HW_CAM

uint8_t buffer[_DEF_QVGA*_DEF_IMAGE_DEPTH2];


static cam_driver_t driver;

static frame_t *gp_frame;

DMA_HandleTypeDef hdma_memtomem_dma2_stream0;


static uint32_t fps_time = 0;
static uint32_t fps_pre_time = 0;
uint32_t camera_fps = 0;

void frameCaptureDoneISR(void)
{
	gp_frame->is_latest_data = false;

	fps_time = millis() - fps_pre_time;

	if (HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream0, (uint32_t)&buffer[0], (uint32_t)(gp_frame->p_frame), sizeof(buffer)) != HAL_OK)
	{
		/* Transfer Error */
		Error_Handler();
	}
}


void frameXferDoneISR(DMA_HandleTypeDef *hdma)
{
	gp_frame->is_latest_data = true;
	fps_pre_time = millis();
	camera_fps = 1000 / fps_time;
}


bool camInit(void)
{
	bool ret = true;
#ifdef _USE_HW_OV7670
	ret &= ov7670DriverInit(&driver);

  __HAL_RCC_DMA2_CLK_ENABLE();

  /* Configure DMA request hdma_memtomem_dma2_stream0 on DMA2_Stream0 */
  hdma_memtomem_dma2_stream0.Instance = DMA2_Stream0;
  hdma_memtomem_dma2_stream0.Init.Channel = DMA_CHANNEL_0;
  hdma_memtomem_dma2_stream0.Init.Direction = DMA_MEMORY_TO_MEMORY;
  hdma_memtomem_dma2_stream0.Init.PeriphInc = DMA_PINC_ENABLE;
  hdma_memtomem_dma2_stream0.Init.MemInc = DMA_MINC_ENABLE;
  hdma_memtomem_dma2_stream0.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_memtomem_dma2_stream0.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
  hdma_memtomem_dma2_stream0.Init.Mode = DMA_NORMAL;
  hdma_memtomem_dma2_stream0.Init.Priority = DMA_PRIORITY_LOW;
  hdma_memtomem_dma2_stream0.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
  hdma_memtomem_dma2_stream0.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
  hdma_memtomem_dma2_stream0.Init.MemBurst = DMA_MBURST_INC4;
  hdma_memtomem_dma2_stream0.Init.PeriphBurst = DMA_PBURST_INC4;
  if (HAL_DMA_Init(&hdma_memtomem_dma2_stream0) != HAL_OK)
  {
    Error_Handler();
  }


  HAL_DMA_RegisterCallback(&hdma_memtomem_dma2_stream0, HAL_DMA_XFER_CPLT_CB_ID, frameXferDoneISR);

  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

#endif

	return ret;
}

bool camOpen(uint32_t resolution, uint8_t color, uint8_t fps, frame_t *p_frame)
{
	bool ret = true;

	gp_frame = p_frame;

	ret &= driver.init(resolution, color, fps);
	ret &= driver.setCallBack(frameCaptureDoneISR);

	ret &= driver.open(buffer);


	return ret;
}

bool camRelease(void)
{
	bool ret = true;

	ret &= driver.release();

	return ret;
}



#endif


