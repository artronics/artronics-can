#include "MessageProcessorFixture.h"
#include "hal/hal_module_id.h"

std::unique_ptr<HalModuleIdMock> MessageProcessorFixture::_halModuleIdMock;
std::unique_ptr<RingBufferMock> MessageProcessorFixture::_ringBufferMock;

int HalModuleId_init(void) {
  return MessageProcessorFixture::_halModuleIdMock->HalModuleId_init();
}

const uint8_t * HalModuleId_get64BitId(void) {
  return MessageProcessorFixture::_halModuleIdMock->HalModuleId_get64BitId();
}

int RingBuffer_put(RingBufferHandler rbd, const void *data) {
  return MessageProcessorFixture::_ringBufferMock->RingBuffer_put(rbd, data);
}
