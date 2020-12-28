#include "can.h"
#include "stm32f0xx_hal_can.h"

HAL_StatusTypeDef init_filter(void);

CAN_HandleTypeDef hcan1;


void CAN_initGpio(void);

HAL_StatusTypeDef  init_can(void) {

  CAN_initGpio();

  hcan1.Instance = CAN;
//  hcan1.State = HAL_CAN_STATE_RESET;
  hcan1.Init.Prescaler = 12;
  hcan1.Init.Mode = CAN_MODE_LOOPBACK;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_6TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_1TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;

  HAL_StatusTypeDef status;
  status = HAL_CAN_Init(&hcan1);
  if (status != HAL_OK) {
    return status;
  }

  return init_filter();
}

void CAN_initGpio(void) {
  GPIO_InitTypeDef gpioInit = {0};
  gpioInit.Pin = GPIO_PIN_11|GPIO_PIN_12;
  gpioInit.Mode = GPIO_MODE_AF_PP;
  gpioInit.Pull = GPIO_NOPULL;
  gpioInit.Speed = GPIO_SPEED_FREQ_HIGH;
  gpioInit.Alternate = GPIO_AF4_CAN;

  HAL_GPIO_Init(GPIOA, &gpioInit);
}

HAL_StatusTypeDef init_filter(void) {
  CAN_FilterTypeDef sFilterConfig; //declare CAN filter structure

  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterIdHigh = 0x0000;
  sFilterConfig.FilterIdLow = 0x0000;
  sFilterConfig.FilterMaskIdHigh = 0x0000;
  sFilterConfig.FilterMaskIdLow = 0x0000;
  sFilterConfig.FilterFIFOAssignment = 0;
  sFilterConfig.FilterActivation = ENABLE;
  sFilterConfig.FilterBank = 7;

  HAL_StatusTypeDef status;
  status = HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig);
  if (status != HAL_OK) {
    return status;
  }

  status = HAL_CAN_Start(&hcan1); //start CAN
  if (status != HAL_OK) {
    return status;
  }

  return HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING); //enable interrupts
}

void send_can_data(CAN_TxHeaderTypeDef *pTxHeader, uint8_t *data) {
  uint32_t mailbox = 0xFF;
  HAL_StatusTypeDef status = HAL_CAN_AddTxMessage(&hcan1, pTxHeader, data, &mailbox);
  if (status != HAL_OK) {
    return;
  }

}

//void HAL_CAN_IRQHandler(CAN_HandleTypeDef *hcan) {
//  __ASM("nop");
//}
