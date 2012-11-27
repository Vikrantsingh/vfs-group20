/*
	File: structure.h
	Description:User interaction
	 
*/

/*

*/


#include <stdio.h>
#define MAX_LENGTH_OF_NAME 30
#define MAX_LENGTH_OF_PATH 500
#define MAX_SUB_DIRECTORY_LEVEL 50
#include "vfs_errorcodes.h"

struct Meta_header{

	char label[MAX_LENGTH_OF_NAME];
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
	char file_name[MAX_LENGTH_OF_NAME];
    char file_location_full_path[MAX_LENGTH_OF_PATH];
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
	char data[1025];
	long int next;
	long int datalength; 
};

struct nary{

    struct file_descriptor file_desp;
    //char name[50]; 
    long int fd_loc;  
    struct nary *child;
    struct nary *sibling;
    struct nary *parent; 

};

struct hashtable{    /////////node--for chaining with BST structure 
    
   char name[MAX_LENGTH_OF_NAME];
   char path[MAX_LENGTH_OF_PATH];
   
   struct hashtable *lnextnode;
   struct hashtable *rnextnode;
   struct hashtable *parent; 
   struct hashtable *next; 
   
};

struct bst
{
    //char path[300];
    //long int fd_loc;
 
    struct nary *nary_node;     ////perform bst on path present in this reference
  //  struct file_descriptor FD;
         
    struct bst  *left;
    struct bst  *right;
    struct bst  *parent;                
    
};
 


/*
Global Variables Across all the files
*/
extern FILE *VFS_MOUNT_POINT;
extern struct Meta_header METADATA;
extern long int MAX_VFS_SIZE;
//extern long int MAX_LENGTH_OF_NAME;
extern int IS_VFS_MOUNTED; //1-YES 0-NO
extern struct nary *NARY_ROOT;
extern struct bst *BST_FULL_PATH;
extern char PRESENT_WORKING_DIRECTORY[10000];
extern struct hashtable *HASH_TABLE[26][26];
extern int *FREELIST;
//extern struct



