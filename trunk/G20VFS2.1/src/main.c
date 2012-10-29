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
struct nary *NARY_ROOT;
struct bst *BST_FULL_PATH;
char PRESENT_WORKING_DIRECTORY[10000];
struct hashtable *HASH_TABLE[26][26];
int *FREELIST;


void main()
{

       
	char name[]="demo.dat";

    /////////INITIALIZATION
    
    strcpy(PRESENT_WORKING_DIRECTORY,"");
    
	MAX_VFS_SIZE=1000000000;

	MAX_LENGTH_OF_NAME=50;

	IS_VFS_MOUNTED=0;

	long int size = 2000000000;
    int i,j;
    for(i=0;i<26;i++)
        for(j=0;j<26;j++)
            HASH_TABLE[i][j]=NULL;



	printf("\n======= Virtual File System ========\n");

	get_command();
 

}
