/*
 * command_processor.c
 *
 * This is the basic command interpreter for the reflow oven.
 */

// Includes
#include "command_processor.h"

// Local functions
static int process_line(void);
static int test(int argc, char *argv[]);

// Local defines
#define	MAX_COMMAND_LENGTH	128
#define	MAX_ELEMENT_LENGTH 	32
#define MAX_ARGS_NUM		4

// New typedefs
typedef struct{
	char*	text_command;
	int 	(*function)(int argc, char *argv[]);
}CONSOLE_COMMAND;

// Here's the list of the allowed commands
CONSOLE_COMMAND allowed_commands[] = {
		{"add_reflow_point", add_reflow_point},
		{"get_reflow_point", get_reflow_point},
		{"clear_reflow_list", clear_reflow_list},
		{"set_PID_parameters", set_PID_parameters},
		{"stop_reflow_process", stop_reflow_process},
		{"start_reflow_process", start_reflow_process},
		{"set_reflow_process_period", set_reflow_process_period},
		{"test", test},
		{NULL, NULL}
};


// Local variables
char single_line[MAX_COMMAND_LENGTH];
uint8_t current_pos = 0;

/*
 * This functions gets called from the Virtual COM when receiving chars from the host PC.
 * Parameters:
 * 	- Data => is the pointer to the received data
 * 	- Len => length of the received data
 * It returns CMD_PROC_OK if the reception is fine; CMD_PROC_FAIL otherwise
 */
int cmd_proc_receive_data(uint8_t* Data, uint32_t *Len)
{
	uint8_t index;
	char *pData;

	for (index=0; index<(*Len); index++){
		switch (Data[index]){
		case '\0':
			// Return an error if the \0 is found in the text
			return -1;
			break;
		case '\n':
		case '\r':
			// Append a '\0' to the local buffer before processing it
			single_line[current_pos] = '\0';
			// Process the line just received
			if (process_line() != 0)
				USB_printf_buff("Error\n");
			break;
		default:
			// Copy the received data to the local buffer (if the buffer has not yet reached
			// its maximum size, of course...
			if (current_pos < (MAX_COMMAND_LENGTH-1)){
				single_line[current_pos] = Data[index];
				current_pos++;
			}else{
				return -2;
			}
		}
	}

	return 0;
}

/*
 * Process the received command line
 */
static int process_line(void)
{
	char *funct = NULL;
	char *args[MAX_ARGS_NUM];
	char *token;
	int argc = 0;
	char separator[2] = " ";

	// Parse all the elements of the command line
	token = strtok(single_line, separator);
	while(token != NULL){
		// The maximum number of elements was reached! Exit this function returning
		// and error
		if (argc == MAX_ARGS_NUM){
			return -1;
		}

		if (funct == NULL){
			// the first element of the line is the function
			funct = token;
		}else{
			// the other elements are the parameters
			args[argc] = token;
			argc++;
		}
		token = strtok(NULL, separator);
	};
	current_pos = 0;
	
	// Search the selected function andd call it with the specified 
	// parameters
	uint8_t i=0;
	uint8_t res;
	
	while (allowed_commands[i].text_command != NULL){
		// Compare the command with the known ones
		res = strcmp(allowed_commands[i].text_command, funct);
		if (res == 0){
			// If a matching was found, then call the function
			allowed_commands[i].function(argc, args);
			return 0;
		}
		i++;
	}
	return -2;
}

/**
 *	@brief	Just send a feedback
 * 	@param	[none]
 */
static int test(int argc, char *argv[])
{
	USB_printf_buff("OK\n");
	return 0;
}
