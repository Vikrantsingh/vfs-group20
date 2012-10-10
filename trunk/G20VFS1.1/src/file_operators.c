/*
	File: file_operators.h
	Description: Function for Manipulating Files and their data in VFS.
	 
*/


#include "../include/structure.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/freelist.h"
#include "../include/string_operations.h"
#include "../include/hashtable.h"
#include "../include/directory_operators.h"

/*
	Add data to blocks.
*/
long int add_data_blocks(char data[],long int start)
{
	long int temp=start;
	long int location = get_new_node();
	struct block b;
	b.next=-1;
	b.isfull = '1';
	strcpy(b.data,data);

	if(location==-1)
	{
		printf("Free Space Not Available");
		return -1;
		  
		
	}
	else
	{

		
		fseek(VFS_MOUNT_POINT,location,SEEK_SET);//-1
		if(fwrite(&b,sizeof(b),1,VFS_MOUNT_POINT)==1)
		{
			//printf("data block added");		
		}
		 
	}

	if (start==-1)
	{
		return location;
	}
	else
	{


		while(1)
		{
			fseek(VFS_MOUNT_POINT,temp,SEEK_SET);	//-1
			if(fread(&b,sizeof(b),1,VFS_MOUNT_POINT)==1)
			{
				if(b.next==-1)
				{	
					break;
				}
				else
				{
					temp=b.next;
				}
			}
		}
		fseek(VFS_MOUNT_POINT,-(sizeof(b)),SEEK_CUR);///-1
		b.next=location;
		if(fwrite(&b,sizeof(b),1,VFS_MOUNT_POINT)==1)
		{}
		return start;				
	}
	
}


/*
	Read data from blocks.
*/
struct block read_data_blocks(long int location)
{
	struct block b;
	if(location==-1)
	{
		printf("Data Block Empty");
		return ;
	}
	else
	{

		
		fseek(VFS_MOUNT_POINT,location,SEEK_SET);//-1
		if(fread(&b,sizeof(b),1,VFS_MOUNT_POINT)==1)
		{
			//printf("reading data blocks...");		
			return b;
		}
		 
	}
}

/*
	Display data to Screen.
*/
int display_data_blocks(struct block b)
{

	if(b.isfull=='0')
	{
		printf("Data Block Empty");
		return -1;
	}
	else
	{

		//puts("Data:");
		printf("%s",b.data);
		 
	}
}


/*
Create a new file named <file_name> with
<data> as content. in path specified by <dest_dir_name>.
*/


int create(char dest_dir[],char file_name[],char data[])
{
	if(METADATA.no_of_file_desp_used>=METADATA.max_no_of_file_desp_available-1)
	{
		puts("\nSpace not Available");
		return 0;
	}


	char *dir_struct[25];//limiting max sub directory level to 25  
	int i,total_dir_in_path;
	
	//tokenizing path
	i = separate_path(dir_struct,dest_dir,&total_dir_in_path);
	 
 	if(i==0)
	{
   	     puts("Unable to process path");
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
	
	
	
	struct file_descriptor file_desp,temp;
	//Storing File Desp data in Struct variable
	strcpy(file_desp.file_name,file_name);
	//////////////Store Path
	strcpy(file_desp.file_location_full_path,dest_dir);
	file_desp.isfull='1';//0-empty 1-full
	file_desp.location_block_number=-1;
	file_desp.FID= METADATA.counter + 1;
	file_desp.sibling=parent_data.child;
	file_desp.child=-1;
	file_desp.parent=parent;///back link to immediate parent dir
	file_desp.file_size=strlen(data);
	strcpy(file_desp.file_type,"file"); /////////edit this: extract file type from file name
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
		printf("\nFile Descriptor Created...");
		////now add data blocks
		struct block b;
		int max_data_in_one_block=sizeof(b.data)-1;
		char *ch =(char *)malloc(sizeof(char)*max_data_in_one_block);
		long int start=0;
		long int j= 0;
		
		
		//Breaking Large data into small chunks , so as to fit into data blocks
		while(j<strlen(data))
		{

			start =j;
/*			if((j+max_data_in_one_block)<strlen(data))
				j = j + max_data_in_one_block; 
			else
*/			j = j + max_data_in_one_block; 
			strcpy(ch,sub_string(data,start,max_data_in_one_block));
			//puts("calling add data");
			//puts(ch);
			//printf("\npaasing = %ld",file_desp.location_block_number);
			file_desp.location_block_number=add_data_blocks(ch, file_desp.location_block_number);
			//printf("\nreturn = %ld",file_desp.location_block_number);
			//puts("\nblocks added");	
		}		
		//return 1;

		//Update File Descriptor with starting block address of data
			fseek(fp,location_of_new_FD,SEEK_SET);
			//location_of_new_FD = ftell(fp);
			fwrite(&file_desp,sizeof(file_desp),1,fp);
		
		///////////UPDATE HASH TABLE/////////////////////////////
		insert_in_hashtable(file_desp.file_name,file_desp.file_location_full_path);

		
		
	}
	else //no free space found to add fd
	{
		printf("\nError in File Creating  Descriptor...");
		//return 0;
	}



	return 1;
}


/*
	Search file and return FD location
*/
long int  search_File(char file_path[])
{
	int i;
	FILE *fp = VFS_MOUNT_POINT;
	struct file_descriptor temp;

	fseek(fp,METADATA.File_descriptor_array,SEEK_SET);

	puts("Searching file");
	
		
	////////////////////////
	char *dir_struct[25];//limiting max sub directory level to 25  
	int total_dir_in_path;
	
	//puts("In listdir");
	//tokenizing path
	i = separate_path(dir_struct,file_path,&total_dir_in_path);
	
	if(i==0)
	{
   	     puts("Unable to process path");
	     return -1;
	}
  //resolve path and find immediate parent

//  	     puts("break");  return -1;

    struct file_descriptor parent_data;
    long int parent = get_Parent_File_Descriptor_Location(dir_struct,total_dir_in_path-1,&parent_data);
    
    
    if(parent==0||parent==-1)
    {
        puts("path not found");
        return -1;
    }
  
	
	long int add=parent_data.child;
	
	while(add!=-1)
	{
	    fseek(fp,add,SEEK_SET);
    
        fread(&temp,sizeof(temp),1,fp);
    
       // puts(temp.file_name);

       if(strcmp(temp.file_name,dir_struct[total_dir_in_path-1])==0)
        {
        	return (ftell(fp)-sizeof(temp));
        }
        add=temp.sibling;        	
	}
	
    return -1;	
	
	/*
	for(i=0;i<METADATA.max_no_of_file_desp_available;i++)
	{
		fread(&temp,sizeof(temp),1,fp);
		if(temp.isfull=='1')
		{
			//if(strcmp(temp.file_name, file_path )==0)			
			if(strcmp(temp.file_name, file_name)==0)			
			{
				puts("file found");			
				break;

			}
			//printf("\nName : %s, FILE ID : %ld",temp.file_name,temp.FID);
		}
	}

	if(i==METADATA.max_no_of_file_desp_available)
	{
		puts("File Not Found");
		return -1;
	}
	
	return (ftell(fp)-sizeof(temp));
	*/

}


/*
View the contents of a file specified by <file_path>.
*/
int listfile(char file_path[],char file_name[]) 
{
	FILE *fp = VFS_MOUNT_POINT;
	struct file_descriptor temp;
	long int loc;
 	int i;
 	//puts("Searching file");
	//loc=search_File(file_name);
	puts(file_path);
	///////////////////////
	char *dir_struct[25];//limiting max sub directory level to 25  
	int total_dir_in_path;

	//tokenizing path
	i = separate_path(dir_struct,file_path,&total_dir_in_path);
	
	 
	if(i==0)
	{
   	     puts("Unable to process path");
	     return -1;
	}
  //resolve path and find immediate parent
    struct file_descriptor parent_data;

    long int parent = get_Parent_File_Descriptor_Location(dir_struct,total_dir_in_path,&parent_data);

    if(parent==0||parent==-1)
    {
        puts("path not found");
        return -1;
    }
 	
    loc = parent_data.child;
    temp = parent_data;
   //Now search for file in that directory
   while(loc!=-1)
   {
    fseek(fp,loc,SEEK_SET);//Jump to FD
	fread(&temp,sizeof(temp),1,fp);//Read FD
    if(strcmp(temp.file_name,file_name)==0)
    {
        break;
    }
    else
    {
        loc=temp.sibling;
    }
   }
   if(loc==-1)
   {
        puts("File not Found");
        return -1;
   }
   
   puts("File Found");
   
    	
	//////////////Logic to display data.......................
	//if(loc==-1) {puts("File Not Found!"); return 0;}
	//fseek(fp,loc,SEEK_SET);//Jump to FD
	//fread(&temp,sizeof(temp),1,fp);//Read FD
	
	////////////check for valid file
	
	/*if(strcmp(temp.file_type,"file")!=0)
	{
	    puts("Cannot Display");
	    return 0;
	}
	*/
	loc=temp.location_block_number; //starting data block location
	if(loc==-1) {puts("File Empty");}
	while(loc!=-1)
	{	
		struct block t = read_data_blocks(loc); //read data from file block by block
		display_data_blocks(t);//Print data on console
		loc=t.next;
		//printf("start %ld, loc=%ld",temp.location_block_number,loc);
		
	}
	

 return 1;
}

/*
list all files in current directory.
*/
int list(char file_path[])
{
	FILE *fp = VFS_MOUNT_POINT;
	struct file_descriptor temp;
	int i;
	puts("\nFile List :");
	
	
	////////////////////////
	char *dir_struct[25];//limiting max sub directory level to 25  
	int total_dir_in_path;
	
	//puts("In listdir");
	//tokenizing path
	i = separate_path(dir_struct,file_path,&total_dir_in_path);
	
	if(i==0)
	{
   	     puts("Unable to process path");
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
  
	
	long int add=parent_data.child;
	
	while(add!=-1)
	{
	    fseek(fp,add,SEEK_SET);
    
        fread(&temp,sizeof(temp),1,fp);
    
       // puts(temp.file_name);

       if(strcmp(temp.file_type,"dir")==0)
        {
        	printf("%c[%d;%dm %s/ %c[%dm\n",27,1,33,temp.file_name,27,0);
        }
        else
        {
        	printf(" %s\n",temp.file_name);
        }	
   
        add=temp.sibling;    
    	
	}
	
	/*
	
	//////// List All --old logic
	
	for(i=0;i<METADATA.max_no_of_file_desp_available;i++)
	{
		fread(&temp,sizeof(temp),1,fp);
		if(temp.isfull=='1')
		{
			
			//printf("\nName : %s, FILE ID : %ld",temp.file_name,temp.FID);
			if(strcmp(temp.file_type,"dir")==0)
    			printf("\n%c[%d;%dm %s %c[%dm",27,1,33,temp.file_name,27,0);//to print colored text
            	//printf("\n%c[%d;%dm %s %c[%dm FID : %ld",27,1,33,temp.file_name,27,0,temp.FID);
    			//printf("\n %s, FID : %ld",temp.file_name,temp.FID);
    		else
    			printf("\n %s ",temp.file_name);
		}
	}
	*/
	

 return 1;
}


/*
Update/Edit the contents of a file specified by <file_path>
with <data> according to <flag>.
*/
int update(char file_path[],char data[])
{
 return 1;
}


/*
Remove a file specified by <file_path>.
*/
int rm(char file_path[])
{
	FILE *fp = VFS_MOUNT_POINT;
	struct file_descriptor temp,temp2;
	long int loc;
 	int i;
 	//puts("Searching file");
	loc=search_File(file_path);
	//////////////Logic to delete file.......................
	if(loc==-1) {puts("File Not Found!"); return 0;}

	fseek(fp,loc,SEEK_SET);//Jump to FD
	fread(&temp,sizeof(temp),1,fp);//Read FD

	
	////////////check for valid file
	
	if(strcmp(temp.file_type,"file")!=0)
	{
	    puts("Cannot DELETE");
	    return 0;
	}
	

	temp.isfull='0';//reseting flag
	fseek(fp,loc,SEEK_SET);//Jump to FD
	fwrite(&temp,sizeof(temp),1,fp);//reset/clear FD
	

	METADATA.no_of_file_desp_used--;//uPDATE mETADATA


    ///Update parent and siblings
    
    long int add= temp.parent;
    if(add!=-1)    
    {
          fseek(fp,add,SEEK_SET);
          
	      fread(&temp2,sizeof(temp2),1,fp);//Read FD
	     
	      if(temp2.child==loc)
	      {
	          temp2.child=temp.sibling;
              fseek(fp,-sizeof(temp2),SEEK_CUR);
	          fwrite(&temp2,sizeof(temp2),1,fp);//write FD

	      }
	 
	      else
	      {
	      
	        add=temp2.child;
            while(add!=-1)
            {
        
                fseek(fp,add,SEEK_SET);//Jump to FD
        	    fread(&temp2,sizeof(temp2),1,fp);//Read FD
        	    if(temp.sibling==loc)
        	    {
        	           fseek(fp,-sizeof(temp2),SEEK_CUR);
        	           temp2.sibling=temp.sibling;
        	           fwrite(&temp2,sizeof(temp2),1,fp);//write FD
	        
        	    }
                add=temp.sibling;
            }
    
	      
	      }   
	
	}  
    
    

	///Now Removing blocks
	
	loc=temp.location_block_number; //starting data block location
	if(loc==-1) {puts("File Empty");}
	while(loc!=-1)
	{	
		struct block t = read_data_blocks(loc); //scanning data block one by one
		printf("\ndelete node:%ld",loc);
		delete_node(loc);//delete node and add it to freelist
		loc=t.next;
		//printf("start %ld, loc=%ld",temp.location_block_number,loc);
		
	}

 return 1;
}
/*

Search file in hash table
*/
int find_file(char name[])
{
    //int a,b;
    //hash(name[0],name[1],&a,&b);
    find_in_hashtable(name);
    //puts("File found");
    return 1;
}
/*
Rename <source_file_path> to
<dest_file_path> file.
*/
int mv(char src[],char dest[])
{
 return 1;
}


/*
To copy source directory/file to
destination specified.
*/
int copy(char src[],char dest[])
{
 return 1;
}


/*
Export the file from the file system into the hard disk
*/
int Export(char file_path[])
{
 return 1;
}

/*--------EOF------*/
