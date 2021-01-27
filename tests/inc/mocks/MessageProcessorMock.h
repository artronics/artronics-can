#ifndef ARTRONICS_MESSAGEPROCESSORMOCK_H
#define ARTRONICS_MESSAGEPROCESSORMOCK_H

#include "gmock/gmock.h"
#include "message_processor.h"

class MessageProcessorMock {
public:
    virtual ~MessageProcessorMock()= default;
    MOCK_METHOD(int, MessageProcessor_init, ());
    MOCK_METHOD(void, MessageProcessor_process, (CanFrame *frame));
};

#endif //ARTRONICS_MESSAGEPROCESSORMOCK_H
