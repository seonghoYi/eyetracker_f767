#ifndef SRC_BSP_BSP_H_
#define SRC_BSP_BSP_H_

#include "def.h"

#include "stm32f7xx_hal.h"

void bspInit();

uint32_t millis();
void delay(uint32_t ms);

void Error_Handler(void);


#endif /* SRC_BSP_BSP_H_ */
