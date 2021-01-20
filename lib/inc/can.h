#ifndef ARTRONICS_CAN_H
#define ARTRONICS_CAN_H
#ifdef __cplusplus
extern C {
#endif
#include "stdbool.h"
#include "stdint.h"

#include "ring_buffer.h"

typedef struct {

} CanInit;

typedef struct {
    uint32_t id;
    uint32_t data[2];
    bool is_remote;
    bool is_extended;
} CanFrame;

int Can_Init(RingBufferHandler rbh);
void Can_Receive(CanFrame const * can_frame);


#ifdef __cplusplus
}
#endif
#endif //ARTRONICS_CAN_H
