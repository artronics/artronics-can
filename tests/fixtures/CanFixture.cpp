#include "CanFixture.h"

std::unique_ptr<HalCanMock> CanFixture::_halCanMock;
std::unique_ptr<RingBufferMock> CanFixture::_ringBufferMock;

int HalCan_init(const HalCanInit * const halCanInit) {
  return CanFixture::_halCanMock->HalCan_init(halCanInit);
}

int HalCan_transmit(const CanFrame * const frame) {
  return CanFixture::_halCanMock->HalCan_transmit(frame);
}

int RingBuffer_put(RingBufferHandler rbd, const void *data) {
  return CanFixture::_ringBufferMock->RingBuffer_put(rbd, data);
}

