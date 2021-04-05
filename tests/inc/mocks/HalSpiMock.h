#ifndef ARTRONICS_HALCANMOCK_H
#define ARTRONICS_HALCANMOCK_H

#include "gmock/gmock.h"
#include "hal/hal_spi.h"

class HalSpiMock {
public:
    virtual ~HalSpiMock()= default;
    MOCK_METHOD(int, HalSpi_init, (const HalSpiInit * const halSpiInit));
};

#endif //ARTRONICS_HALCANMOCK_H
