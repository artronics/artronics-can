
#ifndef ARTRONICS_MESSAGE_PROCESSOR_H
#define ARTRONICS_MESSAGE_PROCESSOR_H
#ifdef __cplusplus
extern "C" {
#endif

#include "can_frame.h"
#include "ring_buffer.h"

int MessageProcessor_init(const RingBufferHandler * can_tx_h);
void MessageProcessor_process(const CanFrame * frame);

#ifdef __cplusplus
}
#endif
#endif //ARTRONICS_MESSAGE_PROCESSOR_H
