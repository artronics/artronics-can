#ifndef ARTRONICS_CAN_FRAME_H
#define ARTRONICS_CAN_FRAME_H
#ifdef __cplusplus
extern "C" {
#endif

#include "stdbool.h"
#include "stdint.h"

#define CAN_FRAME_MAX_DATA_LEN 8

typedef struct CanFrame {
    uint32_t id;
    uint32_t data[2];
//    uint8_t data[CAN_FRAME_MAX_DATA_LEN];
//    size_t data_size;
    bool is_remote;
    bool is_extended;
} CanFrame;

//int CanFrame_make(CanFrame *frame, uint32_t id, uint32_t *data, size_t size, );

#ifdef __cplusplus
}
#endif
#endif //ARTRONICS_CAN_FRAME_H
