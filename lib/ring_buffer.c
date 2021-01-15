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

int RingBuffer_Init(rbd_t *rbd, rb_attr_t *attr) {
  static int idx = 0;
  int err = -1;

  if ((idx < RING_BUFFER_MAX) && (rbd != NULL) && (attr != NULL)) {
    if ((attr->buffer != NULL) && (attr->size_elem > 0)) {
      /* Check that the size of the ring buffer is a power of 2 */
      if (((attr->num_elem - 1) & attr->num_elem) == 0) {
        /* Initialize the ring buffer internal variables */
        rb[idx].head = 0;
        rb[idx].tail = 0;
        rb[idx].buf = attr->buffer;
        rb[idx].size_elem = attr->size_elem;
        rb[idx].num_elem = attr->num_elem;

        *rbd = idx++;
        err = 0;
      }
    }
  }

  return err;
}

static int RingBuffer_full(struct ring_buffer *rb) {
  return ((rb->head - rb->tail) == rb->num_elem) ? 1 : 0;
}

static int RingBuffer_empty(struct ring_buffer *rb) {
  return ((rb->head - rb->tail) == 0U) ? 1 : 0;
}

int RingBuffer_Put(rbd_t rbd, const void *data) {
  int err = 0;

  if ((rbd < RING_BUFFER_MAX) && (RingBuffer_full(&rb[rbd]) == 0)) {
    const size_t offset = (rb[rbd].head & (rb[rbd].num_elem - 1)) * rb[rbd].size_elem;
    memcpy(&(rb[rbd].buf[offset]), data, rb[rbd].size_elem);
    rb[rbd].head++;
  } else {
    err = -1;
  }

  return err;
}

int RingBuffer_Get(rbd_t rbd, void *data)
{
  int err = 0;

  if ((rbd < RING_BUFFER_MAX) && (RingBuffer_empty(&rb[rbd]) == 0)) {
    const size_t offset = (rb[rbd].tail & (rb[rbd].num_elem - 1)) * rb[rbd].size_elem;
    memcpy(data, &(rb[rbd].buf[offset]), rb[rbd].size_elem);
    rb[rbd].tail++;
  } else {
    err = -1;
  }

  return err;
}
