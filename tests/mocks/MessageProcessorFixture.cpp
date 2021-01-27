#include "MessageProcessorFixture.h"
#include "hal_module_id.h"

std::unique_ptr<HalModuleIdMock> MessageProcessorFixture::_halModuleIdMock;

int HalModuleId_init(void) {
  return MessageProcessorFixture::_halModuleIdMock->HalModuleId_init();
}

uint8_t *HalModuleId_get64BitId(void) {
  return MessageProcessorFixture::_halModuleIdMock->HalModuleId_get64BitId();
}
