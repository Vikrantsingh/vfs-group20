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
#include <dirent.h> 
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
	//strcpy(b.data,data);
	int i;
	for(i=0;i<sizeof(b.data);i++)
    	b.data[i]=data[i];

	if(location==-1)
	{
		printf("addingdatablock_FAILURE Free Space Not Available\n");
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
		printf("Data Block Empty\n");
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
		printf("Data Block Empty\n");
		return -1;
	}
	else
	{

		//printf("Data:");
		printf("%s\n",b.data);
		 
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
//		printf("\naddfile_FAILURE FILE_SYSTEM_FULL");
   	  printf("addfile_FAILURE %s\n",ERR_VFS_ADDFILE_04);

		return 0; 
	}

    //Check if file/dir  already exists
    char temp_path[MAX_LENGTH_OF_PATH];
    strcpy(temp_path,parent_path);
    if(strcmp(parent_path,"/")!=0)
        strcat(temp_path,"/");
    strcat(temp_path,file_name);
    
    struct bst *bstnode = (struct bst*)search_bst_path(BST_FULL_PATH,temp_path);

  //  printf("searching %s\n",temp_path);

    if(bstnode!=NULL)
    {
 //       printf("addfile_FAILURE FILE_ALREADY_EXISTS");
    	  printf("addfile_FAILURE %s\n",ERR_VFS_ADDFILE_03);

        return 0;
    }
       
    bstnode = (struct bst*)search_bst_path(BST_FULL_PATH,parent_path);

    if(bstnode==NULL)
    {
        printf("addfile_FAILURE PATH_NOT_FOUND\n");
//   	  printf("addfile_FAILURE %s\n",ERR_VFS_ADDFILE_07);
        
        return 0;
    }
    
    parent = bstnode->nary_node;

///////////check isdir for input file path
	DIR *din = opendir(infile);
	if(din!=NULL)
	{
	    printf("addfile_FAILURE NOT_A_FILE\n");
	    close(din);
	    return 0;
	}
    

	FILE *fin = fopen(infile,"rb");
	if(fin==NULL)
	{
	    printf("addfile_FAILURE CANNOT_OPEN_SPECIFIED_DATAFILE\n");
	    return 0;
	}
	fseek(fin,0,SEEK_END);
	long int file_size = ftell(fin);
//		printf("%ld",file_size);
	//	puts(infile);
	//while(1);
	

	struct block blockdata;	
    long int blocks_req = file_size / sizeof(blockdata.data);
    if(blocks_req >= (METADATA.no_of_blocks - METADATA.no_of_blocks_used))
    {
        printf("addfile_FAILURE FILE_SYSTEM_FULL\n");
        return 0;
    }



 
    ////////////////get file extension
     
    //puts("hiiiiii");
    char tempName[MAX_LENGTH_OF_NAME];
    char exten[MAX_LENGTH_OF_NAME];
    strcpy(tempName,file_name);
    int v=strlen(tempName);
    int j;
    for(i = v-1,j=0 ; i>=0; i--,j++)
    {
    	if(tempName[i] == '.')
    		break;
    	exten[j] = tempName[i];
    }
    if(i>=0)
	{
	    exten[j] = '\0';
	    exten[5] = '\0'; 
	}   
    else
    	{
    	    exten[0]='\0';
    	}
    char ft[5];
    if(strlen(exten)==0)
    {
    	strcpy(ft,"file");
    }
    else
    {
    	strcpy(ft,exten);
  /*  	while(exten[i]!='\0')	
    	{
    		ft[i] = exten[i];
    		i++;
    	}
    	ft[i]='\0';*/
    }
/*    char *tempPtr = strtok(tempName,".");
    char *extension = NULL;
       puts("hielo");
    while(tempPtr!=NULL)     
    {
    	tempPtr =strtok(NULL,".");
        extension = tempPtr;
        tempPtr =strtok(NULL,".");
    }
    if(extension==NULL)
    {
   //     printf("addfile_FAILURE FILENAME_WITHOUT_EXTENSION");
     //   return 0;        
       strcpy(extension,"file");
    }
  */    
  
// return 1;

    //printf("Got ii parent=%s\n",parent->file_desp.file_name);
   
//	Create & Store FD 
	struct file_descriptor file_desp,temp;
	//Storing File Desp data in Struct variable
	strcpy(file_desp.file_name,file_name);
		//////////////Store Path
	strcpy(file_desp.file_location_full_path,parent_path);
	if(strcmp(parent_path,"/")!=0)
    	strcat(file_desp.file_location_full_path,"/");
	strcat(file_desp.file_location_full_path,file_name);
	file_desp.isfull='1';//0-empty 1-full
	file_desp.location_block_number=-1;
	file_desp.FID= METADATA.counter + 1;
	file_desp.sibling=-1;/////////////////////to do
	file_desp.child=-1;
	file_desp.file_size=file_size;
	file_desp.parent=-1;/////////////////////to do
	strcpy(file_desp.file_type,ft);
	
	    //printf("adding %s\n",file_desp.file_location_full_path);
	
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
		//printf("File Descriptor Created for New directory...");
        ////////////Update N-Ary/////////////////////////////////
        
        struct nary *new_node = (struct nary*)malloc(sizeof(struct nary));
        new_node->child=NULL;
        new_node->sibling=NULL;
        new_node->parent=NULL;
        
        new_node->file_desp = file_desp;
        
        new_node->fd_loc = METADATA.File_descriptor_array + i * sizeof(file_desp);
                        
        add_node_to_nary(parent,new_node);
       // printf("display:");
       // display_nary(NARY_ROOT);    
         //       printf("...done:");

        ///////////Update BST////////////////////////////////////

	    struct bst  *b = (struct bst*)malloc (sizeof(struct bst));
	    if(b==NULL){printf("addfile_FAILURE SPACE_NOT_AVAILBLE_FOR_CREATING_BST\n");}
	    else
	    {    
	        //b->FD=temp;  
	        //b->fd_loc=add;
	        b->nary_node=new_node;
	        BST_FULL_PATH= (struct bst*) insert_into_bst_path(BST_FULL_PATH,b);  
	    }    

		///////////UPDATE HASH TABLE/////////////////////////////
		//printf("Name=%s path=%s\n",file_desp.file_name,file_desp.file_location_full_path);
		insert_in_hashtable(file_desp.file_name,file_desp.file_location_full_path);		
        /////////////////////////////////////////////////////////

        //////////////////Write Data Blocks/////////////
        

        fseek(fin,0,SEEK_SET);
        //printf("block size:%d",sizeof(blockdata.data));
        //char *filedata = (char*)malloc(sizeof(filedata)*sizeof(blockdata.data));
        
        char filedata[sizeof(blockdata.data)];
        char ch;// = (char*)malloc(sizeof(ch)*sizeof(blockdata.data));
        int index=0;
        //FILE *ooo = fopen("demo.png","wb+");
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

 			 //fwrite(filedata,1,index,ooo);
 			   // printf("calling add data");
			   // putchar(ch);
			   // printf("paasing = %ld",file_desp.location_block_number);
			    new_node->file_desp.location_block_number=add_data_blocks(filedata, new_node->file_desp.location_block_number);
			   // printf("return = %ld",file_desp.location_block_number);
			   // printf("\nblocks added");	
			    index=0;
		    }		                             
        }
        
        if(index!=0)
        {
       
          //  fwrite(filedata,1,index,ooo);
 			   // printf("calling add data");
			   // putchar(ch);
			   // printf("paasing = %ld",file_desp.location_block_number);
			    new_node->file_desp.location_block_number=add_data_blocks(filedata, new_node->file_desp.location_block_number);
			   // printf("return = %ld",file_desp.location_block_number);
			   // printf("\nblocks added");	
			    index=0;
        }
       // fclose(ooo);
        //free(ch);
        //new_node->file_desp = file_desp;
        
        ////////////////////////////////////////////////
	}
	else
	{
	    	printf("addfile_FAILURE ERROR_IN_CREATING_FILE_DESCRIPTOR\n");
	    	return 0;
	}
		
		
	//printf("\n=========================\nreturn from createe");	
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
        //printf("listfile_FAILURE SOURCE_FILE_PATH_NOT_FOUND");
       	printf("listfile_FAILURE %s\n",ERR_VFS_LISTFILE_01);

        return 0;
     }


    struct file_descriptor file_desp = bstnode->nary_node->file_desp;
 	
 	////////////check for valid file
	
	if(strcmp(file_desp.file_type,"txt")!=0)
	{
//	    printf("listfile_FAILURE NOT_A_TEXT_FILE");
     	printf("listfile_FAILURE %s\n",ERR_VFS_LISTFILE_02);
	    return 0;
	}
	
	long int loc=file_desp.location_block_number; //starting data block location
	if(loc==-1) 
	{
	    printf("listfile_FAILURE FILE_EMPTY\n");
	    return 0;
	}
    FILE *fout = fopen(outputfile,"wb+");
    if(fout==NULL)
    {
//        printf("listfile_FAILURE OUTPUT_FILE_PATH_NOT_FOUND");
     	printf("listfile_FAILURE %s\n",ERR_VFS_LISTFILE_03);

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
        		char ch =  t.data[j++];
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
Remove a file specified by <file_path>.
*/
int rm(char path[])
{
    if(strcmp(path,"/")==0)
    {
        printf("removefile_FAILURE Access Denied\n");
        return 0;
    }
    if(strcmp(path,"/")==0)
    {
        printf("removefile_FAILURE Access Denied\n");
        return 0;
    }
    
 
       
    //delete from N-ARy & BST & Hash TABLE
     struct bst *bstnode = (struct bst*)search_bst_path(BST_FULL_PATH,path);
     if(bstnode==NULL)
     {
 //       printf("removefile_FAILURE CANNOT_FIND_SPECIFIED_PATH_OR_FILE");
      	printf("removefile_FAILURE %s\n",ERR_VFS_REMOVEFILE_01);

        return 0;
     }
               
     struct nary *temp_nary,*parent_nary = bstnode->nary_node->parent;

     struct file_descriptor fd = bstnode->nary_node->file_desp ;

     if(strcmp(bstnode->nary_node->file_desp.file_type,"dir")==0)
     {
        printf("removefile_FAILURE NOT_A_FILE\n");
       return 0;
     }
      //if node n to be deleted is first child
     
     if(parent_nary->child==bstnode->nary_node)
     {
        parent_nary->child=bstnode->nary_node->sibling;
        //Update FD
        parent_nary->file_desp.child=bstnode->nary_node->file_desp.sibling;
     }
     //For other positions of n.........
     else
     {
       
        temp_nary = parent_nary->child;


        while(temp_nary->sibling != bstnode->nary_node)
        {
 
            temp_nary=temp_nary->sibling;
        }
        temp_nary->sibling=bstnode->nary_node->sibling;       
        //Update FD
        temp_nary->file_desp.sibling=bstnode->nary_node->file_desp.sibling;
     }
     
     //To delete subtree rooted by n
     
     ///// tHIS fUNCTION iNTERNALLY caLLS dELETE BST & dELETE hASHTABLE ////
     
     //delete_subtree_from_nary(bstnode->nary_node);
     
     delete_from_hash(bstnode->nary_node->file_desp.file_location_full_path,bstnode->nary_node->file_desp.file_name);
     free(bstnode->nary_node);
     reset_bst_path(); 
    
	//----------------------------------------------------------
	
	//return 1;

    /////////////////////Removing Data Blocks

	FILE *fp = VFS_MOUNT_POINT;
	
	long int loc = -1;
 	int i;
     

	///Now Removing blocks
	
	loc=fd.location_block_number; //starting data block location
	if(loc==-1) 
	{
	//printf("File Empty");
	}
	while(loc!=-1)
	{	
		struct block t = read_data_blocks(loc); //scanning data block one by one
		//printf("delete node:%ld",loc);
		delete_node(loc);//delete node and add it to freelist
		loc=t.next;
		//printf("start %ld, loc=%ld",temp.location_block_number,loc);
		
	}
		--METADATA.no_of_file_desp_used;//iNCREMENT uSED fILE dESCRIPTOR cOUNTER
 return 1;
}
/* 
Search file in hash table
*/
int find_file(char name[], char output_file[])
{
    //int a,b;
    //hash(name[0],name[1],&a,&b);
    FILE *fout = fopen(output_file,"a");
    if(fout==NULL)
    {
        fout = fopen(output_file,"w");
    }    
    if(fout==NULL)
    {
        printf("searchfile_FAILURE CANNOT_FIND/CREATE_SPECIFIED_OUTPUTFILE\n");
        return 0;
    }
    fprintf(fout,"\n\n#Searching file with name %s :\n",name);
    find_in_hashtable(name,fout);
    fprintf(fout,"\n..Searching done.\n");
    fclose(fout);
    //printf("File found");
    return 1;
}
/*
Rename <source_file_path> to
<dest_file_path> file.
*/
int mv(char src_file_path[],char dest_dir_path[])
{
 	//printf("In movedir");
    //find src-dir...
    //remove it....and add to dest-dir
    //delete from N-ARy & BST & Hash TABLE

     struct bst *bstnode_src = (struct bst*)search_bst_path(BST_FULL_PATH,src_file_path);
     if(bstnode_src==NULL)
     {
//        printf("movefile_FAILURE CANNOT_FIND_SOURCEFILE");
     	printf("movefile_FAILURE %s\n",ERR_VFS_MOVEFILE_01);

        return 0;
     }
 

    char dir_struct[MAX_SUB_DIRECTORY_LEVEL][MAX_LENGTH_OF_NAME];//limiting max sub directory level 
	int i,total_dir_in_path;
    char new_dest_dir_path[MAX_LENGTH_OF_PATH];
	struct nary *parent; 
	//tokenizing path
	i = separate_path(dir_struct,dest_dir_path,&total_dir_in_path);
	if(i==0)
	{ 
   	     printf("movefile_FAILURE UNABLE_TO_PROCESS_PATH\n");
	     return 0;
	}
	else //form a new path
	{
	   
	    strcpy(new_dest_dir_path,"/");
	    for(i=0;i<total_dir_in_path-1;i++)
	    {
	           if(strcmp(new_dest_dir_path,"/")!=0)
            	    strcat(new_dest_dir_path,"/");
               strcat(new_dest_dir_path,dir_struct[i]);
	    }

	}

     struct bst *bstnode_dest = (struct bst*)search_bst_path(BST_FULL_PATH,dest_dir_path);
     if(bstnode_dest!=NULL)
     {
        printf("movefile_FAILURE FILE_ALREADY_EXISTS\n");
        return 0;
     }
 
   
     bstnode_dest = (struct bst*)search_bst_path(BST_FULL_PATH,new_dest_dir_path);
     if(bstnode_dest==NULL)
     {
//        printf("movefile_FAILURE CANNOT_FIND_DESTINATION_PATH");
     	printf("movefile_FAILURE %s\n",ERR_VFS_MOVEFILE_02);

        return 0;
     }

     
     if(strcmp(bstnode_src->nary_node->file_desp.file_type,"dir")==0)
     {
        printf("movefile_FAILURE NOT_A_FILE\n");
        return 0;
     }


     if(strcmp(bstnode_dest->nary_node->file_desp.file_type,"dir")!=0)
     {
        printf("movefile_FAILURE DESTINATION_CANNOT_BE_FILE\n");
        return 0;
     }

///////////////temporaray alternate solution

/*
// move = copy + delete

 struct file_descriptor fd1 = bstnode_src->nary_node->file_desp;
 
 int status =   copy_file(src_file_path,dest_dir_path); 


 status = rm(fd1.file_location_full_path);
                

return status;
*/
/////////////   

     struct nary *temp_nary,*parent_nary = bstnode_src->nary_node->parent;

    //renaming file
    char old_file_name[MAX_LENGTH_OF_NAME];
    strcpy(old_file_name,bstnode_src->nary_node->file_desp.file_name);
    strcpy(bstnode_src->nary_node->file_desp.file_name,dir_struct[total_dir_in_path-1]);
    
    ///

           /// sTART mODIFYING nARY tREE //////////////////   
     /////////////////mAKE cHANGES iN sOURCE dIR/////////////////////        
     //if node n to be moved is first child
   if(parent_nary->child==bstnode_src->nary_node)
     {
        //printf("first child");
        parent_nary->child=bstnode_src->nary_node->sibling;
        //printf("new chlid Path %s\n",parent_nary->child->file_desp.file_location_full_path);
        //Update FD
        parent_nary->file_desp.child=bstnode_src->nary_node->file_desp.sibling;
     }
     //For other positions of n.........
     else
     {
        temp_nary = parent_nary->child;
        while(temp_nary->sibling!=bstnode_src->nary_node && temp_nary!=NULL)
        {
            temp_nary=temp_nary->sibling;
        }
        temp_nary->sibling=bstnode_src->nary_node->sibling;       
        //Update FD
        temp_nary->file_desp.sibling=bstnode_src->nary_node->file_desp.sibling;
     }
     /////////////////mAKE cHANGES iN dESTINATION dIR/////////////////////
     bstnode_src->nary_node->sibling = bstnode_dest->nary_node->child;     
     bstnode_src->nary_node->parent = bstnode_dest->nary_node;     //update parent
     bstnode_dest->nary_node->child= bstnode_src->nary_node;
     //uPDATE FD'S      
     bstnode_src->nary_node->file_desp.sibling = bstnode_dest->nary_node->file_desp.child;     
     bstnode_src->nary_node->file_desp.parent = bstnode_dest->nary_node->fd_loc;     
     bstnode_dest->nary_node->file_desp.child = bstnode_src->nary_node->fd_loc;     

 
     
     /// eND mODIFYING nARY tREE //////////////////
     
      
     ///// uPATES  BST & hASHTABLE ////
         ///cALL uPDATE bST
        struct nary *node = bstnode_src->nary_node;         
        char new_path[MAX_LENGTH_OF_PATH],old_path[MAX_LENGTH_OF_PATH];
        //
        strcpy(old_path,node->file_desp.file_location_full_path);   
        //
        strcpy(new_path,new_dest_dir_path);
        if(strcmp(new_path,"/")!=0)
            strcat(new_path,"/");
        strcat(new_path,node->file_desp.file_name);          
        /////////////////    

        strcpy(node->file_desp.file_location_full_path,new_path); 
 
        delete_from_hash(old_path,old_file_name);

        //printf("\nin hash name=%s path%s\n",node->file_desp.file_name,new_path);
        
        insert_in_hashtable(node->file_desp.file_name,new_path);
  
        reset_bst_path();         

	//----------------------------------------------------------
		
     return 1;

}


/*
To copy source directory/file to
destination specified.
*/
int copy_file(char src[],char dest[])
{
    //check for space
	if(METADATA.no_of_file_desp_used>=METADATA.max_no_of_file_desp_available-1)
	{
//		printf("\ncopyfile_FAILURE FILE_SYSTEM_FULL");
     	printf("copyfile_FAILURE %s\n",ERR_VFS_COPYFILE_04);
		return 0; 
	}
     //check file exists or not
     struct bst *src_bstnode = (struct bst*)search_bst_path(BST_FULL_PATH,src);
     if(src_bstnode==NULL)
     {
//        printf("copyfile_FAILURE CANNOT_FIND_SPECIFIED_SOURCEFILE");
     	printf("copyfile_FAILURE %s\n",ERR_VFS_COPYFILE_01);
        return 0;
     }




    char dir_struct[MAX_SUB_DIRECTORY_LEVEL][MAX_LENGTH_OF_NAME];//limiting max sub directory level 
	int i,total_dir_in_path;
    char new_dest_dir_path[MAX_LENGTH_OF_PATH];
	struct nary *parent; 
	//tokenizing path
	i = separate_path(dir_struct,dest,&total_dir_in_path);
	if(i==0)
	{ 
   	     printf("copyfile_FAILURE UNABLE_TO_PROCESS_PATH\n");
	     return 0;
	}
	else //form a new path
	{
	   
	    strcpy(new_dest_dir_path,"/");
	   //puts(new_dest_dir_path);
	    for(i=0;i<total_dir_in_path-1;i++)
	    {
	           if(strcmp(new_dest_dir_path,"/")!=0)
            	    strcat(new_dest_dir_path,"/");
               strcat(new_dest_dir_path,dir_struct[i]);
	    }

	}

     struct bst *bstnode_dest = (struct bst*)search_bst_path(BST_FULL_PATH,dest);
     if(bstnode_dest!=NULL)
     {
        printf("copyfile_FAILURE FILE_ALREADY_EXISTS\n");
     //	printf("copyfile_FAILURE %s\n",ERR_VFS_COPYFILE_05);
        return 0;
     }
 

 	////////////check for destination
	
//	puts(new_dest_dir_path);
	//puts(dest);
     struct bst *dest_bstnode = (struct bst*)search_bst_path(BST_FULL_PATH,new_dest_dir_path);
     if(dest_bstnode==NULL)
     {
//        printf("copyfile_FAILURE CANNOT_FIND_DESTINATIONPATH");
     	printf("copyfile_FAILURE %s\n",ERR_VFS_COPYFILE_02);

        return 0;
     }
     //now copy FD to temp variable
    struct file_descriptor src_file_desp = src_bstnode->nary_node->file_desp; 	
    struct file_descriptor new_file_desp = src_file_desp; 	
     //check file type
     //src must be file and dest must be dir
	if(strcmp(new_file_desp.file_type,"dir")==0)
	{
//	    printf("copyfile_FAILURE NOT_A_FILE");

     	printf("copyfile_FAILURE %s\n",ERR_VFS_COPYFILE_03);
	    return 0;
	}
  	if(strcmp(dest_bstnode->nary_node->file_desp.file_type,"dir")!=0)
	{
//	    printf("copyfile_FAILURE CANNOT_COPY_DIR_TO_FILE");
     	printf("copyfile_FAILURE %s\n",ERR_VFS_COPYFILE_03);
	    return 0;
	}
 
     
     
     //////////////////////////////////////////////////
    /* 	int i;
	struct nary *parent; 
 
 	//check for space

    //Check if file/dir  already exists
    char temp_path[MAX_LENGTH_OF_PATH];
    strcpy(temp_path,dest);
    if(strcmp(temp_path,"/")!=0)
        strcat(temp_path,"/");
    strcat(temp_path,src_file_desp.file_name);
    
    struct bst *new_bstnode = (struct bst*)search_bst_path(BST_FULL_PATH,temp_path);

    if(new_bstnode!=NULL)
    {
        printf("copyfile_FAILURE FILE_WITH_SAME_NAME_EXISTS");
        return 0;
    }
       
   */
    //parent for new file    
    parent = dest_bstnode->nary_node;
    
	struct block blockdata;	
	//Check space availabilty for data blocks
	long int blocks_req = src_file_desp.file_size / sizeof(blockdata.data);
    if(blocks_req >= (METADATA.no_of_blocks - METADATA.no_of_blocks_used))
    {
//        printf("copyfile_FAILURE FILE_SYSTEM_FULL");
     	printf("copyfile_FAILURE %s\n",ERR_VFS_COPYFILE_04);
        return 0;
    }

    //printf("Got ii parent=%s\n",parent->file_desp.file_name);
   
//	Create & Store FD 
//	struct file_descriptor file_desp,temp;
	//Storing File Desp data in Struct variable
	strcpy(new_file_desp.file_name,dir_struct[total_dir_in_path-1]);
		//////////////Store Path
//	strcpy(file_desp.file_location_full_path,parent_path);
//	strcat(file_desp.file_location_full_path,"/");
	strcpy(new_file_desp.file_location_full_path,dest);//temp_path
	new_file_desp.isfull='1';//0-empty 1-full
	new_file_desp.location_block_number=-1;
	new_file_desp.FID= METADATA.counter + 1;
	new_file_desp.sibling=-1;/////////////////////to do
	new_file_desp.child=-1;
	//file_desp.file_size=file_size;
	new_file_desp.parent=-1;/////////////////////to do
	//strcpy(file_desp.file_type,"file");
	
	
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
		//printf("File Descriptor Created for New directory...");
        ////////////Update N-Ary/////////////////////////////////
        
        struct nary *new_node = (struct nary*)malloc(sizeof(struct nary));
        new_node->child=NULL;
        new_node->sibling=NULL;
        new_node->parent=NULL;
        
        new_node->file_desp = new_file_desp;
        
        new_node->fd_loc = METADATA.File_descriptor_array + i * sizeof(new_file_desp);
                        
        add_node_to_nary(parent,new_node);
       // printf("display:");
       // display_nary(NARY_ROOT);    
         //       printf("...done:");

        ///////////Update BST////////////////////////////////////

	    struct bst  *b = (struct bst*)malloc (sizeof(struct bst));
	    if(b==NULL){printf("Space not avaliable for creating bst\n"); return 0;}
	    else
	    {    
	        //b->FD=temp;  
	        //b->fd_loc=add;
	        b->nary_node=new_node;
	        BST_FULL_PATH= (struct bst*) insert_into_bst_path(BST_FULL_PATH,b);  
	    }    

		///////////UPDATE HASH TABLE/////////////////////////////
		//printf("Name=%s path=%s\n",file_desp.file_name,file_desp.file_location_full_path);
		insert_in_hashtable(new_file_desp.file_name,new_file_desp.file_location_full_path);		
        /////////////////////////////////////////////////////////

        //////////////////Write Data Blocks/////////////
        

        
        //printf("block size:%d",sizeof(blockdata.data));
        char *filedata = (char*)malloc(sizeof(filedata)*sizeof(blockdata.data));
        char ch;// = (char*)malloc(sizeof(ch)*sizeof(blockdata.data));
        int index=0;
        long int  loc = src_file_desp.location_block_number;
        while(loc!=-1)
	    {	
    		struct block t = read_data_blocks(loc); //read data from file block by block
    		strcpy(filedata,t.data);
			new_node->file_desp.location_block_number=add_data_blocks(filedata, new_node->file_desp.location_block_number);
			loc = t.next;
		}
        //free(ch);
        //new_node->file_desp = new_file_desp;
        
        ////////////////////////////////////////////////
	}
	else
	{
    	printf("copyfile_FAILURE Error_in_creating_File_Descriptor_for_New_directory\n");
	    	return 0;
	}
		
		 

 return 1;
}
 

/*
Export the file from the file system into the hard disk
*/
int export_file(char file_path[],char outputfile[]) 
{

     struct bst *bstnode = (struct bst*)search_bst_path(BST_FULL_PATH,file_path);
     if(bstnode==NULL)
     {
//        printf("exportfile_FAILURE CANNOT_FIND_SPECIFIED_SOURCEFILE");
     	printf("exportfile_FAILURE %s\n",ERR_VFS_EXPORTFILE_01);
        return 0;
     }

    FILE *fout = fopen(outputfile,"wb+");
    if(fout==NULL)
    {
//        printf("exportfile_FAILURE CANNOT_FIND/CREATE_SPECIFIED_OUTPUTFILE");
     	printf("exportfile_FAILURE %s\n",ERR_VFS_EXPORTFILE_02);
        return 0;
    }
     
    struct file_descriptor file_desp = bstnode->nary_node->file_desp;
 	
 	////////////check for valid file
	
	if(strcmp(file_desp.file_type,"dir")==0)
	{
//	    printf("exportfile_FAILURE CANNOT_EXPORT_DIR");
     	printf("exportfile_FAILURE %s\n",ERR_VFS_EXPORTFILE_03);

	    return 0;
	}
	
	long int loc=file_desp.location_block_number; //starting data block location
	if(loc==-1) 
	{
	    printf("exportfile_FAILURE File_Empty\n");
	    return 0;
	}
	long int index=0;

    long int filesize=file_desp.file_size;

	while(loc!=-1)
	{	 
		struct block t = read_data_blocks(loc); //read data from file block by block
	
		//display_data_blocks(t);//Print data on console
		//write data to file
		int j=0;
		//printf("file size = %ld start %ld, loc=%ld next=%ld\n\n",file_desp.file_size , file_desp.location_block_number,loc,t.next);
   

        
        
        if(filesize>sizeof(t.data))
   		    fwrite(t.data,1,sizeof(t.data),fout);
   		else    
            fwrite(t.data,1,filesize,fout);

        filesize = filesize - sizeof(t.data);

/*		while(index < file_desp.file_size && j< sizeof(t.data))
        {
        		char ch =  t.data[j++];
        		//putchar(ch);
        		fwrite(&ch,sizeof(ch),1,fout);
        		fwrite(filedata,1,index,ooo);
        		index++;
        		
        }
*/
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
int update_file(char path[],char fin[])
{
     //check file exists or not
    //printf("in update\n\n");
    //return 1; 
     //puts(path);
     //puts(fin);
     struct bst *bstnode = (struct bst*)search_bst_path(BST_FULL_PATH,path);
     if(bstnode==NULL)
     {
//        printf("updatefile_FAILURE INTERNAL_FILE_PATH_NOT_FOUND");
     	printf("updatefile_FAILURE %s\n",ERR_VFS_UPDATEFILE_01);

        return 0;
     } 
     //check for input file
     FILE *fp = fopen(fin,"rb");    
     if(fp==NULL)
     {
//        printf("updatefile_FAILURE EXTERNAL_FILE_PATH_NOT_FOUND");
     	printf("updatefile_FAILURE %s\n",ERR_VFS_UPDATEFILE_02);
        return 0; 
     }
     fclose(fp);
     //
     struct file_descriptor file_desp = bstnode->nary_node->file_desp;
     char parent_path[MAX_LENGTH_OF_PATH];
     strcpy(parent_path,bstnode->nary_node->parent->file_desp.file_location_full_path);
     //delete old file.........
     if( rm(path) == 0)
     {
        printf("updatefile_FAILURE UNABLE_DELETE_OLD_DATA\n");
        return 0; 
     }
     
     //add new file.........
     if( create(parent_path,file_desp.file_name,fin)==0)   
     {
        printf("updatefile_FAILURE UNABLE_TO_OVERWRITE\n");
        return 0; 
     }
     ///////////check isdir for input file path
	DIR *din = opendir(fin);
	if(din!=NULL)
	{
	    printf("update_FAILURE NOT_A_FILE\n");
	    close(din);
	    return 0;
	}

     return 1;
        
}
/*--------EOF------*/
