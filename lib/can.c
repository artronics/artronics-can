#include "can.h"
#include "hal/hal_can.h"

static RingBufferHandler can_rx_h;

int receive_cb(const CanFrame * frame);

int Can_init(const RingBufferHandler _can_rx_h) {
  can_rx_h = _can_rx_h;

  const HalCanInit i = {.receive_frame_cb = receive_cb};
  HalCan_init(&i);

  return 0;
}

int Can_transmit(const CanFrame * const frame) {
  HalCan_transmit(frame);

  return 0;
}

int receive_cb(const CanFrame * const frame) {
  RingBuffer_put(can_rx_h, frame);

  return 0;
}
