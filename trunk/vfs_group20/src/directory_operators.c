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
 
    separate path with / ,and return it via call by reference way

*/
int separate_path(char dir_struct[MAX_SUB_DIRECTORY_LEVEL][MAX_LENGTH_OF_NAME],char parent_path[],int *total_dir_in_path)
{
	if(strlen(parent_path)>=MAX_LENGTH_OF_PATH)
	{
	  puts("Path too long.\n");
	  return 0;
	}
	int i=0;
 	char path[MAX_LENGTH_OF_PATH];
	strcpy(path,parent_path);
	
	char *str=strtok(path, "/");	


//    strcpy(dir_struct[i++],"/");

	while(str!=NULL && i<MAX_SUB_DIRECTORY_LEVEL)  //limiting max sub directory level to 25
	{

	    strcpy(dir_struct[i],str);
	    str=strtok(NULL, "/"); 
	    //puts(dir_struct[i]);	    
	    i++;
	}
	if(i==MAX_SUB_DIRECTORY_LEVEL)
    	return 0;
	*total_dir_in_path=i;
	//printf("total=%d",*total_dir_in_path);
	return 1;
        
}


/* To create a new directory whose name should be
<dir_name> in path specified by <parent_path> where '/' is considered as 'root'
directory.
*/

int makedir_recursive(char parent_path[],char dir_name[])
{
    //puts("Recr:");
    char dir_struct[MAX_SUB_DIRECTORY_LEVEL][MAX_LENGTH_OF_NAME];//limiting max sub directory level 
	int i,total_dir_in_path;
	int j,k;
	struct nary *parent; 
	//tokenizing path
	i = separate_path(dir_struct,parent_path,&total_dir_in_path);
	if(i==0)
	{ 
   	     puts("makedir_FAILURE UNABLE_TO_PROCESS_PATH");
	     return 0;
	}
	//puts("i got");
	//for(i=0;i<total_dir_in_path;i++)
	//puts(dir_struct[i]);
	 
	//check for space
	if(METADATA.no_of_file_desp_used>=METADATA.max_no_of_file_desp_available-1)
	{
//		puts("\nmakedir_FAILURE <UNABLE_TO_CREATE_NEWDIR>");
	  printf("makedir_FAILURE %s\n",ERR_VFS_MAKEDIR_01);

		return 0; 
	}

    //Check if file/dir  already exists
    char temp_path[MAX_LENGTH_OF_PATH],temp_path2[MAX_LENGTH_OF_PATH];
    strcpy(temp_path,parent_path);
    if(strcmp(parent_path,"/")!=0)
        strcat(temp_path,"/");
    strcat(temp_path,dir_name);
    
    struct bst *bstnode = (struct bst*)search_bst_path(BST_FULL_PATH,temp_path);

    if(bstnode!=NULL)
    {
        //puts("makedir_FAILURE <DIRECTORY_ALREADY_EXISTS>");
      	  printf("makedir_FAILURE %s\n",ERR_VFS_MAKEDIR_03);

        
        return 0;
    }
    ///////////////////////////////////////////
   	struct file_descriptor file_desp,temp;
	//Storing File Desp data in Struct variable
	file_desp.isfull='1';//0-empty 1-full
	file_desp.location_block_number=-1;
	file_desp.FID= METADATA.counter + 1;
	file_desp.sibling=-1;/////////////////////to do
	file_desp.child=-1;
	file_desp.parent=-1;/////////////////////to do
	strcpy(file_desp.file_type,"dir");

    
    ///////////////////////////////////////////

 
    bstnode = (struct bst*) search_bst_path(BST_FULL_PATH,parent_path);

    if(bstnode==NULL)
    {   
        /////////nOW wE hAVE tO cREATE aLL dIR rECURRSIVELY

   
//        while(1);

            strcpy(temp_path,"/");
            strcpy(temp_path2,"");
         for(j=0;j<total_dir_in_path;j++)        
        {

           strcpy(temp_path2,temp_path);
           if(strcmp(temp_path,"/")!=0)
               strcat(temp_path,"/");
           strcat(temp_path,dir_struct[j]);

            bstnode =(struct bst*) search_bst_path(BST_FULL_PATH,temp_path); 
            if(bstnode==NULL)
            /////////////wRITE cODE tO aDD nEW dIR
            {
                   //printf("\npath %s name %s\n",temp_path2,dir_struct[j]);
                   if(make_dir(temp_path2,dir_struct[j])==0)
                        return 0;
                   
            }
        
        }
        
                  if(make_dir(parent_path,dir_name)==0)
                        return 0;
    
    }
    else
    {
                  if(make_dir(parent_path,dir_name)==0)
                        return 0;
    }
  
    return 1;
}


int make_dir(char parent_path[],char dir_name[])
{
 
    //printf("\n in mk path %s , name=%s ",parent_path,dir_name);
	int i;
	struct nary *parent; 
 
 	//check for space
	if(METADATA.no_of_file_desp_used>=METADATA.max_no_of_file_desp_available-1)
	{
//		puts("\nmakedir_FAILURE <SPACE_NOT_AVAILABLE>");
	  printf("makedir_FAILURE %s\n",ERR_VFS_MAKEDIR_01);

		return 0; 
	}

    //Check if file/dir  already exists
    char temp_path[MAX_LENGTH_OF_PATH];
    strcpy(temp_path,parent_path);
    if(strcmp(parent_path,"/")!=0)
        strcat(temp_path,"/");
    strcat(temp_path,dir_name);
     
    struct bst *bstnode = (struct bst*)search_bst_path(BST_FULL_PATH,temp_path);

    if(bstnode!=NULL)
    { 
//        puts("makedir_FAILURE <DIRECTORY_ALREADY_EXISTS>");
   	  printf("makedir_FAILURE %s\n",ERR_VFS_MAKEDIR_03);

        return 0;
    }
    

    //parent = get_parent_nary(dir_struct,total_dir_in_path);
    bstnode = (struct bst*)search_bst_path(BST_FULL_PATH,parent_path);
    //puts(parent_path);
    if(bstnode==NULL)
    {
        puts("makedir_FAILURE PATH_NOT_FOUND");
        //puts(parent_path);
        return 0;
    }
    
    parent = bstnode->nary_node;
    
    //printf("Got ii parent=%s\n",parent->file_desp.file_name);
   
//	Create & Store FD 
	struct file_descriptor file_desp,temp;
	//Storing File Desp data in Struct variable
	strcpy(file_desp.file_name,dir_name);
		//////////////Store Path
	strcpy(file_desp.file_location_full_path,parent_path);
	if(strcmp(parent_path,"/")!=0)
    	strcat(file_desp.file_location_full_path,"/");
	strcat(file_desp.file_location_full_path,dir_name);
	file_desp.isfull='1';//0-empty 1-full
	file_desp.location_block_number=-1;
	file_desp.FID= METADATA.counter + 1;
	file_desp.sibling=-1;/////////////////////to do
	file_desp.child=-1;
	file_desp.parent=-1;/////////////////////to do
	strcpy(file_desp.file_type,"dir");
	//FILE *fp = VFS_MOUNT_POINT;
	//fseek(fp,METADATA.File_descriptor_array,SEEK_SET);
	
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
    //    printf("\n%s -Parent %s  ",new_node->file_desp.file_name,new_node->parent->file_desp.file_name);
    //   puts("display:");
      //   display_nary_c(NARY_ROOT);    
        //    puts("...done:");

        ///////////Update BST////////////////////////////////////

	    struct bst  *b = (struct bst*)malloc (sizeof(struct bst));
	    if(b==NULL){puts("makedir_FAILURE Space not avaliable for creating bst\n");}
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
        
    //   printf("\n%s -Parent %s  ",new_node->file_desp.file_name,new_node->parent->file_desp.file_name);
        
	}
	else
	{
	    	printf("\nmakedir_FAILURE Error in creating File Descriptor for New directory\n");
	    	return 0;
	}
		
		
	 	
	return 1;
}




/*
To remove a file or a directory as indicated by < name>
*/

int delete_dir(char path[])
{
    if(strcmp(path,"/")==0)
    {
        puts("deletedir_FAILURE ACCESS_DENIED\n");
        return 0;
    }
    if(strcmp(path,"/")==0)
    {
        puts("deletedir_FAILURE ACCESS_DENIED\n");
        return 0;
    }
	//puts("In deletedir");
	

    //--------------------------------------------------
    
    
    //delete from N-ARy & BST & Hash TABLE

     struct bst *bstnode = (struct bst*)search_bst_path(BST_FULL_PATH,path);
     if(bstnode==NULL)
     {
      //  puts("deletedir_FAILURE <CANNOT_FIND_SPECIFIED_PATH_OR_DIR>");
          	  printf("deletedir_FAILURE %s\n",ERR_VFS_DELETEDIR_01);

        return 0;
     }
               
     struct nary *temp_nary,*parent_nary = bstnode->nary_node->parent;
     if(strcmp(bstnode->nary_node->file_desp.file_type,"dir")!=0)
     {
//        puts("deletedir_FAILURE <INVALID_DIRNAME>");
        puts("deletedir_FAILURE NOT_A_DIR\n");
        return 0;
     }

    //check If dir empty or not
     if(bstnode->nary_node->child != NULL)
     {
        //puts("deletedir_FAILURE <DIRECTORY_IS_NOT_EMPTY>");
      	  printf("deletedir_FAILURE %s\n",ERR_VFS_DELETEDIR_02);
       return 0;
     }
    
    
     
     //printf("parent Path %s\n",parent_nary->file_desp.file_location_full_path);
     //if node n to be deleted is first child

     if(parent_nary->child == bstnode->nary_node)
     {
        //puts("first child");
        parent_nary->child = bstnode->nary_node->sibling;
        //printf("new chlid Path %s\n",parent_nary->child->file_desp.file_location_full_path);
        //Update FD
        parent_nary->file_desp.child=bstnode->nary_node->file_desp.sibling;
     }
     //For other positions of n.........
     else
     {
     //printf("parent Path %s\n",parent_nary->file_desp.file_location_full_path);
        //  puts("helooo");
    // while(1); 
         
        temp_nary = parent_nary->child;
        while(temp_nary->sibling!=bstnode->nary_node)
        {
            temp_nary=temp_nary->sibling;
        }
        temp_nary->sibling=bstnode->nary_node->sibling;       
        //Update FD
        temp_nary->file_desp.sibling=bstnode->nary_node->file_desp.sibling;
     }
     
     //To delete subtree rooted by n
     
     ///// tHIS fUNCTION iNTERNALLY caLLS dELETE BST & dELETE hASHTABLE ////
     
     delete_subtree_from_nary(bstnode->nary_node->child); 
     delete_from_hash(bstnode->nary_node->file_desp.file_location_full_path,bstnode->nary_node->file_desp.file_name);
     reset_bst_path();
    
	//----------------------------------------------------------
	//free(bstnode->nary_node);
    --METADATA.no_of_file_desp_used;//iNCREMENT uSED fILE dESCRIPTOR cOUNTER
	return 1;
	
}



/*
To move a sub-tree 
<source_dir_name> to <dest_dir_name> directory.
*/
 
int move_dir(char src_dir_path[],char dest_dir_path[])
{
	//puts("In movedir");
    //find src-dir...
    //remove it....and add to dest-dir
    //delete from N-ARy & BST & Hash TABLE


 //puts("display:");
   //      display_nary_c(NARY_ROOT);    
     //       puts("...done:");
            
 

     struct bst *bstnode_src = (struct bst*)search_bst_path(BST_FULL_PATH,src_dir_path);
     if(bstnode_src==NULL)
     {
       // puts("movedir_FAILURE <CANNOT_FIND_SPECIFIED_SOURCEDIR>");
       	  printf("movedir_FAILURE %s\n",ERR_VFS_MOVEDIR_01);
        return 0;
     }
     if(strcmp(bstnode_src->nary_node->file_desp.file_type,"dir")!=0)
     {
//        puts("movedir_FAILURE <SOURCE_CANNOT_BE_FILE>");
    	  printf("movedir_FAILURE %s\n",ERR_VFS_MOVEDIR_04);
        return 0;
     }
     

       // printf("%s parent %s\n",bstnode_src->nary_node->file_desp.file_name,bstnode_src->nary_node->parent->file_desp.file_name);
     char testing_path[MAX_LENGTH_OF_PATH];
     strcpy(testing_path,dest_dir_path);
     if(strcmp(testing_path,"/")!=0)
        strcat(testing_path,"/");
     strcat(testing_path,bstnode_src->nary_node->file_desp.file_name);
     //puts(testing_path);
     struct bst *test = (struct bst*)search_bst_path(BST_FULL_PATH,testing_path);
     if(test!=NULL)
     {
      //  puts("movedir_FAILURE <DESTINATION_ALREADY_HAVE_SOURCE_DIR>");
   	  printf("movedir_FAILURE %s\n",ERR_VFS_MOVEDIR_05);
      
        return 0;
     }


 

     struct bst *bstnode_dest = (struct bst*)search_bst_path(BST_FULL_PATH,dest_dir_path);
     if(bstnode_dest==NULL)
     {
     //   puts("movedir_FAILURE <CANNOT_FIND_SPECIFIED_DESTINATIONDIR>");
    	  printf("movedir_FAILURE %s\n",ERR_VFS_MOVEDIR_02);
     
        return 0;
     }
     if(strcmp(bstnode_dest->nary_node->file_desp.file_type,"dir")!=0)
     {
//        puts("movedir_FAILURE <DESTINATION_CANNOT_BE_FILE>");
   	  printf("movedir_FAILURE %s\n",ERR_VFS_MOVEDIR_07);

        return 0;
     }
     

    //CHECK12 FOR DESTINATION CHILD OF SOURCE
    
     if(isChild(bstnode_src->nary_node->child,bstnode_dest->nary_node)==1)
     {
//        puts("movedir_FAILURE <CAN'T_MOVE_PARENT_TO_CHILD_DIRIECTORY>");
   	  printf("movedir_FAILURE %s\n",ERR_VFS_MOVEDIR_06);

        return 0;
     }
            
    
    //END OF CHECK12            
               
     struct nary  *temp_nary,*parent_nary = bstnode_src->nary_node->parent;
     /// sTART mODIFYING nARY tREE //////////////////   
     /////////////////mAKE cHANGES iN sOURCE dIR/////////////////////        
     //if node n to be moved is first child
     
     //puts(parent_nary->file_desp.file_name);
     
   if(parent_nary->child==bstnode_src->nary_node)
     {
        parent_nary->child=bstnode_src->nary_node->sibling;
        //Update FD
        parent_nary->file_desp.child=bstnode_src->nary_node->file_desp.sibling;
     }
     //For other positions of n.........
     else
     {
        temp_nary = parent_nary->child;
            // puts(temp_nary->file_desp.file_name);
        while(temp_nary->sibling!=bstnode_src->nary_node && temp_nary!=NULL)
        {
            temp_nary=temp_nary->sibling;
                    //     puts(temp_nary->file_desp.file_name);
        }
        temp_nary->sibling=bstnode_src->nary_node->sibling;       
        //Update FD
        temp_nary->file_desp.sibling=bstnode_src->nary_node->file_desp.sibling;
     }
     
  
     
/*

     struct nary *temp_nary,*parent_nary = bstnode_src->nary_node->parent;
     /////////////////mAKE cHANGES iN sOURCE dIR/////////////////////        
     //if node n to be moved is first child
   if(parent_nary->child==bstnode_src->nary_node)
     {
        //puts("first child");
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
*/
     

     /////////////////mAKE cHANGES iN dESTINATION dIR/////////////////////
     bstnode_src->nary_node->sibling = bstnode_dest->nary_node->child;     
     bstnode_src->nary_node->parent = bstnode_dest->nary_node;    //update parent 
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
        strcpy(new_path,dest_dir_path);
        if(strcmp(new_path,"/")!=0)
            strcat(new_path,"/");
        strcat(new_path,node->file_desp.file_name);          
        /////////////////    

        strcpy(node->file_desp.file_location_full_path,new_path); 
        /*printf("\nupdating path : %s to %s ",old_path,new_path);

        if(update_bst_full_path(old_path,new_path)==0)
        {
            puts("in movedir:unable to delete"); 
            return 0;
        }
        display_bst_all_path(BST_FULL_PATH);  
        */
        if(update_path_in_subtree(node->child)==0)
        {
            puts("movedir_FAILURE ERR_IN_DELETING_SUBTREE");
            return 0;
        }
        
        
        
        delete_from_hash(old_path,node->file_desp.file_name);
        //printf("\n\nin hash name=%s path%s\n",node->file_desp.file_name,new_path);
        
        insert_in_hashtable(node->file_desp.file_name,new_path);
        reset_bst_path(); 
   
     //int update_bst_full_path(char oldpath[],char newpath[])
     
//     puts("helooo");
  //   while(1);
     
     
	//----------------------------------------------------------
	
	
     return 1;
}



/*
List all the contents according to <flag> of the current directory
i.e. specified by <dir_name>. 
*/

int list_dir(char dir_path[],int flag, FILE *fout )
{
    struct bst *bstnode=(struct bst*)search_bst_path(BST_FULL_PATH,dir_path);
    if(bstnode==NULL)
    {
        //puts("listdir_FAILURE <CANNOT_FIND_SPECIFIED_PATH_OR_DIR>");
   	  printf("listdir_FAILURE %s\n",ERR_VFS_LISTDIR_01);
        
        return 0;
    }
    if(flag==1) //recursive listing
    {
        display_nary(bstnode->nary_node,fout);    
    }
    else //non recursive listing
    {
        struct nary *narynode= bstnode->nary_node->child;
        while(narynode!=NULL)
        {
           //printf("\n%s \t %s\n",narynode->file_desp.file_name,narynode->file_desp.file_location_full_path);
           fprintf(fout,"\n%s \t %s\n",narynode->file_desp.file_name,narynode->file_desp.file_location_full_path);
           narynode=narynode->sibling;
        }                
    }    
      return 1;
}



