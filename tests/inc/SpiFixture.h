#ifndef ARTRONICS_SPIFIXTURE_H
#define ARTRONICS_SPIFIXTURE_H

#include "gtest/gtest.h"
#include "mocks/HalSpiMock.h"

class SpiFixture: public ::testing::Test {
public:
    SpiFixture() {
      _halSpiMock = std::make_unique<::testing::NiceMock<HalSpiMock>>();
    }

    ~SpiFixture() override {
      _halSpiMock.reset();
    }

    void SetUp() override{}
    void TearDown() override{}

    static std::unique_ptr<HalSpiMock> _halSpiMock;
};

#endif //ARTRONICS_SPIFIXTURE_H
