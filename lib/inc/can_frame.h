#ifndef ARTRONICS_CAN_FRAME_H
#define ARTRONICS_CAN_FRAME_H
#ifdef __cplusplus
extern "C" {
#endif

#include "stdbool.h"
#include "stdint.h"

typedef struct CanFrame {
    uint32_t id;
    uint32_t data[2];
    bool is_remote;
    bool is_extended;
#ifdef __cplusplus

    bool operator==(const CanFrame &rhs) const {
      return id == rhs.id
             && is_remote == rhs.is_remote
             && is_extended == rhs.is_extended
             && data[0] == rhs.data[0]
             && data[1] == rhs.data[1];
    }

#endif
} CanFrame;

#ifdef __cplusplus
}
#endif
#endif //ARTRONICS_CAN_FRAME_H
