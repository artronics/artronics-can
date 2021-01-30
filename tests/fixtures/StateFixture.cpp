#include "StateFixture.h"
#include "ring_buffer.h"
#include "message_processor.h"

std::unique_ptr<RingBufferMock> StateFixture::_ringBufferMock;
std::unique_ptr<MessageProcessorMock> StateFixture::_messageProcessorMock;

int RingBuffer_init(RingBufferHandler *rbd, RingBufferInit *rb_init) {
  return StateFixture::_ringBufferMock->RingBuffer_init(rbd, rb_init);
}

int RingBuffer_get(RingBufferHandler rbd, void *data) {
  return StateFixture::_ringBufferMock->RingBuffer_get(rbd, data);
}

int RingBuffer_put(RingBufferHandler rbd, const void *data) {
  return StateFixture::_ringBufferMock->RingBuffer_put(rbd, data);
}

void MessageProcessor_process(CanFrame *frame) {
  return StateFixture::_messageProcessorMock->MessageProcessor_process(frame);
}

int MessageProcessor_init(RingBufferHandler rbh) {
  return StateFixture::_messageProcessorMock->MessageProcessor_init(rbh);
}

std::function<int(RingBufferHandler rbh, void *data)> stub_RingBuffer_Get(CanFrame expFrame, int return_v) {
  auto RingBuffer_Get = [expFrame, return_v](RingBufferHandler rbh, void *data) {
      auto *t = (CanFrame *)data;
      *t = expFrame;

      return return_v;
  };

  return RingBuffer_Get;
}
