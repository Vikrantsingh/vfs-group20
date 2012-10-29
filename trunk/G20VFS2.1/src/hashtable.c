/*
	File: hashtable.c
	Description: Maintain HashTable for File Descriptor
	 
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
#include "../include/bst_hash_chain.h"

void hash(char name[],int *a,int *b){

	*a=((int)name[0] -97);
	if(strlen(name)>1)
    	*b=((int)name[1] -97);
	else
	    *b=0;
}


void insert_in_hashtable(char file_name[],char file_path[]) 
{
    //puts("in hash");

    //printf("\nGot NAme: %s Path: %s\n\n",file_name,file_path);    

    int a,b;
    
    hash(file_name,&a,&b);        
    
    struct hashtable *temp,*cur;
  	temp=(struct hashtable*)malloc(sizeof(struct hashtable));
   	strcpy(temp->name,file_name);
   	strcpy(temp->path,file_path);    	
   
    insert_into_bst_hash(a, b,temp);    
}
    

void find_in_hashtable(char search_name[],FILE *fout)
{

    int a,b;
    
    hash(search_name,&a,&b);
    search_bst_hash(HASH_TABLE[a][b],search_name,fout);        
   
}


void display_hashtable()
{
    //puts("display hash");
    struct hashtable *cur; 
    int i,j;
     for(i=0;i<26;i++)
     {
       for(j=0;j<26;j++)
       {
         printf(" %C%C :",i+97,j+97);            
         cur=HASH_TABLE[i][j];
         display_bst_all_hash(cur); 
        }
        
      }	   
}

void delete_from_hash(char path[],char name[])
{
    int a,b; 
    
    hash(name,&a,&b);

    delete_from_bst_hash(HASH_TABLE[a][b],path,name);         
    
}



/*--------EOF--------*/
