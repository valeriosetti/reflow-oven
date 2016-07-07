/**
 * PCD8544 LCD controller driver.
 * This controller is used in Nokia's 3310 and 5110 LCD's.
 *
 * LCD's pin	| STM32F407's	| STM32F407's	|  LCD's function
 * function		| alt. function	| pin			|
 * ---------------------------------------------------------------------
 *		CE		|	SPI2_NSS	|	PB12 		|	SPI chip select
 *		CLK		|	SPI2_SCK	|	PB13		|	SPI clock
 *		D/C		|	PB14		|	PB14		|	data/command select
 *		DIN		|	SPI2_MOSI	|	PB15		|	SPI MOSI
 *		VCC		|	VCC			|	3.3V		|	VCC Power
 *		GND		|	GND			|	GND			|	Ground
 *		RST		|	PB11		|	PB11		|	Reset pin for LCD
 *		LIGHT	|	n/a			|	n/a			|	n/a
 *
 */
#ifndef PCD8544_H
#define PCD8544_H 100

/**
 * Includes
 */
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"

/*
 * Pin definitions
 */
//	CE
#define	PCD8544_CE_PORT					GPIOB
#define	PCD8544_CE_PIN					GPIO_PIN_12
#define	PCD8544_CE_CLKEN				__GPIOB_CLK_ENABLE
//	MOSI
#define	PCD8544_DIN_PORT				GPIOB
#define	PCD8544_DIN_PIN					GPIO_PIN_15
#define	PCD8544_DIN_CLKEN				__GPIOB_CLK_ENABLE
//	CLK
#define	PCD8544_CLK_PORT				GPIOB
#define	PCD8544_CLK_PIN					GPIO_PIN_13
#define	PCD8544_CLK_CLKEN				__GPIOB_CLK_ENABLE
//	RST
#define	PCD8544_RST_PORT				GPIOB
#define	PCD8544_RST_PIN					GPIO_PIN_11
#define	PCD8544_RST_CLKEN				__GPIOB_CLK_ENABLE
//	D/C
#define	PCD8544_DC_PORT					GPIOB
#define	PCD8544_DC_PIN					GPIO_PIN_14
#define	PCD8544_DC_CLKEN				__GPIOB_CLK_ENABLE
//	Backlight
//#define	PCD8544_BKL_PORT				GPIOD
//#define	PCD8544_BKL_PIN					GPIO_PIN_12
//#define	PCD8544_BKL_CLKEN				__GPIOD_CLK_ENABLE


/*
 * Shortcuts
 */
#define PCD8544_CE_LOW			HAL_GPIO_WritePin(PCD8544_CE_PORT, PCD8544_CE_PIN, GPIO_PIN_RESET)
#define PCD8544_CE_HIGH			HAL_GPIO_WritePin(PCD8544_CE_PORT, PCD8544_CE_PIN, GPIO_PIN_SET)
#define PCD8544_DC_LOW			HAL_GPIO_WritePin(PCD8544_DC_PORT, PCD8544_DC_PIN, GPIO_PIN_RESET)
#define PCD8544_DC_HIGH			HAL_GPIO_WritePin(PCD8544_DC_PORT, PCD8544_DC_PIN, GPIO_PIN_SET)
#define PCD8544_RST_LOW			HAL_GPIO_WritePin(PCD8544_RST_PORT, PCD8544_RST_PIN, GPIO_PIN_RESET)
#define PCD8544_RST_HIGH		HAL_GPIO_WritePin(PCD8544_RST_PORT, PCD8544_RST_PIN, GPIO_PIN_SET)

/**
 * Private enums
 */
typedef enum {
	PCD8544_COMMAND = 0,
	PCD8544_DATA = !PCD8544_COMMAND
} PCD8544_WriteType_t;

typedef enum {
	PCD8544_State_Low = 0,
	PCD8544_State_High = !PCD8544_State_Low
} PCD8544_State_t;

typedef enum {
	PCD8544_Pin_DC = 1,
	PCD8544_Pin_RST = 2
} PCD8544_Pin_t;

/**
 * Public enums
 */
/**
 * Used for pixel "color"
 * LCD is 2 bit, so pixel can be just set or clear
 */
typedef enum {
	PCD8544_Pixel_Clear = 0,
	PCD8544_Pixel_Set = !PCD8544_Pixel_Clear
} PCD8544_Pixel_t;

/**
 * Font size for text
 * There are 2 sizes included
 */
typedef enum {
	PCD8544_FontSize_5x7 = 0,
	PCD8544_FontSize_3x5 = !PCD8544_FontSize_5x7
} PCD8544_FontSize_t;

/**
 * Used to invert pixels
 */
typedef enum {
	PCD8544_Invert_Yes,
	PCD8544_Invert_No
} PCD8544_Invert_t;


#define PCD8544_WIDTH				84
#define PCD8544_HEIGHT				48

// PCD8544 Commandset
// ------------------
// General commands
#define PCD8544_POWERDOWN			0x04
#define PCD8544_ENTRYMODE			0x02
#define PCD8544_EXTENDEDINSTRUCTION	0x01
#define PCD8544_DISPLAYBLANK		0x00
#define PCD8544_DISPLAYNORMAL		0x04
#define PCD8544_DISPLAYALLON		0x01
#define PCD8544_DISPLAYINVERTED		0x05
// Normal instruction set
#define PCD8544_FUNCTIONSET			0x20
#define PCD8544_DISPLAYCONTROL		0x08
#define PCD8544_SETYADDR			0x40
#define PCD8544_SETXADDR			0x80
// Extended instruction set
#define PCD8544_SETTEMP				0x04
#define PCD8544_SETBIAS				0x10
#define PCD8544_SETVOP				0x80
// Display presets
#define PCD8544_LCD_BIAS			0x03	// Range: 0-7 (0x00-0x07)
#define PCD8544_LCD_TEMP			0x02	// Range: 0-3 (0x00-0x03)
#define PCD8544_LCD_CONTRAST		0x46	// Range: 0-127 (0x00-0x7F)

#define PCD8544_CHAR5x7_WIDTH		6 //5*8
#define PCD8544_CHAR5x7_HEIGHT		8
#define PCD8544_CHAR3x5_WIDTH		4 //3*5
#define PCD8544_CHAR3x5_HEIGHT		6

#define PCD8544_BUFFER_SIZE 		PCD8544_WIDTH * PCD8544_HEIGHT / 8


/**
 * Initialize all IO pins and peripheral
 *
 */
extern void PCD8544_InitIO(void);

/**
 * Send data to LCD
 *
 */
extern void PCD8544_send(unsigned char data);

/**
 * Set DC pin state
 *
 */
extern void PCD8544_Pin(PCD8544_Pin_t pin, PCD8544_State_t state);

/**
 * Specify delay in microseconds
 *
 */
extern void PCD8544_Delay(unsigned long micros);

/**
 * Write data/command to LCD
 *
 */
extern void PCD8544_Write(PCD8544_WriteType_t cd, unsigned char data);

/**
 * Set area for refresh display
 *
 */
extern void PCD8544_UpdateArea(unsigned char xMin, unsigned char yMin, unsigned char xMax, unsigned char yMax);

/**
 * Initialize LCD
 *
 * Parameters
 * - unsigned char contrast: Contrast can be software selectable, from 0x00 to 0x7F
 */
extern void PCD8544_Init(unsigned char contrast);

/**
 * Set contrast for LCD
 *
 * Parameters
 * - unsigned char contrast: Contrast can be software selectable, from 0x00 to 0x7F
 */
extern void PCD8544_SetContrast(unsigned char contrast);

/**
 * Invert LCD
 *
 * Parameters
 * - PCD8544_Invert_t invert
 * 		- PCD8544_Invert_Yes: Invert pixels on lcd
 * 		- PCD8544_Invert_No: Return pixels to orignal
 */
extern void PCD8544_Invert(PCD8544_Invert_t invert);

/**
 * Clear lcd
 */
extern void PCD8544_Clear(void);

/**
 * Set/clear pixel at specific location
 *
 * Parameters
 * - unsigned char x: x position of pixel
 * - unsigned char y: y position of pixel
 * - PCD8544_PCD8544_Pixel_t pixel: Set or clear pixel
 * 		- PCD8544_Pixel_Set
 * 		- PCD8544_Pixel_Clear
 */
extern void PCD8544_DrawPixel(unsigned char x, unsigned char y, PCD8544_Pixel_t pixel);

/**
 * Set cursor on lcd at 0, 0 location
 *
 */
extern void PCD8544_Home(void);

/**
 * Put data from internal buffer to lcd
 *
 */
extern void PCD8544_Refresh(void);

/**
 * Set cursor to desired position
 *
 * Parameters:
 * - unsigned char x: x from 0 to 83
 * - unsigned char y: y from 0 to 47
 */
extern void PCD8544_GotoXY(unsigned char x, unsigned char y);

/**
 * Put character on LCD
 *
 * Parameters:
 * - char c: char to display
 * - PCD8544_FontSize_t size: Font size
 * 		- PCD8544_FontSize_5x7
 * 		- PCD8544_FontSize_3x5
 * - PCD8544_PCD8544_Pixel_t color
 * 		- PCD8544_Pixel_Set
 * 		- PCD8544_Pixel_Clear
 */
extern void PCD8544_Putc(char c, PCD8544_Pixel_t color, PCD8544_FontSize_t size);

/**
 * Put string on LCD
 *
 * Parameters:
 * - char *c: pointer to first character of string
 * - PCD8544_FontSize_t size: Font size
 * 		- PCD8544_FontSize_5x7
 * 		- PCD8544_FontSize_3x5
 * - PCD8544_PCD8544_Pixel_t color
 * 		- PCD8544_Pixel_Set
 * 		- PCD8544_Pixel_Clear
 */
extern void PCD8544_Puts(char *c, PCD8544_Pixel_t color, PCD8544_FontSize_t size);

/**
 * Draw line on LCD
 *
 * Parameters:
 * - unsigned char x0: X coordinate of starting point
 * - unsigned char y0: Y coordinate of starting point
 * - unsigned char x1: X coordinate of ending point
 * - unsigned char y1: Y coordinate of ending point
 * - PCD8544_PCD8544_Pixel_t color
 * 		- PCD8544_Pixel_Set
 * 		- PCD8544_Pixel_Clear
 */
extern void PCD8544_DrawLine(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, PCD8544_Pixel_t color);

/**
 * Draw rectangle on LCD
 *
 * Parameters:
 * - unsigned char x0: X coordinate of top left point
 * - unsigned char y0: Y coordinate of top left point
 * - unsigned char x1: X coordinate of bottom right point
 * - unsigned char y1: Y coordinate of bottom right point
 * - PCD8544_PCD8544_Pixel_t color
 * 		- PCD8544_Pixel_Set
 * 		- PCD8544_Pixel_Clear
 */
extern void PCD8544_DrawRectangle(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, PCD8544_Pixel_t color);

/**
 * Draw filled rectangle on LCD
 *
 * Parameters:
 * - unsigned char x0: X coordinate of top left point
 * - unsigned char y0: Y coordinate of top left point
 * - unsigned char x1: X coordinate of bottom right point
 * - unsigned char y1: Y coordinate of bottom right point
 * - PCD8544_PCD8544_Pixel_t color
 * 		- PCD8544_Pixel_Set
 * 		- PCD8544_Pixel_Clear
 */
extern void PCD8544_DrawFilledRectangle(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, PCD8544_Pixel_t color);

/**
 * Draw circle on LCD
 *
 * Parameters:
 * - char x0: X coordinate of center circle point
 * - char y0: Y coordinate of center circle point
 * - char r: circle radius
 * - PCD8544_PCD8544_Pixel_t color
 * 		- PCD8544_Pixel_Set
 * 		- PCD8544_Pixel_Clear
 */
extern void PCD8544_DrawCircle(char x0, char y0, char r, PCD8544_Pixel_t color);

/**
 * Draw filled circle on LCD
 *
 * Parameters:
 * - char x0: X coordinate of center circle point
 * - char y0: Y coordinate of center circle point
 * - char r: circle radius
 * - PCD8544_PCD8544_Pixel_t color
 * 		- PCD8544_Pixel_Set
 * 		- PCD8544_Pixel_Clear
 */
extern void PCD8544_DrawFilledCircle(char x0, char y0, char r, PCD8544_Pixel_t color);

/**
 * Set backlight intensity
 *
 * Parameters:
 * - uint16 value: the backlight's duty cycle value
 */
extern void PCD8544_SetBacklight(uint8_t value);

#endif
