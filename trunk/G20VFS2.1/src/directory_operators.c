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
 
 
	//puts("Searching file");
	
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

int makedir_recursive(char parent_path[],char dir_name[])
{
    //puts("Recr:");
    char *dir_struct[25];//limiting max sub directory level to 25  
	int i,total_dir_in_path;
	int j,k;
	struct nary *parent; 
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

    //Check if file/dir  already exists
    char temp_path[500],temp_path2[500];
    strcpy(temp_path,parent_path);
    strcat(temp_path,"/");
    strcat(temp_path,dir_name);
    
    struct bst *bstnode = (struct bst*)search_bst_path(BST_FULL_PATH,temp_path);

    if(bstnode!=NULL)
    {
        puts("Dir with same name exists.");
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

    //parent = get_parent_nary(dir_struct,total_dir_in_path);
    bstnode = (struct bst*) search_bst_path(BST_FULL_PATH,parent_path);

    if(bstnode==NULL)
    {   
        /////////nOW wE hAVE tO cREATE aLL dIR rECURRSIVELY

   
//        while(1);

        for(j=0;j<total_dir_in_path;j++)        
        {
            strcpy(temp_path,"");
            strcpy(temp_path2,"");
            for(k=0;k<=j;k++)
            {
               strcat(temp_path,"/");
               strcat(temp_path,dir_struct[k]);
               if(k<j)
               strcpy(temp_path2,temp_path);
            }
            
            bstnode =(struct bst*) search_bst_path(BST_FULL_PATH,temp_path); 
            if(bstnode==NULL)
            /////////////wRITE cODE tO aDD nEW dIR
            {
                   if(make_dir(temp_path2,dir_struct[k-1])==0)
                        return 0;
                   //printf("\npath %s name %s",temp_path2,dir_struct[k-1]);
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
  

}


int make_dir(char parent_path[],char dir_name[])
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
    strcat(temp_path,dir_name);
    
    struct bst *bstnode = (struct bst*)search_bst_path(BST_FULL_PATH,temp_path);

    if(bstnode!=NULL)
    {
        puts("Dir with same name exists.");
        return 0;
    }
    

    //parent = get_parent_nary(dir_struct,total_dir_in_path);
    bstnode = (struct bst*)search_bst_path(BST_FULL_PATH,parent_path);

    if(bstnode==NULL)
    {
        puts("path not found");
        return 0;
    }
    
    parent = bstnode->nary_node;
    
    //printf("Got ii parent=%s",parent->file_desp.file_name);
   
//	Create & Store FD 
	struct file_descriptor file_desp,temp;
	//Storing File Desp data in Struct variable
	strcpy(file_desp.file_name,dir_name);
		//////////////Store Path
	strcpy(file_desp.file_location_full_path,parent_path);
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

int delete_dir(char path[])
{
    if(strcmp(path,"/root")==0)
    {
        puts("Access Denied...");
        return 0;
    }
	//puts("In deletedir");
	

    //--------------------------------------------------
    
    
    //delete from N-ARy & BST & Hash TABLE

     struct bst *bstnode = (struct bst*)search_bst_path(BST_FULL_PATH,path);
     if(bstnode==NULL)
     {
        puts("File not found");
        return 0;
     }
               
     struct nary *pre,*temp = bstnode->nary_node->parent;
     
     //if node n to be deleted is first child
     
     if(temp->child==bstnode->nary_node)
     {
        temp->child=bstnode->nary_node->sibling;
        //Update FD
        temp->file_desp.child=bstnode->nary_node->file_desp.sibling;
     }
     //For other positions of n.........
     else
     {
        pre=NULL;
        temp = temp->child;
        while(temp->sibling!=bstnode->nary_node)
        {
            temp=temp->sibling;
        }
        temp->sibling=bstnode->nary_node->sibling;       
        //Update FD
        temp->file_desp.sibling=bstnode->nary_node->file_desp.sibling;
     }
     
     //To delete subtree rooted by n
     
     ///// tHIS fUNCTION iNTERNALLY caLLS dELETE BST & dELETE hASHTABLE ////
     
     delete_subtree_from_nary(bstnode->nary_node);
    
	//----------------------------------------------------------
	
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

     struct bst *bstnode_src = (struct bst*)search_bst_path(BST_FULL_PATH,src_dir_path);
     if(bstnode_src==NULL)
     {
        puts("Source path not found");
        return 0;
     }

     struct bst *bstnode_dest = (struct bst*)search_bst_path(BST_FULL_PATH,dest_dir_path);
     if(bstnode_dest==NULL)
     {
        puts("Destination path not found");
        return 0;
     }
                
               
     struct nary *pre,*temp = bstnode_src->nary_node->parent;
     /// sTART mODIFYING nARY tREE //////////////////   
     /////////////////mAKE cHANGES iN sOURCE dIR/////////////////////        
     //if node n to be moved is first child
     
     if(temp->child==bstnode_src->nary_node)
     {
        temp->child=bstnode_src->nary_node->sibling;
        //Update FD
        temp->file_desp.child=bstnode_src->nary_node->file_desp.sibling;
     }
     //For other positions of n.........
     else
     {
        pre=NULL;
        temp = temp->child;
        while(temp->sibling!=bstnode_src->nary_node)
        {
            temp=temp->sibling;
        }
        temp->sibling=bstnode_src->nary_node->sibling;       
        //Update FD
        temp->file_desp.sibling=bstnode_src->nary_node->file_desp.sibling;
     }
     /////////////////mAKE cHANGES iN dESTINATION dIR/////////////////////
     bstnode_src->nary_node->sibling = bstnode_dest->nary_node->child;     
     bstnode_dest->nary_node->child= bstnode_src->nary_node;
     //uPDATE FD'S      
     bstnode_src->nary_node->file_desp.sibling = bstnode_dest->nary_node->file_desp.child;     
     bstnode_dest->nary_node->file_desp.child = bstnode_src->nary_node->fd_loc;     
     
     /// eND mODIFYING nARY tREE //////////////////
     
      
     ///// uPATES  BST & hASHTABLE ////
         ///cALL uPDATE bST
        struct nary *node = bstnode_src->nary_node;         
        char new_path[500],old_path[500];
        //
        strcpy(old_path,node->file_desp.file_location_full_path);   
        //
        strcpy(new_path,dest_dir_path);
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
            puts("in movedir: err in deleting subtree");
            return 0;
        }
        reset_bst_path(); 
   
     //int update_bst_full_path(char oldpath[],char newpath[])
     
     
     //delete_subtree_from_nary(bstnode->nary_node);
    
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
        puts("Path Not Found");
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
           //printf("\n%s \t %s",narynode->file_desp.file_name,narynode->file_desp.file_location_full_path);
           fprintf(fout,"\n%s \t %s",narynode->file_desp.file_name,narynode->file_desp.file_location_full_path);
           narynode=narynode->sibling;
        }                
    }    
      return 1;
}



