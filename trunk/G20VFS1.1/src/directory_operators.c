/*

	File: directory_operators.c
	Description:Functions for Managing Directory-Subdirectory and their content

*/


#include "../include/structure.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/freelist.h"
#include "../include/string_operations.h"
#include "../include/hashtable.h"


/*

for #mkdir a/b/c/d demo
This function will go to a->b->c->d and return address of immediate parent 'd'

*/
long int  get_Parent_File_Descriptor_Location(char **file_path,int level, struct file_descriptor *parent)
{
	int i;
	FILE *fp = VFS_MOUNT_POINT;
	struct file_descriptor temp,temp2;
	
	if(level==0) //////Empty path level
	{       return -1;    	}

    char dir_struct[50][50];
	//puts("NOw in GET PARENT");
    for(i=0;i<level;i++)	///just keeping backup
    {
        //printf("i=%d, dir = %s\n",i,file_path[i]);
        strcpy(dir_struct[i],file_path[i]);
  
    }
	 
	fseek(fp,METADATA.File_descriptor_array,SEEK_SET);//jump to root discriptor
 
 
	puts("Searching file");
	
    for(i=0;i<level;i++)
     {
     
    //   printf("i=%d, dir = %s\n",i,dir_struct[i]);
	
	    int found =0;	
	 
	    fread(&temp,sizeof(temp),1,fp);
	    //printf("Current location: %ld",ftell(fp));
   	    	 
	    if(strcmp(temp.file_name,dir_struct[i])==0)
       	 	{
         	   *parent = temp;
         	   //puts(temp.file_name);
         	   found =1;
       	 	}
        
	    long int siblings=temp.child;
	    
	    while(siblings!=-1 && i<level-1 )
	    {
	        fseek(fp,siblings,SEEK_SET);
	        
	        fread(&temp2,sizeof(temp2),1,fp);
	        
	       if(strcmp(temp2.file_name,file_path[i+1])==0 && strcmp(temp2.file_type,"dir")==0 )
               {
                   *parent=temp2;
                    found = 1;
                   // puts(temp2.file_name);
                    break;
               }     
            else
               {
                    found = 0;
                    siblings=temp2.sibling;
               }     
                
	    }
	  
	    if(found==0) 
	    {
	        puts("Dir not found.");
	        return 0;
	    }
	    else
	    {
	        fseek(fp,-sizeof(temp),SEEK_CUR);
	    }
	
	}
	
	return (ftell(fp));//return starting address of immediate parent

return 0;
}


int list_dir_structure(long int add)

{

	//int i;
	FILE *fp = VFS_MOUNT_POINT;
	struct file_descriptor temp;
    //fseek(fp,METADATA.File_descriptor_array,SEEK_SET);//jump to root discriptor
    
    if(add==-1) return 1;
    
    fseek(fp,add,SEEK_SET);
    
    fread(&temp,sizeof(temp),1,fp);
    
   // puts(temp.file_name);

   if(strcmp(temp.file_type,"dir")==0)
    {
    	printf("%c[%d;%dm %s/ %c[%dm\n",27,1,33,temp.file_name,27,0);
    }
    else
    {
    	printf("%s\n",temp.file_name);
    }	
   
    
  
    //printf("\nFor %s @(%ld) Parent = %ld , sibling = %ld:",temp.file_name,add,temp.parent,temp.sibling);
    printf("\nFor %s  , sibling = ",temp.file_name);

    list_dir_structure(temp.sibling);
    
    if(strcmp(temp.file_type,"dir")==0)
    {
    	//printf("%c[%d;%dm %s %c[%dm\n",27,1,33,temp.file_name,27,0);
    	
    	//printf("\nFor %s @(%ld) Parent = %ld , child = %ld:",temp.file_name,add,temp.parent,temp.child);
    	printf("\nFor %s  , child = ",temp.file_name);

    	list_dir_structure(temp.child);
    }
    return 1;


}

/*

    separate path with / ,and return it via call by reference way

*/
int separate_path(char** dir_struct,char parent_path[],int *total_dir_in_path)
{
	if(strlen(parent_path)>=1000)
	{
	  puts("Path too long.");
	  return 0;
	}
	int i=0;
	//char *dir_struct[25];
	char path[1000];
	strcpy(path,parent_path);

	dir_struct[i]=strtok(path, "/");	
 //	puts(dir_struct[i]);

	while(dir_struct[i]!=NULL && i<25)  //limiting max sub directory level to 25
	{
	    i++;
	    dir_struct[i]=strtok(NULL, "/"); //
    //     if(dir_struct[i]!=NULL)    {    	//puts(dir_struct[i]);         }

	}
	*total_dir_in_path=i;
//	printf("total=%d",*total_dir_in_path);
	return 1;
        
}


/* To create a new directory whose name should be
<dir_name> in path specified by <parent_path> where '/' is considered as 'root'
directory.
*/


int makedir(char parent_path[],char dir_name[])
{
 
	char *dir_struct[25];//limiting max sub directory level to 25  
	int i,total_dir_in_path;
	
	//tokenizing path
	i = separate_path(dir_struct,parent_path,&total_dir_in_path);
	
	
	
	if(i==0)
	{
   	     puts("Unable to process path");
	     return 0;
	}


	//for(i=0;i<total_dir_in_path;i++)
	  //puts(dir_struct[i]);

	//check for space
	if(METADATA.no_of_file_desp_used>=METADATA.max_no_of_file_desp_available-1)
	{
		puts("\nSpace not Available");
		return 0;
	}

    //resolve path and find immediate parent
    struct file_descriptor parent_data;
    long int parent = get_Parent_File_Descriptor_Location(dir_struct,total_dir_in_path,&parent_data);
    
    
    if(parent==0)
    {
        puts("path not found");
        return 0;
    }
    //puts(parent_data.file_name);
    

// 	To Do
	

/*
	  Validate Name for already existence
*/
//	If Not Exist  then proceed



//	Create & Store FD 
	struct file_descriptor file_desp,temp;
	//Storing File Desp data in Struct variable
	strcpy(file_desp.file_name,dir_name);
		//////////////Store Path
	strcpy(file_desp.file_location_full_path,parent_path);
	file_desp.isfull='1';//0-empty 1-full
	file_desp.location_block_number=-1;
	file_desp.FID= METADATA.counter + 1;
	file_desp.sibling=parent_data.child;
	file_desp.child=-1;
	file_desp.parent=parent;
	strcpy(file_desp.file_type,"dir");
	FILE *fp = VFS_MOUNT_POINT;
	fseek(fp,METADATA.File_descriptor_array,SEEK_SET);
	
	long int location_of_new_FD=-1;
	//search free space in header to add file desp..
	for(i=0;i<METADATA.max_no_of_file_desp_available;i++)
	{
		fread(&temp,sizeof(temp),1,fp);
		if(temp.isfull=='0')
		{//free space found, add file desp now @ this location.
			fseek(fp,-sizeof(temp),SEEK_CUR);
			location_of_new_FD = ftell(fp);
			fwrite(&file_desp,sizeof(file_desp),1,fp);
			
			//also add link to parent
			
			parent_data.child=ftell(fp)-sizeof(temp);
			fseek(fp,parent,SEEK_SET);
			fwrite(&parent_data,sizeof(parent_data),1,fp);
			
			break;
		}
	}
	if(i<METADATA.max_no_of_file_desp_available)//if file desp added
	{
		++METADATA.counter; //Increment ID Counter Variable
		++METADATA.no_of_file_desp_used;//iNCREMENT uSED fILE dESCRIPTOR cOUNTER
		printf("\nFile Descriptor Created for New directory...");


		///////////UPDATE HASH TABLE/////////////////////////////
		insert_in_hashtable(file_desp.file_name,file_desp.file_location_full_path);

	

	}
	else
	{
	    	printf("\nError in creating File Descriptor for New directory...");
	    	return 0;
	}
	
	
	
	
	return 1;
}




/*
To remove a file or a directory as indicated by < name>
*/

int deletedir(char name[])
{
	puts("In deletedir");
	return 1;
}



/*
To move a sub-tree
<source_dir_name> to <dest_dir_name> directory.
*/

int movedir(char src_dir_name[],char dest_dir_name[])
{
	puts("In movedir");
 return 1;
}



/*
List all the contents according to <flag> of the current directory
i.e. specified by <dir_name>.
*/

int listdir(char dir_path[],char flag[] )
{
	char *dir_struct[25];//limiting max sub directory level to 25  
	int i,total_dir_in_path;
	
	//puts("In listdir");
	//tokenizing path
	i = separate_path(dir_struct,dir_path,&total_dir_in_path);
	
	if(i==0)
	{
   	     puts("Unable to process path");
	     return 0;
	}
	//puts("Again in listdir");
    //for(i=0;i<total_dir_in_path;i++)	
    //{
    //    puts(dir_struct[i]);
    //}
	
  //resolve path and find immediate parent
    struct file_descriptor parent_data;
    
    long int parent = get_Parent_File_Descriptor_Location(dir_struct,total_dir_in_path,&parent_data);
    
    
    if(parent==0)
    {
        puts("path not found");
        return 0;
    }
  
	return list_dir_structure(parent_data.child);
 //return 1;
}



