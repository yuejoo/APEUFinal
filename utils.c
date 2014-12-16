#include<stdio.h>
#include<stdlib.h>
#include<pwd.h>
#include<unistd.h>
#include<limits.h>
#include<sys/types.h>
#include<string.h>

#include<readline/readline.h>

#include"main.h"

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

char hostname[HOST_NAME_MAX];
char pwd[PATH_MAX];
struct passwd* hostuser;

char* display_prompt(){


	if( gethostname(hostname,HOST_NAME_MAX) != SUCCESS)
		stdErr();

	/* hostuser to get both username and the pwd */
	if( (hostuser = getpwuid(getuid()))  == NULL)
		stdErr();

	if( getcwd(pwd, PATH_MAX) == NULL)
		stdErr();

	checkHomeDir(pwd);

	int lenth = strlen(hostuser->pw_name)+strlen(pwd)+9;
	char *out;

	if((out = (char*)malloc(lenth)) == NULL)
		stdErr();
	strcpy(out, hostuser->pw_name);
	strcat(out, "@sish:");
	strcat(out, pwd);
	strcat(out, "$ ");

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
	/* Handle the non-input in cmd */
	free(in);
	in = tempCmd;
	if(strlen(in) == 0)
		return;

	if( strcpy(cmd,in) == NULL)
		stdErr();

	int length = sperateCMD(cmd);
	if( length == 0){
		cmd[0] = '\0';
	}
	else{
		int i=0, index=1;
		param[0] = cmd;
		for(i=0; i< length; i++){
			if(cmd[i] == '\0')
				param[index++] = &cmd[0]+i+1;
		}
	}
}


int buildin_cmd(char* in, char *param[]){
	if( strcmp(in,"quit")==0 )
		exit(0);

	if( strcmp(param[0],"cd")==0 ){
		if(param[1] == NULL || strcmp(param[1],".") == 0 )
			return 1;
		if( strcmp(param[1],"..") == 0)
			chdir("../");
		else{
			chdir(param[1]);	
		}

		return 1;	
	}
	if( strcmp(in,"echo")==0 ){
		int index = 1;
		while(param[index] != NULL){
			fprintf(stdout, "%s", param[index]);
			index++;
			if(param[index] != NULL)
				fprintf(stdout, " ");
		}
		return 1;
	}
	return 0;
}
