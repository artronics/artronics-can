
#ifndef ARTRONICS_MESSAGE_PROCESSOR_H
#define ARTRONICS_MESSAGE_PROCESSOR_H
#ifdef __cplusplus
extern "C" {
#endif

#include "can_frame.h"

int MessageProcessor_init(void);
void MessageProcessor_Process(CanFrame *frame);

#ifdef __cplusplus
}
#endif
#endif //ARTRONICS_MESSAGE_PROCESSOR_H
