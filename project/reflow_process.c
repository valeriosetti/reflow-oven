/*
 * reflow_process.c
 *
 *  Created on: 17/mag/2016
 *      Author: valerio
 */

#include "reflow_process.h"
#include "stdlib.h"
#include "string.h"

// Local functions
static void reflow_proc_send_update(uint16_t current_temp);

// New types
typedef struct{
	uint16_t 	seconds;
	uint16_t	temperature;
}REFLOW_POINT;

// Local variables
#define REFLOW_LIST_MAX_SIZE		10
REFLOW_POINT reflow_list[REFLOW_LIST_MAX_SIZE];		// List of reflow points
uint16_t reflow_list_size = 0;			// Size of the reflow list
uint8_t reflow_process_enabled = 0;		// Bool variable to start/stop the reflow process
uint32_t reflow_process_tick;			// Local tick count for the reflow process


/**
 *	@brief	Add a new point to the current reflow list
 * 	@param	[none]
 * 	@return	0 in case of success; a negative value otherwise
 */
int add_reflow_point(int argc, char *argv[])
{
	if (reflow_list_size < REFLOW_LIST_MAX_SIZE){
		// Check if there's still space in the local buffer to store the data
		reflow_list[reflow_list_size].seconds = atoi(argv[0]);
		reflow_list[reflow_list_size].temperature = atoi(argv[1]);
		reflow_list_size++;
		USB_printf("OK\n");
		return 0;
	}else{
		// Otherwise return an error
		USB_printf("Error\n");
		return -1;
	}
}

/**
 *	@brief	Retrieve the current list
 * 	@param	[none]
 */
int get_reflow_list(int argc, char *argv[])
{
	uint16_t i;

	for (i=0; i<reflow_list_size; i++){
		USB_printf("point %d time %d temp %d\n", i, reflow_list[i].seconds, reflow_list[i].temperature);
	}
	return 0;
}

/**
 *	@brief	Clear the reflow list
 * 	@param	[none]
 */
int clear_reflow_list(int argc, char *argv[])
{
	reflow_list_size = 0;
	USB_printf("OK\n");
	return 0;
}

/**
 *	@brief	Start the reflow process
 * 	@param	argv[1]	=> is the boolean value {1,0} which enables/disables the reflow process
 */
int start_stop_reflow_process(int argc, char *argv[])
{
	reflow_process_enabled = atoi(argv[0]);
}

/**
 * Reflow process
 */
void reflow_process(uint32_t tick_interval)
{
	// If the reflow process isn't started yet, then return immediately
	if (reflow_process_enabled == 0)
		return;

	// Update the local tick count
	reflow_process_tick += tick_interval;

	/* TODO:
	 * - compute the next desired temperature for the current time
	 * - update SSR's PWM in order to match this temperature
	 */

	// Send a feedback to the host PC about the current status
	reflow_proc_send_update(0);
}

/**
 *
 */
static void reflow_proc_send_update(uint16_t current_temp)
{
	USB_printf("time %d temp %d\n", reflow_process_tick, current_temp);
}
