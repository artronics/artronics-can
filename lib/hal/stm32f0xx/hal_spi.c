#include "hal/hal_spi.h"
#include "stm32f042x6.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_spi.h"

SPI_HandleTypeDef spi0Handler;

void config_ss_gpio(GPIO_TypeDef *, uint32_t pin);

int HalSpi_init(HalSpiHandler * const spiHandler, const HalSpiInit * const halSpiInit) {
  if (halSpiInit->device == ART_SPI_DEV_0) {
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

    spiHandler->device = ART_SPI_DEV_0;

  } else {
    return 1; // TODO: error handling -> NOT_SUPPORTED
  }

  // TODO: this only assumes ART_SPI_SS_1
  if (halSpiInit->slaves & 0x1) {

  } else {
    return 1; // TODO: error handling -> NOT_SUPPORTED
  }

  HAL_StatusTypeDef status = HAL_SPI_Init(&spi0Handler);
  if (status != HAL_OK) {
    return status;
  }
  config_ss_gpio(GPIOA, GPIO_PIN_4);

  return 0;
}

void HalSpi_transfer(struct HalSpiHandler *handler, HalSpiSlaveSelect slave, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size) {
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(&spi0Handler, pTxData, pRxData, Size, 1000);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}

void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi) {
  GPIO_InitTypeDef gpio_init = {0};
  if(hspi->Instance==SPI1) {
    /**SPI1 GPIO Configuration
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI
    */
    gpio_init.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    gpio_init.Mode = GPIO_MODE_AF_PP;
    gpio_init.Pull = GPIO_NOPULL;
    gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio_init.Alternate = GPIO_AF0_SPI1;
    HAL_GPIO_Init(GPIOA, &gpio_init);

    /* SPI1 interrupt Init */
    HAL_NVIC_SetPriority(SPI1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(SPI1_IRQn);
  }
}

void config_ss_gpio(GPIO_TypeDef * gpio, uint32_t pin) {
  GPIO_InitTypeDef gpio_init = {0};
  gpio_init.Pin = pin;
  gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
  gpio_init.Pull = GPIO_PULLUP;
  gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(gpio, &gpio_init);

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}
