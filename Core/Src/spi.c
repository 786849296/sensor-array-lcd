/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    spi.c
  * @brief   This file provides code for the configuration
  *          of the SPI instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "spi.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* SPI2 init function */
void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  LL_SPI_InitTypeDef SPI_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI2);

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
  /**SPI2 GPIO Configuration
  PB13   ------> SPI2_SCK
  PB14   ------> SPI2_MISO
  PB15   ------> SPI2_MOSI
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_13|LL_GPIO_PIN_15;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_14;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* SPI2 DMA Init */

  /* SPI2_TX Init */
  LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_5, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);

  LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_5, LL_DMA_PRIORITY_HIGH);

  LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_5, LL_DMA_MODE_NORMAL);

  LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_5, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_5, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_5, LL_DMA_PDATAALIGN_BYTE);

  LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_5, LL_DMA_MDATAALIGN_BYTE);

  /* USER CODE BEGIN SPI2_Init 1 */
  LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_5);
  /* USER CODE END SPI2_Init 1 */
  SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
  SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
  SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
  SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
  SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
  SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
  SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV2;
  SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
  SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
  SPI_InitStruct.CRCPoly = 10;
  LL_SPI_Init(SPI2, &SPI_InitStruct);
  /* USER CODE BEGIN SPI2_Init 2 */
  LL_SPI_EnableDMAReq_TX(SPI2);
  LL_SPI_Enable(SPI2);
  /* USER CODE END SPI2_Init 2 */

}

/* USER CODE BEGIN 1 */

static void init(Spi* self, osSemaphoreId dmaSemaphore)
{
	self->dmaSemaphore = dmaSemaphore;
	LL_SPI_Enable(self->SPIx);
}

static void deinit(Spi* self)
{
	osSemaphoreDelete(self->dmaSemaphore);
	LL_SPI_Disable(self->SPIx);
}

static uint8_t SpiWriteRead8(Spi* self, uint8_t byte)
{
	assert_param(!LL_GPIO_IsOutputPinSet(self->NSS.group, self->NSS.pin));
	while (!LL_SPI_IsActiveFlag_TXE(self->SPIx));
	LL_SPI_TransmitData8(self->SPIx, byte);
	while (!LL_SPI_IsActiveFlag_RXNE(self->SPIx));
	return LL_SPI_ReceiveData8(self->SPIx);
}

/**
  * @brief  Writes data to SPI using DMA. 
  * @note	Need put data to buffer before call this function.
  * @param  self: Pointer to the Spi structure.
  * @param  timeoutMs: Timeout value in milliseconds.
  * @retval None
  */
static void Spi2DmaWrite(Spi* self, uint8_t* buffer, uint32_t length, uint32_t timeoutMs)
{
	assert_param(!LL_GPIO_IsOutputPinSet(self->NSS.group, self->NSS.pin));
	if (osErrorOS == osSemaphoreWait(*(self->dmaSemaphore), timeoutMs))
	{
		printf("spi dma timeout\n");
		return;
	}
	LL_IWDG_ReloadCounter(IWDG);
	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_5);
	LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_5, (uint32_t)buffer, LL_SPI_DMA_GetRegAddr(SPI2), LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_5));
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_5, length);
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_5);
}

//��̬ʵ��
Spi SPI2Instance = {
	.SPIx = SPI2,
	.NSS = { GPIOB, LL_GPIO_PIN_12 },
	.dmaSemaphore = &spi2DmaSemaphoreHandle,
	.init = init,
	.deinit = deinit,
	.writeRead8 = SpiWriteRead8,
	.dmaWrite = Spi2DmaWrite
};
/* USER CODE END 1 */
