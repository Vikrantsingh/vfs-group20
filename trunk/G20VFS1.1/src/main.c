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
#include<stdio.h>

FILE *VFS_MOUNT_POINT;
struct Meta_header METADATA;

void main()
{
	char name[]="demo.dat";

	long int size = 2000000000;

	printf("\n======= Virtual File System ========\n");

	create_vfs(name,size);

	mount_vfs(name);

	create("/","abc.txt","Hello This is our first sample data ");

	create("/","xyz.txt","Hello This is our second sample data ");

	//import()

	listfile("/");

	unmount_vfs();
}
