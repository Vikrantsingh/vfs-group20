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
		printf("\ncreatevfs_FAILURE <%s VFS is alreay mounted, unmount it first>\n",METADATA.label);
		return 0;
	}

	if(strlen(name) == 0 || (size == -20))
	{
	  //puts("createvfs_FAILURE <VFS_INSUFFICIENT_ARGUMENTS>");
	  printf("\ncreatevfs_FAILURE %s",ERR_VFS_CREATE_00);
	  return 0;
	}

 	if(strlen(name)>=MAX_LENGTH_OF_NAME)
	{
//	  puts("createvfs_FAILURE <VFS_LABEL_TOO_LARGE>");
	  printf("\ncreatevfs_FAILURE %s",ERR_VFS_CREATE_05);
	  return 0;
	}


    FILE *fp;		
	if((fp=fopen(name,"r"))!=NULL)	////checking file existence
	{
	//	puts("createvfs_FAILURE <DATA_FILE_ALREADY_EXISTS> ");		    
  	    printf("\ncreatevfs_FAILURE %s",ERR_VFS_CREATE_01);
		fclose(fp);
	    return 0;
	}
 
     
	if((size > MAX_VFS_SIZE) || (size < 1)) ////checking size limits
	{
//		puts("createvfs_FAILURE <INVALID_SIZE> ");	
	  printf("\ncreatevfs_FAILURE %s",ERR_VFS_CREATE_04);

		return 0;
	}
	
   if(validate_name(name)==0)
   {
//            puts("createvfs_FAILURE <INVALID_CHARACTER_IN_NAME> ");	
  	  printf("\ncreatevfs_FAILURE %s",ERR_VFS_CREATE_03);
          return 0;
   }
     			
	if( create_vfs(name,size*1024)==1)
	{
	    puts("\ncreatevfs_SUCCESS");
	    return 1;
	
	}
	else
	{
	   // puts("createvfs_FAILURE <CANNOT_CREATE_DATAFILE>");	
	   return 0;
	}	
 
}

/*
 */
int validate_mount(char *name)
{ 
	if(IS_VFS_MOUNTED==1)
	{
	//	puts("mountvfs_FAILURE  <VFS_ALREADY_MOUNTED>");
	  printf("\nmountvfs_FAILURE %s",ERR_VFS_MOUNT_03);

		return 0;
	}
	if(strlen(name) == 0)
	{
	//	puts("mountvfs_FAILURE  <VFS_INSUFFICIENT_ARGUMENTS>");
		  printf("\nmountvfs_FAILURE %s",ERR_VFS_MOUNT_00);

		return 0;
	}

/*  if(validate_name(name)==0) 
       {
            puts("mountvfs_FAILURE <INVALID_CHARACTER_IN_NAME> ");	
            return 0;
    }
 
  */  FILE *fp;		
	if((fp=fopen(name,"r"))==NULL)	////checking file existence
	{
		//puts("mountvfs_FAILURE <DATA_FILE_NOT_FOUND> ");		    
	  printf("\nmountvfs_FAILURE %s",ERR_VFS_MOUNT_01);

		IS_VFS_MOUNTED = 0;
	    return 0;
	}	
	fclose(fp);
	if(mount_vfs(name)==1)
	{
	    puts("\nmountvfs_SUCCESS");
    	IS_VFS_MOUNTED = 1;	
	}
	else
	{
	   // puts("\nmountvfs_FAILURE <err no.>");	
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
//		printf("\nunmountvfs_FAILURE <VFS_NOT_MOUNTED>\n ");
	  printf("\nunmountvfs_FAILURE %s",ERR_VFS_UNMOUNT_03);

		return 0;
	} 
	if( strlen(name) == 0)  //any extra arguments
	{
//	    puts("\nunmountvfs_FAILURE <VFS_INSUFFICIENT_ARGUMENTS>");
	  printf("\nunmountvfs_FAILURE %s",ERR_VFS_UNMOUNT_00);

        return 0;
	}
			
 /*  if(validate_name(name)==0)
    {
            puts("unmountvfs_FAILURE <INVALID_CHARACTER_IN_NAME> ");	
            return 0;
    }
   */ 
	if(strcmp(METADATA.label,name)!=0)		
	{
//		printf("\nunmountvfs_FAILURE <ACCESS_DENIED>\n ");
	  printf("\nunmountvfs_FAILURE %s",ERR_VFS_UNMOUNT_01);

		return 0;	
	}
	    
	
    int status = unmount_vfs();
	if(status==1)
	{
	    puts("\nunmountvfs_SUCCESS");
    	IS_VFS_MOUNTED = 0;	
	}
	else
	{
	   // puts("\nunmountvfs_FAILURE <CANNOT_WRITE_TO_FILE>");	
	}
	return status;	
}

/*
 */
int validate_create_file(char *path,char *name,char *infile)
{


	if(IS_VFS_MOUNTED==0)
	{
//		printf("\nFirst Mount VFS\n ");
   	  printf("\naddfile_FAILURE %s",ERR_VFS_ADDFILE_07);

		return 0;
	}

	else if((strlen(path) == 0) || (strlen(name)==0) || (strlen(infile) == 0) )
	{
//	  puts("addfile_FAILURE <VFS_INSUFFICIENT_ARGUMENTS>");
   	  printf("\naddfile_FAILURE %s",ERR_VFS_ADDFILE_00);
	  return 0;
	}
		 
				
	else if(validate_name(name)==0)
     {
//            puts("addfile_FAILURE <INVALID_CHARACTER_IN_NAME> ");	
   	  printf("\naddfile_FAILURE %s",ERR_VFS_ADDFILE_02);

            return 0;
     }
             
	else if(strlen(name)>=MAX_LENGTH_OF_NAME)
	{
	  puts("addfile_FAILURE <NAME_TOO_LONG>");
	  return 0;
	}
 	else if(strlen(path)>=MAX_LENGTH_OF_PATH)
	{
	  puts("addfile_FAILURE <PATH_TOO_LONG>");
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
	    puts("\naddfile_SUCCESS");
     
	}
	else
	{
//	    puts("\naddfile_FAILURE <UNABLE_TO_CREATE_NEWFILE>");	
	}
	return status;	
}

/*
 */
int validate_list_file(char *filepath,char *outputpath)
{
	if(IS_VFS_MOUNTED==0)
	{
//	    puts("\nlistfile_FAILURE <VFS_NOT_MOUNTED>");	
     	printf("\nlistfile_FAILURE %s",ERR_VFS_LISTFILE_04);
		return 0;
	}

	if((strlen(filepath) == 0) || (strlen(outputpath) == 0))
	{
//	    puts("\nlistfile_FAILURE <VFS_INSUFFICIENT_ARGUMENTS>");	
     	printf("\nlistfile_FAILURE %s",ERR_VFS_LISTFILE_00);
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
	    puts("\nlistfile_SUCCESS");
    
	}
	else
	{
	    //puts("\nlistfile_FAILURE <err no.>");	
	}
	return status;	

}
 /*
 */
int validate_rm_file(char file_path[])
{
	
    if(IS_VFS_MOUNTED==0)
	{
//		printf("\nremovefile_FAILURE <VFS_NOT_MOUNTED>\n ");
     	printf("\nremovefile_FAILURE %s",ERR_VFS_REMOVEFILE_02);

		return 0;
	}
	if( strlen(file_path) == 0)  //any extra arguments
    {
 //       puts("removefile_FAILURE <VFS_INSUFFICIENT_ARGUMENTS> ");
      	printf("\nremovefile_FAILURE %s",ERR_VFS_REMOVEFILE_00);

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
	    puts("removefile_SUCCESS");
	}
	else
	{
	    //puts("\nremovefile_FAILURE <err no.>");	
	}
	return status;		

}

/*
 */
int validate_memory_statistics()
{
	if(IS_VFS_MOUNTED==0)
	{
		printf("\nFirst Mount VFS\n ");
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
		printf("\nFirst Mount VFS\n ");
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
//		printf("\nmakedir_FAILURE <VFS_NOT_MOUNTED>\n ");
	   printf("\nmakedir_FAILURE %s",ERR_VFS_MAKEDIR_05);

		return 0;
	}

	if( (strlen(path) == 0) || (strlen(name) == 0) ) 
	{
		//puts("makedir_FAILURE <VFS_INSUFFICIENT_ARGUMENTS>");
	  printf("\nmakedir_FAILURE %s",ERR_VFS_MAKEDIR_00);

		return 0;
	}

	 if(validate_name(name)==0)
     { 
           // puts("makedir_FAILURE <INVALID_CHARACTER_IN_NAME> ");	
           	  printf("\nmakedir_FAILURE %s",ERR_VFS_MAKEDIR_02);

            return 0;
     }	 	 
	if((strlen(path)+strlen(name)+5)>=MAX_LENGTH_OF_PATH)
	{
	  puts("makedir_FAILURE <PATH_TOO_LONG>");
	  return 0;
	}
	if(strlen(name)>=MAX_LENGTH_OF_NAME)
	{
	  puts("makedir_FAILURE <NAME_TOO_LONG>");
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
	    puts("\nmakedir_SUCCESS");
	}
	else
	{
	   // puts("\nmakedir_FAILURE <err no.>");	
	}
	return status;	

}

/*
 */
int validate_listdir( char *path, int flag, char *output_file )
{
	if(IS_VFS_MOUNTED==0)
	{
//		printf("\nlistdir_FAILURE <VFS_NOT_MOUNTED>\n ");
   	  printf("\nlistdir_FAILURE %s",ERR_VFS_LISTDIR_03);

		return 0;
	}

	if((strlen(path) == 0) || (strlen(output_file) == 0) )//check path
	{
//		puts("listdir_FAILURE <VFS_INSUFFICIENT_ARGUMENTS>");
   	  printf("\nlistdir_FAILURE %s",ERR_VFS_LISTDIR_00);
        return 0;
	}
	if(flag != 0 && flag!= 1)//check flag
	{
    	  printf("\nlistdir_FAILURE %s",ERR_VFS_LISTDIR_02);
//		puts("listdir_FAILURE <INVALID_FLAG>");
        return 0;
	}
	 	 	//puts(path);
	 	 	
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
        //puts("listdir_FAILURE <CANNOT_FIND_OUTPUTFILE>");
         printf("\nlistdir_FAILURE %s",ERR_VFS_LISTDIR_04);

        return 0;
    }
    fprintf(fout,"\n\n#Listing Files under path: %s:\n",path);
    int status = list_dir (path,flag,fout);
    fprintf(fout,"\n..Listing done.");
    fclose(fout);
	if(status==1)
	{
	    puts("\nlistdir_SUCCESS");
	}
	else
	{
	  //  puts("\nlistdir_FAILURE <err no.>");	
	}
	return status;	
}


/*
 */
int validate_change_dir(char input[])
{
	if(IS_VFS_MOUNTED==0)
	{
		printf("\nFirst Mount VFS\n ");
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
    	//puts("Path cannot be empty");
        return -1;
    }
	//puts(path);
	
}


/*
 */
int validate_find(char *file_name, char *outputfile)
{
	if(IS_VFS_MOUNTED==0)
	{
//		printf("\nsearchfile_FAILURE <VFS_NOT_MOUNTED>\n ");
     	printf("\nsearchfile_FAILURE %s",ERR_VFS_SEARCHFILE_02);

		return 0;
	}

	if((strlen(file_name) == 0) ||(strlen(outputfile)==0) )//check path
	{
     	printf("\nsearchfile_FAILURE %s",ERR_VFS_SEARCHFILE_00);
        return 0;
	}
  
	
	//puts(path);

    int status = find_file(file_name,outputfile);
	if(status==1)
	{
	    puts("\nsearchfile_SUCCESS");
	}
	else
	{
	    //puts("\nsearchfile_FAILURE <UNKNOWN>");	
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
		//printf("\ndeletedir_FAILURE <VFS_NOT_Mounted>\n ");
	    printf("\ndeletedir_FAILURE %s",ERR_VFS_DELETEDIR_04);
		return 0;
	}

	if(strlen(path) == 0)//check path
	{
//		printf("\ndeletedir_FAILURE <VFS_INSUFFICIENT_ARGUMENTS>\n ");
      	  printf("\ndeletedir_FAILURE %s",ERR_VFS_DELETEDIR_00);
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
	    puts("\ndeletedir_SUCCESS");
	}
	else
	{
//	    puts("\ndeletedir_FAILURE <err no.>");	
	}
	return status;	
}
/*

*/

int separate_path2(char dir_struct[MAX_SUB_DIRECTORY_LEVEL][MAX_LENGTH_OF_NAME],char parent_path[],int *total_dir_in_path)
{
	if(strlen(parent_path)>=MAX_LENGTH_OF_PATH)
	{
	  puts("Path too long.");
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
	//printf("\ntotal=%d",*total_dir_in_path);
	return 1;
        
}

int validate_move_dir(char *src,char *dest)
{ 
    if(IS_VFS_MOUNTED==0)
	{
//		printf("\nmovedir_FAILURE <VFS_NOT_MOUNTED>\n ");
   	  printf("\nmovedir_FAILURE %s",ERR_VFS_MOVEDIR_08);

		return 0;
	}

	if( (strlen(src) == 0) || (strlen(dest)==0) )//check path
	{
//	    puts("movedir_FAILURE <VFS_INSUFFICIENT_ARGUMENTS>");
   	  printf("\nmovedir_FAILURE %s",ERR_VFS_MOVEDIR_00);
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
   	     puts("movedir_FAILURE <UNABLE_TO_PROCESS_PATH>");
	     return 0;
	}
    i = separate_path2(dest_dir_struct,dest,&dest_dir);
    if(i==0)
	{ 
   	     puts("movedir_FAILURE <UNABLE_TO_PROCESS_PATH>");
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
   	     puts("movedir_FAILURE <CAN'T_MOVE_PARENT_TO_CHILD_DIRIECTORY>");
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
	    puts("\nmovedir_SUCCESS");
	}
	else
	{
	    //puts("\nmovedir_FAILURE <err no.>");	
	}
	return status;	

}

int validate_move_file(char *src,char *dest)
{
    if(IS_VFS_MOUNTED==0)
	{
//		printf("\nmovefile_FAILURE <VFS_NOT_MOUNTED>\n ");
     	printf("\nmovefile_FAILURE %s",ERR_VFS_MOVEFILE_06);


		return 0;
	}

	if ((strlen(src) == 0)||(strlen(dest)==0))//check path
	{
//	    puts("movefile_FAILURE <VFS_INSUFFICIENT_ARGUMENTS>");
     	printf("\nmovefile_FAILURE %s",ERR_VFS_MOVEFILE_00);

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
	    puts("\nmovefile_SUCCESS");
	}
	else
	{
	    //puts("\nmovefile_FAILURE <err no.>");	
	}
	return status;	

}


int validate_export_file(char *src,char *dest)
{
    if(IS_VFS_MOUNTED==0)
	{
//		printf("\nexportfile_FAILURE <VFS_NOT_MOUNTED>\n ");
     	printf("\nexportfile_FAILURE %s",ERR_VFS_EXPORTFILE_04);
		return 0;
	}

	if((strlen(src) == 0)  || (strlen(dest)==0))//check path
	{
//	    puts("exportfile_FAILURE <VFS_INSUFFICIENT_ARGUMENTS>");
     	printf("\nexportfile_FAILURE %s",ERR_VFS_EXPORTFILE_00);

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
	    
	     puts("exportfile_FAILURE <OUTPUT_FILE_ALREADY_EXISTS>");   
	     fclose(fp);	
	     return 0;	    
	}

    int status =   export_file(src,dest);
	if(status==1)
	{
	    puts("\nexportfile_SUCCESS");
	}
	else
	{
	    //puts("\nexportfile_FAILURE <err no.>");	
	}
	return status;	

}



int validate_copy_file(char *src,char *dest)
{
    if(IS_VFS_MOUNTED==0)
	{
//		printf("\ncopyfile_FAILURE <VFS_NOT_MOUNTED>\n ");
     	printf("\ncopyfile_FAILURE %s",ERR_VFS_COPYFILE_05);

		return 0;
	} 
	
	if((strlen(src) == 0) || (strlen(dest)==0))//check path
	{
//	    puts("copyfile_FAILURE <VFS_INSUFFICIENT_ARGUMENTS>");
     	printf("\ncopyfile_FAILURE %s",ERR_VFS_COPYFILE_00);

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
	    puts("\ncopyfile_SUCCESS");
	}
	else 
	{
	    //puts("\ncopyfile_FAILURE <err no.>");	
	}
	return status;	
}


int validate_update_file(char *path,char *fin)
{
    if(IS_VFS_MOUNTED==0)
	{
//		printf("\nupdatefile_FAILURE <VFS_NOT_MOUNTED>\n ");
     	printf("\nupdatefile_FAILURE %s",ERR_VFS_UPDATEFILE_04);

		return 0;
	}

	if((strlen(path) == 0) || (strlen(fin)==0)  )//check path
	{
//	    puts("updatefile_FAILURE <VFS_INSUFFICIENT_ARGUMENTS>");
     	printf("\nupdatefile_FAILURE %s",ERR_VFS_UPDATEFILE_00);
	    return 0;
	}
	
 	if(strcmp(path,"/")!=0)
     if(path[strlen(path)-1] =='/')
     {        
        path[strlen(path)-1] ='\0';
     }	 

     int status =  update_file(path,fin); 
     
    ////////////////update file same as delete old one and add new one...
    
    //puts("heloo");
	if(status == 1)
	{
	    puts("\nupdatefile_SUCCESS");
	}
	else 
	{
	   // puts("\nupdatefile_FAILURE <err no.>");	
	}
	return status;	
}

/*--------EOF--------*/
