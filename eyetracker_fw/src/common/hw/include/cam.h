#ifndef SRC_COMMON_HW_INCLUDE_CAM_H_
#define SRC_COMMON_HW_INCLUDE_CAM_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "hw_def.h"

#ifdef _USE_HW_CAM


typedef struct
{
	bool 			(*init)(uint32_t resolution, uint8_t depth, uint8_t fps);
	bool			(*open)(uint8_t *frame_buffer);
	bool		 	(*release)(void);
	bool 			(*setCallBack)(void (*func)(void));
} cam_driver_t;


typedef struct
{
	bool is_latest_data;
	uint8_t *p_frame;
} frame_t;

bool camInit(void);
bool camOpen(uint32_t resolution, uint8_t color, uint8_t fps, frame_t *frame);
bool camRelease(void);

#endif

#ifdef __cplusplus
}
#endif

#endif /* SRC_COMMON_HW_INCLUDE_CAM_H_ */
