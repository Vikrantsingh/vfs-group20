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

	*a=((int)name[0] % 26);
	if(strlen(name)>1)
    	*b=((int)name[1] % 26);
	else
	    *b=0;
}


void insert_in_hashtable(char file_name[],char file_path[]) 
{
    //puts("in hash");

    //printf("\nGot NAme for insert: %s Path: %s",file_name,file_path);    

    int a,b;
    
    hash(file_name,&a,&b);        
    
    struct hashtable *temp,*cur;
  	temp=(struct hashtable*)malloc(sizeof(struct hashtable));
   	strcpy(temp->name,file_name);
   	strcpy(temp->path,file_path);    	
   
    //insert_into_bst_hash(a, b,temp);    
    temp->next=HASH_TABLE[a][b];
    HASH_TABLE[a][b]=temp;
  //  display_hashrow(a,b);
}
    

void find_in_hashtable(char search_name[],FILE *fout)
{

    int a,b;
    
    hash(search_name,&a,&b);
//    search_bst_hash(HASH_TABLE[a][b],search_name,fout);
    struct hashtable *temp = HASH_TABLE[a][b];       
    while(temp!=NULL) 
    {
        if(strncmp(temp->name,search_name,strlen(search_name))==0)
        {
              fprintf(fout,"\nName: %s Path: %s\n",temp->name,temp->path);
//              printf("\nName: %s Path: %s",temp->name,temp->path);
        }
        temp=temp->next; 
    }
   
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


void display_hashrow(int a,int b)
{

    struct hashtable *pre = NULL, *temp = HASH_TABLE[a][b];       
    puts("\n Hash Row :");
    while(temp!=NULL) 
    {
 
         printf("\nNam: %s Pa: %s",temp->name,temp->path);
         temp=temp->next; 
    }
}

void delete_from_hash(char path[],char name[])
{
    int a,b; 
  //  printf("\nGot NAme for delete: %s Path: %s",name,path);    
    hash(name,&a,&b);
    struct hashtable *pre = NULL, *temp = HASH_TABLE[a][b];       
    while(temp!=NULL) 
    {
        if(strcmp(temp->name,name)==0 && strcmp(temp->path,path)==0)
        {
              break;
        }
        pre = temp;
        temp=temp->next; 
    }
    if(temp == NULL)
    {
        //puts("Not found");
        return;
    }
    else if(pre==NULL)
    {
        HASH_TABLE[a][b] = temp->next;       
    }
    else  
    {         
        pre->next=temp->next;
    }

    free(temp);
//    display_hashrow(a,b);
         
//    delete_from_bst_hash(HASH_TABLE[a][b],path,name);
    
 }



/*--------EOF--------*/
