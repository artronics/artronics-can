#ifndef ARTRONICS_HAL_MODULE_ID_H
#define ARTRONICS_HAL_MODULE_ID_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

uint8_t *HalModuleId_init(void);
uint8_t *HalModuleId_64b(void);

#ifdef __cplusplus
}
#endif
#endif //ARTRONICS_HAL_MODULE_ID_H
