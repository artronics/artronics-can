#include "message_processor.h"
#include "hal/hal_module_id.h"
#include "message.h"

static RingBufferHandler can_tx_h;

void module_id(void);

int MessageProcessor_init(RingBufferHandler can_tx_h) {
  HalModuleId_init();

  return 0;
}

void MessageProcessor_process(CanFrame *frame) {
  switch (frame->id) {
    case ModuleId:
      module_id();
      break;
    default:
      break;
  }
}

void module_id(void) {
  uint8_t *id = HalModuleId_get64BitId();
}
