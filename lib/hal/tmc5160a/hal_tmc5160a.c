#include "hal/tmc/hal_tmc5160a.h"
#include "hal/hal_spi.h"
#include "hal/tmc/TMC5160.h"

static HalSpiHandler spiHandler;
static TMC5160TypeDef tmc5160TypeDef;
static ConfigurationTypeDef config;
static int32_t regResetState;

void HalTmc5160_init(const HalTmc5160Init *const halTmc5160Init) {
  HalSpiInit spiInit = {.device = ART_SPI_DEV_0, .slaves = ART_SPI_SS_1};
  HalSpi_init(&spiHandler, &spiInit);

//  void tmc5160_init(TMC5160TypeDef *tmc5160, uint8_t channel, ConfigurationTypeDef *config, const int32_t *registerResetState)
  tmc5160_init(&tmc5160TypeDef, 0, &config, &regResetState);
  tmc5160_rotate(&tmc5160TypeDef, 100);
}

void tmc5160_readWriteArray(uint8_t channel, uint8_t *data, size_t length) {
  HalSpi_transfer(&spiHandler, ART_SPI_SS_1, data, data, length);
}
