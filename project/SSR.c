/*
 * SSR.c
 *
 * That's the driver for the PWMs which will control the Solid State Relays (SSR).
 * Pin PA1 is connected to TIM5_CH2, whereas
 * pin PA2 is connected to TIM5_CH3.
 */

#include "SSR.h"

/* Private defines */
// The timer will configured for a 100Hz period PWM
#define TIMER			TIM5	// The input clock for this timer is PCLK2=42MHz
#define TIMER_CLKEN		__TIM5_CLK_ENABLE
#define TIMER_PRESCALER	7		// The counter will be clocked at 42/7=6MHz
#define TIMER_PERIOD	60000	// The counter will count up to 60000

/* Private variables */
TIM_HandleTypeDef	hTIM;
TIM_OC_InitTypeDef 	TIM_OC_init;

/*
 * Initialize the timer and the GPIO for the SSR
 */
void SSR_init()
{
	GPIO_InitTypeDef 	GPIO_InitStruct;
	uint32_t tmp;

	// Configure the GPIOs
	__GPIOA_CLK_ENABLE();
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM5;
	GPIO_InitStruct.Pin = GPIO_PIN_1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = GPIO_PIN_2;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	// Configure the timer and output-compare
	TIMER_CLKEN();
	// Configure OutputCompare for PWM mode 1
	// Enable the preload register
	(TIMER)->CCMR1 = (TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2PE);
	(TIMER)->CCMR2 = (TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3PE);
	// Set the counter, prescaler and the auto-reload for the timer
	(TIMER)->PSC = TIMER_PRESCALER;
	(TIMER)->ARR = TIMER_PERIOD;
	// Set the minimum duty cycle at the beginning
	(TIMER)->CCR2 = SSR_MIN_DUTY;
	(TIMER)->CCR3 = SSR_MIN_DUTY;
	// Enable the buffering of ARR
	(TIMER)->CR1 = TIM_CR1_ARPE;
}

/*
 *	Enable the PWM
 */
void SSR_start()
{
	// Enable OC2 and OC3
	(TIMER)->CCER |= (TIM_CCER_CC2E | TIM_CCER_CC3E);
	// Enable the timer
	(TIMER)->CR1 = TIM_CR1_CEN;
}

/*
 *	Stop the PWM
 */
void SSR_stop()
{
	// Disable OC2 and OC3
	(TIMER)->CCER &= ((~TIM_CCER_CC2E) & (~TIM_CCER_CC3E));
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
			(TIMER)->CCR2 = duty;
			break;
		case SENSOR_2:
			(TIMER)->CCR3 = duty;
			break;
	}
}
