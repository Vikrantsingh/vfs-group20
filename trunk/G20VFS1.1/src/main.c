/*
	File: main.c
	Description:Functions for Creating - mounting filesystem
	Modified On:26/08/2012
*/


#include "../include/structure.h"
#include <stdio.h>
#include <string.h>

#include"../include/file_system.h"
#include"../include/directory_operators.h"
#include"../include/file_operators.h"
#include"../include/ui.h"
#include<stdio.h>

FILE *VFS_MOUNT_POINT;
struct Meta_header METADATA;
long int MAX_VFS_SIZE;
long int MAX_LENGTH_OF_NAME;
int IS_VFS_MOUNTED; //1-YES 0-NO
void main()
{
	char name[]="demo.dat";

	MAX_VFS_SIZE=1000000000;

	MAX_LENGTH_OF_NAME=50;

	IS_VFS_MOUNTED=0;

	long int size = 2000000000;

	printf("\n======= Virtual File System ========\n");


	get_command();
/*
create_vfs(name,size);
	mount_vfs(name);
	create("/","abc.txt","Hello This is our first sample data ");
	create("/","xyz.txt","Hello This is our second sample data ");
	//import()
	listfile("/");
	unmount_vfs();
*/



}
