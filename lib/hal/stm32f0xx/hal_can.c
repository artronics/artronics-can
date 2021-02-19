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

static int (*receive_cb)(const CanFrame *frame);

void get_can_frame(int mailboxIndex, CanFrame *f);

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

  // Start CAN
  status = HAL_CAN_Start(&canHandler);

  return status;
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
  UNUSED(hcan);
  CanFrame f;
  get_can_frame(0, &f);
  receive_cb(&f);
}

void get_can_frame(int mailboxIndex, CanFrame *f) {
  CAN_FIFOMailBox_TypeDef mailbox = CAN->sFIFOMailBox[mailboxIndex];

  f->is_extended = mailbox.RIR & CAN_RI0R_IDE;
  f->is_remote = mailbox.RIR & CAN_RI0R_RTR;
  f->id = f->is_extended ? mailbox.RIR >> CAN_RI0R_EXID_Pos : mailbox.RIR >> CAN_RI0R_STID_Pos;
  f->data_size = (mailbox.RDTR & CAN_RDT0R_DLC) >> CAN_RDT0R_DLC_Pos;
  for (int i = 0; i < 4; i++) {
    f->data[i] = (mailbox.RDLR >> i * 8) & 0xFF;
  }
  for (int i = 4; i < 8; i++) {
    f->data[i] = (mailbox.RDHR >> i * 8) & 0xFF;
  }
}

int HalCan_transmit(const CanFrame *frame) {
  return 0;
}

// The callback function doesn't get called because for some reason HAL doesn't have a handler
// Calling it manually
void CEC_CAN_IRQHandler(void) {
  HAL_CAN_IRQHandler(&canHandler);
}
