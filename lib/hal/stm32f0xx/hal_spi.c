#include "hal/hal_spi.h"
#include "stm32f042x6.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_spi.h"

SPI_HandleTypeDef spi0Handler;

int HalSpi_init(const HalSpiInit * const halSpiInit) {

  spi0Handler.Instance = SPI1;
  spi0Handler.Init.Mode = SPI_MODE_MASTER;
  spi0Handler.Init.Direction = SPI_DIRECTION_2LINES;
  spi0Handler.Init.DataSize = SPI_DATASIZE_8BIT;
  spi0Handler.Init.CLKPolarity = SPI_POLARITY_LOW;
  spi0Handler.Init.CLKPhase = SPI_PHASE_1EDGE;
  spi0Handler.Init.NSS = SPI_NSS_SOFT;
  spi0Handler.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  spi0Handler.Init.FirstBit = SPI_FIRSTBIT_MSB;
  spi0Handler.Init.TIMode = SPI_TIMODE_DISABLE;
  spi0Handler.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  spi0Handler.Init.CRCPolynomial = 7;
  spi0Handler.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  spi0Handler.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;

  HAL_StatusTypeDef status = HAL_SPI_Init(&spi0Handler);
  if (status != HAL_OK) {
    return status;
  }

  return 0;
}

void HalSpi_transfer(int *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size) {
  HAL_SPI_TransmitReceive(&spi0Handler, pTxData, pRxData, Size, 1000);
}

void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(hspi->Instance==SPI1) {
    __HAL_RCC_SPI1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /**SPI1 GPIO Configuration
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* SPI1 interrupt Init */
    HAL_NVIC_SetPriority(SPI1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(SPI1_IRQn);
  }
}
