#include "can.h"
#include "stm32f0xx_hal_can.h"
#include "stm32f042x6.h"

HAL_StatusTypeDef init_filter(void);
HAL_StatusTypeDef init_filter_module_id(void);

static CAN_HandleTypeDef hCan;

HAL_StatusTypeDef CAN_Init(void) {
  HAL_CAN_Init(&hCan);
}

void CAN_initGpio(void);

HAL_StatusTypeDef  init_can(void) {

  CAN_initGpio();

  hCan.Instance = CAN;
//  hCan.State = HAL_CAN_STATE_RESET;
  hCan.Init.Prescaler = 12;
  hCan.Init.Mode = CAN_MODE_NORMAL;
  hCan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hCan.Init.TimeSeg1 = CAN_BS1_6TQ;
  hCan.Init.TimeSeg2 = CAN_BS2_1TQ;
  hCan.Init.TimeTriggeredMode = DISABLE;
  hCan.Init.AutoBusOff = DISABLE;
  hCan.Init.AutoWakeUp = DISABLE;
  hCan.Init.AutoRetransmission = DISABLE;
  hCan.Init.ReceiveFifoLocked = DISABLE;
  hCan.Init.TransmitFifoPriority = DISABLE;

  HAL_StatusTypeDef status;
  status = HAL_CAN_Init(&hCan);
  if (status != HAL_OK) {
    return status;
  }
  CAN->MCR = CAN->MCR && (~0x0100);
  __HAL_UNFREEZE_CAN_DBGMCU();
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
  sFilterConfig.FilterIdHigh = 0x0;
  sFilterConfig.FilterIdLow = 0x0;
  sFilterConfig.FilterMaskIdHigh = 0x0;
  sFilterConfig.FilterMaskIdLow = 0x0;
  sFilterConfig.FilterFIFOAssignment = 0;
  sFilterConfig.FilterActivation = ENABLE;
  sFilterConfig.FilterBank = 0;

  HAL_StatusTypeDef status;
  status = HAL_CAN_ConfigFilter(&hCan, &sFilterConfig);
  if (status != HAL_OK) {
    return status;
  }
//  status = init_filter_module_id();
//  if (status != HAL_OK) {
//    return status;
//  }

  status = HAL_CAN_Start(&hCan); //start CAN
  if (status != HAL_OK) {
    return status;
  }

  return HAL_CAN_ActivateNotification(&hCan, CAN_IT_RX_FIFO0_MSG_PENDING); //enable interrupts
}

HAL_StatusTypeDef init_filter_module_id(void) {
  CAN_FilterTypeDef sFilterConfig; //declare CAN filter structure

  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterIdHigh = 0x0000;
  sFilterConfig.FilterIdLow = 0x0000;
  sFilterConfig.FilterMaskIdHigh = 0x0000;
  sFilterConfig.FilterMaskIdLow = 0x000F;
  sFilterConfig.FilterFIFOAssignment = 0;
  sFilterConfig.FilterActivation = ENABLE;
  sFilterConfig.FilterBank = 0;

  HAL_StatusTypeDef status;
  status = HAL_CAN_ConfigFilter(&hCan, &sFilterConfig);
  if (status != HAL_OK) {
    return status;
  }

}

void send_can_data(CAN_TxHeaderTypeDef *pTxHeader, uint8_t *data) {
  uint32_t mailbox = 0xFF;
  HAL_StatusTypeDef status = HAL_CAN_AddTxMessage(&hCan, pTxHeader, data, &mailbox);
  if (status != HAL_OK) {
    return;
  }

}

//void HAL_CAN_IRQHandler(CAN_HandleTypeDef *hcan) {
//  __ASM("nop");
//}
