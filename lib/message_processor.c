#include "message_processor.h"
#include "hal/hal_module_id.h"
#include "message.h"

static RingBufferHandler can_tx_h;

void module_id(void);

int MessageProcessor_init(const RingBufferHandler _can_tx_h) {
  can_tx_h = _can_tx_h;
  HalModuleId_init();

  return 0;
}

void MessageProcessor_process(const CanFrame * const frame) {
  switch (frame->id) {
    case ModuleId:
      module_id();
      break;
    default:
      break;
  }
}

void module_id(void) {
  const uint8_t * const id = HalModuleId_get64BitId();
  CanFrame f = {
          .data = {0},
          .data_size = CAN_FRAME_MAX_DATA_LEN,
          .id = ModuleId,
          .is_extended = false,
          .is_remote = false,
  };
  for (int i = 0; i < 8; i++) {
    f.data[i] = id[i];
  }

  RingBuffer_put(can_tx_h, &f);
}
