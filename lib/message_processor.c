#include "message_processor.h"
#include "hal_module_id.h"

int MessageProcessor_init(void) {
  HalModuleId_init();

  return 0;
}

void MessageProcessor_Process(CanFrame *frame) {

}
