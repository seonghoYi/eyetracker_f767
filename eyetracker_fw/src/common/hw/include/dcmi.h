#ifndef SRC_COMMON_HW_INCLUDE_DCMI_H_
#define SRC_COMMON_HW_INCLUDE_DCMI_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "hw_def.h"

#ifdef _USE_HW_DCMI



bool dcmiInit(void);
bool dcmiStart(uint32_t p_data, uint32_t length);
bool dcmiStop(void);
bool dcmiSuspend(void);
bool dcmiResume(void);

#endif

#ifdef __cplusplus
}
#endif

#endif /* SRC_COMMON_HW_INCLUDE_DCMI_H_ */
