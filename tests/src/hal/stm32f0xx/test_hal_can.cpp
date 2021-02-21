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
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CAN_ActivateNotification, CAN_HandleTypeDef *, uint32_t);
FAKE_VOID_FUNC(HAL_CAN_IRQHandler, CAN_HandleTypeDef *);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CAN_Start, CAN_HandleTypeDef *);

typedef void (*pCallback)(CAN_HandleTypeDef *);

FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CAN_RegisterCallback, CAN_HandleTypeDef *, HAL_CAN_CallbackIDTypeDef, pCallback);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CAN_GetRxMessage, CAN_HandleTypeDef *, uint32_t, CAN_RxHeaderTypeDef *,
                uint8_t *);
FAKE_VALUE_FUNC(HAL_StatusTypeDef, HAL_CAN_AddTxMessage, CAN_HandleTypeDef *, CAN_TxHeaderTypeDef *, uint8_t *,
                uint32_t *)

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
      RESET_FAKE(HAL_CAN_ActivateNotification)
      RESET_FAKE(HAL_CAN_IRQHandler)
      RESET_FAKE(HAL_CAN_Start)
      RESET_FAKE(HAL_CAN_GetRxMessage)
      RESET_FAKE(HAL_CAN_AddTxMessage)
      FFF_RESET_HISTORY()
    }
};

TEST_F(HalCanTest, HalCan_init__should_init_CAN_GPIOs) {
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

TEST_F(HalCanTest, HalCan_init__should_set_CAN_IRQs) {
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

TEST_F(HalCanTest, HalCan_init__should_init_CAN_HAL) {
  // Given
  CAN_HandleTypeDef actCanHandle;
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
//  EXPECT_EQ(actCanHandle.Instance, CAN);
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

TEST_F(HalCanTest, HalCan_init__should_init_HAL_CAN_ConfigFilter) {
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
  const HalCanInit canInit{};

  // When
  HalCan_init(&canInit);

  // Then
  auto *canHandler0 = HAL_CAN_RegisterCallback_fake.arg0_history[0];
  auto *canHandler1 = HAL_CAN_RegisterCallback_fake.arg0_history[1];
  auto cbId0 = HAL_CAN_RegisterCallback_fake.arg1_history[0];
  auto cbId1 = HAL_CAN_RegisterCallback_fake.arg1_history[1];
  auto cb0 = HAL_CAN_RegisterCallback_fake.arg2_history[0];
  auto cb1 = HAL_CAN_RegisterCallback_fake.arg2_history[1];

  ASSERT_EQ(HAL_CAN_RegisterCallback_fake.call_count, 2);
  EXPECT_TRUE(canHandler0 != nullptr);
  EXPECT_EQ(canHandler0, canHandler1);
  EXPECT_EQ(cbId0, HAL_CAN_RX_FIFO0_MSG_PENDING_CB_ID);
  EXPECT_EQ(cbId1, HAL_CAN_RX_FIFO1_MSG_PENDING_CB_ID);
  EXPECT_TRUE(cb0 != nullptr);
  EXPECT_TRUE(cb1 != nullptr);
  EXPECT_TRUE(cb1 != cb0);
}

TEST_F(HalCanTest, HalCan_init__should_activate_notifications) {
  // Given
  HalCanInit canInit;

  // When
  HalCan_init(&canInit);

  // Then
  ASSERT_EQ(HAL_CAN_ActivateNotification_fake.call_count, 2);
  EXPECT_EQ(HAL_CAN_ActivateNotification_fake.arg1_history[0], CAN_IT_RX_FIFO0_MSG_PENDING);
  EXPECT_EQ(HAL_CAN_ActivateNotification_fake.arg1_history[1], CAN_IT_RX_FIFO1_MSG_PENDING);
}

TEST_F(HalCanTest, HalCan_init__should_read_rx_fifo_and_create_CanFrame_fifo0_1) {
  CanFrame convertedFrame;
  const HalCanInit canInit{
          .receivedFrameCallback = lambdaToFunPointer([&](const CanFrame *const frame) -> int {
              convertedFrame = *frame;
              return 0;
          })
  };
  auto expDataSize = 3;
  auto expIsRemote = 0; // expected to be converted to false
  auto expIsStdId = 0; // IDE value; 0 means standard id
  auto standardId = 123;
  auto HAL_CAN_GetRxMessage_stub = lambdaToFunPointer(
          [&](CAN_HandleTypeDef *hcan, uint32_t RxFifo, CAN_RxHeaderTypeDef *pHeader,
              uint8_t aData[]) -> HAL_StatusTypeDef {
              for (auto i = 0; i < 8; i++) {
                aData[i] = i;
              }
              pHeader->DLC = expDataSize;
              pHeader->RTR = expIsRemote;
              pHeader->IDE = expIsStdId;
              pHeader->StdId = standardId;
              pHeader->ExtId = standardId + 1000; // just make sure ext != std

              return HAL_OK;
          });
  HAL_CAN_GetRxMessage_fake.custom_fake = HAL_CAN_GetRxMessage_stub;

  // When
  HalCan_init(&canInit);

  // Then
  auto registeredCb = HAL_CAN_RegisterCallback_fake.arg2_history[0];
  auto canHandler = HAL_CAN_RegisterCallback_fake.arg0_history[0];
  registeredCb(canHandler);

  ASSERT_EQ(HAL_CAN_GetRxMessage_fake.call_count, 1);
  EXPECT_EQ(HAL_CAN_GetRxMessage_fake.arg1_val, CAN_RX_FIFO0);
  for (auto i = 0; i < 8; i++) {
    EXPECT_EQ(convertedFrame.data[i], i);
  }
  EXPECT_EQ(convertedFrame.data_size, expDataSize);
  EXPECT_EQ(convertedFrame.is_remote, expIsRemote);
  EXPECT_EQ(convertedFrame.is_extended, expIsStdId);
  EXPECT_EQ(convertedFrame.id, standardId);
}

TEST_F(HalCanTest, HalCan_init__should_read_rx_fifo_and_create_CanFrame_fifo0_2) {
  CanFrame convertedFrame;
  const HalCanInit canInit{
          .receivedFrameCallback = lambdaToFunPointer([&](const CanFrame *const frame) -> int {
              convertedFrame = *frame;
              return 0;
          })
  };
  auto expIsRemote = 1; // expected to be converted to true
  auto expIsStdId = 1; // IDE value; 1 means extended id
  auto extendedId = 123;
  auto HAL_CAN_GetRxMessage_stub = lambdaToFunPointer(
          [&](CAN_HandleTypeDef *hcan, uint32_t RxFifo, CAN_RxHeaderTypeDef *pHeader,
              uint8_t aData[]) -> HAL_StatusTypeDef {
              pHeader->RTR = expIsRemote;
              pHeader->IDE = expIsStdId;
              pHeader->StdId = extendedId - 20; // just make sure ext != std
              pHeader->ExtId = extendedId;

              return HAL_OK;
          });
  HAL_CAN_GetRxMessage_fake.custom_fake = HAL_CAN_GetRxMessage_stub;

  // When
  HalCan_init(&canInit);

  // Then
  auto registeredCb = HAL_CAN_RegisterCallback_fake.arg2_history[0];
  auto canHandler = HAL_CAN_RegisterCallback_fake.arg0_history[0];
  registeredCb(canHandler);

  EXPECT_EQ(convertedFrame.is_remote, expIsRemote);
  EXPECT_EQ(convertedFrame.is_extended, expIsStdId);
  EXPECT_EQ(convertedFrame.id, extendedId);
}

TEST_F(HalCanTest, HalCan_init__should_read_rx_fifo_and_create_CanFrame_fifo1_1) {
  CanFrame convertedFrame;
  const HalCanInit canInit{
          .receivedFrameCallback = lambdaToFunPointer([&](const CanFrame *const frame) -> int {
              convertedFrame = *frame;
              return 0;
          })
  };
  auto expDataSize = 3;
  auto expIsRemote = 0; // expected to be converted to false
  auto expIsStdId = 0; // IDE value; 0 means standard id
  auto standardId = 123;
  auto HAL_CAN_GetRxMessage_stub = lambdaToFunPointer(
          [&](CAN_HandleTypeDef *hcan, uint32_t RxFifo, CAN_RxHeaderTypeDef *pHeader,
              uint8_t aData[]) -> HAL_StatusTypeDef {
              for (auto i = 0; i < 8; i++) {
                aData[i] = i;
              }
              pHeader->DLC = expDataSize;
              pHeader->RTR = expIsRemote;
              pHeader->IDE = expIsStdId;
              pHeader->StdId = standardId;
              pHeader->ExtId = standardId + 1000; // just make sure ext != std

              return HAL_OK;
          });
  HAL_CAN_GetRxMessage_fake.custom_fake = HAL_CAN_GetRxMessage_stub;

  // When
  HalCan_init(&canInit);

  // Then
  auto registeredCb = HAL_CAN_RegisterCallback_fake.arg2_history[1];
  auto canHandler = HAL_CAN_RegisterCallback_fake.arg0_history[1];
  registeredCb(canHandler);

  ASSERT_EQ(HAL_CAN_GetRxMessage_fake.call_count, 1);
  EXPECT_EQ(HAL_CAN_GetRxMessage_fake.arg1_val, CAN_RX_FIFO1);
  for (auto i = 0; i < 8; i++) {
    EXPECT_EQ(convertedFrame.data[i], i);
  }
  EXPECT_EQ(convertedFrame.data_size, expDataSize);
  EXPECT_EQ(convertedFrame.is_remote, expIsRemote);
  EXPECT_EQ(convertedFrame.is_extended, expIsStdId);
  EXPECT_EQ(convertedFrame.id, standardId);
}

TEST_F(HalCanTest, HalCan_init__should_read_rx_fifo_and_create_CanFrame_fifo1_2) {
  CanFrame convertedFrame;
  const HalCanInit canInit{
          .receivedFrameCallback = lambdaToFunPointer([&](const CanFrame *const frame) -> int {
              convertedFrame = *frame;
              return 0;
          })
  };
  auto expIsRemote = 1; // expected to be converted to true
  auto expIsStdId = 1; // IDE value; 1 means extended id
  auto extendedId = 123;
  auto HAL_CAN_GetRxMessage_stub = lambdaToFunPointer(
          [&](CAN_HandleTypeDef *hcan, uint32_t RxFifo, CAN_RxHeaderTypeDef *pHeader,
              uint8_t aData[]) -> HAL_StatusTypeDef {
              pHeader->RTR = expIsRemote;
              pHeader->IDE = expIsStdId;
              pHeader->StdId = extendedId - 20; // just make sure ext != std
              pHeader->ExtId = extendedId;

              return HAL_OK;
          });
  HAL_CAN_GetRxMessage_fake.custom_fake = HAL_CAN_GetRxMessage_stub;

  // When
  HalCan_init(&canInit);

  // Then
  auto registeredCb = HAL_CAN_RegisterCallback_fake.arg2_history[1];
  auto canHandler = HAL_CAN_RegisterCallback_fake.arg0_history[1];
  registeredCb(canHandler);

  EXPECT_EQ(convertedFrame.is_remote, expIsRemote);
  EXPECT_EQ(convertedFrame.is_extended, expIsStdId);
  EXPECT_EQ(convertedFrame.id, extendedId);
}

TEST_F(HalCanTest, HalCan_transmit__should_transmit_frame) {
  // Given
  CanFrame f = {.data = {0}};

  uint8_t *expData = f.data;
  uint8_t *actData = nullptr;
  uint32_t *acPTxMailbox = nullptr;
  CAN_HandleTypeDef  *actCanHandler = nullptr;
  CAN_TxHeaderTypeDef actHeader;

  auto HAL_CAN_AddTxMessage_stub = lambdaToFunPointer(
          [&](CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *pHeader, uint8_t aData[], uint32_t *pTxMailbox) -> HAL_StatusTypeDef {
              actData = aData;
              acPTxMailbox = pTxMailbox;
              actCanHandler = hcan;
              actHeader = *pHeader;

              return HAL_OK;
          });
  HAL_CAN_AddTxMessage_fake.custom_fake = HAL_CAN_AddTxMessage_stub;

  // When
  HalCan_transmit(&f);

  // Then
  ASSERT_EQ(HAL_CAN_AddTxMessage_fake.call_count, 1);
  EXPECT_TRUE(actData != nullptr);
  EXPECT_EQ(expData, actData);
  EXPECT_TRUE(acPTxMailbox != nullptr);
  EXPECT_TRUE(acPTxMailbox != nullptr);
  EXPECT_TRUE(actCanHandler != nullptr);
}
