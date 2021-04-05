#include "gtest/gtest.h"
#include "hal/hal_spi.h"
#include "stm32f0xx_hal.h"
#include "stm32f042x6.h"
#include "memory"
#include "test_utils.h"

#include "fff.h"

DEFINE_FFF_GLOBALS


using namespace testing;
using namespace test_utils::stub;

class HalSpiTest : public ::testing::Test {
public:
    ~HalSpiTest() override = default;

    void SetUp() override {
      FFF_RESET_HISTORY()
    }
};

TEST_F(HalSpiTest, HalSpi_init__should_init_) {
  // Given
  const HalSpiInit halSpiInit = HalSpiInit {.device = SPI0};

  // When
  int res = HalSpi_init(&halSpiInit);

  // Then
  ASSERT_EQ(res,0);
}
