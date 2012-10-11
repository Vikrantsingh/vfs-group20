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
	char file_name[50];
    char file_location_full_path[200];
	char file_type[5];
	long int file_size;
	long int location_block_number;//location of first data block
	long int sibling;
	long int child;
	long int parent;

};


struct free_list {

	long int block_number;
	long int next;
};

struct block{
	char isfull;//0-empty 1-full
	char data[1024];
	long int next; 
};

struct nary{

    char name[50];
    long int FD;
    struct nary *child;
    struct nary *sibling;

};

struct hashtable{    /////////node--for chaining with BST structure 
    
   char name[50];
   char path[200];
   
   struct hashtable *lnextnode;
   struct hashtable *rnextnode;
   
};




/*
Global Variables Across all the files
*/
extern FILE *VFS_MOUNT_POINT;
extern struct Meta_header METADATA;
extern long int MAX_VFS_SIZE;
extern long int MAX_LENGTH_OF_NAME;
extern int IS_VFS_MOUNTED; //1-YES 0-NO
extern struct nary *NARY_ROOT;
extern char PRESENT_WORKING_DIRECTORY[10000];
extern struct hashtable *HASH_TABLE[26][26];


