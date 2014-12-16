#include<stdio.h>
#include<stdlib.h>
#include<pwd.h>
#include<unistd.h>
#include<limits.h>
#include<sys/types.h>
#include<string.h>

#include<readline/readline.h>

#include"main.h"

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
/* Get from getconf ARG_MAX in Ubuntu 14.04 */
#define ARG_MAX 2097152
#endif

void stdErr(){
	exit(1);
}

void checkHomeDir(char* dir){
	char* tempdir;
	if( (tempdir = strstr( dir, "/home")) != NULL){
		if(tempdir == dir){
			int len = strlen(dir);
			int i=5, index=1;
			for(;i<len;i++)
				dir[index++]=dir[i];
			dir[index] = '\0';
			dir[0]='~';
		}
	}
}

char* display_prompt(){
	
	char hostname[HOST_NAME_MAX];
	char cwd[PATH_MAX];
	struct passwd* hostuser;
	
	if( gethostname(hostname,HOST_NAME_MAX) != SUCCESS)
		stdErr();
	
	/* hostuser to get both username and the pwd */
	if( (hostuser = getpwuid(getuid()))  == NULL)
		stdErr();

	if( getcwd(cwd, PATH_MAX) == NULL)
		stdErr();
	checkHomeDir(cwd);

	int lenth = strlen(hostuser->pw_name)+strlen(cwd)+8;
	char *out;

	if((out = (char*)malloc(lenth)) == NULL)
		stdErr();
	strcpy(out, hostuser->pw_name);
	strcat(out, "@sish:");
	strcat(out, cwd);
	strcat(out, "$");

	return out;
}

char cmd[ARG_MAX];

int sperateCMD(char* in){
	int i=0;
	int len = strlen(in);
	for(; i<len; i++){
		if(in[i] == ' ')
			in[i]='\0';
	}
	return len;
}


void read_cmd(char* in, char* param[]){
	
	char *tempCmd;
	tempCmd  = readline(in);
	free(in);
	in = tempCmd;

	if( strcpy(cmd,in) == NULL)
		stdErr();
	
	int length = sperateCMD(cmd);
	if( length == 0){
		cmd[0] = '\0';
	}
	else{
		int i=0, index=0;
		for(i=0; i< length; i++){
			if(cmd[i] == '\0')
				param[index] = &cmd[0]+i+1;
		}
	}

}
