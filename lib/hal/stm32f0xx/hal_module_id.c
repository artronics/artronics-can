#include "hal/hal_module_id.h"

static uint8_t module_id[8] = {0};

uint8_t *HalModuleId_init(void) {

}

uint8_t *HalModuleId_64b(void) {
  return module_id;
}
