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
static CMD_PROC_RET process_line(void);
static void add_reflow_point(int argc, char *argv[]);
static void get_reflow_list(int argc, char *argv[]);
static void clear_reflow_list(int argc, char *argv[]);

// Local defines
#define	MAX_COMMAND_LENGTH	128
#define	MAX_ELEMENT_LENGTH 	32
#define MAX_ARGS_NUM		4

#define CDC_Transmit_FS_const_char(_text_) 		CDC_Transmit_FS(_text_, sizeof(_text_))

// New types
typedef struct{
	uint16_t 	seconds;
	uint16_t	temperature;
	struct REFLOW_POINT*	prev;
	struct REFLOW_POINT*	next;
}REFLOW_POINT;

// Local variables
REFLOW_POINT *reflow_list = NULL;

typedef struct{
	char*	text_command;
	void 	(*function)(int argc, char *argv[]);
}CONSOLE_COMMAND;

CONSOLE_COMMAND allowed_commands[] = {
		{"add_reflow_point", add_reflow_point},
		{"get_reflow_list", get_reflow_list},
		{"clear_reflow_list", clear_reflow_list},
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
CMD_PROC_RET cmd_proc_receive_data(uint8_t* Data, uint32_t *Len)
{
	uint8_t index;
	char *pData;

	for (index=0; index<(*Len); index++){
		switch (Data[index]){
		case '\0':
			// Return an error if the \0 is found in the text
			return CMD_PROC_FAIL;
			break;
		case '\n':
		case '\r':
			// Process the line whenever \n or \r are received.
			// First set the console to a new line
			CDC_Transmit_FS_const_char("\n\r");
			// Append a '\0' to the local buffer before processing it
			single_line[current_pos] = '\0';
			// Process the line just received
			process_line();
			break;
			break;
		default:
			// Copy the received data to the local buffer (if the buffer has not yet reached
			// its maximum size, of course...
			if (current_pos < (MAX_COMMAND_LENGTH-1)){
				single_line[current_pos] = Data[index];
				current_pos++;
			}else{
				return CMD_PROC_FAIL;
			}
			// Echo the data back to the console as reference
			CDC_Transmit_FS(Data, (uint16_t) *Len);
		}
	}

	return CMD_PROC_OK;
}

/*
 * Process the received command line
 */
static CMD_PROC_RET process_line(void)
{
	char *funct;
	char *args[MAX_ARGS_NUM];
	char *token;
	int argc = 0;
	char separator[2] = " ";
	uint8_t i;

	// Parse all the elements of the command line
	token = strtok(single_line, separator);
	while(token != NULL){
		// The maximum number of elements was reached! Exit this function returning
		// and error
		if (argc == MAX_ARGS_NUM){
			CDC_Transmit_FS_const_char("Too many arguments");
			return CMD_PROC_FAIL;
		}

		if (argc == 0){
			// the first element of the line is the function
			funct = token;
		}else{
			// the other elements are the parameters
			args[argc-1] = token;
		}
		argc++;
		token = strtok(NULL, separator);
	};

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
