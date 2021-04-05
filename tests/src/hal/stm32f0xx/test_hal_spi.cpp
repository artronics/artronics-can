#include "gtest/gtest.h"
#include "hal/hal_spi.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_spi.h"
#include "stm32f042x6.h"
#include "memory"
#include "test_utils.h"

#include "fff.h"

DEFINE_FFF_GLOBALS

FAKE_VOID_FUNC(HAL_GPIO_Init, GPIO_TypeDef  *, GPIO_InitTypeDef *);
FAKE_VOID_FUNC(HAL_NVIC_SetPriority, IRQn_Type, uint32_t, uint32_t);
FAKE_VOID_FUNC(HAL_NVIC_EnableIRQ, IRQn_Type);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SPI_Init, SPI_HandleTypeDef *);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SPI_TransmitReceive, SPI_HandleTypeDef *, uint8_t *, uint8_t *, uint16_t, uint32_t);

using namespace testing;
using namespace test_utils::stub;

class HalSpiTest : public ::testing::Test {
public:
    ~HalSpiTest() override = default;

    void SetUp() override {
      RESET_FAKE(HAL_SPI_Init)

      FFF_RESET_HISTORY()
    }
};

TEST_F(HalSpiTest, HalSpi_init__should_init_SPI1) {
  // Given
  const HalSpiInit halSpiInit = HalSpiInit {.device = ART_SPI_DEV_0};

  SPI_HandleTypeDef actSpiHandleTypeDef;
  auto HAL_SPI_Init_stub = lambdaToFunPointer([&](SPI_HandleTypeDef *handleTypeDef) -> HAL_StatusTypeDef {
      actSpiHandleTypeDef = *handleTypeDef;

      return HAL_OK;
  });
  HAL_SPI_Init_fake.custom_fake = HAL_SPI_Init_stub;

  // When
  int res = HalSpi_init(&halSpiInit);

  // Then
  ASSERT_EQ(HAL_SPI_Init_fake.call_count, 1);
  ASSERT_EQ(res, HAL_OK);

  EXPECT_EQ(actSpiHandleTypeDef.Instance, SPI1);
  auto init = actSpiHandleTypeDef.Init;
  EXPECT_EQ(init.Mode, SPI_MODE_MASTER);
  EXPECT_EQ(init.Direction, SPI_DIRECTION_2LINES);
  EXPECT_EQ(init.DataSize, SPI_DATASIZE_8BIT);
  EXPECT_EQ(init.CLKPolarity, SPI_POLARITY_LOW);
  EXPECT_EQ(init.CLKPhase, SPI_PHASE_1EDGE);
  EXPECT_EQ(init.NSS, SPI_NSS_SOFT);
  EXPECT_EQ(init.BaudRatePrescaler, SPI_BAUDRATEPRESCALER_32);
  EXPECT_EQ(init.FirstBit, SPI_FIRSTBIT_MSB);
  EXPECT_EQ(init.TIMode, SPI_TIMODE_DISABLE);
  EXPECT_EQ(init.CRCCalculation, SPI_CRCCALCULATION_DISABLE);
  EXPECT_EQ(init.CRCPolynomial, 7);
  EXPECT_EQ(init.CRCLength, SPI_CRC_LENGTH_DATASIZE);
  EXPECT_EQ(init.NSSPMode, SPI_NSS_PULSE_ENABLE);
}

// TODO: For now we only support ART_SPI_DEV_0
TEST_F(HalSpiTest, HalSpi_init__should_error_when_init_SPI2) {
  // Given
  const HalSpiInit halSpiInit = HalSpiInit {.device = ART_SPI_DEV_1};

  // When
  int res = HalSpi_init(&halSpiInit);

  // Then
  ASSERT_EQ(HAL_SPI_Init_fake.call_count, 0);
  ASSERT_EQ(res, 1);
}

TEST_F(HalSpiTest, HalSpi_init__should_init_Msp_SPI) {
  // Given
  GPIO_TypeDef *actTypeDef;
  GPIO_InitTypeDef actInitTypeDef;

  auto HAL_GPIO_Init_stub = lambdaToFunPointer([&](GPIO_TypeDef *typeDef, GPIO_InitTypeDef *initTypeDef) {
      actTypeDef = typeDef;
      actInitTypeDef = *initTypeDef;
  });

  HAL_GPIO_Init_fake.custom_fake = HAL_GPIO_Init_stub;

  // When
  SPI_HandleTypeDef spiHandleTypeDef = {.Instance = SPI1};
  HAL_SPI_MspInit(&spiHandleTypeDef);

  // Then
  ASSERT_EQ(HAL_GPIO_Init_fake.call_count, 1);
  EXPECT_EQ(actTypeDef, GPIOA);
  EXPECT_EQ(actInitTypeDef.Pin, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
  EXPECT_EQ(actInitTypeDef.Mode, GPIO_MODE_AF_PP);
  EXPECT_EQ(actInitTypeDef.Pull, GPIO_NOPULL);
  EXPECT_EQ(actInitTypeDef.Speed, GPIO_SPEED_FREQ_HIGH);
  EXPECT_EQ(actInitTypeDef.Alternate, GPIO_AF0_SPI1);

  // Interrupts
  ASSERT_EQ(HAL_NVIC_SetPriority_fake.call_count, 1);
  EXPECT_EQ(HAL_NVIC_SetPriority_fake.arg0_val, SPI1_IRQn);
  EXPECT_EQ(HAL_NVIC_SetPriority_fake.arg1_val, 0);
  EXPECT_EQ(HAL_NVIC_SetPriority_fake.arg2_val, 0);

  ASSERT_EQ(HAL_NVIC_EnableIRQ_fake.call_count, 1);
  EXPECT_EQ(HAL_NVIC_EnableIRQ_fake.arg0_val, SPI1_IRQn);
}
