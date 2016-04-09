/*
 * SSR.h
 *
 * Header file for the SSR driver
 */

#ifndef SSR_H_
#define SSR_H_

// Includes
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_tim.h"

// Public defines
#define	SSR_MAX_DUTY	60000
#define	SSR_MIN_DUTY	0

// Public functions
void SSR_init();
void SSR_start();
void SSR_stop();
void SSR_set_duty_cycle(uint16_t duty);

#endif /* SSR_H_ */
