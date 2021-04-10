#ifndef ARTRONICS_HAL_TMC5160A_H
#define ARTRONICS_HAL_TMC5160A_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

#include "hal/hal_spi.h"

typedef struct HalTmc5160SpiConfig {
    HalSpiDevice spiDevice;
    HalSpiSlaveSelect spiChannel;
} HalTmc5160SpiConfig;

typedef struct HalTmc5160Init {
  HalTmc5160SpiConfig spiConfig;
} HalTmc5160Init;

void HalTmc5160_init(const HalTmc5160Init *);

#ifdef __cplusplus
}
#endif
#endif //ARTRONICS_HAL_TMC5160A_H
