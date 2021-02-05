#ifndef ARTRONICS_STATE_H
#define ARTRONICS_STATE_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "ring_buffer.h"

#define STATE_CAN_RX_BUF_SIZE 8
#define STATE_CAN_TX_BUF_SIZE 8

int State_init(void);
int State_start(bool isThread);

#ifdef __cplusplus
}
#endif
#endif //ARTRONICS_STATE_H
