#ifndef SRC_COMMON_HW_INCLUDE_CAM_H_
#define SRC_COMMON_HW_INCLUDE_CAM_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "hw_def.h"

#ifdef _USE_HW_CAM


typedef struct
{
	bool 			(*init)(uint8_t resolution, uint8_t format, uint8_t fps);
	bool			(*open)(void);
	bool		 	(*release)(void);
	uint32_t 	(*getWidth)(void);
	uint32_t 	(*getHeight)(void);
	bool			(*setCallBack)(void (*func)(void));
	bool			(*sendbuffer)(uint8_t *buf);
} cam_driver_t;




#endif

#ifdef __cplusplus
}
#endif

#endif /* SRC_COMMON_HW_INCLUDE_CAM_H_ */
