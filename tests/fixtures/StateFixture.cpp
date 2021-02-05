#include "StateFixture.h"
#include "ring_buffer.h"
#include "message_processor.h"

std::unique_ptr<RingBufferMock> StateFixture::_ringBufferMock;
std::unique_ptr<MessageProcessorMock> StateFixture::_messageProcessorMock;
std::unique_ptr<CanMock> StateFixture::_canMock;

int RingBuffer_init(RingBufferHandler *rbd, RingBufferInit *rb_init) {
  return StateFixture::_ringBufferMock->RingBuffer_init(rbd, rb_init);
}

int RingBuffer_get(RingBufferHandler rbd, void *data) {
  return StateFixture::_ringBufferMock->RingBuffer_get(rbd, data);
}

int RingBuffer_put(RingBufferHandler rbd, const void *data) {
  return StateFixture::_ringBufferMock->RingBuffer_put(rbd, data);
}

int Can_init(const RingBufferHandler * const  can_rx_h) {
  return StateFixture::_canMock->Can_init(can_rx_h);
}

void MessageProcessor_process(const CanFrame * const frame) {
  return StateFixture::_messageProcessorMock->MessageProcessor_process(frame);
}

int MessageProcessor_init(const RingBufferHandler * const rbh) {
  return StateFixture::_messageProcessorMock->MessageProcessor_init(rbh);
}

/**
 * This function will return a lambda which can be used as a stub for #RingBuffer_get
 * @param expFrame the frame which the stub will return as function parameter
 * @param return_v the return value of the stub (error no)
 * @return
 */

std::function<int(RingBufferHandler rbh, void *data)> stub_RingBuffer_Get(CanFrame expFrame, int return_v) {
  auto RingBuffer_Get = [expFrame, return_v](RingBufferHandler rbh, void *data) {
      auto *t = (CanFrame *)data;
      *t = expFrame;

      return return_v;
  };

  return RingBuffer_Get;
}
