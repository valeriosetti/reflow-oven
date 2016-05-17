/*
 * reflow_process.h
 *
 *  Created on: 17/mag/2016
 *      Author: valerio
 */

#ifndef REFLOW_PROCESS_H_
#define REFLOW_PROCESS_H_

#include "command_processor.h"

// Public functions
int add_reflow_point(int argc, char *argv[]);
int get_reflow_list(int argc, char *argv[]);
int clear_reflow_list(int argc, char *argv[]);
int start_stop_reflow_process(int argc, char *argv[]);
void reflow_process(uint32_t tick_interval);

#endif /* REFLOW_PROCESS_H_ */
