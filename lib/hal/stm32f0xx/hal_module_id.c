#include "hal/hal_module_id.h"
#include "stm32f0xx_hal.h"

static uint8_t module_id[8] = {0};

int HalModuleId_init(void) {
  uint32_t w = HAL_GetUIDw0();
  module_id[0] = w;
  module_id[1] = w >> 8;
  module_id[2] = w >> 8 * 2;
  module_id[3] = w >> 8 * 3;

  w = HAL_GetUIDw1();
  module_id[4] = w;
  module_id[5] = w >> 8;
  module_id[6] = w >> 8 * 2;
  module_id[7] = w >> 8 * 3;

  // TODO: the UID for stm32 is 96bit. We need a hash/digest, otherwise the uniqueness is not guaranteed.
//  HAL_GetUIDw2();

  return 0;
}

const uint8_t * HalModuleId_get64BitId(void) {
  return module_id;
}
