#include"main.h"
#include<stdio.h>
#include<string.h>
int parseInfoInit(parseinfo* info){
	info->flag = 0;
	info->infile = NULL;
	info->outfile = NULL;
	info->command2 = NULL;
	info->parameters2 = NULL;
	return 0;
}


int parse(char** param, int* para, parseinfo* info){
	int i;
	parseInfoInit(info);
	int numberofCommand=1;	
	int paraNum = (*para);
	if( strcmp(param[paraNum-1],"&") == 0){
		info->flag |= 1;
		param[paraNum-1] = NULL;
		paraNum--;
	}
	for(i=0; i<paraNum;){
		
		if(strcmp(param[i],"<")==0)
		{
			info->flag |= REDIRECT_IN;
			info->infile = param[i+1];
			param[i] = NULL;
			i+=2;
		}
		else if(strcmp(param[i],">")==0)
		{
			info->flag |= REDIRECT_OUT;
			info->outfile = param[i+1];
			param[i] = NULL;
			i+=2;
		}
		else if(strcmp(param[i],">>")==0)
		{
			info->flag |= REDIRECT_APPEND;
			info->outfile = param[i+1];
			param[i] = NULL;
			i+=2;
		}
		else if(strcmp(param[i],"|")==0)
		{
			char* temp;
			info->flag |= PIPED;
			param[i] = NULL;
			info->command2 = param[i+1];
			info->parameters2 = &param[i+1];
			for(temp = info->parameters2[0]+strlen(info->parameters2[0]);
					temp!=&(info->parameters2[0][0]) && *temp!='/';temp--)
			if(*temp == '/')
				temp++;
			info->parameters2[0] = temp;
			numberofCommand++;
			i++;
			break;
		}
		else
			i++;
	}
	
	(*para) -= i;
	return numberofCommand;

}
