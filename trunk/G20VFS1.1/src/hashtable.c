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

void hash(char name[],int *a,int *b){

	*a=((int)name[0] -97);
	*b=((int)name[1] -97);
}


void insert_in_hashtable(char file_name[],char file_path[]) 
{


    int a,b;
    
    hash(file_name,&a,&b);
    
    printf("\nGot NAme: %s Path: %s",file_name,file_path);    

    struct hashtable *temp,*cur;
    if(HASH_TABLE[a][b]==NULL)
    {
    	temp=(struct hashtable*)malloc(sizeof(struct hashtable));
    	strcpy(temp->name,file_name);
    	strcpy(temp->path,file_path);    	
    	temp->nextnode=NULL;
    	HASH_TABLE[a][b]=temp;

    }
    else
    {
    	cur=HASH_TABLE[a][b];
    	while(cur->nextnode!=NULL&&strcmp(cur->name,file_name)!=0)
    	{
    		cur=cur->nextnode;
    	}
    	if(cur->nextnode==NULL)
    	{
    		temp=(struct hashtable*)malloc(sizeof(struct hashtable));
    		strcpy(temp->name,file_name);
    		strcpy(temp->path,file_path); 
    		temp->nextnode=NULL;
    		cur->nextnode=temp;
    	}
    }
}
    

void find_in_hashtable(char search_name[])
{

    int a,b;
    
    hash(search_name,&a,&b);

    struct hashtable *cur;
    if(HASH_TABLE[a][b]==NULL)
        printf("no file\n");
    else
    {
        cur=HASH_TABLE[a][b];
        int flag=0;
        while(cur!=NULL)
        {
        
	       if(strcmp(cur->name,search_name)==0)
	        {
	            flag=1;
	            printf("\n Name: %s Path: %s",cur->name,cur->path);
	        }

	       cur=cur->nextnode;
        }       
        if(flag==0)
        {
            puts("File not found");
        }
    }
     
}


void display_hashtable()
{
puts("display hash");
struct hashtable *cur;
int i,j;
     for(i=0;i<26;i++)
     {
       for(j=0;j<26;j++)
       {
         printf(" %C%C :",i+97,j+97);            
         cur=HASH_TABLE[i][j];
         while(cur!=NULL)
         {
        
	        printf(" Name: %s Path: %s",cur->name,cur->path);
	        cur=cur->nextnode;
	        
         }
        }
        
      }
	   

}



/*--------EOF--------*/
