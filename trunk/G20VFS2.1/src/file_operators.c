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


int create(char parent_path[],char file_name[],char infile[])
{ 

	int i;
	struct nary *parent; 
 
 	//check for space
	if(METADATA.no_of_file_desp_used>=METADATA.max_no_of_file_desp_available-1)
	{
		puts("\nSpace not Available");
		return 0; 
	}

    //Check if file/dir  already exists
    char temp_path[500];
    strcpy(temp_path,parent_path);
    strcat(temp_path,"/");
    strcat(temp_path,file_name);
    
    struct bst *bstnode = (struct bst*)search_bst_path(BST_FULL_PATH,temp_path);

    if(bstnode!=NULL)
    {
        puts("File with same name exists.");
        return 0;
    }
       
    bstnode = (struct bst*)search_bst_path(BST_FULL_PATH,parent_path);

    if(bstnode==NULL)
    {
        puts("path not found");
        return 0;
    }
    
    parent = bstnode->nary_node;
    

	FILE *fin = fopen(infile,"rb+");
	fseek(fin,0,SEEK_END);
	long int file_size = ftell(fin);
	struct block blockdata;	
    long int blocks_req = file_size / sizeof(blockdata.data);
    if(blocks_req >= (METADATA.no_of_blocks - METADATA.no_of_blocks_used))
    {
        puts("No  Space Available for Data");
        return 0;
    }

    //printf("Got ii parent=%s",parent->file_desp.file_name);
   
//	Create & Store FD 
	struct file_descriptor file_desp,temp;
	//Storing File Desp data in Struct variable
	strcpy(file_desp.file_name,file_name);
		//////////////Store Path
	strcpy(file_desp.file_location_full_path,parent_path);
	strcat(file_desp.file_location_full_path,"/");
	strcat(file_desp.file_location_full_path,file_name);
	file_desp.isfull='1';//0-empty 1-full
	file_desp.location_block_number=-1;
	file_desp.FID= METADATA.counter + 1;
	file_desp.sibling=-1;/////////////////////to do
	file_desp.child=-1;
	file_desp.file_size=file_size;
	file_desp.parent=-1;/////////////////////to do
	strcpy(file_desp.file_type,"file");
	
	
	//long int location_of_new_FD=-1;
	//search free space in header to add file desp..
	
	for(i=0;i<METADATA.max_no_of_file_desp_available;i++)
	{
	    if(FREELIST[i]==0)
	    {
	        FREELIST[i]=1;        
	        break;	        	        
	    } 
	}
	
    
	if(i<METADATA.max_no_of_file_desp_available)//if file desp added
	{
	
	    
		++METADATA.counter; //Increment ID Counter Variable
		++METADATA.no_of_file_desp_used;//iNCREMENT uSED fILE dESCRIPTOR cOUNTER
		//printf("\nFile Descriptor Created for New directory...");
        ////////////Update N-Ary/////////////////////////////////
        
        struct nary *new_node = (struct nary*)malloc(sizeof(struct nary));
        new_node->child=NULL;
        new_node->sibling=NULL;
        new_node->parent=NULL;
        
        new_node->file_desp = file_desp;
        
        new_node->fd_loc = METADATA.File_descriptor_array + i * sizeof(file_desp);
                        
        add_node_to_nary(parent,new_node);
       // puts("display:");
       // display_nary(NARY_ROOT);    
         //       puts("...done:");

        ///////////Update BST////////////////////////////////////

	    struct bst  *b = (struct bst*)malloc (sizeof(struct bst));
	    if(b==NULL){puts("Space not avaliable for creating bst");}
	    else
	    {    
	        //b->FD=temp;  
	        //b->fd_loc=add;
	        b->nary_node=new_node;
	        BST_FULL_PATH= (struct bst*) insert_into_bst_path(BST_FULL_PATH,b);  
	    }    

		///////////UPDATE HASH TABLE/////////////////////////////
		//printf("Name=%s path=%s",file_desp.file_name,file_desp.file_location_full_path);
		insert_in_hashtable(file_desp.file_name,file_desp.file_location_full_path);		
        /////////////////////////////////////////////////////////

        //////////////////Write Data Blocks/////////////
        

        fseek(fin,0,SEEK_SET);
        //printf("block size:%d",sizeof(blockdata.data));
        unsigned char *filedata = (char*)malloc(sizeof(filedata)*sizeof(blockdata.data));
        unsigned char ch;// = (char*)malloc(sizeof(ch)*sizeof(blockdata.data));
        int index=0;
        while(!feof(fin))
        {
           // long int remaining_bytes = file_size-ftell(fin);
            fread(&ch,sizeof(ch),1,fin);
           // putchar(ch);
            
            //write data blocks to file
            filedata[index]=ch;
            index++;
       		if((index) == sizeof(blockdata.data) )
		    {

 			   // puts("calling add data");
			   // putchar(ch);
			   // printf("\npaasing = %ld",file_desp.location_block_number);
			    file_desp.location_block_number=add_data_blocks(filedata, file_desp.location_block_number);
			   // printf("\nreturn = %ld",file_desp.location_block_number);
			   // puts("\nblocks added");	
			    index=0;
		    }		                             
        }
        if(index!=0)
        {
 			   // puts("calling add data");
			   // putchar(ch);
			   // printf("\npaasing = %ld",file_desp.location_block_number);
			    file_desp.location_block_number=add_data_blocks(filedata, file_desp.location_block_number);
			   // printf("\nreturn = %ld",file_desp.location_block_number);
			   // puts("\nblocks added");	
			    index=0;
        }
        //free(ch);
        new_node->file_desp = file_desp;
        
        ////////////////////////////////////////////////
	}
	else
	{
	    	printf("\nError in creating File Descriptor for New directory...");
	    	return 0;
	}
		
		
	//puts("\n=========================\nreturn from createe");	
	return 1;


}

 
/*
View the contents of a file specified by <file_path>.
*/
int list_file(char file_path[],char outputfile[]) 
{

     struct bst *bstnode = (struct bst*)search_bst_path(BST_FULL_PATH,file_path);
     if(bstnode==NULL)
     {
        puts("File not found");
        return 0;
     }

    FILE *fout = fopen(outputfile,"wb+");
     
    struct file_descriptor file_desp = bstnode->nary_node->file_desp;
 	
 	////////////check for valid file
	
	if(strcmp(file_desp.file_type,"file")!=0)
	{
	    puts("Cannot Display");
	    return 0;
	}
	
	long int loc=file_desp.location_block_number; //starting data block location
	if(loc==-1) 
	{
	    puts("File Empty");
	    return 0;
	}
	long int index=0;
	while(loc!=-1)
	{	
		struct block t = read_data_blocks(loc); //read data from file block by block
		
		//display_data_blocks(t);//Print data on console
		//write data to file
		int j=0;
		//printf("file size = %ld start %ld, loc=%ld next=%ld\n\n",file_desp.file_size , file_desp.location_block_number,loc,t.next);
   

		while(index < file_desp.file_size && j< sizeof(t.data))
        {
        		unsigned char ch =  t.data[j++];
        		//putchar(ch);
        		fwrite(&ch,sizeof(ch),1,fout);
        		index++;
        		
        }
		loc = t.next;
		//printf("start %ld, loc=%ld",file_desp.location_block_number,loc);
		
	}
	fclose(fout);

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
//////////////////TO DO
/*
	FILE *fp = VFS_MOUNT_POINT;
	struct file_descriptor temp,temp2;
	long int loc;
 	int i;
     

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
*/
 return 1;
}
/* 
Search file in hash table
*/
int find_file(char name[], char output_file[])
{
    //int a,b;
    //hash(name[0],name[1],&a,&b);

    FILE *fout = fopen(output_file,"w");
    if(fout==NULL)
    {
        puts("Cannot open output file");
        return 0;
    }
    find_in_hashtable(name,fout);
    fclose(fout);
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
