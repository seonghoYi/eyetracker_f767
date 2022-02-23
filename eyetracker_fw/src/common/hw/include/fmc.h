#ifndef SRC_COMMON_HW_INCLUDE_FMC_H_
#define SRC_COMMON_HW_INCLUDE_FMC_H_


#ifdef __cplusplus
extern "C" {
#endif


#include "hw_def.h"


#ifdef _USE_HW_FMC




bool fmcInit(void);
uint8_t fmcReadByte(uint32_t address);
bool fmcWriteByte(uint32_t address, uint8_t data);
uint32_t fmcReadBytes(uint32_t address, uint8_t *p_data, uint32_t length);
uint32_t fmcWriteBytes(uint32_t address, uint8_t *p_data, uint32_t length);




#endif

#ifdef __cplusplus
}
#endif

#endif /* SRC_COMMON_HW_INCLUDE_FMC_H_ */
