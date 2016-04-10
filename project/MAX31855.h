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

// Global defines
#define MAX31855_OC_FAULT		0x00000001
#define MAX31855_SCG_FAULT		0x00000002
#define MAX31855_SCV_FAULT		0x00000004
#define MAX31855_FAULT			0x00010000

// Public functions
void MAX31855_init();
uint8_t MAX31855_read(uint16_t* thermo_temp, uint16_t* int_temp);

#endif /* MAX31855_H_ */
