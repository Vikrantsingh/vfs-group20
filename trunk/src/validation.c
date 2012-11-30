/*
	File: validation.h
	Description:validation of user input
	 
*/
#include<stdio.h>
#include<string.h>
#include"../include/structure.h"
#include"../include/file_system.h"
#include"../include/directory_operators.h"
#include"../include/file_operators.h"
 

#include"../include/string_operations.h"
#include<ctype.h>
/*
*/

FILE *VFS_MOUNT_POINT;
struct Meta_header METADATA;
long int MAX_VFS_SIZE=1024;
int IS_VFS_MOUNTED=0; //1-YES 0-NO
struct nary *NARY_ROOT;
struct bst *BST_FULL_PATH;
char PRESENT_WORKING_DIRECTORY[10000];
struct hashtable *HASH_TABLE[26][26];
int *FREELIST;
 

int validate_name(char name[])
{
    int len = strlen(name);
    int i;
    for(i=0;i<len;i++)
    {
        if(name[i] == '/' )
        {
            return 0;
        }
     }
    return 1;
}

 

int validate_create_vfs(char *name,int size)
{
	if(IS_VFS_MOUNTED==1)
	{
		printf("createvfs_FAILURE %s\n",ERR_VFS_MOUNT_03);
		return 0;
	}

	if(strlen(name) == 0 || (size == -20))
	{
	  //printf("createvfs_FAILURE <VFS_INSUFFICIENT_ARGUMENTS>");
	  printf("createvfs_FAILURE %s\n",ERR_VFS_CREATE_00);
	  return 0;
	}

 	if(strlen(name)>=MAX_LENGTH_OF_NAME)
	{
//	  printf("createvfs_FAILURE <VFS_LABEL_TOO_LARGE>");
	  printf("createvfs_FAILURE %s\n",ERR_VFS_CREATE_05);
	  return 0;
	}


    FILE *fp;		
	if((fp=fopen(name,"r"))!=NULL)	////checking file existence
	{
	//	printf("createvfs_FAILURE <DATA_FILE_ALREADY_EXISTS> ");		    
  	    printf("createvfs_FAILURE %s\n",ERR_VFS_CREATE_01);
		fclose(fp);
	    return 0;
	}
 
     
	if((size > MAX_VFS_SIZE) || (size < 1)) ////checking size limits
	{
//		printf("createvfs_FAILURE <INVALID_SIZE> ");	
	  printf("createvfs_FAILURE %s\n",ERR_VFS_CREATE_04);

		return 0;
	}
	
   if(validate_name(name)==0)
   {
//            printf("createvfs_FAILURE <INVALID_CHARACTER_IN_NAME> ");	
  	  printf("createvfs_FAILURE %s\n",ERR_VFS_CREATE_03);
          return 0;
   }
     			
	if( create_vfs(name,size*1024)==1)
	{
	    printf("createvfs_SUCCESS\n");
	    return 1;
	
	}
	else
	{
	   // printf("createvfs_FAILURE <CANNOT_CREATE_DATAFILE>");	
	   return 0;
	}	
 
}

/*
 */
int validate_mount(char *name)
{ 
	if(IS_VFS_MOUNTED==1)
	{
	//	printf("mountvfs_FAILURE  <VFS_ALREADY_MOUNTED>");
	  printf("mountvfs_FAILURE %s\n",ERR_VFS_MOUNT_03);

		return 0;
	}
	if(strlen(name) == 0)
	{
	//	printf("mountvfs_FAILURE  <VFS_INSUFFICIENT_ARGUMENTS>");
		  printf("mountvfs_FAILURE %s\n",ERR_VFS_MOUNT_00);

		return 0;
	}

/*  if(validate_name(name)==0) 
       {
            printf("mountvfs_FAILURE <INVALID_CHARACTER_IN_NAME> ");	
            return 0;
    }
 
  */  FILE *fp;		
	if((fp=fopen(name,"r"))==NULL)	////checking file existence
	{
		//printf("mountvfs_FAILURE <DATA_FILE_NOT_FOUND> ");		    
	  printf("mountvfs_FAILURE %s\n",ERR_VFS_MOUNT_01);

		IS_VFS_MOUNTED = 0;
	    return 0;
	}	
	fclose(fp);
	if(mount_vfs(name)==1)
	{
	    printf("mountvfs_SUCCESS\n");
    	IS_VFS_MOUNTED = 1;	
	}
	else
	{
	   // printf("mountvfs_FAILURE <err no.>");	
    	IS_VFS_MOUNTED = 0;
	}	
	return IS_VFS_MOUNTED;
}

/*
 */
int validate_unmount(char *name)
{
	if(IS_VFS_MOUNTED==0)
	{
//		printf("unmountvfs_FAILURE <VFS_NOT_MOUNTED>\n ");
	  printf("unmountvfs_FAILURE %s\n",ERR_VFS_UNMOUNT_03);

		return 0;
	} 
	if( strlen(name) == 0)  //any extra arguments
	{
//	    printf("unmountvfs_FAILURE <VFS_INSUFFICIENT_ARGUMENTS>");
	  printf("unmountvfs_FAILURE %s\n",ERR_VFS_UNMOUNT_00);

        return 0;
	}
			
 /*  if(validate_name(name)==0)
    {
            printf("unmountvfs_FAILURE <INVALID_CHARACTER_IN_NAME> ");	
            return 0;
    }
   */ 
	if(strcmp(METADATA.label,name)!=0)		
	{
//		printf("unmountvfs_FAILURE <ACCESS_DENIED>\n ");
	  printf("unmountvfs_FAILURE %s\n",ERR_VFS_UNMOUNT_01);

		return 0;	
	}
	    
	
    int status = unmount_vfs();
	if(status==1)
	{
	    printf("unmountvfs_SUCCESS\n");
    	IS_VFS_MOUNTED = 0;	
	}
	else
	{
	   // printf("unmountvfs_FAILURE <CANNOT_WRITE_TO_FILE>");	
	}
	return status;	
}

/*
 */
int validate_create_file(char *path,char *name,char *infile)
{


	if(IS_VFS_MOUNTED==0)
	{
//		printf("First Mount VFS\n ");
   	  printf("addfile_FAILURE %s\n",ERR_VFS_ADDFILE_07);

		return 0;
	}
 
	else if((strlen(path) == 0) || (strlen(name)==0) || (strlen(infile) == 0) )
	{
//	  printf("addfile_FAILURE <VFS_INSUFFICIENT_ARGUMENTS>");
   	  printf("addfile_FAILURE %s\n",ERR_VFS_ADDFILE_00);
	  return 0;
	}
		 
				
	else if(validate_name(name)==0)
     {
//            printf("addfile_FAILURE <INVALID_CHARACTER_IN_NAME> ");	
   	  printf("addfile_FAILURE %s\n",ERR_VFS_ADDFILE_02);

            return 0;
     }
             
	else if(strlen(name)>=MAX_LENGTH_OF_NAME)
	{
	  printf("addfile_FAILURE NAME_TOO_LONG\n");
	  return 0;
	}
 	else if(strlen(path)>=MAX_LENGTH_OF_PATH)
	{
	  printf("addfile_FAILURE PATH_TOO_LONG\n");
	  return 0;
	}
	
	if(strcmp(path,"/")!=0)
	 if(path[strlen(path)-1] =='/')
     {        
        path[strlen(path)-1] ='\0';
     }	 
	
	int status = create(path,name,infile);   
	if(status==1)
	{
	    printf("addfile_SUCCESS\n");
     
	}
	else
	{
//	    printf("addfile_FAILURE <UNABLE_TO_CREATE_NEWFILE>");	
	}
	return status;	
}

/*
 */
int validate_list_file(char *filepath,char *outputpath)
{
	if(IS_VFS_MOUNTED==0)
	{
//	    printf("listfile_FAILURE <VFS_NOT_MOUNTED>");	
     	printf("listfile_FAILURE %s\n",ERR_VFS_LISTFILE_04);
		return 0;
	}

	if((strlen(filepath) == 0) || (strlen(outputpath) == 0))
	{
//	    printf("listfile_FAILURE <VFS_INSUFFICIENT_ARGUMENTS>");	
     	printf("listfile_FAILURE %s\n",ERR_VFS_LISTFILE_00);
	    return 0;
	}
	
	if(strcmp(filepath,"/")!=0)	    
 	 if(filepath[strlen(filepath)-1] =='/')
     {        
        filepath[strlen(filepath)-1] ='\0';
     }	 
	if(strcmp(outputpath,"/")!=0)
	 if(outputpath[strlen(outputpath)-1] =='/')
     {        
        outputpath[strlen(outputpath)-1] ='\0';
     }	 


	int status = list_file(filepath,outputpath);
	if(status==1)
	{
	    printf("listfile_SUCCESS\n");
    
	}
	else
	{
	    //printf("listfile_FAILURE <err no.>");	
	}
	return status;	

}
 /*
 */
int validate_rm_file(char file_path[])
{
	
    if(IS_VFS_MOUNTED==0)
	{
//		printf("removefile_FAILURE <VFS_NOT_MOUNTED>\n ");
     	printf("removefile_FAILURE %s\n",ERR_VFS_REMOVEFILE_02);

		return 0;
	}
	if( strlen(file_path) == 0)  //any extra arguments
    {
 //       printf("removefile_FAILURE <VFS_INSUFFICIENT_ARGUMENTS> ");
      	printf("removefile_FAILURE %s\n",ERR_VFS_REMOVEFILE_00);

        return 0;
    }

	if(strcmp(file_path,"/")!=0)
    if(file_path[strlen(file_path)-1] =='/')
     {        
        file_path[strlen(file_path)-1] ='\0';
     }	 

    int status = rm(file_path);
	if(status==1)
	{
	    printf("removefile_SUCCESS\n");
	}
	else
	{
	    //printf("removefile_FAILURE <err no.>");	
	}
	return status;		

}

/*
 */
int validate_memory_statistics()
{
	if(IS_VFS_MOUNTED==0)
	{
		printf("First Mount VFS\n ");
		return 0;
	}

	 return memory_statistics();


}

/*
 */
int validate_freelist(char input[])
{
	if(IS_VFS_MOUNTED==0)
	{
		printf("First Mount VFS\n ");
		return 0;
	}

	 return display_freelist();


}

 
/*

*/
int validate_mkdir(char *path,char *name)
{


	if(IS_VFS_MOUNTED==0)
	{
//		printf("makedir_FAILURE <VFS_NOT_MOUNTED>\n ");
	   printf("makedir_FAILURE %s\n",ERR_VFS_MAKEDIR_05);

		return 0;
	}

	if( (strlen(path) == 0) || (strlen(name) == 0) ) 
	{
		//printf("makedir_FAILURE <VFS_INSUFFICIENT_ARGUMENTS>");
	  printf("makedir_FAILURE %s\n",ERR_VFS_MAKEDIR_00);

		return 0;
	}

	 if(validate_name(name)==0)
     { 
           // printf("makedir_FAILURE <INVALID_CHARACTER_IN_NAME> ");	
           	  printf("makedir_FAILURE %s\n",ERR_VFS_MAKEDIR_02);

            return 0;
     }	 	 
	if((strlen(path)+strlen(name)+5)>=MAX_LENGTH_OF_PATH)
	{
	  printf("makedir_FAILURE PATH_TOO_LONG\n");
	  return 0;
	}
	if(strlen(name)>=MAX_LENGTH_OF_NAME)
	{
	  printf("makedir_FAILURE NAME_TOO_LONG\n");
	  return 0;
	}


	if(strcmp(path,"/")!=0)
     if(path[strlen(path)-1] =='/')
     {        
        path[strlen(path)-1] ='\0';
     }	 

    int status = makedir_recursive(path,name);;
	if(status==1)
	{
	    printf("makedir_SUCCESS\n");
	}
	else
	{
	   // printf("makedir_FAILURE <err no.>");	
	}
	return status;	

}

/*
 */
int validate_listdir( char *path, int flag, char *output_file )
{
	if(IS_VFS_MOUNTED==0)
	{
//		printf("listdir_FAILURE <VFS_NOT_MOUNTED>\n ");
   	  printf("listdir_FAILURE %s\n",ERR_VFS_LISTDIR_03);

		return 0;
	}

	if((strlen(path) == 0) || (strlen(output_file) == 0) )//check path
	{
//		printf("listdir_FAILURE <VFS_INSUFFICIENT_ARGUMENTS>");
   	  printf("listdir_FAILURE %s\n",ERR_VFS_LISTDIR_00);
        return 0;
	}
	if(flag != 0 && flag!= 1)//check flag
	{
    	  printf("listdir_FAILURE %s\n",ERR_VFS_LISTDIR_02);
//		printf("listdir_FAILURE <INVALID_FLAG>");
        return 0;
	}
	 	 	//printf(path);
	 	 	
 	if(strcmp(path,"/")!=0)
     if(path[strlen(path)-1] =='/')
     {        
        path[strlen(path)-1] ='\0';
     }	 
 	 	
    FILE *fout = fopen(output_file,"a");
    if(fout==NULL)
    {
        fout = fopen(output_file,"w");     
    }
    if(fout==NULL)
    {
        //printf("listdir_FAILURE <CANNOT_FIND_OUTPUTFILE>");
         printf("listdir_FAILURE %s\n",ERR_VFS_LISTDIR_04);

        return 0;
    }
    fprintf(fout,"\n\n#Listing Files under path: %s:\n",path);
    int status = list_dir (path,flag,fout);
    fprintf(fout,"\n..Listing done.");
    fclose(fout);
	if(status==1)
	{
	    printf("listdir_SUCCESS\n");
	}
	else
	{
	  //  printf("listdir_FAILURE <err no.>");	
	}
	return status;	
}


/*
 */
int validate_change_dir(char input[])
{
	if(IS_VFS_MOUNTED==0)
	{
		printf("First Mount VFS\n ");
		return 0;
	}


	char temp[1000],default_path[1000];
	strcpy(temp,input);
	char *path = strtok(temp, " ");	
	
	path = strtok(NULL, " "); //get path

	if(path != NULL)//check path
	{
		strcpy(PRESENT_WORKING_DIRECTORY,path);
    	path=default_path;
        return 1;
	}
    else
    {
    	//printf("Path cannot be empty");
        return -1;
    }
	//printf(path);
	
}


/*
 */
int validate_find(char *file_name, char *outputfile)
{
	if(IS_VFS_MOUNTED==0)
	{
//		printf("searchfile_FAILURE <VFS_NOT_MOUNTED>\n ");
     	printf("searchfile_FAILURE %s\n",ERR_VFS_SEARCHFILE_02);

		return 0;
	}

	if((strlen(file_name) == 0) ||(strlen(outputfile)==0) )//check path
	{
     	printf("searchfile_FAILURE %s\n",ERR_VFS_SEARCHFILE_00);
        return 0;
	}
  
	
	//printf(path);

    int status = find_file(file_name,outputfile);
	if(status==1)
	{
	    printf("searchfile_SUCCESS\n");
	}
	else
	{
	    //printf("searchfile_FAILURE <UNKNOWN>");	
	}
	return status;	

}

/* 

*/
int validate_copy_dir(char input[])
{
    ///NOT MENTION IN PROBLEM STATEMENT SOOO SKIPPING
}
/*

*/
int validate_delete_dir(char *path)
{
	if(IS_VFS_MOUNTED==0)
	{
		//printf("deletedir_FAILURE <VFS_NOT_Mounted>\n ");
	    printf("deletedir_FAILURE %s\n",ERR_VFS_DELETEDIR_04);
		return 0;
	}

	if(strlen(path) == 0)//check path
	{
//		printf("deletedir_FAILURE <VFS_INSUFFICIENT_ARGUMENTS>\n ");
      	  printf("deletedir_FAILURE %s\n",ERR_VFS_DELETEDIR_00);
        return 0;      
	} 
  
  
  	if(strcmp(path,"/")!=0)
     if(path[strlen(path)-1] =='/')
     {        
        path[strlen(path)-1] ='\0';
     }	 
 
    int status =  delete_dir(path);
	if(status==1)
	{
	    printf("deletedir_SUCCESS\n");
	}
	else
	{
//	    printf("deletedir_FAILURE <err no.>");	
	}
	return status;	
}
/*

*/

int separate_path2(char dir_struct[MAX_SUB_DIRECTORY_LEVEL][MAX_LENGTH_OF_NAME],char parent_path[],int *total_dir_in_path)
{
	if(strlen(parent_path)>=MAX_LENGTH_OF_PATH)
	{
	  printf("Path too long\n");
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
	    //printf(dir_struct[i]);	    
	    i++;
	}
	if(i==MAX_SUB_DIRECTORY_LEVEL)
    	return 0;
	*total_dir_in_path=i;
	//printf("total=%d",*total_dir_in_path);
	return 1;
        
}

int validate_move_dir(char *src,char *dest)
{ 
    if(IS_VFS_MOUNTED==0)
	{
//		printf("movedir_FAILURE <VFS_NOT_MOUNTED>\n ");
   	  printf("movedir_FAILURE %s\n",ERR_VFS_MOVEDIR_08);

		return 0;
	}

	if( (strlen(src) == 0) || (strlen(dest)==0) )//check path
	{
//	    printf("movedir_FAILURE <VFS_INSUFFICIENT_ARGUMENTS>");
   	  printf("movedir_FAILURE %s\n",ERR_VFS_MOVEDIR_00);
	    return 0;
	}
	 

   

  
/*
  ////////Check dest IsChild
    int i,src_dir,dest_dir;
    
     char src_dir_struct[MAX_SUB_DIRECTORY_LEVEL][MAX_LENGTH_OF_NAME];//limiting max sub directory level 
     char dest_dir_struct[MAX_SUB_DIRECTORY_LEVEL][MAX_LENGTH_OF_NAME];//limiting max sub directory level 

    i = separate_path2(src_dir_struct,src,&src_dir);
    if(i==0)
	{ 
   	     printf("movedir_FAILURE <UNABLE_TO_PROCESS_PATH>");
	     return 0;
	}
    i = separate_path2(dest_dir_struct,dest,&dest_dir);
    if(i==0)
	{ 
   	     printf("movedir_FAILURE <UNABLE_TO_PROCESS_PATH>");
	     return 0;
	}
	int flag = 1;
	for(i=0; i<src_dir && i < dest_dir ;i++ )	
	{
	    if(strcmp(src_dir_struct[i],dest_dir_struct[i])!=0)
	    {
	        flag=0;
	        break;
	    }
	}
    if(flag==1 )
    {
   	     printf("movedir_FAILURE <CAN'T_MOVE_PARENT_TO_CHILD_DIRIECTORY>");
	     return 0;
    
    }
    */
   	if(strcmp(src,"/")!=0)
     if(src[strlen(src)-1] =='/')
     {        
        src[strlen(src)-1] ='\0';
     }	 
	if(strcmp(dest,"/")!=0)     
     if(dest[strlen(dest)-1] =='/')
     {        
        dest[strlen(dest)-1] ='\0';
     }	 
    ////////////////////////////////////	
    int status =   move_dir (src,dest);
	if(status==1)
	{
	    printf("movedir_SUCCESS\n");
	}
	else
	{
	    //printf("movedir_FAILURE <err no.>");	
	}
	return status;	

}

int validate_move_file(char *src,char *dest)
{
    if(IS_VFS_MOUNTED==0)
	{
//		printf("movefile_FAILURE <VFS_NOT_MOUNTED>\n ");
     	printf("movefile_FAILURE %s\n",ERR_VFS_MOVEFILE_06);


		return 0;
	}

	if ((strlen(src) == 0)||(strlen(dest)==0))//check path
	{
//	    printf("movefile_FAILURE <VFS_INSUFFICIENT_ARGUMENTS>");
     	printf("movefile_FAILURE %s\n",ERR_VFS_MOVEFILE_00);

	    return 0;
	}
	
	if(strcmp(src,"/")!=0)
     if(src[strlen(src)-1] =='/')
     {        
        src[strlen(src)-1] ='\0';
     }	 
	if(strcmp(dest,"/")!=0)    
     if(dest[strlen(dest)-1] =='/')
     {        
        dest[strlen(dest)-1] ='\0';
     }	 
 	
    int status =   mv(src,dest);
	if(status==1)
	{
	    printf("movefile_SUCCESS\n");
	}
	else
	{
	    //printf("movefile_FAILURE <err no.>");	
	}
	return status;	

}


int validate_export_file(char *src,char *dest)
{
    if(IS_VFS_MOUNTED==0)
	{
//		printf("exportfile_FAILURE <VFS_NOT_MOUNTED>\n ");
     	printf("exportfile_FAILURE %s\n",ERR_VFS_EXPORTFILE_04);
		return 0;
	}

	if((strlen(src) == 0)  || (strlen(dest)==0))//check path
	{
//	    printf("exportfile_FAILURE <VFS_INSUFFICIENT_ARGUMENTS>");
     	printf("exportfile_FAILURE %s\n",ERR_VFS_EXPORTFILE_00);

	    return 0;
	}
	
	 
    if(strcmp(src,"/")!=0) 
     if(src[strlen(src)-1] =='/')
     {        
        src[strlen(src)-1] ='\0';
     }	 
  	if(strcmp(dest,"/")!=0)
     if(dest[strlen(dest)-1] =='/')
     {        
        dest[strlen(dest)-1] ='\0';
     }	 
	
	FILE *fp=fopen(dest,"r");
	if(fp!=NULL)
	{
	    
	     printf("exportfile_FAILURE OUTPUT_FILE_ALREADY_EXISTS\n");   
	     fclose(fp);	
	     return 0;	    
	}

    int status =   export_file(src,dest);
	if(status==1)
	{
	    printf("exportfile_SUCCESS\n");
	}
	else
	{
	    //printf("exportfile_FAILURE <err no.>");	
	}
	return status;	

}



int validate_copy_file(char *src,char *dest)
{
    if(IS_VFS_MOUNTED==0)
	{
//		printf("copyfile_FAILURE <VFS_NOT_MOUNTED>\n ");
     	printf("copyfile_FAILURE %s\n",ERR_VFS_COPYFILE_05);

		return 0;
	} 
	
	if((strlen(src) == 0) || (strlen(dest)==0))//check path
	{
//	    printf("copyfile_FAILURE <VFS_INSUFFICIENT_ARGUMENTS>");
     	printf("copyfile_FAILURE %s\n",ERR_VFS_COPYFILE_00);

	    return 0;
	}
 
 	if(strcmp(src,"/")!=0)
     if(src[strlen(src)-1] =='/')
     {        
        src[strlen(src)-1] ='\0';
     }	 
  	if(strcmp(dest,"/")!=0) 
     if(dest[strlen(dest)-1] =='/')
     {        
        dest[strlen(dest)-1] ='\0';
     }	 
	
    int status =   copy_file(src,dest); 
	if(status==1)
	{
	    printf("copyfile_SUCCESS\n");
	}
	else 
	{
	    //printf("copyfile_FAILURE <err no.>");	
	}
	return status;	
}


int validate_update_file(char *path,char *fin)
{
    if(IS_VFS_MOUNTED==0)
	{
//		printf("updatefile_FAILURE <VFS_NOT_MOUNTED>\n ");
     	printf("updatefile_FAILURE %s\n",ERR_VFS_UPDATEFILE_04);

		return 0;
	}

	if((strlen(path) == 0) || (strlen(fin)==0)  )//check path
	{
//	    printf("updatefile_FAILURE <VFS_INSUFFICIENT_ARGUMENTS>");
     	printf("updatefile_FAILURE %s\n",ERR_VFS_UPDATEFILE_00);
	    return 0;
	}
	
 	if(strcmp(path,"/")!=0)
     if(path[strlen(path)-1] =='/')
     {        
        path[strlen(path)-1] ='\0';
     }	 

     int status =  update_file(path,fin); 
     
    ////////////////update file same as delete old one and add new one...
    
    //printf("heloo");
	if(status == 1)
	{
	    printf("updatefile_SUCCESS\n");
	}
	else 
	{
	   // printf("updatefile_FAILURE <err no.>");	
	}
	return status;	
}

/*--------EOF--------*/
