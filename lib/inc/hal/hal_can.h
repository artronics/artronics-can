#ifndef ARTRONICS_HAL_CAN_H
#define ARTRONICS_HAL_CAN_H
#ifdef __cplusplus
extern "C" {
#endif

#include "can_frame.h"

typedef int (*ReceiveFrameCb)(const CanFrame * const frame);

typedef struct HalCanInit {
    ReceiveFrameCb receivedFrameCallback;

} HalCanInit;

int HalCan_init(const HalCanInit *halCanInit);
int HalCan_transmit(const CanFrame *frame);

#ifdef __cplusplus
}
#endif
#endif //ARTRONICS_HAL_CAN_H
