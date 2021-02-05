#include "state.h"
#include "ring_buffer.h"
#include "can_frame.h"
#include "message_processor.h"
#include "can.h"

_Noreturn static void State_start_thread(void);

static void State_process(void);

static CanFrame can_rx_buf[STATE_CAN_RX_BUF_SIZE];
static RingBufferHandler can_rx_h;
static CanFrame can_tx_buf[STATE_CAN_TX_BUF_SIZE];
static RingBufferHandler can_tx_h;

int State_init(void) {
  // Init dependencies
  // TODO: implement error handling
  RingBufferInit rb_tx_init = {
          .size_elem=sizeof(can_tx_buf[0]),
          .num_elem=STATE_CAN_TX_BUF_SIZE,
          .buffer=can_tx_buf,
  };
  RingBuffer_init(&can_tx_h, &rb_tx_init);
  MessageProcessor_init(can_tx_h);

  RingBufferInit rb_init = {
          .size_elem=sizeof(can_rx_buf[0]),
          .num_elem=STATE_CAN_RX_BUF_SIZE,
          .buffer=can_rx_buf,
  };
  RingBuffer_init(&can_rx_h, &rb_init);
  Can_init(can_rx_h);

  return 0;
}

int State_start(bool isThread) {
  // Can't do-while here cause can't use _Noreturn
  // FIXME: is there a better way to do this? i.e. to make a testable super-loop

  if (isThread) {
    State_start_thread();
  } else {
    State_process();
  }
  return 0;
}

_Noreturn static void State_start_thread(void) {
  for (;;) {
    State_process();
  }
}

static void State_process(void) {
  CanFrame frame;
  while (RingBuffer_get(can_rx_h, &frame) == RING_BUFFER_OK) {
    MessageProcessor_process(&frame);
  }
  while (RingBuffer_get(can_tx_h, &frame) == RING_BUFFER_OK) {
    Can_transmit(&frame);
  }

}
