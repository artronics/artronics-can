#include "StateFixture.h"
#include "ring_buffer.h"
#include "message_processor.h"

std::unique_ptr<RingBufferMock> StateFixture::_ringBufferMock;
std::unique_ptr<MessageProcessorMock> StateFixture::_messageProcessorMock;

int RingBuffer_Init(RingBufferHandler *rbd, RingBufferInit *rb_init) {
  return StateFixture::_ringBufferMock->RingBuffer_Init(rbd, rb_init);
}

int RingBuffer_Get(RingBufferHandler rbd, void *data) {
  return StateFixture::_ringBufferMock->RingBuffer_Get(rbd, data);
}

void MessageProcessor_Process(CanFrame *frame) {
  return StateFixture::_messageProcessorMock->MessageProcessor_Process(frame);
}

int MessageProcessor_init(void) {
  return StateFixture::_messageProcessorMock->MessageProcessor_init();
}

std::function<int(RingBufferHandler rbh, void *data)> stub_RingBuffer_Get(CanFrame expFrame, int return_v) {
  auto RingBuffer_Get = [expFrame, return_v](RingBufferHandler rbh, void *data) {
      auto *t = (CanFrame *)data;
      *t = expFrame;

      return return_v;
  };

  return RingBuffer_Get;
}
