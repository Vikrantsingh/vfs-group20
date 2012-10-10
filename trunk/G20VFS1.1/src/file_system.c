/*
	File: file_system.c
	Description:Functions for Creating - mounting filesystem
	 
*/


#include "../include/structure.h"
#include "../include/hashtable.h"
#include <stdio.h>
#include <string.h>

/*
	Determine File Property...Memory Utilization
*/
int memory_statistics()
{
	if(IS_VFS_MOUNTED==0)	
		return 0;
//Space occupied till now...
	int meta_size=sizeof(struct Meta_header);
	int file_desp_size=sizeof(struct file_descriptor);
	int block_size=sizeof(struct block);
	long int unoccupied_space= METADATA.vfs_size - (meta_size + METADATA.max_no_of_file_desp_available * file_desp_size + METADATA.no_of_blocks * block_size  );
	puts("\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-");
	printf(" -*- Current Memory Statistics :");
	printf("\nMeta header Size= %d Bytes",sizeof(METADATA));
	printf("\nFile Descriptor header %d Bytes x %ld File Desp. = %ld Bytes",file_desp_size,METADATA.max_no_of_file_desp_available,file_desp_size * METADATA.max_no_of_file_desp_available);
	printf("\nFile Desp. Used = %ld / %ld",METADATA.no_of_file_desp_used,METADATA.max_no_of_file_desp_available);

	printf("\nSize for Blocks %ld Blocks x %d Size = %ld Bytes",METADATA.no_of_blocks,block_size,(METADATA.no_of_blocks*block_size));
	printf("\nBlocks. Used = %ld / %ld",METADATA.no_of_blocks_used,METADATA.no_of_blocks);

	printf("\nAdded Padding of size = %ld Bytes",unoccupied_space);
	printf("\nTotal size = %ld Bytes",meta_size+(file_desp_size * METADATA.max_no_of_file_desp_available)+(METADATA.no_of_blocks*block_size)+unoccupied_space);
	printf("\nDebug info:");
	printf("\nFD Pointer = %ld Bytes",METADATA.File_descriptor_array);
	printf("\nFreelist Pointer = %ld Bytes",METADATA.freelist);

	puts("\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-");	
	return 1;
}

/*
To create a Virtual File System. <vfs_label> is the name
of VFS.
*/


int create_vfs(char vfs_label[],long int size)
{
	//printf("VFS Created %s %ld\n",vfs_label,size);
	FILE *fp=fopen(vfs_label,"wb");
	if(fp==NULL) { puts("\nInvalid path/filename.");return 0; }

	int meta_size=sizeof(struct Meta_header);
	int file_desp_size=sizeof(struct file_descriptor);
	int block_size=sizeof(struct block);
	
	//printf("%d %d %d\n",file_desp_size,meta_size,block_size);
	
        //--------------- Creating Meta Data
	struct Meta_header meta,meta1;
	strcpy(meta.label,vfs_label);//copy VFS Name
	meta.vfs_size=size;
	meta.no_of_file_desp_used=1;
	meta.no_of_blocks_used=0;
	//meta.max_no_of_file_desp_available=1000;
	meta.max_no_of_file_desp_available=(meta.vfs_size/(sizeof(struct block)+sizeof(struct file_descriptor)));
	meta.freelist=meta_size + meta.max_no_of_file_desp_available * file_desp_size + 1;
	meta.counter=1;
	meta.File_descriptor_array=meta_size + 1;
	meta.no_of_blocks=(meta.vfs_size-meta.freelist) / block_size;
	//Storing Meta Data
	printf("\nWritting Meta Data to file...");
	if(fwrite(&meta,sizeof(meta),1,fp)==1)
	{
		printf("done.\n");
	}

	//check Meta Stored correctly or not
	fclose(fp);
	fp=fopen(vfs_label,"rb+");
	//if(fread(&meta1,sizeof(meta1),1,fp)==1)
	//{
		//printf("\nMeta data from file %s %ld %ld %ld %ld %ld %ld\n", meta1.label, meta1.no_of_file_desp_used, meta1.max_no_of_file_desp_available, meta1.freelist, meta1.counter, meta1.File_descriptor_array,meta1.no_of_blocks);
	//}

	//-------------allocating space for header 
	fseek(fp,meta.File_descriptor_array,SEEK_SET);
	struct file_descriptor file;
	
	/*
	char isfull;//0-empty 1-full
	long int FID;
	char file_name[20];
	//char location_full_path[50];
	char file_type[5];
	long int file_size;
	long int location_block_number;//location of first data block
	long int sibling;
	long int child;
	*/
	long int i;
	file.FID=0;
	file.isfull='1';//0-empty 1-full
  	file.sibling=-1;
	file.child=-1;
	file.parent=-1;
	file.location_block_number=-1;
	file.file_size=0;
	strcpy(file.file_type,"dir");//default extension
	strcpy(file.file_name,"root");//deafault one FD allocated for Root
	strcpy(file.file_location_full_path,"root");//deafault one FD allocated for Root
	if(fwrite(&file,sizeof(file),1,fp)==1)	//writing File Descriptor for Root Directory
	{
		//printf("\nloc =%ld",ftell(fp));
	//	printf("\nheader Written to file...");
	}
	//reset FD
	file.isfull='0';//0-empty 1-full
	strcpy(file.file_name,"");
	printf("\nWritting File Descriptor (Header) to file...");
	for(i=0;i<meta.max_no_of_file_desp_available-1;i++)
	if(fwrite(&file,sizeof(file),1,fp)==1)
	{
		//printf("\nloc =%ld",ftell(fp));
	//	printf("\nheader Written to file...");
	}		
	puts("done.");

	//--------------Creating data blocks
	fseek(fp,meta.freelist,SEEK_SET);
	struct block b;
	//b.isfull='0';//0-empty 1-full
	//b.next=fp+sizeof(b);
	puts("\nCreating  Data Blocks...");
	for(i=0;i<meta.no_of_blocks;i++)
	{
		b.isfull='0';//0-empty 1-full
		b.next=ftell(fp)+sizeof(b);///Creating Free-linked-list
		if(i==(meta.no_of_blocks-1)) 
			b.next=-1; ///tracking end of free-list
		//puts("Blocks Free list");
		//printf("\nloc =%ld next =%ld",ftell(fp),b.next);
		printf("\rStatus: %15ld / %-15ld blocks added",i+1,meta.no_of_blocks);
		if(fwrite(&b,sizeof(b),1,fp)==1)
		{
		//	printf("\nheader Written to file...");
		}
	}		
	printf("\nBlocks Creation done.");

	///add padding to fixed size
	long int unoccupied_space= meta.vfs_size - (meta_size + meta.max_no_of_file_desp_available * file_desp_size + meta.no_of_blocks * block_size  );
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
	if(fp==NULL) { puts("\nInvalid path/filename.");return 0; }
	puts("Mounting...");
	VFS_MOUNT_POINT = fp;
	//load meta data
	if(fread(&METADATA,sizeof(METADATA),1,fp)==1)
	{}
	
	////Change PWD
	
	strcpy(PRESENT_WORKING_DIRECTORY,"root/");
	

	
	///Create N - Ary Tree....
	
    NARY_ROOT=NULL;
	create_nary(METADATA.File_descriptor_array);
	display_nary(NARY_ROOT);

	///Create Hash Table....
	puts("creating hash table");
	//display_hashtable();

 
	return 1;
}

/*
To unmount the VFS. It involves saving index information
and tree information back into the hard disk.
*/
int unmount_vfs()
{

    strcpy(PRESENT_WORKING_DIRECTORY,"");
    
	rewind(VFS_MOUNT_POINT);	
	if(fwrite(&METADATA,sizeof(METADATA),1,VFS_MOUNT_POINT)==1)
	{}
	fclose(VFS_MOUNT_POINT);
	return 1;
}

/*--------EOF--------*/
