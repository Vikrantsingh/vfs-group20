/*
	File: String_operations.c
	Description: Manipulation Strings..
	 
*/

#include "../include/structure.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*
 parameters: input,start,length
*/
char* sub_string(char ch[],long int start, long int length)
{
	char *p;
	p=(char *)malloc(sizeof(char)*(length+1));
	int i=0;
	int j=start;
	while(i<length && ch[j]!='\0')	
	{
		p[i]=ch[j];
		i++;j++;
	}
	p[i]='\0';
	//printf("\nstring = %s start=%ld, length =%ld",ch,start,length);
	//puts("substrin:");
	//puts(p);
	return p;
	
}

