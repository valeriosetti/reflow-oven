/*
 * command_processor.h
 *
 *  Created on: 11/apr/2016
 *      Author: valerio
 */

#ifndef COMMAND_PROCESSOR_H_
#define COMMAND_PROCESSOR_H_

// Includes
#include "stm32f4xx_hal.h"
#include "usbd_cdc_if.h"
#include "pcd8544.h"
#include "stdlib.h"
#include "string.h"
#include "reflow_process.h"
#include "USB_printf.h"
#include "main.h"
#include "SSR.h"

// Public functions
int cmd_proc_receive_data(uint8_t* Data, uint32_t *Len);


#endif /* COMMAND_PROCESSOR_H_ */
