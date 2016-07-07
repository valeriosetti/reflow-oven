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
#define MAX31855_RESERVED_BIT_MASK			((uint32_t) 1 << 17 |	\
											 (uint32_t) 1 << 3	)
#define THERMOCOUPLE_PRECISION				0.25
#define INTERNAL_PRECISION					0.0625
//// CS for sensor #1
#define	MAX31855_CS1_PORT				GPIOA
#define	MAX31855_CS1_PIN				GPIO_PIN_3
#define	MAX31855_CS1_CLKEN				__GPIOA_CLK_ENABLE
//// CS for sensor #2
#define	MAX31855_CS2_PORT				GPIOA
#define	MAX31855_CS2_PIN				GPIO_PIN_8
#define	MAX31855_CS2_CLKEN				__GPIOA_CLK_ENABLE

//// MOSI
#define	MAX31855_MISO_PORT				GPIOB
#define	MAX31855_MISO_PIN				GPIO_PIN_4
#define	MAX31855_MISO_CLKEN				__GPIOA_CLK_ENABLE
//// CLK
#define	MAX31855_CLK_PORT				GPIOB
#define	MAX31855_CLK_PIN				GPIO_PIN_3
#define	MAX31855_CLK_CLKEN				__GPIOA_CLK_ENABLE
//// CS for the LIS3DSH
#define	LIS3DSH_CLK_PORT				GPIOE
#define	LIS3DSH_CLK_PIN					GPIO_PIN_3
#define	LIS3DSH_CLK_CLKEN				__GPIOE_CLK_ENABLE
//// Shortcuts for pin handling
#define MAX31855_CS1_LOW			HAL_GPIO_WritePin(MAX31855_CS1_PORT, MAX31855_CS1_PIN, GPIO_PIN_RESET)
#define MAX31855_CS1_HIGH			HAL_GPIO_WritePin(MAX31855_CS1_PORT, MAX31855_CS1_PIN, GPIO_PIN_SET)
#define MAX31855_CS2_LOW			HAL_GPIO_WritePin(MAX31855_CS2_PORT, MAX31855_CS2_PIN, GPIO_PIN_RESET)
#define MAX31855_CS2_HIGH			HAL_GPIO_WritePin(MAX31855_CS2_PORT, MAX31855_CS2_PIN, GPIO_PIN_SET)

// sign bit position in thermocouple and internal temperature values
#define THERMOCOUPLE_SIGN_BIT_POSITION		((uint32_t) 1 << 13)
#define INTERNAL_SIGN_BIT_POSITION			((uint32_t) 1 << 11)


// Local variables
static SPI_HandleTypeDef hSPI;


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

	// Configure the software controlled CS pins
	GPIO_InitStruct.Pin = MAX31855_CS1_PIN;
	HAL_GPIO_Init(MAX31855_CS1_PORT, &GPIO_InitStruct);
	MAX31855_CS1_HIGH;

	GPIO_InitStruct.Pin = MAX31855_CS2_PIN;
	HAL_GPIO_Init(MAX31855_CS2_PORT, &GPIO_InitStruct);
	MAX31855_CS2_HIGH;

	// Configure the SPI pins
	MAX31855_CS1_CLKEN();
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
 *  - id => specifies which sensor should be read
 * 	- thermo_temp => it returns the 14bit thermocouple's temperature
 * 	- int_temp => it returns the 12bit internal temperature
 * Returns:
 * 	- an "uint8_t" which contains possible faults. Using masks defined in the header
 * 	file it's possible to determine the type of fault.
 */
uint8_t MAX31855_read(SENSOR_ID id, float* thermo_temp, float* int_temp)
{
	uint8_t  raw_reading[4];
	uint32_t raw_reading_reordered;
	uint32_t tmp;

	if (id == SENSOR_1) {
		MAX31855_CS1_LOW;
	} else {
		MAX31855_CS2_LOW;
	}

	HAL_SPI_Receive(&hSPI, raw_reading, 4, HAL_MAX_DELAY);

	if (id == SENSOR_1) {
		MAX31855_CS1_HIGH;
	} else {
		MAX31855_CS2_HIGH;
	}

	// Convert the read value to an uint32_t variable with reordered bytes
	raw_reading_reordered = (((uint32_t) raw_reading[0]) << 24) |
							(((uint32_t) raw_reading[1]) << 16) |
							(((uint32_t) raw_reading[2]) << 8 ) |
							(((uint32_t) raw_reading[3])      );

	// If any of the reserved bits is read as '1' then there was a communication error
	if (raw_reading_reordered & MAX31855_RESERVED_BIT_MASK) {
		return 0xFF; // Custom error
	}

	// Extract thermocouple's temperature data
	tmp = (raw_reading_reordered & MAX31855_THERMOCOUPLE_DATA_MASK) >> 18;
	if (tmp & THERMOCOUPLE_SIGN_BIT_POSITION) {
		// It's a negative number, so the two's complement value is calculated
		tmp ^= (MAX31855_THERMOCOUPLE_DATA_MASK >> 18);
		tmp++;
		*thermo_temp = tmp * (-1.0) * THERMOCOUPLE_PRECISION;
	}else{
		// Positive value
		*thermo_temp = tmp * THERMOCOUPLE_PRECISION;
	}

	// Extract internal temperature data
	tmp = (raw_reading_reordered & MAX31855_INTERNAL_DATA_MASK) >> 4;
	if (tmp & INTERNAL_SIGN_BIT_POSITION) {
		// It's a negative number
		tmp ^= (MAX31855_INTERNAL_DATA_MASK >> 4);
		tmp++;
		*int_temp = tmp * (-1.0) * INTERNAL_PRECISION;
	}else{
		// Positive value
		*int_temp = tmp * INTERNAL_PRECISION;
	}

	// Extract status informations
	tmp = raw_reading_reordered & (MAX31855_SCV_FAULT | MAX31855_SCG_FAULT | MAX31855_OC_FAULT);
	// Move the MAX31855_FAULT bit to the 8th place in order to have the status represented with
	// only one byte
	tmp |= (raw_reading_reordered & MAX31855_FAULT) >> 9;

	return (uint8_t) tmp;
}



