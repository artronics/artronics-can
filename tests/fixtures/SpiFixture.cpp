#include "SpiFixture.h"

std::unique_ptr<HalSpiMock> SpiFixture::_halSpiMock;

int HalSpi_init(HalSpiHandler * spiHandler, const HalSpiInit * const halSpiInit) {
  return SpiFixture::_halSpiMock->HalSpi_init(spiHandler, halSpiInit);
}

void HalSpi_transfer(struct HalSpiHandler *handler, HalSpiSlaveSelect slave, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size) {
  return SpiFixture::_halSpiMock->HalSpi_transfer(handler, slave, pTxData, pRxData, Size);
}

