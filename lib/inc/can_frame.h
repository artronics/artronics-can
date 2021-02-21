#ifndef ARTRONICS_CAN_FRAME_H
#define ARTRONICS_CAN_FRAME_H
#ifdef __cplusplus
extern "C" {
#endif

#include "stdbool.h"
#include "stdint.h"
#include "stddef.h"

#define CAN_FRAME_MAX_DATA_LEN 8

typedef struct {
    uint8_t data[CAN_FRAME_MAX_DATA_LEN];
    size_t data_size;
    uint32_t id;
    bool is_remote;
    bool is_extended;
} CanFrame;


#ifdef __cplusplus
}
#endif
#endif //ARTRONICS_CAN_FRAME_H
