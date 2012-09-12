/*
	File: structure.h
	Description:User interaction
	 
*/

/*

*/


#include <stdio.h>

struct Meta_header{

	char label[50];
	long int no_of_file_desp_used;
	long int max_no_of_file_desp_available;
	long int freelist;
	long int counter;
	long int File_descriptor_array;
	long int no_of_blocks;
	long int no_of_blocks_used;
	long int vfs_size;

};

struct file_descriptor{
	char isfull;//0-empty 1-full
	long int FID;
	char file_name[20];
	//char location_full_path[50];
	char file_type[5];
	long int file_size;
	long int location_block_number;//location of first data block

};


struct free_list {

	long int block_number;
	long int next;
};

struct block{
	char isfull;//0-empty 1-full
	char data[20];
	long int next; 
};

/*
Global Variables Across all the files
*/
extern FILE *VFS_MOUNT_POINT;
extern struct Meta_header METADATA;
extern long int MAX_VFS_SIZE;
extern long int MAX_LENGTH_OF_NAME;
extern int IS_VFS_MOUNTED; //1-YES 0-NO

