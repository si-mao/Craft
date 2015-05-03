/*
 * stm32_spi.c
 * SPI通信模块
 * 创建者：死猫
 * 创建时间：2015.01.16
 * 修改者：（每次修改后，请将最后修改者置于末尾，其他人顺序保持不变）
 *     死猫
 * 最终修改时间：2015.04.05
 */

/*
 * SPI1_SCK  <-> PA5
 * SPI1_MISO <-> PA6
 * SPI1_MOSI <-> PA7
 * SPI1_CS1  <-> PC4 (defined by user)
 */

/*
 * SPI2_SCK  <-> PB13
 * SPI2_MISO <-> PB14
 * SPI2_MOSI <-> PB15
 * SPI2_CS1  <-> PB12 (defined by user)
 */
 

#include "system_define.h"
#include "hardware.h"		//io端口宏定义
#include "stm32_config.h"
#include "error.h"

#include "stm32_delay.h"
#include "stm32_spi.h"

#define TIMEOUT_SPI 100
void spi_wait_txe_n(SPI_TypeDef* SPIx)
{
	uint32_t timeout;
	timeout = TIMEOUT_SPI;
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET)
	{
		timeout--;
		if(timeout <= 0)
		{
			error_alarm(ERROR_CODE_EX_SPI | ERROR_CODE_TIME_OUT);
			break;
		}
		delayus(1);
	}
}

void spi_wait_rxe(SPI_TypeDef* SPIx)
{
	uint32_t timeout;
	timeout = TIMEOUT_SPI;
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET)
	{
		timeout--;
		if(timeout <= 0)
		{
			error_alarm(ERROR_CODE_EX_SPI | ERROR_CODE_TIME_OUT);
			break;
		}
		delayus(1);
	}
}

#if (SPI1_EN == 1)

void spi1_init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

	//////下面是SPI相关GPIO初始化//////
	//SPI1模块对应的SCK、MISO、MOSI为AF引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* SPI1 configuration */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI1_BAUD_SCALER;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	
	//CS
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	PCout(4) = 1;

	SPI_Cmd(SPI1, ENABLE);					   //Enable SPI
}

uint8_t spi1_rw(uint8_t dat)
{
	spi_wait_txe_n(SPI1);
	SPI_I2S_SendData(SPI1, dat);
	spi_wait_rxe(SPI1);
	return SPI_I2S_ReceiveData(SPI1);
}
#endif

#if (SPI2_EN == 1)

void spi2_init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	//////下面是SPI相关GPIO初始化//////
	//SPI1模块对应的SCK、MISO、MOSI为AF引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* SPI1 configuration */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI2_BAUD_SCALER;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);
	
	//CS
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	PBout(12) = 1;

	SPI_Cmd(SPI2, ENABLE);					   //Enable SPI
}

uint8_t spi2_rw(uint8_t dat)
{
	spi_wait_txe_n(SPI2);
	SPI_I2S_SendData(SPI2, dat);
	spi_wait_rxe(SPI2);
	return SPI_I2S_ReceiveData(SPI2);
}
#endif

void spi_init(void)
{
#if (SPI1_EN == 1)
	spi1_init();
#endif

#if (SPI2_EN == 1)
	spi2_init();
#endif
}

#define SPI_CR1_BR_MASK ((uint16_t)0x0038)
void spi_set_baud(SPI_TypeDef* SPIx, uint16_t SPI_BaudRatePrescaler)
{
	uint16_t tmpreg = 0;
	tmpreg = SPIx->CR1;
	tmpreg &= ~SPI_CR1_BR_MASK;
	tmpreg |= SPI_BaudRatePrescaler;
	SPIx->CR1 = tmpreg;
}
