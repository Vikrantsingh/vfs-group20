/*
	File: bst_full_path.c
	Description: Maintain Binary Search Tree for Chaining in Hashtable
	 
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
  
struct bst* insert_into_bst_path(struct bst *root,struct bst *new_node)
{
    struct bst *parent=NULL;    
    struct bst *temp=root;   
    new_node->parent=NULL;
    new_node->left=NULL;
    new_node->right=NULL; 
    if(root==NULL)
        {
            new_node->parent=NULL;
            return new_node;                     
        }    
   
   
    while(temp!=NULL)
    {
         if(strcmp(temp->nary_node->file_desp.file_location_full_path, 
                    new_node->nary_node->file_desp.file_location_full_path) > 0 )
            {
                parent=temp;
                temp=temp->left;    
            }
         else
            {
                parent=temp;
                temp=temp->right;
            }   
    }
   
   new_node->parent=parent;
   if(strcmp(parent->nary_node->file_desp.file_location_full_path, new_node->nary_node->file_desp.file_location_full_path)>0)
    {
          parent->left=new_node;
          new_node->parent=parent;    
    }
    else
    {
          parent->right=new_node;
          new_node->parent=parent;    
    }   

    return root;
     
}
 
int delete_from_bst_path(char path[])
{ 
    
    struct bst *node = BST_FULL_PATH;
    /////////Find Node to be deleted
    while(node!=NULL)
    {
        if(strcmp(node->nary_node->file_desp.file_location_full_path,path)==0)
        {
            break;    
        }
        else if(strcmp(node->nary_node->file_desp.file_location_full_path,path)>0)
        {   
            node=node->left;
        }
        else
        {
            node=node->right;
        }           
    }
    if(node==NULL)
    {
        puts("in del bst:path not found");
        return 0;
    }
    /////////Check Condition and perform respective delete operations
    /////////Node with no child
    if(node->left==NULL && node->right==NULL)
    {
        if(node->parent->left==node)
            node->parent->left=NULL;
        else
            node->parent->right=NULL;
    }
    
    ////////Node with one child
    else if(node->left==NULL)
    {
        if(node->parent->left==node)
            node->parent->left=node->right;
        else
            node->parent->right=node->right;
    }
    else if(node->right==NULL)
    {
        if(node->parent->left==node)
            node->parent->left=node->left;
        else
            node->parent->right=node->left;
    }
    
    
    ////////Node with Two Child
    else
    {
        //now find inorder successor for replacement
        struct bst *suc = node->right;
        while(suc->left!=NULL)
        {
            suc=suc->left;
        }
        //update parent of inorder successor
        if(suc->parent->left==suc)
            suc->parent->left=suc->right;
        else
            suc->parent->right=suc->right;             
        
        //final touch....
        if(node->parent->left==node)
            node->parent->left=suc;
        else
            node->parent->right=suc;

        suc->left=node->left;
        suc->right=node->right    ;
            
    }
    
    free(node);
  
}

struct bst* search_bst_path(struct bst *b,char path[])
{
    if(b==NULL)
        return NULL;
        
    if(strcmp(b->nary_node->file_desp.file_location_full_path,path)==0)
        return b;
        
    else if(strcmp(b->nary_node->file_desp.file_location_full_path,path)>0)
        return  search_bst_path(b->left,path);        
               
    else
        return  search_bst_path(b->right,path);       
                    
}

int display_bst_all_path(struct bst *b)
{
    if(b==NULL) return;
    
    puts(b->nary_node->file_desp.file_location_full_path); 
    
    display_bst_all_path(b->left);
    
    display_bst_all_path(b->right);
    
}
int update_bst_full_path(char oldpath[],char newpath[]) 
{ 
    ///struct bst *node = BST_FULL_PATH;        
    struct bst *old = (struct bst*)search_bst_path(BST_FULL_PATH,oldpath);
    if(old==NULL){puts("in update bst: old path not found"); return 0;}
    /////////Find Node to be deleted
    struct nary  *node = old->nary_node;
    delete_from_bst_path(oldpath);
    //debug
    printf("\ndeleted %s\n",oldpath);
    display_bst_all_path(BST_FULL_PATH);
    int oo;
    scanf(" enter %d",&oo);
    ///////////
    struct bst  *new = (struct bst*)malloc (sizeof(struct bst));
	if(new==NULL){puts("Space not avaliable for creating temp bst node"); return 0;}
	else
	  {     
	      strcpy(node->file_desp.file_location_full_path,newpath);  
	      new->nary_node=node;
	      BST_FULL_PATH=insert_into_bst_path(BST_FULL_PATH,new);  
	  }       
    //debug
    printf("\nadded %s\n",newpath);
    display_bst_all_path(BST_FULL_PATH);
    // oo;
    scanf(" enter %d",&oo);
   return 1;

}
int reset_bst_path()
{
    delete_all_bst_path(BST_FULL_PATH);
    BST_FULL_PATH=NULL;
    build_bst(NARY_ROOT);    
}
int delete_all_bst_path(struct bst *node)
{
    if(node==NULL) return;
    delete_all_bst_path(node->left);
    delete_all_bst_path(node->right);
    free(node);
}
int build_bst(struct nary* node)
{
    if(node==NULL)return;
    
    struct bst  *b = (struct bst*)malloc (sizeof(struct bst));
	if(b==NULL){puts("Space not avaliable for creating bst");}
	else
	{    
	    //b->FD=temp;  
	    //b->fd_loc=add;
	    b->nary_node=node;
	    BST_FULL_PATH=insert_into_bst_path(BST_FULL_PATH,b);  
	}    	         
    build_bst(node->sibling);
    build_bst(node->child);    

}
/*--------EOF--------*/
