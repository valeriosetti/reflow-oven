/*
 * command_processor.c
 *
 * This is the basic command interpreter for the reflow oven.
 */

// Includes
#include "command_processor.h"
#include "pcd8544.h"
#include "stdlib.h"

// Local functions
static CMD_PROC_RET process_line(void);
static void add_reflow_point(int argc, char *argv[]);
static void get_reflow_list(int argc, char *argv[]);
static void clear_reflow_list(int argc, char *argv[]);

// Local defines
typedef struct{
	uint16_t 	seconds;
	uint16_t	temperature;
	struct REFLOW_POINT*	prev;
	struct REFLOW_POINT*	next;
}REFLOW_POINT;

REFLOW_POINT *reflow_list = NULL;

typedef struct{
	char*	text_command;
	void 	(*function)(int argc, char *argv[]);
}COMMAND_ASSOCIATION;

COMMAND_ASSOCIATION allowed_commands[] = {
		{"add_reflow_point", add_reflow_point},
		{"get_reflow_list", get_reflow_list},
		{"clear_reflow_list", clear_reflow_list},
		{NULL, NULL}
};


// Local variables
char single_line[128];
uint8_t current_pos = 0;

/*
 * This functions gets called from the Virtual COM when receiving chars from the host PC.
 * Parameters:
 * 	- Data => is the pointer to the received data
 * 	- Len => length of the received data
 * It returns CMD_PROC_OK if the reception is fine; CMD_PROC_FAIL otherwise
 */
CMD_PROC_RET cmd_proc_receive_data(uint8_t* Data, uint32_t *Len)
{
	uint8_t index;

	for (index=0; index<(*Len); index++){
		switch (*Data){
		case '\0':
			// Return an error if the \0 is found in the text
			return CMD_PROC_FAIL;
			break;
		case '\n':
		case '\r':
			// Process the line whenever \n or \r are received
			process_line();
			break;
			break;
		default:
			// Copy the received data to the local buffer
			single_line[current_pos] = *Data;
			Data++;
			current_pos++;
		}
	}

	return CMD_PROC_OK;
}

/*
 * Process the received command line
 */
static CMD_PROC_RET process_line(void)
{
	CDC_Transmit_FS(single_line,current_pos);
	current_pos = 0;
	return CMD_PROC_OK;
}

/*
 *
 */
static void add_reflow_point(int argc, char *argv[])
{
	REFLOW_POINT* new_element = malloc(sizeof(REFLOW_POINT));
	if (reflow_list == NULL){
		reflow_list = new_element;
		reflow_list->prev = NULL;
		reflow_list->next = NULL;
		reflow_list->seconds = 0;
		reflow_list->temperature = 25;
	}
}

/*
 *
 */
static void get_reflow_list(int argc, char *argv[])
{
	if (reflow_list == NULL)
		return;
}
/*
 *
 */
static void clear_reflow_list(int argc, char *argv[])
{
	if (reflow_list == NULL)
		return;
}
