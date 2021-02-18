#include "hal/hal_can.h"
#include "stm32f042x6.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_gpio_ex.h"
#include "stm32f0xx_hal_can.h"

static CAN_HandleTypeDef canHandler = {
        .Instance = CAN,
        .Init = {
                .Prescaler = 12,
                .Mode = CAN_MODE_NORMAL,
                .SyncJumpWidth = CAN_SJW_1TQ,
                .TimeSeg1 = CAN_BS1_6TQ,
                .TimeSeg2 = CAN_BS2_1TQ,
                .TimeTriggeredMode = DISABLE,
                .AutoBusOff = DISABLE,
                .AutoWakeUp = DISABLE,
                .AutoRetransmission = DISABLE,
                .ReceiveFifoLocked = DISABLE,
                .TransmitFifoPriority = DISABLE,
        }
};

static int (*receive_cb)(const CanFrame * frame);

int HalCan_init(const HalCanInit *const hal_can_init) {
  // Init GPIOs
  GPIO_InitTypeDef gpioInit = {0};
  gpioInit.Pin = GPIO_PIN_11 | GPIO_PIN_12;
  gpioInit.Mode = GPIO_MODE_AF_PP;
  gpioInit.Pull = GPIO_NOPULL;
  gpioInit.Speed = GPIO_SPEED_FREQ_HIGH;
  gpioInit.Alternate = GPIO_AF4_CAN;

  HAL_GPIO_Init(GPIOA, &gpioInit);

  // Set IRQ
  HAL_NVIC_SetPriority(CEC_CAN_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(CEC_CAN_IRQn);

  // Init HAL CAN
  HAL_StatusTypeDef status;
  status = HAL_CAN_Init(&canHandler);
  if (status != HAL_OK) {
    return status;
  }

  // Config HAL CAN filter
  CAN_FilterTypeDef sFilterConfig = {
          .FilterMode = CAN_FILTERMODE_IDMASK,
          .FilterScale = CAN_FILTERSCALE_32BIT,
          .FilterIdHigh = 0x0000,
          .FilterIdLow = 0x0000,
          .FilterMaskIdHigh = 0x0000,
          .FilterMaskIdLow = 0x000F,
          .FilterFIFOAssignment = 0,
          .FilterActivation = ENABLE,
          .FilterBank = 0,
  };

  status = HAL_CAN_ConfigFilter(&canHandler, &sFilterConfig);
  if (status != HAL_OK) {
    return status;
  }

  // Set notifications and register callbacks
  receive_cb = hal_can_init->receive_frame_cb;
  status = HAL_CAN_RegisterCallback(&canHandler, HAL_CAN_RX_FIFO0_MSG_PENDING_CB_ID, HAL_CAN_RxFifo0MsgPendingCallback);
  if (status != HAL_OK) {
    return status;
  }

  status = HAL_CAN_ActivateNotification(&canHandler, CAN_IT_RX_FIFO0_MSG_PENDING); //enable interrupts
  if (status != HAL_OK) {
    return status;
  }

  return 0;
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  CanFrame f;
  receive_cb(&f);
}

int HalCan_transmit(const CanFrame *frame) {
  return 0;
}

// The callback function doesn't get called because for some reason HAL doesn't have a handler
// Calling it manually
void CEC_CAN_IRQHandler(void) {
  HAL_CAN_IRQHandler(&canHandler);
}
