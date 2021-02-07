#ifndef ARTRONICS_CANFIXTURE_H
#define ARTRONICS_CANFIXTURE_H

#include "gtest/gtest.h"
#include "mocks/HalCanMock.h"
#include "mocks/RingBufferMock.h"

class CanFixture: public ::testing::Test {
public:
    CanFixture() {
      _halCanMock = std::make_unique<::testing::NiceMock<HalCanMock>>();
      _ringBufferMock = std::make_unique<::testing::NiceMock<RingBufferMock>>();
    }

    ~CanFixture() override {
      _halCanMock.reset();
      _ringBufferMock.reset();
    }

    void SetUp() override{}
    void TearDown() override{}

    static std::unique_ptr<HalCanMock> _halCanMock;
    static std::unique_ptr<RingBufferMock> _ringBufferMock;
};

#endif //ARTRONICS_CANFIXTURE_H
