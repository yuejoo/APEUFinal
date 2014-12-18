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
#define MAX_PIPES 20

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
	int in_fd,out_fd, old_in, old_out;
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
		char** curParam = param;
		in = display_prompt();
		paraNum = read_cmd(in, param);

		if( paraNum == 0 )
			continue;
		if( strcmp(param[0],"exit") == 0){
			printf("quit");
			exit(0);
		}

		parseinfo info;
		int numcmd = parse(param,&paraNum,&info);

		if(pipe(pipefd)< 0 )
			stdErr();
		if( buildin_cmd(cmd, param))
			continue;
		/* input command */
		int chpid;
		int status = 0;
		if( (chpid = fork()) != 0){
			waitpid(chpid,&status,0);
		}
		else{
			int indexCmd = 0;
			int re = 0;
			int tpipe = 0;
			

			while( numcmd >= 1){
				int chpid1=0;
				pipe(pipefd);
				if( (chpid1=fork()) == 0){
					if(info.flag & REDIRECT_IN){
						in_fd = open(info.infile, O_CREAT | O_RDONLY, 0666);
						close(STDIN_FILENO);
						dup2(in_fd,STDIN_FILENO);
						close(in_fd);
					}

					if(info.flag & REDIRECT_OUT){
						out_fd = open(info.outfile, O_WRONLY|O_CREAT| O_TRUNC,0666);
						close(STDOUT_FILENO);
						dup2(out_fd, STDOUT_FILENO);
						close(out_fd);
					}

					if(info.flag & REDIRECT_APPEND){
						out_fd = open(info.outfile, O_WRONLY | O_APPEND | O_CREAT,0666);
						close(STDOUT_FILENO);
						dup2(out_fd, STDOUT_FILENO);
						close(out_fd);
					}

					if( (re == 1 && indexCmd!=0) || ( info.flag & PIPED && indexCmd != 0 )){
						dup2(tpipe, 0);
					}

					if(info.flag & PIPED ){
						dup2(pipefd[1], 1);
					}

					close(pipefd[0]);
					if( curParam[1]!=NULL && 
						strcmp(curParam[0], "echo") == 0 &&
						curParam[1][0] == '$'){ 
						if(strcmp(curParam[1], "$$")  == 0)
						fprintf(stdout,"%d\n", getpid());
						else if(strcmp(curParam[1],"$?")==0 )
						fprintf(stdout,"lastestCmd\n" );
						else 
						fprintf(stdout,"%s\n",getenv(curParam[1]+1));
					}
					else
						execvp( *curParam, curParam);

				}
				wait(NULL);
				close(pipefd[1]);
				tpipe = pipefd[0];
				/* move to next cmd */
				curParam = info.parameters2;
				/* remaining cmd in the cmd buffer */
				re = 0;
				if(info.flag & PIPED ){
					re = parse(curParam, &paraNum,&info);
					numcmd++;
				}
				
				if(re == 0)
					numcmd = 0;
				else
					numcmd --;
				/* count i'th cmd */
				indexCmd++;
			}
			releaseMemory(param);
		}	

	}
	return 0;
}
