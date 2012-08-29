/*
	File: file_system.c
	Description:Functions for Creating - mounting filesystem
	 
*/


#include "../include/structure.h"
#include <stdio.h>
#include <string.h>

/*
To create a Virtual File System. <vfs_label> is the name
of VFS.
*/


int create_vfs(char vfs_label[],long int size)
{
	//printf("VFS Created %s %ld\n",vfs_label,size);
	FILE *fp=fopen(vfs_label,"wb");
	if(fp==NULL) return 0;

	int meta_size=sizeof(struct Meta_header);
	int file_desp_size=sizeof(struct file_descriptor);
	int block_size=sizeof(struct block);
	
	//printf("%d %d %d\n",file_desp_size,meta_size,block_size);
	
        //--------------- Creating Meta Data
	struct Meta_header meta,meta1;
	strcpy(meta.label,vfs_label);
	meta.no_of_file_desp_used=0;
	meta.max_no_of_file_desp_available=1000;
	//meta.max_no_of_file_desp_available=(size/(sizeof(struct block)*sizeof(struct block)));
	meta.freelist=meta_size + meta.max_no_of_file_desp_available * file_desp_size + 1;
	meta.counter=1;
	meta.File_descriptor_array=meta_size + 1;
	meta.no_of_blocks=(size-meta.freelist) / block_size;
	//Storing Meta Data
	if(fwrite(&meta,sizeof(meta),1,fp)==1)
	{
		printf("\nMeta Data Written to file...");
	}

	//check Meta Stored correctly or not
	fclose(fp);
	fp=fopen(vfs_label,"rb+");
	if(fread(&meta1,sizeof(meta1),1,fp)==1)
	{
		printf("\nMeta data from file %s %ld %ld %ld %ld %ld %ld\n", meta1.label, meta1.no_of_file_desp_used, meta1.max_no_of_file_desp_available, meta1.freelist, meta1.counter, meta1.File_descriptor_array,meta1.no_of_blocks);
	}

	//-------------allocating space for header 
	fseek(fp,0,SEEK_END);
	struct file_descriptor file;
	file.isfull='0';//0-empty 1-full
	/*
	Rest All Garbage will be stored
	long int FID;
	char file_name[50];
	char location_full_path[50];
	char file_type[5];
	long int file_size;
	long int location_block_number;
	*/
	int i;	
	for(i=0;i<meta.max_no_of_file_desp_available;i++)
	if(fwrite(&file,sizeof(file),1,fp)==1)
	{
	//	printf("\nheader Written to file...");
	}		
	printf("header adding done");

	//--------------Creating data blocks
	fseek(fp,0,SEEK_END);
	struct block b;
	//b.isfull='0';//0-empty 1-full
	//b.next=fp+sizeof(b);
	for(i=0;i<meta.no_of_blocks;i++)
	{
		b.isfull='0';//0-empty 1-full
		b.next=ftell(fp)+sizeof(b);///Creating Free-linked-list
		if(i==(meta.no_of_blocks-1))
			b.next=-1; ///tracking end of free-list
		if(fwrite(&b,sizeof(b),1,fp)==1)
		{
		//	printf("\nheader Written to file...");
		}
	}		
	printf("\nBlocks Creation done");

	///add padding to fixed size
	long int unoccupied_space= size - (meta_size + meta.max_no_of_file_desp_available * file_desp_size + meta.no_of_blocks * block_size  );
	printf("\nAdded Padding ");
	char ch='$';	
	for(i=0;i<unoccupied_space;i++)
	if(fwrite(&ch,sizeof(ch),1,fp)==1)
	{}

	//Space occupied till now...
	puts("\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-");
	printf(" -*- Current Memory Statistics :");
	printf("\nMeta header = %d Bytes",meta_size);
	printf("\nFile Descriptor header = %ld Bytes",file_desp_size * meta.max_no_of_file_desp_available);
	printf("\nMax no. of Files & Dir. holding capacity = %ld ",meta.max_no_of_file_desp_available);
	printf("\nSize for Blocks %ld Blocks x %d Size = %ld Bytes",meta.no_of_blocks,block_size,(meta.no_of_blocks*block_size));
	printf("\nAdded Padding of size = %ld Bytes",unoccupied_space);
	printf("\nTotal size = %ld Bytes",meta_size+(file_desp_size * meta.max_no_of_file_desp_available)+(meta.no_of_blocks*block_size)+unoccupied_space);
	puts("\nVirtual File System Created Successfully");
	puts("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-");
	fclose(fp);
	return 1;
}

/*
To Mount the VFS path on current directory. This involves
loading the file/directory descriptors into various data structures
*/

int mount_vfs(char name[])
{
	FILE *fp=fopen(name,"rb+");
	VFS_MOUNT_POINT = fp;
	//load meta data
	if(fread(&METADATA,sizeof(METADATA),1,fp)==1)
	{}
	
	///Create Hash Table....
	///Create N - Ary Tree....
	return 1;
}

/*
To unmount the VFS. It involves saving index information
and tree information back into the hard disk.
*/
int unmount_vfs()
{
	rewind(VFS_MOUNT_POINT);	
	if(fwrite(&METADATA,sizeof(METADATA),1,VFS_MOUNT_POINT)==1)
	{}
	fclose(VFS_MOUNT_POINT);
	return 1;
}

/*--------EOF--------*/
