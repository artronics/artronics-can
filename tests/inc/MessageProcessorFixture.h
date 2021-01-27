#ifndef ARTRONICS_MESSAGEPROCESSORFIXTURE_H
#define ARTRONICS_MESSAGEPROCESSORFIXTURE_H

#include "gtest/gtest.h"
#include "mocks/HalModuleIdMock.h"

class MessageProcessorFixture: public ::testing::Test {
public:
    MessageProcessorFixture() {
      _halModuleIdMock.reset(new ::testing::NiceMock<HalModuleIdMock>());
    }

    ~MessageProcessorFixture() override {
      _halModuleIdMock.reset();
    }
    void SetUp() override{}
    void TearDown() override{}

    static std::unique_ptr<HalModuleIdMock> _halModuleIdMock;
};

#endif //ARTRONICS_MESSAGEPROCESSORFIXTURE_H
