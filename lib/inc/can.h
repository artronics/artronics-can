#ifndef ARTRONICS_CAN_H
#define ARTRONICS_CAN_H
#ifdef __cplusplus
extern "C" {
#endif
#include "can_frame.h"
#include "ring_buffer.h"


int Can_init(const RingBufferHandler * can_rx_h);
void Can_receive(CanFrame const * can_frame);


#ifdef __cplusplus
}
#endif
#endif //ARTRONICS_CAN_H
