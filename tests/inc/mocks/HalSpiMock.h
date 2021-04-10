#ifndef ARTRONICS_HALCANMOCK_H
#define ARTRONICS_HALCANMOCK_H

#include "gmock/gmock.h"
#include "hal/hal_spi.h"

class HalSpiMock {
public:
    virtual ~HalSpiMock()= default;
    MOCK_METHOD(int, HalSpi_init, (HalSpiHandler * spiHandler, const HalSpiInit * halSpiInit));
    MOCK_METHOD(void, HalSpi_transfer, (HalSpiHandler *handler, HalSpiSlaveSelect slave, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size));
};

#endif //ARTRONICS_HALCANMOCK_H
