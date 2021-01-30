#ifndef ARTRONICS_RINGBUFFERMOCK_H
#define ARTRONICS_RINGBUFFERMOCK_H

#include "gmock/gmock.h"
#include "ring_buffer.h"
#include "can_frame.h"

class RingBufferMock {
public:
    virtual ~RingBufferMock()= default;;
    MOCK_METHOD(int, RingBuffer_init, (RingBufferHandler *, RingBufferInit *));
    MOCK_METHOD(int, RingBuffer_get, (RingBufferHandler, void *));
    MOCK_METHOD(int, RingBuffer_put, (RingBufferHandler, const void *));
};

#endif //ARTRONICS_RINGBUFFERMOCK_H
