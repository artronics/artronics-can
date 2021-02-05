#ifndef ARTRONICS_CANMOCK_H
#define ARTRONICS_CANMOCK_H

#include "gmock/gmock.h"
#include "can.h"

class CanMock {
public:
    virtual ~CanMock()= default;;
    MOCK_METHOD(int, Can_init, (const RingBufferHandler can_rx_h));
    MOCK_METHOD(int, Can_transmit, (const CanFrame * const frame));
};



#endif //ARTRONICS_CANMOCK_H
