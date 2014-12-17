#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>       
#include <sys/wait.h>
#include <sys/stat.h>
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
	int in_fd, old_in, old_out;
	old_out = dup(STDOUT_FILENO);
	old_in = dup(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDIN_FILENO);

	while(TRUE){
		dup2(old_in, STDIN_FILENO);
		dup2(old_out, STDOUT_FILENO);

		int paraNum = 0;
		int pipefd[2];
		char* in=NULL;
		char* param[NUM_ARG_MAX] = {NULL};
		in = display_prompt();
		paraNum = read_cmd(in, param);

		if( paraNum == 0 )
			continue;

		parseinfo info;
		parse(param,paraNum,&info);

		if(info.flag & PIPED){
			if(pipe(pipefd)< 0 )
				stdErr();
		}
		/* input command */

		if( fork() != 0){
			int status = 0;
			if(info.flag & PIPED){
				printf("Pipe dect\n");
				int chpid1;
				if(( chpid1 = fork()) == 0){
					close(pipefd[1]);
					close(fileno(stdin)); 
					dup2(pipefd[0], fileno(stdin));
					close(pipefd[0]); 
					execvp(info.command2,info.parameters2);
				}
				else
				{
					close(pipefd[0]);
					close(pipefd[1]);
					waitpid(chpid1,&status,0);
				}

			}
			waitpid(-1,&status,0);
		}
		else{

			if(info.flag & REDIRECT_IN){
				in_fd = open(info.infile, O_CREAT | O_RDONLY, 0666);
				dup2(in_fd,STDIN_FILENO);
				close(in_fd);
			}
			int t=0;

			while( param[t] != NULL  ){
				printf("---%s\n", param[t++]);
			}

			if( paraNum == 0 || buildin_cmd(cmd, param))
				continue;

			execvp(param[0] , param);
			releaseMemory(param);
		}	

	}
	return 0;
}
