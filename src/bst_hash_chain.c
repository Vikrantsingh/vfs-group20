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
         
   // puts(path);
   // puts("");
    struct hashtable *node = h;
    if(node==NULL) 
    {
        //puts("Hash Location Empty");
        return 0;
    }    
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
    puts(node->path);
    /////////Check Condition and perform respective delete operations
    if(node==NULL) 
    {
        //puts("Not Found");
        return 0;
    }    
    /////////Node with no child
    if(node->lnextnode==NULL && node->rnextnode==NULL)
    {

        if(node==h)//root node
        {
            free(node);
            int a,b;     
            hash(name,&a,&b);
            HASH_TABLE[a][b]=NULL;
            return 1 ;
        }

        if(node->parent->lnextnode==node )
            node->parent->lnextnode=NULL;
        else
            node->parent->rnextnode=NULL;
    }
    
    ////////Node with one child
    else if(node->lnextnode==NULL)
    {
        if(node==h)//root node
        {
            int a,b;     
            hash(name,&a,&b);
            HASH_TABLE[a][b]=node->rnextnode;
            free(node);
            return 1 ;
        }

        if(node->parent->lnextnode==node)
            node->parent->lnextnode=node->rnextnode;
        else
            node->parent->rnextnode=node->rnextnode;
    }
    else if(node->rnextnode==NULL)
    {
        if(node==h)//root node
        {
            int a,b;     
            hash(name,&a,&b);
            HASH_TABLE[a][b]=node->rnextnode;
            free(node);
            return 1;
        }
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


        suc->lnextnode=node->lnextnode;
        suc->rnextnode=node->rnextnode;
        
        if(node==h)//root node
        {
            int a,b;     
            hash(name,&a,&b);
            HASH_TABLE[a][b]=suc;
            free(node);
            return 1;
        }

        //final touch....
        if(node->parent->lnextnode==node)
            node->parent->lnextnode=suc;
        else
            node->parent->rnextnode=suc;

            
    }
    
    free(node);
    return 1;


}

int search_bst_hash(struct hashtable *h,char name[],FILE *fout)
{
   if (h==NULL) return;     
   if(strncmp(h->name,name,strlen(name))==0)
   {
        //puts(h->name);
        //printf("\n Name: %s Path: %s",h->name,h->path);
        fprintf(fout,"\nName: %s Path: %s\n",h->name,h->path);
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
