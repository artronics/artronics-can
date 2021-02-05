#ifndef ARTRONICS_CANMOCK_H
#define ARTRONICS_CANMOCK_H

#include "gmock/gmock.h"
#include "can.h"

class CanMock {
public:
    virtual ~CanMock()= default;;
    MOCK_METHOD(int, Can_init, (const RingBufferHandler * const can_rx_h));
};



#endif //ARTRONICS_CANMOCK_H
