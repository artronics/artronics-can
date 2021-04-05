#ifndef ARTRONICS_HAL_SPI_H
#define ARTRONICS_HAL_SPI_H
#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

typedef enum SpiDevice {
   SPI0,
} SpiDevice;

typedef struct HalSpiInit {
    SpiDevice device;
} HalSpiInit;

int HalSpi_init(const HalSpiInit * halSpiInit);

void HalSpi_transfer(int *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size);

#ifdef __cplusplus
}
#endif
#endif //ARTRONICS_HAL_SPI_H
