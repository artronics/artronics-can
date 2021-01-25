#ifndef ARTRONICS_CAN_H
#define ARTRONICS_CAN_H
#ifdef __cplusplus
extern C {
#endif
#include "can_frame.h"

typedef struct {

} CanInit;


int Can_Init(void);
void Can_Receive(CanFrame const * can_frame);


#ifdef __cplusplus
}
#endif
#endif //ARTRONICS_CAN_H
