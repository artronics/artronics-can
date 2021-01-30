#include "hal/hal_module_id.h"

static const uint8_t module_id[8] = {0};

int HalModuleId_init(void) {

}

const uint8_t * HalModuleId_64b(void) {
  return module_id;
}
