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
#define TIMER_PRESCALER	42000	// The counter will be clocked at 42MHz/42000=1kHz
#define TIMER_PERIOD	1000	// The counter will count up to 1000. This reason for this
								// is that:
								//		- main supply is 50Hz, which means 100 half-periods per second
								//		- regulating the capture-compare register in multiple of 10 allows
								//			to obtain 100 different levels of regulation. Each 10 units
								//			a semi-period is added or removed.
#define	SSR_MAX_CCR_VALUE	(1000+1) 	// The +1 is added in order to have the chance
										// to keep the output continuously high
#define	SSR_MIN_CCR_VALUE	0

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

/**
 *	@brief	Change the PWM duty cycle
 *	@param	id => it's the ID of the SSR whose duty cycle will be changed
 * 	@param	duty => is the new duty cycle (between 0 and 255)
 */
void SSR_set_duty_cycle(SENSOR_ID id, uint8_t duty)
{
	// Set the duty cycle for the proper SSR
	switch (id)
	{
		case SENSOR_1:
			(TIMER)->CCR2 = ((uint32_t) SSR_MAX_CCR_VALUE*duty)/SSR_MAX_DUTY;
			break;
		case SENSOR_2:
			(TIMER)->CCR3 = ((uint32_t) SSR_MAX_CCR_VALUE*duty)/SSR_MAX_DUTY;;
			break;
	}
}

/**
 *	@brief	This is meant to be called from the PC in order to force a specific duty cycle. It's
 *			mainly purpose if for testing. This command will not be included into the GUI, but it will be
 *			accessible from the command line.
 *	@param	argv[0]	-> it's the ID of the SSR
 *	@param	argv[1] -> it's the value of the duty cycle, between 0 and 255
 */
int SSR_force_duty_cycle(int argc, char *argv[])
{
	uint8_t duty;
	SENSOR_ID id;

	// Basic parameters check
	if (argc != 2) {
		USB_printf_buff("Error\n");
		return -1;
	}

	id = atoi(argv[0]);
	duty = atoi(argv[1]);

	if ((id!=SENSOR_1)||(id!=SENSOR_2)) {
		USB_printf_buff("Error\n");
		return -2;
	}

	SSR_set_duty_cycle(id, duty);

	return 0;
}
