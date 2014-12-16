#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>       
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include "main.h"
#define TRUE 1


int xflg,cflg;

void usage()
{
	printf("Usage: sish [ −x] [ -c command]\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
	int ch;
	while ((ch = getopt(argc, argv, "dehp:s:"))!= -1)
	{
		switch (ch)
		{
			case 'x':
				usage();
				break;
			case 'c':
				break;
			default:
				usage();
				return EXIT_SUCCESS;
		}
	}
	
	while(TRUE){
		char* in=NULL;
		char* param[NUM_ARG_MAX] = {NULL};
		in = display_prompt();
		read_cmd(in, param);
		printf("%s justout\n",param[0]);	
		
		/* input command */
		in = param[0];
		
		if( strlen(in) == 0 || buildin_cmd(cmd, param))
			continue;
		
		if( fork() != 0){
			int status = 0;

			waitpid(-1,&status,0);
		}
		else{
			printf("%s exe\n", param[0]);
			execvp(param[0] , param);				
		}	

	}
	return 0;
}
