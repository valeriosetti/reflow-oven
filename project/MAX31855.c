/*
 * MAX31855.c
 *
 * This is the driver for the MAX31855 "Cold-Junction Compensated Thermocouple-to-Digital
 * Converter". SPI1 will be used to interface to this IC with the following pinout:
 * 	- PA5 => CLK
 * 	- PA6 => MOSI (actually this will not be configured since the IC has no input pin...)
 * 	- PA7 => MISO
 * 	- PA3 => CS (this will be controlled by software, not by HW)
 *
 * 	Unfortunately this SPI port is already used on the F4 discovery board and it's
 * 	connected to the LIS3DSH sensor. In order to multiplex this bus, the accelerometer's
 * 	CS signal will be initialized as GPIO and forced high.
 */


// Includes
#include "MAX31855.h"

// Local defines
#define MAX31855_THERMOCOUPLE_DATA_MASK		((uint32_t)0xFFFC0000)
#define MAX31855_INTERNAL_DATA_MASK			((uint32_t)0x0000FFF0)
//// CE
#define	MAX31855_CS_PORT				GPIOA
#define	MAX31855_CS_PIN					GPIO_PIN_3
#define	MAX31855_CS_CLKEN				__GPIOA_CLK_ENABLE
//// MOSI
#define	MAX31855_MISO_PORT				GPIOA
#define	MAX31855_MISO_PIN				GPIO_PIN_7
#define	MAX31855_MISO_CLKEN				__GPIOA_CLK_ENABLE
//// CLK
#define	MAX31855_CLK_PORT				GPIOA
#define	MAX31855_CLK_PIN				GPIO_PIN_5
#define	MAX31855_CLK_CLKEN				__GPIOA_CLK_ENABLE
//// CS for the LIS3DSH
#define	LIS3DSH_CLK_PORT				GPIOE
#define	LIS3DSH_CLK_PIN					GPIO_PIN_3
#define	LIS3DSH_CLK_CLKEN				__GPIOE_CLK_ENABLE
//// Shortcuts for pin handling
#define MAX31855_CE_LOW			HAL_GPIO_WritePin(MAX31855_CS_PORT, MAX31855_CS_PIN, GPIO_PIN_RESET)
#define MAX31855_CE_HIGH		HAL_GPIO_WritePin(MAX31855_CS_PORT, MAX31855_CS_PIN, GPIO_PIN_SET)

// Local variables
SPI_HandleTypeDef hSPI;
typedef union {
	uint8_t  bytes[4];
	uint32_t reg;
}MAX31855_data;

/*
 * Initialize the GPIOs and the SPI peripheral
 */
void MAX31855_init()
{
	GPIO_InitTypeDef 	GPIO_InitStruct;

	// Disable the onboard MEMS sensor
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Pin = LIS3DSH_CLK_PIN;
	HAL_GPIO_Init(LIS3DSH_CLK_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(LIS3DSH_CLK_PORT, LIS3DSH_CLK_PIN, GPIO_PIN_SET);

	// Configure the software controlled CS pin
	GPIO_InitStruct.Pin = MAX31855_CS_PIN;
	HAL_GPIO_Init(MAX31855_CS_PORT, &GPIO_InitStruct);
	MAX31855_CE_HIGH;

	// Configure the SPI pins
	MAX31855_CS_CLKEN();
	MAX31855_MISO_CLKEN();
	MAX31855_CLK_CLKEN();
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
	//// MISO
	GPIO_InitStruct.Pin = MAX31855_MISO_PIN;
	HAL_GPIO_Init(MAX31855_MISO_PORT, &GPIO_InitStruct);
	//// CLK
	GPIO_InitStruct.Pin = MAX31855_CLK_PIN;
	HAL_GPIO_Init(MAX31855_CLK_PORT, &GPIO_InitStruct);

	// Initialize SPI
	__HAL_RCC_SPI1_CLK_ENABLE();
	hSPI.Instance = SPI1;
	hSPI.Init.Mode = SPI_MODE_MASTER;
	hSPI.Init.Direction = SPI_DIRECTION_2LINES;
	hSPI.Init.DataSize = SPI_DATASIZE_8BIT;
	hSPI.Init.CLKPolarity = SPI_POLARITY_LOW;
	hSPI.Init.CLKPhase = SPI_PHASE_1EDGE;
	hSPI.Init.NSS = SPI_NSS_SOFT;
	hSPI.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
	hSPI.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hSPI.Init.TIMode = SPI_TIMODE_DISABLED;
	hSPI.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
	hSPI.Init.CRCPolynomial = 7;
	HAL_SPI_Init(&hSPI);
}

/*
 * Performs a read from the MAX31855.
 * Parameters:
 * 	- thermo_temp => it returns the 14bit thermocouple's temperature
 * 	- int_temp => it returns the 12bit internal temperature
 * Returns:
 * 	- an "uint8_t" which contains possible faults. Using masks defined in the header
 * 	file it's possible to determine the type of fault.
 */
void MAX31855_read(uint16_t* thermo_temp, uint16_t* int_temp, uint8_t* status)
{
	MAX31855_data  raw_reading;
	uint32_t tmp;

	MAX31855_CE_LOW;
	HAL_SPI_Receive(&hSPI, &raw_reading.bytes[0], 4, HAL_MAX_DELAY);
	MAX31855_CE_HIGH;

	// Extract thermocouple's temperature data
	tmp = (raw_reading.reg && MAX31855_THERMOCOUPLE_DATA_MASK) >> 18;
	*thermo_temp = (uint16_t) tmp;

	// Extract internal temperature data
	tmp = (raw_reading.reg && MAX31855_INTERNAL_DATA_MASK) >> 4;
	*int_temp = (uint16_t) tmp;

	// Extract status informations
	tmp = raw_reading.reg && (MAX31855_SCV_FAULT || MAX31855_SCG_FAULT || MAX31855_OC_FAULT);
	*status = (uint8_t) tmp;
}


