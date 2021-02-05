#ifndef ARTRONICS_MESSAGEPROCESSORMOCK_H
#define ARTRONICS_MESSAGEPROCESSORMOCK_H

#include "gmock/gmock.h"
#include "message_processor.h"
#include "ring_buffer.h"

class MessageProcessorMock {
public:
    virtual ~MessageProcessorMock()= default;
    MOCK_METHOD(int, MessageProcessor_init, (const RingBufferHandler rbh));
    MOCK_METHOD(void, MessageProcessor_process, (const CanFrame * const frame));
};

#endif //ARTRONICS_MESSAGEPROCESSORMOCK_H
