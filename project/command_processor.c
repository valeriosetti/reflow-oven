/*
 * command_processor.c
 *
 * This is the basic command interpreter for the reflow oven.
 */

// Includes
#include "command_processor.h"
#include "pcd8544.h"
#include "stdlib.h"
#include "string.h"

// Local functions
static int process_line(void);
static int test(int argc, char *argv[]);
static int add_reflow_point(int argc, char *argv[]);
static int get_reflow_list(int argc, char *argv[]);
static int clear_reflow_list(int argc, char *argv[]);

// Local defines
#define	MAX_COMMAND_LENGTH	128
#define	MAX_ELEMENT_LENGTH 	32
#define MAX_ARGS_NUM		4

#define CDC_Transmit_FS_const_char(_text_) 		CDC_Transmit_FS(_text_, sizeof(_text_))

// New types
typedef struct{
	uint16_t 	seconds;
	uint16_t	temperature;
}REFLOW_POINT;

// Local variables
#define REFLOW_LIST_MAX_SIZE		10
REFLOW_POINT reflow_list[REFLOW_LIST_MAX_SIZE];
uint16_t reflow_list_size = 0;

typedef struct{
	char*	text_command;
	int 	(*function)(int argc, char *argv[]);
}CONSOLE_COMMAND;

CONSOLE_COMMAND allowed_commands[] = {
		{"add_reflow_point", add_reflow_point},
		{"get_reflow_list", get_reflow_list},
		{"clear_reflow_list", clear_reflow_list},
		{"test", test},
		{NULL, NULL}
};

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
				CDC_Transmit_FS_const_char("Error\n");
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
	char *funct;
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

		if (argc == 0){
			// the first element of the line is the function
			funct = token;
		}else{
			// the other elements are the parameters
			args[argc-1] = token;
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
 *	@brief	Add a new point to the current reflow list
 * 	@param	
 * 	@return	0 in case of success; a negative value otherwise
 */
static int add_reflow_point(int argc, char *argv[])
{
	// Check if there's still space in the local buffer to store the data
	if (reflow_list_size < REFLOW_LIST_MAX_SIZE){
		reflow_list[reflow_list_size].seconds = 0;
		reflow_list[reflow_list_size].temperature = 0;
		reflow_list_size++;
		CDC_Transmit_FS_const_char("OK\n");
		return 0;
	}else{
		// Otherwise return an error
		CDC_Transmit_FS_const_char("Error\n");
		return -1;
	}
}

/**
 *	@brief	Retrieve the current list
 * 	@param	[none]
 */
static int get_reflow_list(int argc, char *argv[])
{
	uint16_t i;
	
	for (i=0; i<reflow_list_size; i++){
		CDC_Transmit_FS_const_char("time ");
		CDC_Transmit_FS_const_char("?? ");
		CDC_Transmit_FS_const_char("temperature ");
		CDC_Transmit_FS_const_char("?? ");
		CDC_Transmit_FS_const_char("\n");
	}
	return 0;
}

/**
 *	@brief	Clear the reflow list
 * 	@param	[none]
 */
static int clear_reflow_list(int argc, char *argv[])
{
	reflow_list_size = 0;
	CDC_Transmit_FS_const_char("OK\n");
	return 0;
}

/**
 *	@brief	Just send a feedback
 * 	@param	[none]
 */
static int test(int argc, char *argv[])
{
	CDC_Transmit_FS_const_char("OK\n");
}
