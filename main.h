#include <limits.h>
/* The number will be invalid if all the arguments' length are smaller than 4 */
#define NUM_ARG_MAX (2097152/4-1)

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 255
#endif

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#ifndef FILENAME_MAX
#define FILENAME_MAX 256
#endif

#define SUCCESS 0

#ifndef ARG_MAX
/* Get from getconf ARG_MAX in Ubuntu 14.04 64-Bit */
#define ARG_MAX 2097152
#endif


extern char cmd[ARG_MAX]; 

/* display promopt on the screen */
char* display_prompt();
/* read input from terminal */
void read_cmd(char* command, char* param[]);

/* Standard Out */
void stdErr();

int buildin_cmd(char* in, char* param[]);
