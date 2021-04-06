#ifndef ARTRONICS_HAL_SPI_H
#define ARTRONICS_HAL_SPI_H
#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

typedef enum SpiDevice {
    ART_SPI_DEV_0,
    ART_SPI_DEV_1,
} SpiDevice;

typedef enum SpiSlaveSelect {
    ART_SPI_SS_1 = 0x01,
    ART_SPI_SS_2 = 0x02,
    ART_SPI_SS_3 = 0x04,
    ART_SPI_SS_4 = 0x08,
    ART_SPI_SS_5 = 0x10,
    ART_SPI_SS_6 = 0x20,
    ART_SPI_SS_7 = 0x40,
    ART_SPI_SS_8 = 0x80,
} SpiSlaveSelect;

typedef uint8_t SpiSlaves;

typedef struct HalSpiInit {
    SpiDevice device;
    SpiSlaves slaves;
} HalSpiInit;

typedef struct HalSpiHandler {
    SpiDevice device;
} HalSpiHandler;

int HalSpi_init(HalSpiHandler * spiHandler, const HalSpiInit * halSpiInit);

void HalSpi_transfer(HalSpiHandler *handler, SpiSlaveSelect slave, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size);

#ifdef __cplusplus
}
#endif
#endif //ARTRONICS_HAL_SPI_H
