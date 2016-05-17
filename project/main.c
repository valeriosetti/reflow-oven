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

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

// Private defines
#define MAIN_LOOP_PERIOD 	100 	// Delay in ms between two consecutive main loop's iterations


int main(void)
{
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_USB_DEVICE_Init();
	BSP_LED_Init(LED3);
	BSP_LED_On(LED3);
	BSP_LED_Init(LED4);
	BSP_LED_Off(LED4);
	BSP_LED_Init(LED5);
	BSP_LED_Off(LED5);

	/* Configure the LCD */
	PCD8544_Init(0x38);
	PCD8544_GotoXY(0,0);
	PCD8544_Puts("    Welcome    ", PCD8544_Pixel_Set, PCD8544_FontSize_5x7);
	PCD8544_GotoXY(0,10);
	PCD8544_Puts("    to  the    ", PCD8544_Pixel_Set, PCD8544_FontSize_5x7);
	PCD8544_GotoXY(0,20);
	PCD8544_Puts("  reflow oven  ", PCD8544_Pixel_Set, PCD8544_FontSize_5x7);
	PCD8544_Refresh();

	/* Initialize the SSR's PWM */
	SSR_init();
	SSR_set_duty_cycle(SSR_MAX_DUTY/2);
	SSR_start();

	/* Infinite loop */
	uint16_t tmp = 1;
	uint8_t direction = 1; // 1 is for upcounting; 0 for downcounting

	while (1)
	{
		// Call the reflow process every time. If it's not running it will
		// return immediately.
		reflow_process(MAIN_LOOP_PERIOD);

		// Use LED4 to signal if the USB device is connected or not
		if (hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED){
		  BSP_LED_On(LED4);
		}else{
		  BSP_LED_Off(LED4);
		}

		// Just blink LED5 to signal that the main process is still alive
		BSP_LED_Toggle(LED5);

		// Test the SSR module with an LED (debug phase)
		SSR_set_duty_cycle(tmp);
		if (direction){
		  tmp = tmp << 1;
		}else{
		  tmp = tmp >> 1;
		}
		if (tmp == 0x8000)
		  direction = 0;

		if (tmp == 0x0001)
		  direction = 1;

		// Wait for the predefined amount of time before the next iteration
		HAL_Delay(MAIN_LOOP_PERIOD);
	}
}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;

	__HAL_RCC_PWR_CLK_ENABLE();

	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 4;
	RCC_OscInitStruct.PLL.PLLN = 72;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 3;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
							  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif
