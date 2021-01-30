#ifndef ARTRONICS_MESSAGEPROCESSORFIXTURE_H
#define ARTRONICS_MESSAGEPROCESSORFIXTURE_H

#include <memory>

#include "gtest/gtest.h"
#include "mocks/HalModuleIdMock.h"
#include "mocks/RingBufferMock.h"

class MessageProcessorFixture: public ::testing::Test {
public:
    MessageProcessorFixture() {
      _halModuleIdMock = std::make_unique<::testing::NiceMock<HalModuleIdMock>>();
      _ringBufferMock = std::make_unique<::testing::NiceMock<RingBufferMock>>();
    }

    ~MessageProcessorFixture() override {
      _halModuleIdMock.reset();
      _ringBufferMock.reset();
    }

    void SetUp() override{}
    void TearDown() override{}

    static std::unique_ptr<HalModuleIdMock> _halModuleIdMock;
    static std::unique_ptr<RingBufferMock> _ringBufferMock;
};

#endif //ARTRONICS_MESSAGEPROCESSORFIXTURE_H
