#include "can.h"
#include "hal/hal_can.h"

static RingBufferHandler canRXHandler;

int receivedFrameCallback(const CanFrame * frame);

int Can_init(const RingBufferHandler _canRxH) {
  canRXHandler = _canRxH;

  const HalCanInit i = {.receivedFrameCallback = receivedFrameCallback};
  HalCan_init(&i);

  return 0;
}

int Can_transmit(const CanFrame * const frame) {
  HalCan_transmit(frame);

  return 0;
}

int receivedFrameCallback(const CanFrame *frame) {
  RingBuffer_put(canRXHandler, frame);

  return 0;
}
