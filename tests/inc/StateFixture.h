#ifndef ARTRONICS_STATEFIXTURE_H
#define ARTRONICS_STATEFIXTURE_H

#include <memory>

#include "mocks/RingBufferMock.h"
#include "mocks/MessageProcessorMock.h"
#include "mocks/CanMock.h"

class StateFixture: public ::testing::Test {
public:
    StateFixture() {
      _ringBufferMock = std::make_unique<::testing::NiceMock<RingBufferMock>>();
      _messageProcessorMock = std::make_unique<::testing::NiceMock<MessageProcessorMock>>();
      _canMock = std::make_unique<::testing::NiceMock<CanMock>>();
    }

    ~StateFixture() override {
      _ringBufferMock.reset();
      _messageProcessorMock.reset();
      _canMock.reset();
    }
    void SetUp() override{}
    void TearDown() override{}

    static std::unique_ptr<RingBufferMock> _ringBufferMock;
    static std::unique_ptr<MessageProcessorMock> _messageProcessorMock;
    static std::unique_ptr<CanMock> _canMock;
};

std::function<int(RingBufferHandler rbh, void *data)> stub_RingBuffer_Get(CanFrame expFrame, int return_v);

#endif //ARTRONICS_STATEFIXTURE_H
