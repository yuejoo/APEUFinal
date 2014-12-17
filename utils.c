#include<stdio.h>
#include<stdlib.h>
#include<pwd.h>
#include<unistd.h>
#include<limits.h>
#include<sys/types.h>
#include<string.h>

#include<readline/readline.h>
#include"main.h"
#include"stack.h"

char* makeOperator(char in){
	char *temp = (char*)malloc(3);
	temp[0] = in;
	temp[1] = '\0';
	temp[2] = '\0';
	if(in == 'd'){
		temp[0] = '>';
		temp[1] = '>';
	}
	return temp;
}

void stdErr(){
	exit(1);
}

void releaseMemory(char* param[]){
	int index=0;
	while(param[index] != NULL){
		if(param[index][0] == '>' || 
		   param[index][0] == '<' || 
		   param[index][0] == '|' || 
		   param[index][0] == '&'){
		   	free(param[index]);
		 }
		 index++;
	}
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
	/* flag to determine if we should skip the space in 
	a string */
	int findQuote = 0;
	/* Stack to valid the quotes pair */
	Stack	stk;
	Stack_Init(&stk);

	for(; i<len; i++){
		if(in[i] == ' ' && !findQuote)
			in[i]='\0';
		if( in[i] == '\'' || in[i] == '\"' ){
			if( stk.size > 0 && Stack_Top(&stk) == in[i])
				Stack_Pop(&stk);
			else
				Stack_Push(&stk,in[i]);
			findQuote++;
			if(stk.size == 0)
				findQuote = 0;
		}
	}
	return len;
}


int read_cmd(char* in, char* param[]){

	char *tempCmd;
	 
	tempCmd  = readline(in);
	int paraNum;
	/* Handle the non-input in cmd */
	free(in);
	in = tempCmd;
	
	if(strlen(in) == 0){
		param[0] = in;
		return 0;
	}

	if( strcpy(cmd,in) == NULL)
		stdErr();
	
	int length = sperateCMD(cmd);
	if( length < 0){
		cmd[0] = '\0';
	}
	else{
		int i=0, index=1;
		for(i=0; i< length; i++)
			if(cmd[i]!='\0'){
				param[0] = &cmd[i];
				break;
			}

		/* skip the space before the first char in cmd*/
		int firstItem = i;
		for(i = firstItem+1; i< length; i++){
			if(cmd[i] == '>' || cmd[i] == '<' || cmd[i] == '|' || cmd[i] == '&'){
				if(cmd[i+1] == '>' && cmd[i] == '>'){
					param[index++] = makeOperator('d');
					cmd[i+1] = '\0';cmd[i]='\0';
				}
				else{
					param[index++] = makeOperator(cmd[i]);
					cmd[i] = '\0';
				}
			}
			else if(cmd[i] != '\0' && cmd[i-1] == '\0')
				param[index++] = &cmd[firstItem]+i;
		}
		paraNum = index;
	}
	return paraNum;
}


int buildin_cmd(char* in, char *param[]){
	in = param[0];
	
	if( strcmp(in,"quit")==0 ){
		exit(0);
	}

	if( strcmp(param[0],"cd")==0 ){
		if(param[1] == NULL || strcmp(param[1],".") == 0 )
			return 1;
		if( strcmp(param[1],"..") == 0)
			chdir("../");
		else{
			chdir(param[1]);	
		}

		return 0;	
	}
	if( strcmp(in,"echo")==0 ){
		int index = 1;
		while(param[index] != NULL){
			fprintf(stdout, "%s", param[index]);
			index++;
			if(param[index] != NULL)
				fprintf(stdout, " ");
		}
		return 0;
	}
	return 0;
}
