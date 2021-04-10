#include <test_utils.h>
#include "gtest/gtest.h"
#include "hal/tmc/hal_tmc5160a.h"
#include "SpiFixture.h"


using namespace testing;
using namespace test_utils::stub;

class HalTmc5160Test : public ::SpiFixture {
public:
    ~HalTmc5160Test() override = default;

    void SetUp() override {}
};

TEST_F(HalTmc5160Test, HalTmc5160_init__should_init_SPI1_and_TMC5160) {
  // Given
  const HalTmc5160Init init = {.spiConfig = {.spiDevice = ART_SPI_DEV_1, .spiChannel = ART_SPI_SS_2}};
  EXPECT_CALL(*_halSpiMock, HalSpi_init(_, _))
          .Times(1);

  // When
  HalTmc5160_init(&init);
}
