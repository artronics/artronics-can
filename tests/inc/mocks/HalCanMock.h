#ifndef ARTRONICS_HALCANMOCK_H
#define ARTRONICS_HALCANMOCK_H

#include "gmock/gmock.h"
#include "hal/hal_can.h"

class HalCanMock {
public:
    virtual ~HalCanMock()= default;
    MOCK_METHOD(int, HalCan_init, (const HalCanInit * const hal_can_init));
    MOCK_METHOD(int, HalCan_transmit, (const CanFrame * const frame));
};

#endif //ARTRONICS_HALCANMOCK_H
