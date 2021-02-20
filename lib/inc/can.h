#ifndef ARTRONICS_CAN_H
#define ARTRONICS_CAN_H
#ifdef __cplusplus
extern "C" {
#endif
#include "can_frame.h"
#include "ring_buffer.h"


int Can_init(RingBufferHandler _canRxH);
int Can_transmit(const CanFrame * frame);


#ifdef __cplusplus
}
#endif
#endif //ARTRONICS_CAN_H
