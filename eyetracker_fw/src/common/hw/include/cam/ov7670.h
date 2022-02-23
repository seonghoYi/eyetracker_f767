#ifndef SRC_COMMON_HW_INCLUDE_CAM_OV7670_H_
#define SRC_COMMON_HW_INCLUDE_CAM_OV7670_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "hw_def.h"

#ifdef _USE_HW_OV7670

bool ov7670Init(void);
bool ov7670DriverInit();



#endif

#ifdef __cplusplus
}
#endif

#endif /* SRC_COMMON_HW_INCLUDE_CAM_OV7670_H_ */
