#ifndef __CAN_H
#define __CAN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f0xx.h"
#include "stm32f0xx_hal_can.h"

HAL_StatusTypeDef CAN_Init(void);
HAL_StatusTypeDef init_can(void);
void send_can_data(CAN_TxHeaderTypeDef *pTxHeader, uint8_t *data);

#ifdef __cplusplus
}
#endif
#endif /* __CAN_H */
