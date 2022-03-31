#ifndef SRC_COMMON_HW_INCLUDE_CAM_OV7670_H_
#define SRC_COMMON_HW_INCLUDE_CAM_OV7670_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "hw_def.h"

#ifdef _USE_HW_OV7670
#define INPUT_CLOCK				8

#include "cam.h"

bool ov7670Init(uint32_t resolution, uint8_t depth, uint8_t fps);
bool ov7670DriverInit(cam_driver_t *driver);



#endif

#ifdef __cplusplus
}
#endif

#endif /* SRC_COMMON_HW_INCLUDE_CAM_OV7670_H_ */
