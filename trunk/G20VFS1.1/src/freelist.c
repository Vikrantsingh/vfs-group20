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
	long int location = METADATA.freelist;
	fseek(VFS_MOUNT_POINT,METADATA.freelist-1,SEEK_SET);	
	struct block b;
	if(fread(&b,sizeof(b),1,VFS_MOUNT_POINT)==1)
	{}
	METADATA.freelist = b.next;
	return location;

}


/*
	Deleting a block by adding it to free list.
*/

int delete_node(long int location)
{
	struct block b;
	b.next=METADATA.freelist;
	b.isfull = '0';
	fseek(VFS_MOUNT_POINT,location-1,SEEK_SET);
	if(fwrite(&b,sizeof(b),1,VFS_MOUNT_POINT)==1)
	{}
	METADATA.freelist = location;
	return 1;
}







