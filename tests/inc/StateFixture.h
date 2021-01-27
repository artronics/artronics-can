#ifndef ARTRONICS_STATEFIXTURE_H
#define ARTRONICS_STATEFIXTURE_H

#include "RingBufferMock.h"
#include "MessageProcessorMock.h"

class StateFixture: public ::testing::Test {
public:
    StateFixture() {
      _ringBufferMock.reset(new ::testing::NiceMock<RingBufferMock>());
      _messageProcessorMock.reset(new ::testing::NiceMock<MessageProcessorMock>());
    }

    ~StateFixture() override {
      _ringBufferMock.reset();
      _messageProcessorMock.reset();
    }
    void SetUp() override{}
    void TearDown() override{}

    static std::unique_ptr<RingBufferMock> _ringBufferMock;
    static std::unique_ptr<MessageProcessorMock> _messageProcessorMock;
};

std::function<int(RingBufferHandler rbh, void *data)> stub_RingBuffer_Get(CanFrame expFrame, int return_v);

#endif //ARTRONICS_STATEFIXTURE_H
