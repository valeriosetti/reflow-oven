/*
 * MAX31855.h
 *
 *  Created on: 10/apr/2016
 *      Author: valerio
 */

#ifndef MAX31855_H_
#define MAX31855_H_

// Includes
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_spi.h"

#define NUMBER_OF_SENSORS	2

typedef enum {
	SENSOR_1,
	SENSOR_2
} SENSOR_ID;

// Global defines
#define MAX31855_OC_FAULT		0x00000001
#define MAX31855_SCG_FAULT		0x00000002
#define MAX31855_SCV_FAULT		0x00000004
#define MAX31855_FAULT			0x00010000

// Public functions
void MAX31855_init();
uint8_t MAX31855_read(SENSOR_ID id, float* thermo_temp, float* int_temp);

#endif /* MAX31855_H_ */
