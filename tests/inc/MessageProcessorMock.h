#ifndef ARTRONICS_MESSAGEPROCESSORMOCK_H
#define ARTRONICS_MESSAGEPROCESSORMOCK_H

#include "gmock/gmock.h"
#include "message_processor.h"

class MessageProcessorMock {
public:
    virtual ~MessageProcessorMock()= default;
    MOCK_METHOD(void, MessageProcessor_Process, (CanFrame *frame));
};

typedef struct {
    void MessageProcessor_Process(CanFrame *frame) {
      _frame = frame;
    }

    CanFrame *_frame;
} MessageProcessorArgCaptor;
#endif //ARTRONICS_MESSAGEPROCESSORMOCK_H
