/*
 * reflow_process.c
 *
 *  Created on: 17/mag/2016
 *      Author: valerio
 */

#include "reflow_process.h"
#include "SSR.h"

// Local functions
static float compute_target_temp(uint32_t milliseconds);
static uint8_t	compute_duty_cycle(SENSOR_ID id, uint32_t delta_time, float thermo_temp, float target_temp);
static uint8_t	compute_fake_duty_cycle(SENSOR_ID id, uint32_t delta_time, float thermo_temp, float target_temp);

// Macros
#define max(a,b)	((a > b) ? a : b)

// New types
typedef struct{
	uint32_t 	milliseconds;
	float	temperature;
}REFLOW_POINT;

// Local variables
#define REFLOW_LIST_MAX_SIZE		10

REFLOW_POINT reflow_list[REFLOW_LIST_MAX_SIZE];		// List of reflow points
uint16_t reflow_list_size = 0;			// Size of the reflow list
uint8_t reflow_process_enabled = 0;		// Bool variable to start/stop the reflow process
uint32_t reflow_process_tick;			// Local tick count for the reflow process
float 	Pcoeff[NUMBER_OF_SENSORS],
		Icoeff[NUMBER_OF_SENSORS],
		Dcoeff[NUMBER_OF_SENSORS];		// PID coefficients

// Global variables used for PID controller's calculations
float previous_error = 0.0;
float integral_error = 0.0;

/**
 *	@brief	Add a new point to the current reflow list
 * 	@param	[none]
 * 	@return	0 in case of success; a negative value otherwise
 */
int add_reflow_point(int argc, char *argv[])
{
	if (reflow_list_size < REFLOW_LIST_MAX_SIZE){
		// Check if there's still space in the local buffer to store the data
		reflow_list[reflow_list_size].milliseconds = atoi(argv[0])*1000;
		reflow_list[reflow_list_size].temperature = atof(argv[1]);
		reflow_list_size++;
		USB_printf_buff("OK\n");
		return 0;
	}else{
		// Otherwise return an error
		USB_printf_buff("Error\n");
		return -1;
	}
}

/**
 *	@brief	Retrieve the current list
 * 	@param	[none]
 */
int get_reflow_point(int argc, char *argv[])
{
	if (argc != 1) {
		USB_printf_buff("Error\n");
		return -1;
	}

	uint16_t index = atoi(argv[0]);
	uint32_t temp_int = reflow_list[index].temperature;;

	USB_printf_buff("point %d -> time %d temp %d\n", index,
						reflow_list[index].milliseconds/1000, temp_int);
	return 0;
}

/**
 *	@brief	Clear the reflow list
 * 	@param	[none]
 */
int clear_reflow_list(int argc, char *argv[])
{
	reflow_list_size = 0;
	USB_printf_buff("OK\n");
	return 0;
}

/**
 *	@brief	Start the reflow process
 * 	@param	[none]
 */
int start_reflow_process(int argc, char *argv[])
{
	// Reset global variables before starting
	reflow_process_tick = 0;
	integral_error = 0.0;
	previous_error = 0.0;

	reflow_process_enabled = 1;

	// Set SSRs' PWMs to the minimum value, but enable them
	SSR_set_duty_cycle(SENSOR_1, SSR_MIN_DUTY);
	SSR_set_duty_cycle(SENSOR_2, SSR_MIN_DUTY);
	SSR_start();

	USB_printf_buff("OK\n");
	return 0;
}

/**
 *	@brief	This is the same as "private_stop_reflow_process", but returns
 *			also a feedback through the virtualCOM.
 * 	@param	[none]
 */
int stop_reflow_process(int argc, char *argv[])
{
	int ret_val = private_stop_reflow_process();
	USB_printf_buff("OK\n");
	return ret_val;
}

/**
 *	@brief	This function stops the reflow process, but it's available
 *			only to the STM32.
 * 	@param	[none]
 */
int private_stop_reflow_process()
{
	reflow_process_enabled = 0;

	// Turn off the SSR
	SSR_set_duty_cycle(SENSOR_1, SSR_MIN_DUTY);
	SSR_set_duty_cycle(SENSOR_2, SSR_MIN_DUTY);
	SSR_stop();

	// Reset global variables
	reflow_process_tick = 0;
	integral_error = 0.0;
	previous_error = 0.0;

	return 0;
}

/**
 *	@brief	Configure the PID parameters for the reflow process
 * 	@param	argv[]	=> are the P-I-D coefficients
 */
int set_PID_parameters(int argc, char *argv[])
{
	if (argc != 4) {
		USB_printf_buff("Error\n");
		return -1;
	}

	// Read which PID group should be set
	// Note: sensor index is '1' based, whereas arrays are '0' based
	SENSOR_ID id = atoi(argv[0]) - 1 ;

	Pcoeff[id] = atof(argv[1]);
	Icoeff[id] = atof(argv[2]);
	Dcoeff[id] = atof(argv[3]);

	USB_printf_buff("OK\n");

	return 0;
}

/**
 * Reflow process
 */
void reflow_process(uint32_t tick_interval)
{
	float thermo_temp_1, internal_temp_1, thermo_temp_2, internal_temp_2;
	float avg_thermo;
	float target_temp;
	uint8_t status;
	uint8_t SSR_duty1, SSR_duty2;

	// If the reflow process isn't started yet, then return immediately
	if (reflow_process_enabled == 0) {
		PCD8544_Clear();
		status = MAX31855_read(SENSOR_1, &thermo_temp_1, &internal_temp_1);
		PCD8544_GotoXY(0,0);
		PCD8544_printf_buff("Thermo1 = %d", (int32_t)thermo_temp_1);
		PCD8544_GotoXY(0,7);
		PCD8544_printf_buff("Status1 = %x", status);
		status = MAX31855_read(SENSOR_2, &thermo_temp_2, &internal_temp_2);
		PCD8544_GotoXY(0,14);
		PCD8544_printf_buff("Thermo2 = %d", (int32_t)thermo_temp_2);
		PCD8544_GotoXY(0,21);
		PCD8544_printf_buff("Status2 = %x", (int32_t)status);
		PCD8544_GotoXY(0,28);
		PCD8544_printf_buff("Device in idle state");
		PCD8544_Refresh();
		return;
	}

	// Get a global status value from the two sensors
	status = ( 	MAX31855_read(SENSOR_1, &thermo_temp_1, &internal_temp_1) ||
				MAX31855_read(SENSOR_2, &thermo_temp_2, &internal_temp_2)  );

	// In case of any error from thermocouples, then stop the process
	if (status != 0) {
		private_stop_reflow_process();
		USB_printf_buff("Error\n");
		return;
	}

	avg_thermo = (thermo_temp_1+thermo_temp_2)/2;

	// Compute the next desired temperature for the current time (it's also converted to float)
	target_temp = compute_target_temp(reflow_process_tick);

	// Update SSR's PWM in order to match this temperature
	SSR_duty1 = compute_fake_duty_cycle(SENSOR_1, tick_interval, avg_thermo, target_temp);
	SSR_duty2 = compute_fake_duty_cycle(SENSOR_2, tick_interval, avg_thermo, target_temp);
	SSR_set_duty_cycle(SENSOR_1, max(SSR_duty1, SSR_duty2));
	SSR_set_duty_cycle(SENSOR_2, max(SSR_duty1, SSR_duty2));

	// Send a feedback to the host PC about the current status
	USB_printf_buff("time %d target %d temp_1 %d temp_2 %d duty_1 %d duty_2 %d\n",
						reflow_process_tick,
						(int32_t) target_temp,
						(int32_t) thermo_temp_1,
						(int32_t) thermo_temp_2,
						(uint8_t) SSR_duty1,
						(uint8_t) SSR_duty2);

	// Print temperature data on the LCD
	PCD8544_Clear();
	PCD8544_GotoXY(0,0);
	PCD8544_printf_buff("Duty1 = %d", SSR_duty1);
	PCD8544_GotoXY(0,7);
	PCD8544_printf_buff("Duty2 = %d", SSR_duty2);
	PCD8544_GotoXY(0,14);
	PCD8544_printf_buff("Thermo1 = %d", (int32_t)thermo_temp_1);
	PCD8544_GotoXY(0,21);
	PCD8544_printf_buff("Thermo2 = %d", (int32_t)thermo_temp_2);
	PCD8544_GotoXY(0,28);
	PCD8544_printf_buff("Target = %d", (int32_t)target_temp);
	PCD8544_GotoXY(0,35);
	PCD8544_printf_buff("Time = %d", (int32_t)reflow_process_tick);
	PCD8544_Refresh();

	// Update the local tick count
	reflow_process_tick += tick_interval;

	// If the next iteration is over the last reflow point then stop the process
	if (reflow_process_tick > reflow_list[reflow_list_size-1].milliseconds) {
		private_stop_reflow_process();
		USB_printf_buff("Stop\n");
	}
}

/**
 *	@brief	Compute the target temperature at the specified time (in milliseconds)
 *	@param	millisecond => the time value for which the temperature is computed
 *	@return	The target temperature at the specified time
 */
static float compute_target_temp(uint32_t millisecond)
{
	uint16_t i;
	float dx, dy, float_ret_value;

	// Place the specified millisecond value inside the predefined reflow list
	for (i=0; (i<reflow_list_size) && (millisecond >= reflow_list[i].milliseconds); i++) {
		// In case of a perfect match there's no need to interpolate, and the exact
		// value can just be returned
		if (millisecond == reflow_list[i].milliseconds)
			return reflow_list[i].temperature;
	}

	// Otherwise an interpolation is needed (at the end of the previous for-cycle,
	// "i" points to a point which comes later than the requested one)
	dx = reflow_list[i].milliseconds - reflow_list[i-1].milliseconds;
	dy = reflow_list[i].temperature - reflow_list[i-1].temperature;
	float_ret_value = reflow_list[i-1].temperature +
			(dy/dx)*(millisecond - reflow_list[i-1].milliseconds);

	return float_ret_value;
}

/**
 *	@brief	Compute the new value of PWM which should be used in order to reach the
 *			desired temperature
 *	@param	delta_time => is the time interval used for PID controller's computations
 *			thermo_temp => is the current thermocouple's temperature
 *			target_temp => is the desired temperature
 *	@return	The new duty cycle value that should be set to the SSR
 */
static uint8_t	compute_duty_cycle(SENSOR_ID id, uint32_t delta_time, float thermo_temp, float target_temp)
{
	float current_error, derivative_error;
	uint32_t duty;

	// Compute the theoretical duty cycle
	current_error = target_temp - thermo_temp;
	integral_error = integral_error + current_error*(delta_time/1000.0f);
	derivative_error = (current_error - previous_error) / (delta_time/1000.0f);
	duty = 	Pcoeff[id]*current_error +
			Icoeff[id]*integral_error +
			Dcoeff[id]*derivative_error;

	// Update previous_error for the next cycle
	previous_error = current_error;

	// If it's outside allowed range, then limit it
	if (duty > SSR_MAX_DUTY) {
		duty = SSR_MAX_DUTY;
	}

	return (uint8_t)duty;
}

// That's a basic version of the above function which just implements ON and OFF of the SSR
static uint8_t	compute_fake_duty_cycle(SENSOR_ID id, uint32_t delta_time, float thermo_temp, float target_temp)
{
	if ((target_temp - thermo_temp) >= 0) {
		return SSR_MAX_DUTY;
	} else {
		return SSR_MIN_DUTY;
	}
}
