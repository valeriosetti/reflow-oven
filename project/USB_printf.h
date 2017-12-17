/*
 * alt_printf.h
 *
 *  Created on: 05/mar/2016
 *      Author: valerio
 */

#ifndef PRINTF_STDARG_H_
#define PRINTF_STDARG_H_

#include <stdarg.h>
#include "stm32f4xx_hal.h"
#include "usbd_cdc_if.h"

int USB_printf(const char *format, ...);
int USB_printf_buff(const char *format, ...);
int PCD8544_printf_buff(const char *format, ...);

#endif /* PRINTF_STDARG_H_ */
