#include "gtest/gtest.h"
#include "hal/hal_can.h"
#include "stm32f0xx_hal.h"
#include "stm32f042x6.h"
#include "memory"
#include "test_utils.h"

#include "fff.h"

DEFINE_FFF_GLOBALS

FAKE_VOID_FUNC(HAL_GPIO_Init, GPIO_TypeDef  *, GPIO_InitTypeDef *);
FAKE_VOID_FUNC(HAL_NVIC_SetPriority, IRQn_Type, uint32_t, uint32_t);
FAKE_VOID_FUNC(HAL_NVIC_EnableIRQ, IRQn_Type);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CAN_Init, CAN_HandleTypeDef *);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CAN_ConfigFilter, CAN_HandleTypeDef *, CAN_FilterTypeDef *);

typedef void (*pCallback)(CAN_HandleTypeDef *);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CAN_RegisterCallback, CAN_HandleTypeDef *, HAL_CAN_CallbackIDTypeDef, pCallback)

using namespace testing;
using namespace test_utils::stub;

class HalCanTest : public ::testing::Test {
public:
    ~HalCanTest() override = default;

    void SetUp() override {
      RESET_FAKE(HAL_GPIO_Init)
      RESET_FAKE(HAL_NVIC_SetPriority)
      RESET_FAKE(HAL_NVIC_EnableIRQ)
      RESET_FAKE(HAL_CAN_Init)
      RESET_FAKE(HAL_CAN_ConfigFilter)
      RESET_FAKE(HAL_CAN_RegisterCallback)
      FFF_RESET_HISTORY()
    }
};

TEST_F(HalCanTest, HalCan_init__should_init_CAN_init_GPIO) {
  // Given
  GPIO_TypeDef *actTypeDef;
  GPIO_InitTypeDef actInitTypeDef;

  const HalCanInit init{};
  auto HAL_GPIO_Init_stub = lambdaToFunPointer([&](GPIO_TypeDef *typeDef, GPIO_InitTypeDef *initTypeDef) {
      actTypeDef = typeDef;
      actInitTypeDef = *initTypeDef;
  });

  HAL_GPIO_Init_fake.custom_fake = HAL_GPIO_Init_stub;

  // When
  HalCan_init(&init);

  // Then
  ASSERT_EQ(HAL_GPIO_Init_fake.call_count, 1);
  EXPECT_EQ(actTypeDef, GPIOA);
  EXPECT_EQ(actInitTypeDef.Pin, GPIO_PIN_11 | GPIO_PIN_12);
  EXPECT_EQ(actInitTypeDef.Mode, GPIO_MODE_AF_PP);
  EXPECT_EQ(actInitTypeDef.Pull, GPIO_NOPULL);
  EXPECT_EQ(actInitTypeDef.Speed, GPIO_SPEED_FREQ_HIGH);
  EXPECT_EQ(actInitTypeDef.Alternate, GPIO_AF4_CAN);
}

TEST_F(HalCanTest, HalCan_init__should_init_CAN_set_IRQs) {
  // Given
  const HalCanInit init{};

  // When
  HalCan_init(&init);

  // Then
  ASSERT_EQ(HAL_NVIC_SetPriority_fake.call_count, 1);
  EXPECT_EQ(HAL_NVIC_SetPriority_fake.arg0_val, CEC_CAN_IRQn);
  EXPECT_EQ(HAL_NVIC_SetPriority_fake.arg1_val, 0);
  EXPECT_EQ(HAL_NVIC_SetPriority_fake.arg2_val, 0);

  ASSERT_EQ(HAL_NVIC_EnableIRQ_fake.call_count, 1);
  EXPECT_EQ(HAL_NVIC_EnableIRQ_fake.arg0_val, CEC_CAN_IRQn);
}

TEST_F(HalCanTest, HalCan_init__should_init_CAN_HAL_CAN_Init) {
  // Given
  CAN_HandleTypeDef actCanHandle;
//  auto HAL_CAN_Init_stub = [&](HAL_StatusTypeDef returnVal) {
//      return lambdaToFunPointer([&](CAN_HandleTypeDef *handle) -> HAL_StatusTypeDef {
//          actCanHandle = *handle;
//          return returnVal;
//      });
//  };
  auto HAL_CAN_Init_stub = lambdaToFunPointer([&](CAN_HandleTypeDef *handle) -> HAL_StatusTypeDef {
      actCanHandle = *handle;
      return HAL_OK;
  });

  HAL_CAN_Init_fake.custom_fake = HAL_CAN_Init_stub;

  const HalCanInit canInit{};

  // When
  HalCan_init(&canInit);

  // Then
  ASSERT_EQ(HAL_CAN_Init_fake.call_count, 1);
  EXPECT_EQ(actCanHandle.Instance, CAN);
  auto init = actCanHandle.Init;
  EXPECT_EQ(init.Prescaler, 12);
  EXPECT_EQ(init.Mode, CAN_MODE_NORMAL);
  EXPECT_EQ(init.SyncJumpWidth, CAN_SJW_1TQ);
  EXPECT_EQ(init.TimeSeg1, CAN_BS1_6TQ);
  EXPECT_EQ(init.TimeSeg2, CAN_BS2_1TQ);
  EXPECT_EQ(init.TimeTriggeredMode, DISABLE);
  EXPECT_EQ(init.AutoBusOff, DISABLE);
  EXPECT_EQ(init.AutoWakeUp, DISABLE);
  EXPECT_EQ(init.AutoRetransmission, DISABLE);
  EXPECT_EQ(init.ReceiveFifoLocked, DISABLE);
  EXPECT_EQ(init.TransmitFifoPriority, DISABLE);
}

TEST_F(HalCanTest, HalCan_init__should_init_CAN_HAL_CAN_ConfigFilter) {
  // Given
  CAN_HandleTypeDef *actCanHandle;
  CAN_FilterTypeDef actFilterType;

  auto HAL_CAN_ConfigFilter_stub = lambdaToFunPointer(
          [&](CAN_HandleTypeDef *handle, CAN_FilterTypeDef *sFilterConfig) -> HAL_StatusTypeDef {
              actCanHandle = handle;
              actFilterType = *sFilterConfig;
              return HAL_OK;
          });
  HAL_CAN_ConfigFilter_fake.custom_fake = HAL_CAN_ConfigFilter_stub;


  const HalCanInit canInit{};

  // When
  HalCan_init(&canInit);

  // Then
  ASSERT_EQ(HAL_CAN_ConfigFilter_fake.call_count, 1);
  ASSERT_TRUE(actCanHandle != nullptr);
  EXPECT_EQ(actFilterType.FilterMode, CAN_FILTERMODE_IDMASK);
  EXPECT_EQ(actFilterType.FilterScale, CAN_FILTERSCALE_32BIT);
  EXPECT_EQ(actFilterType.FilterIdHigh, 0);
  EXPECT_EQ(actFilterType.FilterIdLow, 0);
  EXPECT_EQ(actFilterType.FilterMaskIdHigh, 0);
  EXPECT_EQ(actFilterType.FilterMaskIdLow, 0x000F);
  EXPECT_EQ(actFilterType.FilterFIFOAssignment, 0);
  EXPECT_EQ(actFilterType.FilterActivation, ENABLE);
  EXPECT_EQ(actFilterType.FilterBank, 0);
}

TEST_F(HalCanTest, HalCan_init__should_register_rx_callback) {
  // Given
  CAN_HandleTypeDef *actCanHandle;
  HAL_CAN_CallbackIDTypeDef actCbId;
  pCallback actCb;

  auto HAL_CAN_RegisterCallback_stub = lambdaToFunPointer(
          [&](CAN_HandleTypeDef *hcan, HAL_CAN_CallbackIDTypeDef callbackID, pCallback cb) -> HAL_StatusTypeDef {
              actCanHandle = hcan;
              actCbId = callbackID;
              actCb = cb;
              return HAL_OK;
          });
  HAL_CAN_RegisterCallback_fake.custom_fake = HAL_CAN_RegisterCallback_stub;

  const HalCanInit canInit{};

  // When
  HalCan_init(&canInit);

  // Then
  ASSERT_EQ(HAL_CAN_RegisterCallback_fake.call_count, 1);
  EXPECT_EQ(actCbId, HAL_CAN_RX_FIFO0_MSG_PENDING_CB_ID);
  EXPECT_TRUE(actCb != nullptr);
}

TEST_F(HalCanTest, HalCan_init__should_call_callback_upon_receiving_rx_frame) {
  // Given
  auto cbHasBeenCalled = false;
  const HalCanInit canInit{
          .receive_frame_cb = lambdaToFunPointer([&](const CanFrame *const frame) -> int {
            cbHasBeenCalled = true;
            return 0;
          })
  };

  // When
  HalCan_init(&canInit);

  // Then
  // We call the callback that we registered with HAL. This callback should call the "receive callback" that it received from init
  auto registeredCb = HAL_CAN_RegisterCallback_fake.arg2_val;
  auto canHandler = HAL_CAN_RegisterCallback_fake.arg0_val;
  registeredCb(canHandler);

  ASSERT_TRUE(cbHasBeenCalled);
}

TEST_F(HalCanTest, HalCan_init__should_get_received_frame_and_convert_it_to_CanFrame_and_call_callback) {
  // Given
  CanFrame convertedFrame;
  const HalCanInit canInit{
          .receive_frame_cb = lambdaToFunPointer([&](const CanFrame *const frame) -> int {
              convertedFrame = *frame;
              return 0;
          })
  };

  // When
  HalCan_init(&canInit);

  // Then
  auto registeredCb = HAL_CAN_RegisterCallback_fake.arg2_val;
  auto canHandler = HAL_CAN_RegisterCallback_fake.arg0_val;
  registeredCb(canHandler);
}
