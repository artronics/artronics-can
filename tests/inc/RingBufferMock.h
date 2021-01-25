#ifndef ARTRONICS_RINGBUFFERMOCK_H
#define ARTRONICS_RINGBUFFERMOCK_H

#include "gmock/gmock.h"
#include "ring_buffer.h"
#include "can_frame.h"

class RingBufferMock {
public:
    virtual ~RingBufferMock()= default;;
    MOCK_METHOD(int, RingBuffer_Foo, (int));
    MOCK_METHOD(int, RingBuffer_Init, (RingBufferHandler *, RingBufferInit *));
    MOCK_METHOD(int, RingBuffer_Get, (RingBufferHandler, void *));
};

typedef struct {
    int RingBuffer_Init(RingBufferHandler *rbh, RingBufferInit *rb_init) {
      _rb_init = *rb_init;
      _rbh = rbh;

      return 0;
    }

    ::RingBufferInit _rb_init;
    RingBufferHandler *_rbh;
} RingBufferArgCaptor;


#endif //ARTRONICS_RINGBUFFERMOCK_H
