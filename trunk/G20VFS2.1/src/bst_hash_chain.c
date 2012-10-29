/*
	File: bst_hash_chain.c
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

int insert_into_bst_hash(int a,int b, struct hashtable *h)
{
    if(HASH_TABLE[a][b]==NULL)
    {
        HASH_TABLE[a][b]=h;
    }
    else
    {
        struct hashtable *parent,*temp=HASH_TABLE[a][b];
        parent=NULL;
        while(temp!=NULL)
        {                
            parent=temp;
            
            if(strcmp(temp->name,h->name)>0)
            {
                temp=temp->lnextnode;
            }
            else
            {
                temp=temp->rnextnode;
            }
        
        }
        if(strcmp(parent->name,h->name)>0)
            {
                parent->lnextnode=h;
                h->parent=parent;
            }
            else
            {
                parent->rnextnode=h;
                h->parent=parent;
            }
        
    }

}
 


int delete_from_bst_hash(struct hashtable *h,char path[],char name[])
{ 
         
    struct hashtable *node = h;
    /////////Find Node to be deleted
    while(node!=NULL)
    {
        if(strcmp(node->name,name)==0 && strcmp(node->path,path)==0)
        {
            break;    
        }
        else if(strcmp(node->name,name)>0)
        {   
            node=node->lnextnode;
        }
        else
        {
            node=node->rnextnode;
        }           
    }
    /////////Check Condition and perform respective delete operations
    /////////Node with no child
    if(node->lnextnode==NULL && node->rnextnode==NULL)
    {
        if(node->parent->lnextnode==node)
            node->parent->lnextnode=NULL;
        else
            node->parent->rnextnode=NULL;
    }
    
    ////////Node with one child
    else if(node->lnextnode==NULL)
    {
        if(node->parent->lnextnode==node)
            node->parent->lnextnode=node->rnextnode;
        else
            node->parent->rnextnode=node->rnextnode;
    }
    else if(node->rnextnode==NULL)
    {
        if(node->parent->lnextnode==node)
            node->parent->lnextnode=node->lnextnode;
        else
            node->parent->rnextnode=node->lnextnode;
    }
    
    
    ////////Node with Two Child
    else
    {
        //now find inorder successor for replacement
        struct hashtable *suc = node->rnextnode;
        while(suc->lnextnode!=NULL)
        {
            suc=suc->lnextnode;
        }
        //update parent of inorder successor
        if(suc->parent->lnextnode==suc)
            suc->parent->lnextnode=suc->rnextnode;
        else
            suc->parent->rnextnode=suc->rnextnode;             
        
        //final touch....
        if(node->parent->lnextnode==node)
            node->parent->lnextnode=suc;
        else
            node->parent->rnextnode=suc;

        suc->lnextnode=node->lnextnode;
        suc->rnextnode=node->rnextnode;
            
    }
    
    free(node);


}

int search_bst_hash(struct hashtable *h,char name[],FILE *fout)
{
   if (h==NULL) return;     
   if(strcmp(h->name,name)==0)
   {
        //puts(h->name);
        //printf("\n Name: %s Path: %s",h->name,h->path);
        fprintf(fout,"Name: %s Path: %s\n",h->name,h->path);
        //return h;
   }
   search_bst_hash(h->lnextnode,name,fout);
   search_bst_hash(h->rnextnode,name,fout);
   
}

int display_bst_all_hash(struct hashtable *h)
{
   if (h==NULL) return;     
   printf("\n Name: %s Path: %s",h->name,h->path);
   display_bst_all_hash(h->lnextnode);
   display_bst_all_hash(h->rnextnode);
}

/*--------------EOF------------------*/
