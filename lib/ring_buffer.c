#include <string.h>
#include "ring_buffer.h"

struct ring_buffer {
    size_t size_elem;
    size_t num_elem;
    uint8_t *buf;
    volatile size_t head;
    volatile size_t tail;
};

static struct ring_buffer rb[RING_BUFFER_MAX];

int RingBuffer_init(RingBufferHandler *rbd, RingBufferInit *rb_init) {
  static int idx = 0;
  int err = RING_BUFFER_ERROR;

  if ((idx < RING_BUFFER_MAX) && (rbd != NULL) && (rb_init != NULL)) {
    if ((rb_init->buffer != NULL) && (rb_init->size_elem > 0)) {
      /* Check that the size of the ring buffer is a power of 2 */
      if (((rb_init->num_elem - 1) & rb_init->num_elem) == 0) {
        /* Initialize the ring buffer internal variables */
        rb[idx].head = 0;
        rb[idx].tail = 0;
        rb[idx].buf = rb_init->buffer;
        rb[idx].size_elem = rb_init->size_elem;
        rb[idx].num_elem = rb_init->num_elem;

        *rbd = idx++;
        err = RING_BUFFER_OK;
      }
    }
  }

  return err;
}

static int RingBuffer_full(struct ring_buffer *_rb) {
  return ((_rb->head - _rb->tail) == _rb->num_elem) ? 1 : 0;
}

static int RingBuffer_empty(struct ring_buffer *_rb) {
  return ((_rb->head - _rb->tail) == 0U) ? 1 : 0;
}

int RingBuffer_put(RingBufferHandler rbd, const void *data) {
  int err = RING_BUFFER_OK;

  if ((rbd < RING_BUFFER_MAX) && (RingBuffer_full(&rb[rbd]) == 0)) {
    const size_t offset = (rb[rbd].head & (rb[rbd].num_elem - 1)) * rb[rbd].size_elem;
    memcpy(&(rb[rbd].buf[offset]), data, rb[rbd].size_elem);
    rb[rbd].head++;
  } else {
    err = RING_BUFFER_ERROR;
  }

  return err;
}

int RingBuffer_get(RingBufferHandler rbd, void *data) {
  int err = RING_BUFFER_OK;

  if ((rbd < RING_BUFFER_MAX) && (RingBuffer_empty(&rb[rbd]) == 0)) {
    const size_t offset = (rb[rbd].tail & (rb[rbd].num_elem - 1)) * rb[rbd].size_elem;
    memcpy(data, &(rb[rbd].buf[offset]), rb[rbd].size_elem);
    rb[rbd].tail++;
  } else {
    err = RING_BUFFER_ERROR;
  }

  return err;
}
