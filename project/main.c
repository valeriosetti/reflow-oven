/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "usb_device.h"
#include "gpio.h"
#include "usbd_cdc.h"
#include "stm32f4_discovery.h"
#include "pcd8544.h"
#include "SSR.h"
#include "MAX31855.h"
#include "reflow_process.h"
#include "USB_printf.h"

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* Private variables */
uint32_t main_loop_period = 1000; // Delay in ms between two consecutive main loop's iterations

typedef enum {
	WAITING_FOR_USB_CONNECTION,
	USB_CONNECTED
} MAIN_LOOP_STATUS;

/**
 * Main loop
 */
int main(void)
{
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_USB_DEVICE_Init();
	BSP_LED_Init(LED3);
	BSP_LED_Off(LED3);
	BSP_LED_Init(LED4);
	BSP_LED_Off(LED4);
	BSP_LED_Init(LED5);
	BSP_LED_Off(LED5);
	BSP_LED_Init(LED6);
	BSP_LED_Off(LED6);

	/* Configure the LCD */
	PCD8544_Init(0x38);

	/* Initialize the SSR's PWM */
	SSR_init();
	SSR_set_duty_cycle(SENSOR_1, SSR_MIN_DUTY);
	SSR_set_duty_cycle(SENSOR_2, SSR_MIN_DUTY);
	SSR_stop();

	/* Initialize the MAX31855 */
	HAL_Delay(100);
	MAX31855_init();

	uint32_t start_tick = HAL_GetTick();	// Get the start tick value
	uint32_t next_tick;

	/* Infinite loop */
	while (1)
	{
		// Get the tick value at the beginning of the main loop
		start_tick = HAL_GetTick();

		// Use LED4 to signal if the USB device is connected or not
		if (hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED)
		{
			// Signal that the USB is correctly configured
			BSP_LED_On(LED4);

			// Call the reflow process every time. If it's not running it will
			// return immediately.
			reflow_process(main_loop_period);
		}
		else
		{
			// Signal that the USB is not connected
			BSP_LED_Off(LED4);

			// Force the reflow process to stop
			private_stop_reflow_process();

			// Print a warning message on the LCD
			PCD8544_Clear();
			PCD8544_GotoXY(0,0);
			PCD8544_Puts(" Waiting for   ", PCD8544_Pixel_Set, PCD8544_FontSize_5x7);
			PCD8544_GotoXY(0,10);
			PCD8544_Puts(" USB           ", PCD8544_Pixel_Set, PCD8544_FontSize_5x7);
			PCD8544_Refresh();
		}

		// Just blink LED3 to signal that the main process is still alive
		BSP_LED_Toggle(LED3);

		// Wait for the predefined amount of time before the next iteration
		while ((HAL_GetTick()-start_tick) < main_loop_period);
	}
}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{
	uint32_t tmp;

	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	// Turn on the HSE oscillator and wait for it to stabilize
	RCC->CR |= RCC_CR_HSEON;
	while ((RCC->CR & RCC_CR_HSERDY) == 0);

	// Configure the PLL
	tmp = RCC->PLLCFGR;
	tmp = (tmp & ~RCC_PLLCFGR_PLLQ) | (7UL << 24);  // Q=7
	tmp = (tmp & ~RCC_PLLCFGR_PLLP) | (0UL << 16);  // P=2
	tmp = (tmp & ~RCC_PLLCFGR_PLLN) | (168UL << 6);  // N=168
	tmp = (tmp & ~RCC_PLLCFGR_PLLM) | (4UL << 0); // M=4
	tmp |= RCC_PLLCFGR_PLLSRC;  // HSE is PLL's source
	RCC->PLLCFGR = tmp;

	// Turn on the PLL and wait for it to lock
	RCC->CR |= RCC_CR_PLLON;
	while ((RCC->CR & RCC_CR_PLLRDY) == 0);

	// Configure peripherals' dividers
	tmp = RCC->CFGR;
	tmp = (tmp & ~RCC_CFGR_PPRE2) | RCC_CFGR_PPRE2_DIV2;  // APB2 clock = 84MHz
	tmp = (tmp & ~RCC_CFGR_PPRE1) | RCC_CFGR_PPRE1_DIV4;  // APB1 clock = 42MHz
	tmp = (tmp & ~RCC_CFGR_PPRE1) | RCC_CFGR_HPRE_DIV1;  // AHB clock = 168MHz (SysTick will be clocked at 21MHz)
	RCC->CFGR = tmp;

	// Configure the Flash latency to 5 wait states (see Table 7 in the Reference Manual)
	tmp = FLASH->ACR;
	tmp = (tmp & ~FLASH_ACR_LATENCY) | FLASH_ACR_LATENCY_5WS;
	FLASH->ACR = tmp;

	// Switch to the PLL generated clock
	tmp = RCC->CFGR;
	tmp = (tmp & ~RCC_CFGR_SW) | RCC_CFGR_SW_PLL;
	RCC->CFGR = tmp;
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);


	// Enable the interrupt from SysTick
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

	// Configure the SysTick
	//	- the counter is reset every 1ms
	//	- interrupt generation is enabled
	//	- the counter is enabled
	SysTick_Config(168000);
	NVIC_EnableIRQ(SysTick_IRQn);
}


int set_reflow_process_period(int argc, char *argv[])
{
	main_loop_period = atoi(argv[0]);
	USB_printf_buff("OK\n");
}
