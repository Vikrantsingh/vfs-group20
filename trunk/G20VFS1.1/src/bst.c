/*
	File: bst.h
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

int insert_into_bst(int a,int b, struct hashtable *h)
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
            }
            else
            {
                parent->rnextnode=h;
            }
        
    }

}



int delete_into_bst(char name[]);

int search_bst(struct hashtable *h,char name[])
{
   if (h==NULL) return;     
   if(strcmp(h->name,name)==0)
   {
        puts(h->name);
        printf("\n Name: %s Path: %s",h->name,h->path);
        //return h;
   }
   search_bst(h->lnextnode,name);
   search_bst(h->rnextnode,name);
   

}

int display_bst_all(struct hashtable *h)
{
   if (h==NULL) return;     
   printf("\n Name: %s Path: %s",h->name,h->path);
   display_bst_all(h->lnextnode);
   display_bst_all(h->rnextnode);
}

/*--------------EOF------------------*/
