#ifndef ARTRONICS_STATE_H
#define ARTRONICS_STATE_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "ring_buffer.h"

#define STATE_CAN_RX_BUF_SIZE 8

int State_Init(void);
int State_Start(bool isThread);

RingBufferHandler *State_GetCanRxBufHandler(void);

#ifdef __cplusplus
}
#endif
#endif //ARTRONICS_STATE_H
