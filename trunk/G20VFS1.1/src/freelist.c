/*
	File: freelist.h
	Description: Maintain Track of Available Free Tracks
	 
*/

#include "../include/structure.h"
#include <stdio.h>
#include <string.h>

/*
	Creating block to store data by removing  it from freelist
*/

long int  get_new_node()
{
	if(METADATA.freelist==-1)
		{
			puts("\nNo Free node Available"); 
			return 0;
		}
	long int location = METADATA.freelist;
	fseek(VFS_MOUNT_POINT,METADATA.freelist,SEEK_SET);	
	struct block b;
	if(fread(&b,sizeof(b),1,VFS_MOUNT_POINT)==1)
	{}
	//printf("\nFree list old=%ld and new = %ld",METADATA.freelist,b.next);
	METADATA.freelist = b.next;
	++METADATA.no_of_blocks_used;
	return location;

}


/*
	Deleting a block by adding it to free list.
*/

int delete_node(long int location)
{
	
	if(location==-1)
		{
			puts("\nAlready Empty"); 
			return 0;
		}	
	/*to do: write logic to validate location of block to delete*/	

	//Deleting block and adding it to freelist
	struct block b;
	b.next=METADATA.freelist;
	b.isfull = '0';
	fseek(VFS_MOUNT_POINT,location,SEEK_SET);//-1
	if(fwrite(&b,sizeof(b),1,VFS_MOUNT_POINT)==1)
	{}
	METADATA.freelist = location;
	--METADATA.no_of_blocks_used;
	return 1;
}







