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
#include "../include/bst_full_path.h"
#include "../include/nary.h"
#include "../include/directory_operators.h"
/*

 
*/ 



struct nary* create_nary(long int add,struct nary* parent) 
{


    if(add==-1) return NULL;
	int i;
	FILE *fp = VFS_MOUNT_POINT;
	struct file_descriptor temp;

	
	struct nary *n;//New Tree Node
	
	n = (struct nary *)malloc(sizeof(struct nary));//Create Node in RAM    
	
	fseek(fp,add,SEEK_SET);//jump to file discriptor
    
    fread(&temp,sizeof(temp),1,fp);//read FD
	 
	//strcpy(n->name,temp.file_name);//Copy file name to 
	
	n->file_desp = temp; ///load complete FD in N -Ary  

	n->fd_loc=add;
	
	//printf("\nNAme: %s Path: %s",temp.file_name,temp.file_location_full_path);

    ////////////////Update FREELIST ARRAY IN RAM
    
    FREELIST[add/(sizeof(temp))]=1;

	
	///////////////Also add respective entry in Hash tables
	//int a,b;
	//hash(n->name[0],n->name[1],&a,&b);
   // if(strcmp(n->file_desp.file_type,"dir")!=0)
	    insert_in_hashtable(temp.file_name,temp.file_location_full_path);
 	
	///////////////ADD TO BST;
 
	struct bst  *b = (struct bst*)malloc (sizeof(struct bst));
	if(b==NULL){puts("Space not avaliable for creating bst");}
	else
	{    
	    //b->FD=temp;  
	    //b->fd_loc=add;
	    b->nary_node=n;
	    BST_FULL_PATH=insert_into_bst_path(BST_FULL_PATH,b);  
	}    
	 
	///////////////////////////
	
	//puts(n->name);
	n->child=NULL; //Initialize
	n->sibling=NULL; //Initialize
	n->parent = parent;

    if(NARY_ROOT==NULL)//Global Var, pointing to Root Directory/node
    {
        //printf("\nCreating N-Ary Tree....");
    
        NARY_ROOT = n;//initialize root to n
    }   
    
    //parent = n;  
	if(temp.sibling!=-1)  //recurrsive call for each sibling      
        n->sibling=create_nary(temp.sibling,parent);
    if(temp.child!=-1)    //recurrsive call for each child
        n->child=create_nary(temp.child,n); 
   
    return n;
       
        
}

/* 
*/

struct nary* get_parent_nary(char **dir_struct,int total_dir_in_path)
{

    struct nary *n = NARY_ROOT;
    struct nary *parent = NULL;
    int found=0,i,level=total_dir_in_path;
    //puts("In get parent");
    for(i=0;i<level;i++)
   	{
   
        //printf("in get parent dirname=%s\n\n",dir_struct[i]); 
 	    if(strcmp(n->file_desp.file_name,dir_struct[i])==0)
       	 	{
         	   parent = n;
         	   //puts(temp.file_name);
         	   found = 1;
         	   if(i==level-1)
         	    return parent;
       	 	}
        else
            {
                puts("Path not avail");
                return NULL;
            }
            
        struct nary *sib=n->child; 
//while(1); 
	    if(sib==NULL) return NULL;
	    
	    while(sib!=NULL && i<level-1 )
	    {

	        
	       if(strcmp(sib->file_desp.file_name,dir_struct[i+1])==0 && 
	                            strcmp(sib->file_desp.file_type,"dir")==0 )
               {
                   parent = sib;
                    found = 1;
                   // puts(temp2.file_name);
                    break;
               }     
            else
               {
                    found = 0;
                    sib=sib->sibling;
               }     
                 
	    }
	  
	    if(found==0) 
	    {
            
            puts("Dir not found.");
            
	        return NULL;

	    }
        else
        {
            n=parent->child;
        }

    }
    return parent;		

}

int add_node_to_nary(struct nary *parent ,struct nary *new_node)
{
 	//puts("adding new node");
 	//printf("parent name=%s, new file name=%s",parent->file_desp.file_name,new_node->file_desp.file_name);
    new_node->parent = parent;
	new_node->sibling =  parent->child;
    parent->child = new_node;
    /////////////update file location pointer in fd
    new_node->file_desp.sibling = parent->file_desp.child;
    new_node->file_desp.parent = parent->fd_loc; 
    parent->file_desp.child = new_node->fd_loc;     
  
 
    // printf("\nadd node NAme=%s loc=%ld p=%ld s=%ld c=%ld",new_node->file_desp.file_name,new_node->fd_loc,new_node->file_desp.parent,new_node->file_desp.sibling,new_node->file_desp.child);
    return 1;    
    
}


/*

*/
int delete_subtree_from_nary(struct nary *node)
{

 if(node==NULL)return;   
 
 delete_from_hash(node->file_desp.file_location_full_path,node->file_desp.file_name);
 
 delete_subtree_from_nary(node->child);
 
 delete_subtree_from_nary(node->sibling);
 
 ///cALLS tO dELETE eNTRY fROM bINARY sEARCH tREE
 
    //-----delete_from_bst_path(node->file_desp.file_location_full_path);
 
 ///caLL tO dELETE eNTRY fROM hASHTABLE
 
 
 ///dEALLOC mEMORY fROM rAM
 free(node);
 
}
/*
*/

int delete_node_from_nary()
{


}


/*
*/

int display_nary(struct nary *node,FILE *fout)
{

    if(node==NULL) return;

//    printf("\n%s \t %s",node->file_desp.file_name,node->file_desp.file_location_full_path);
    fprintf(fout,"\n%s \t %s",node->file_desp.file_name,node->file_desp.file_location_full_path);
 
  
    display_nary(node->child,fout);
    
    display_nary(node->sibling,fout);
    

}
int display_nary_c(struct nary *node)
{

    if(node==NULL) return;

//    printf("\n%s \t %s",node->file_desp.file_name,node->file_desp.file_location_full_path);

    if(node->parent!=NULL)
     	printf("\nparent name=%s, new file name=%s",node->parent->file_desp.file_name,node->file_desp.file_name);
    else
        printf("\nparent name=NULL, new file name=%s",node->file_desp.file_name);
  

   printf("\n in dis NAme=%s loc=%ld p=%ld s=%ld c=%ld",node->file_desp.file_name,node->fd_loc,node->file_desp.parent,node->file_desp.sibling,node->file_desp.child); 
    display_nary_c(node->child);
    
    display_nary_c(node->sibling);
    

}

/*
 */
int search_file_in_nary(char input[])
{

}

int write_back_nary(struct nary *node,FILE *fp) 
{
    if(node==NULL) return;
   
    fseek(fp,node->fd_loc,SEEK_SET);
    
    //printf("\n NAme=%s loc=%ld p=%ld s=%ld c=%ld",node->file_desp.file_name,node->fd_loc,node->file_desp.parent,node->file_desp.sibling,node->file_desp.child);
       
    fwrite(&node->file_desp,sizeof(node->file_desp),1,fp);
   
    write_back_nary(node->child,fp);

    write_back_nary(node->sibling,fp);
            
}

int update_path_in_subtree(struct nary *node)
{
    if(node==NULL) return 1;
     
    ///cALL uPDATE bST
    char new_path[MAX_LENGTH_OF_PATH],old_path[MAX_LENGTH_OF_PATH];
    //
    strcpy(old_path,node->file_desp.file_location_full_path);   
    //
    strcpy(new_path,node->parent->file_desp.file_location_full_path);
    if(strcmp(new_path,"/")!=0)
        strcat(new_path,"/");
    strcat(new_path,node->file_desp.file_name);        
    ///////////////// 
    strcpy(node->file_desp.file_location_full_path,new_path); 
    ///////////////update hash
    delete_from_hash(old_path,node->file_desp.file_name);
    insert_in_hashtable(node->file_desp.file_name,new_path);
    //update it from BST
/*    printf("updating path : %s to %s ",old_path,new_path);
    if(update_bst_full_path(old_path,new_path)==0) 
    {
        puts("in update subtree :error");
        return 0;
    }
    display_bst_all_path(BST_FULL_PATH);    
  */
    //recursive call for each node in subtree
     update_path_in_subtree(node->sibling);
     update_path_in_subtree(node->child);
    return 1;
}

int isChild(struct nary *parent, struct nary *child)
{


    if(parent == NULL)
        return 0;
    //puts(parent->file_desp.file_name);    
    
    if(parent == child)    
        return 1;
    
    return (isChild(parent->sibling,child) || isChild(parent->child,child) );    
        
}

/*--------EOF--------*/
