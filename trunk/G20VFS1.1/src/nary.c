/*
	File: narray.c
	Description: Maintain N-array for File Descriptor
	 
*/

#include "../include/structure.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/freelist.h"
#include "../include/string_operations.h"
#include "../include/hashtable.h"
#include "../include/nary.h"
#include "../include/directory_operators.h"
/*


*/



struct nary* create_nary(long int add)
{

    if(add==-1) return NULL;
    
	int i;
	FILE *fp = VFS_MOUNT_POINT;
	struct file_descriptor temp;
	
	struct nary *n;//New Tree Node
	
	n = (struct nary *)malloc(sizeof(struct nary));//Create Node in RAM    
	
	fseek(fp,add,SEEK_SET);//jump to file discriptor
    
    fread(&temp,sizeof(temp),1,fp);//read FD
	
	strcpy(n->name,temp.file_name);//Copy file name to 
	
	printf("\nNAme: %s Path: %s",temp.file_name,temp.file_location_full_path);
	
	///////////////Also add respective entry in Hash tables
	//int a,b;
	//hash(n->name[0],n->name[1],&a,&b);
	insert_in_hashtable(temp.file_name,temp.file_location_full_path);
	
	 
	
	
	//puts(n->name);
	n->child=NULL; //Initialize
	n->sibling=NULL; //Initialize
	

    if(NARY_ROOT==NULL)//Global Var, pointing to Root Directory/node
    {
        printf("\nCreating N-Ary Tree....");
    
        NARY_ROOT = n;//initialize root to n
    }   
    
  
	if(temp.sibling!=-1)  //recurrsive call for each sibling      
        n->sibling=create_nary(temp.sibling);
    if(temp.child!=-1)    //recurrsive call for each child
        n->child=create_nary(temp.child); 
   
    return n;
       
        
}

/*
*/

int add_node_to_nary()
{

}


/*
*/

int delete_node_from_nary()
{


}


/*
*/

int display_nary(struct nary *node)
{

    if(node==NULL) return;

    puts(node->name);
    
    display_nary(node->child);
    
    display_nary(node->sibling);
    

}


/*
 */
int search_file_in_nary(char input[])
{

}


/*--------EOF--------*/
