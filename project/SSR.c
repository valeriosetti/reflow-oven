/*
 * SSR.c
 *
 * That's the driver for the PWM which will control the Solid State Relay (SSR).
 * Pin PA1, whose alternate function is TIM5_CH2, will be used for the PWM generation.
 */

#include "SSR.h"

// Private defines
//// This is the GPIO used for PWM (PA1)
#define	GPIO_PORT		GPIOA
#define GPIO_PIN_NUM	GPIO_PIN_1
#define GPIO_CLKEN		__GPIOA_CLK_ENABLE
//// This is for the timer. It will configured for a 100Hz period PWM
#define TIMER			TIM5	// The input clock for this timer is PCLK2=42MHz
#define TIMER_CLKEN		__TIM5_CLK_ENABLE
#define TIMER_PRESCALER	6		// The counter will be clocked at 42/6=7MHz
#define TIMER_PERIOD	60000	// The counter will count up to 60000

// Private variables
TIM_HandleTypeDef	hTIM;
TIM_OC_InitTypeDef 	TIM_OC_init;

/*
 * Initialize the timer and the GPIO for the SSR
 */
void SSR_init()
{
	GPIO_InitTypeDef 	GPIO_InitStruct;
	uint32_t tmp;

	// Configure the GPIO
	GPIO_CLKEN();
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM5;
	GPIO_InitStruct.Pin = GPIO_PIN_NUM;
	HAL_GPIO_Init(GPIO_PORT, &GPIO_InitStruct);

	// Configure the timer and output-compare
	TIMER_CLKEN();
	// Configure OutputCompare for PWM mode 1
	// Enable the preload register
	(TIMER)->CCMR1 = (TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2PE);
	// Set the counter, prescaler and the auto-reload for the timer
	(TIMER)->PSC = TIMER_PRESCALER;
	(TIMER)->ARR = TIMER_PERIOD;
	// Set the minimum duty cycle at the beginning
	(TIMER)->CCR2 = SSR_MIN_DUTY;
	// Enable the buffering of ARR
	(TIMER)->CR1 = TIM_CR1_ARPE;
}

/*
 *	Enable the PWM
 */
void SSR_start()
{
	// Enable OC2
	(TIMER)->CCER |= TIM_CCER_CC2E;
	// Enable the timer
	(TIMER)->CR1 = TIM_CR1_CEN;
}

/*
 *	Stop the PWM
 */
void SSR_stop()
{
	// Disable OC2
	(TIMER)->CCER &= (~TIM_CCER_CC2E);
	// Stop the timer
	(TIMER)->CR1 &= (~TIM_CR1_CEN);
}

/*
 *	Change the PWM duty cycle
 * 		uint16_t duty => is the new duty cycle (0 is the minimum value, 60000 the maximum)
 */
void SSR_set_duty_cycle(SENSOR_ID id, uint16_t duty)
{
	switch (id)
	{
		case SENSOR_1:
		// TODO
			break;
		case SENSOR_2:
		// TODO
			break;
	}
//	(TIMER)->CCR2 = (duty);
}
