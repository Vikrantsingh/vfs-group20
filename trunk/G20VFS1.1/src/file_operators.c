/*
	File: file_operators.h
	Description: Function for Manipulating Files and their data in VFS.
	 
*/


#include "../include/structure.h"
#include <stdio.h>
#include <string.h>
#include "../include/freelist.h"
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

		
		fseek(VFS_MOUNT_POINT,location-1,SEEK_SET);
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
			fseek(VFS_MOUNT_POINT,temp-1,SEEK_SET);
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
		fseek(VFS_MOUNT_POINT,-(sizeof(b)-1),SEEK_CUR);
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

		
		fseek(VFS_MOUNT_POINT,location-1,SEEK_SET);
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

		puts("Data:");
		puts(b.data);
		 
	}
}


/*
Create a new file named <file_name> with
<data> as content. in path specified by <dest_dir_name>.
*/


int create(char dest_dir[],char file_name[],char data[])
{
	struct file_descriptor file_desp,temp;
	strcpy(file_desp.file_name,file_name);
	file_desp.isfull='1';//0-empty 1-full
	file_desp.location_block_number=-1;
	file_desp.FID= ++METADATA.counter;
	file_desp.file_size=strlen(data);
	FILE *fp = VFS_MOUNT_POINT;
	fseek(fp,METADATA.File_descriptor_array-1,SEEK_SET);
	int i;

	for(i=0;i<METADATA.max_no_of_file_desp_available;i++)
	{
		fread(&temp,sizeof(temp),1,fp);
		if(temp.isfull=='0')
		{
			fseek(fp,-sizeof(temp),SEEK_CUR);
			fwrite(&file_desp,sizeof(file_desp),1,fp);
			break;
		}
	}
	if(i<METADATA.File_descriptor_array)
	{
		printf("\nFile Descriptor Created...");
		puts("\nCalling add blocks");
		file_desp.location_block_number=add_data_blocks(data, file_desp.location_block_number);
		puts("\nCalling add blocks.done");	
		//return 1;
	}
	else
	{
		printf("\nError in File Creating  Descriptor...");
		//return 0;
	}

	//puts("\nDisplay blocks");
	struct block t = read_data_blocks(file_desp.location_block_number);
	display_data_blocks(t);
	//puts("\nCalling add blocks.done");


	return 1;
	/*
	char location_full_path[50];
	char file_type[5];
	long int file_size;
	long int location_block_number;
*/
// return 1;
}


/*
View the contents of a file specified by <file_path>.
*/
int listfile(char file_path[])
{
	FILE *fp = VFS_MOUNT_POINT;
	struct file_descriptor temp;
	fseek(fp,METADATA.File_descriptor_array-1,SEEK_SET);
	int i;
	puts("\nFile List :");
	for(i=0;i<METADATA.max_no_of_file_desp_available;i++)
	{
		fread(&temp,sizeof(temp),1,fp);
		if(temp.isfull=='1')
		{
			printf("\nName : %s, FILE ID : %ld",temp.file_name,temp.FID);
		}
	}
	

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
