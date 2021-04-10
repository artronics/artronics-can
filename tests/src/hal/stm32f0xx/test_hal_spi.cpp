#include <test_utils.h>
#include "gtest/gtest.h"
#include "hal/hal_spi.h"
#include "fff.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_gpio.h"
#include "stm32f0xx_hal_spi.h"

DEFINE_FFF_GLOBALS

FAKE_VOID_FUNC(HAL_GPIO_Init, GPIO_TypeDef  *, GPIO_InitTypeDef *);
FAKE_VOID_FUNC(HAL_GPIO_WritePin, GPIO_TypeDef*, uint16_t, GPIO_PinState);
FAKE_VOID_FUNC(HAL_NVIC_SetPriority, IRQn_Type, uint32_t, uint32_t);
FAKE_VOID_FUNC(HAL_NVIC_EnableIRQ, IRQn_Type);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SPI_Init, SPI_HandleTypeDef *);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_SPI_TransmitReceive, SPI_HandleTypeDef *, uint8_t *, uint8_t *, uint16_t,
                uint32_t);

using namespace testing;
using namespace test_utils::stub;

class HalSpiTest : public ::testing::Test {
public:
    ~HalSpiTest() override = default;

    void SetUp() override {
      RESET_FAKE(HAL_GPIO_Init);
      RESET_FAKE(HAL_GPIO_WritePin);
      RESET_FAKE(HAL_NVIC_SetPriority);
      RESET_FAKE(HAL_NVIC_EnableIRQ);
      RESET_FAKE(HAL_SPI_Init);
      RESET_FAKE(HAL_SPI_TransmitReceive);

      FFF_RESET_HISTORY()
    }
};

TEST_F(HalSpiTest, HalSpi_init__should_init_SPI1) {
  // Given
  const HalSpiInit halSpiInit = HalSpiInit{.device = ART_SPI_DEV_0, .slaves = ART_SPI_SS_1};

  SPI_HandleTypeDef actSpiHandleTypeDef;
  auto HAL_SPI_Init_stub = lambdaToFunPointer([&](SPI_HandleTypeDef *handleTypeDef) -> HAL_StatusTypeDef {
      actSpiHandleTypeDef = *handleTypeDef;

      return HAL_OK;
  });
  HAL_SPI_Init_fake.custom_fake = HAL_SPI_Init_stub;

  GPIO_TypeDef *actGpio;
  GPIO_InitTypeDef actGpioInit;
  auto HAL_GPIO_Init_stub = lambdaToFunPointer([&](GPIO_TypeDef *typeDef, GPIO_InitTypeDef *initTypeDef) {
      actGpio = typeDef;
      actGpioInit = *initTypeDef;
  });
  HAL_GPIO_Init_fake.custom_fake = HAL_GPIO_Init_stub;

  // When
  HalSpiHandler spiHandler = {.device = ART_SPI_DEV_1}; // set values to one not expected
  int res = HalSpi_init(&spiHandler, &halSpiInit);

  // Then
  ASSERT_EQ(HAL_SPI_Init_fake.call_count, 1);
  ASSERT_EQ(res, HAL_OK);
  ASSERT_EQ(spiHandler.device, ART_SPI_DEV_0);

  // Configure SPI
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

  // Configure SPI CSS - For now only CSS_1
  ASSERT_EQ(actGpio, GPIOA);
  EXPECT_EQ(actGpioInit.Mode, GPIO_MODE_OUTPUT_PP);
  EXPECT_EQ(actGpioInit.Pin, GPIO_PIN_4);
  EXPECT_EQ(actGpioInit.Mode, GPIO_MODE_OUTPUT_PP);
  EXPECT_EQ(actGpioInit.Pull, GPIO_PULLUP);
  EXPECT_EQ(actGpioInit.Speed, GPIO_SPEED_FREQ_HIGH);

  ASSERT_EQ(HAL_GPIO_WritePin_fake.call_count, 1);
  EXPECT_EQ(HAL_GPIO_WritePin_fake.arg0_val, GPIOA);
  EXPECT_EQ(HAL_GPIO_WritePin_fake.arg1_val, GPIO_PIN_4);
  EXPECT_EQ(HAL_GPIO_WritePin_fake.arg2_val, GPIO_PIN_SET);
}

// TODO: For now we only support ART_SPI_DEV_0
TEST_F(HalSpiTest, HalSpi_init__should_error_when_init_SPI2) {
  // Given
  const HalSpiInit halSpiInit = HalSpiInit{.device = ART_SPI_DEV_1, .slaves = ART_SPI_SS_1};

  // When
  HalSpiHandler spiHandler = {};
  int res = HalSpi_init(&spiHandler, &halSpiInit);

  // Then
  ASSERT_EQ(HAL_SPI_Init_fake.call_count, 0);
  ASSERT_EQ(HAL_GPIO_Init_fake.call_count, 0);
  ASSERT_EQ(res, 1);
}

// TODO: For now we only support ART_SPI_SS_1
TEST_F(HalSpiTest, HalSpi_init__should_error_when_init_is_not_SPI_CSS_1) {
  // Given
  const HalSpiInit halSpiInit = HalSpiInit{.device = ART_SPI_DEV_0, .slaves = ART_SPI_SS_2};

  // When
  HalSpiHandler spiHandler = {};
  int res = HalSpi_init(&spiHandler, &halSpiInit);

  // Then
  ASSERT_EQ(HAL_SPI_Init_fake.call_count, 0);
  ASSERT_EQ(HAL_GPIO_Init_fake.call_count, 0);
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
  EXPECT_EQ(actInitTypeDef.Pin, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
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

TEST_F(HalSpiTest, HalSpi_transfer__transfer_data) {
  // Give
  HalSpiHandler handler = {.device = ART_SPI_DEV_0};
  uint8_t expTxData[] = {1, 2, 3, 4};
  uint8_t expRxData[4] = {0};
  uint8_t *actTxData;
  uint8_t *actRxData;
  uint32_t actTimout;
  uint32_t actSize;
  auto HAL_SPI_TransmitReceive_stub = lambdaToFunPointer(
          [&](SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size,
              uint32_t Timeout) -> HAL_StatusTypeDef {
              actTxData = pTxData;
              actRxData = pRxData;
              actTimout = Timeout;
              actSize = Size;
              return HAL_OK;
          });

  HAL_SPI_TransmitReceive_fake.custom_fake = HAL_SPI_TransmitReceive_stub;

  // When
  HalSpi_transfer(&handler, ART_SPI_SS_1, expTxData, expRxData, 4);

  // Then
  ASSERT_EQ(HAL_SPI_TransmitReceive_fake.call_count, 1);
  EXPECT_EQ(expTxData, actTxData);
  EXPECT_EQ(expRxData, actRxData);
  EXPECT_EQ(actTimout, 1000);
  EXPECT_EQ(actSize, 4);

  ASSERT_EQ(HAL_GPIO_WritePin_fake.call_count, 2);
  EXPECT_EQ(HAL_GPIO_WritePin_fake.arg0_history[0], GPIOA);
  EXPECT_EQ(HAL_GPIO_WritePin_fake.arg0_history[1], GPIOA);
  EXPECT_EQ(HAL_GPIO_WritePin_fake.arg1_history[0], GPIO_PIN_4);
  EXPECT_EQ(HAL_GPIO_WritePin_fake.arg1_history[1], GPIO_PIN_4);
  EXPECT_EQ(HAL_GPIO_WritePin_fake.arg2_history[0], GPIO_PIN_RESET);
  EXPECT_EQ(HAL_GPIO_WritePin_fake.arg2_history[1], GPIO_PIN_SET);
}
