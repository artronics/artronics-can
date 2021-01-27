#include "state.h"
#include "ring_buffer.h"
#include "can_frame.h"
#include "message_processor.h"

_Noreturn static void State_start_thread(void);
static void State_process(void);

static CanFrame can_rx_buf[STATE_CAN_RX_BUF_SIZE];
static RingBufferHandler can_rx_h;


int State_init(void) {
  // Init dependencies
  // TODO: implement error handling
  MessageProcessor_init();

  RingBufferInit rb_init = {
          .size_elem=sizeof(can_rx_buf[0]),
          .num_elem=STATE_CAN_RX_BUF_SIZE,
          .buffer=can_rx_buf,
  };
  RingBuffer_init(&can_rx_h, &rb_init);
}

int State_start(bool isThread) {
  if (isThread) {
    State_start_thread();
  } else {
    State_process();
  }
}

RingBufferHandler *State_GetCanRxBufHandler(void) {
  return &can_rx_h;
}

_Noreturn static void State_start_thread(void) {
  for (;;) {
    State_process();
  }
}

static void State_process(void) {
  CanFrame frame;
  while (RingBuffer_get(can_rx_h, &frame) == RING_BUFFER_OK){
    MessageProcessor_process(&frame);
  }

}
