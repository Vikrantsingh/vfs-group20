/*
	File: validation.h
	Description:validation of user input
	 
*/
#include<stdio.h>
#include<string.h>
#include"../include/file_system.h"
#include"../include/directory_operators.h"
#include"../include/file_operators.h"
#include"../include/ui.h"
#include"../include/structure.h"
#include"../include/string_operations.h"
/*
*/

FILE *VFS_MOUNT_POINT;
struct Meta_header METADATA;
long int MAX_VFS_SIZE=1000000000;;
long int MAX_LENGTH_OF_NAME;
int IS_VFS_MOUNTED; //1-YES 0-NO
struct nary *NARY_ROOT;
struct bst *BST_FULL_PATH;
char PRESENT_WORKING_DIRECTORY[10000];
struct hashtable *HASH_TABLE[26][26];
int *FREELIST;
 

int validate_create_vfs(char *name,int size)
{
	if(IS_VFS_MOUNTED==1)
	{
		printf(" %s VFS is alreay mounted, unmount it first\n",METADATA.label);
		return 0;
	}

	if(name == NULL)
		return 0;

	if(size < 1)
		return 0;
 
	if(size>MAX_VFS_SIZE) ////checking size limits
	{
		puts("VFS Size too large");	
		return 0;
	}
		
	if( create_vfs(name,size*1024)==1)
	{
	    puts("\ncreatevfs_SUCCESS");
	
	}
	else
	{
	    puts("createvfs_FAILURE <err no.>");	
	}	
 
}

/*
 */
int validate_mount(char *name)
{
	if(IS_VFS_MOUNTED==1)
	{
		printf(" %s VFS is alreay mounted, unmount it first\n",METADATA.label);
		return 0;
	}
	if(name == NULL)
		return 0;
	
	if(mount_vfs(name)==1)
	{
	    puts("\nmountvfs_SUCCESS");
    	IS_VFS_MOUNTED = 1;	
	}
	else
	{
	    puts("\nmountvfs_FAILURE <err no.>");	
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
		printf("\nFirst Mount VFS\n ");
		return 0;
	}
	if( name == NULL)  //any extra arguments
		return 0;	
			
	
    int status = unmount_vfs();
	if(status==1)
	{
	    puts("\nunmountvfs_SUCCESS");
    	IS_VFS_MOUNTED = 0;	
	}
	else
	{
	    puts("\nunmountvfs_FAILURE <err no.>");	
	}
	return status;	
}

/*
 */
int validate_create_file(char *path,char *name,char *infile)
{
	if(IS_VFS_MOUNTED==0)
	{
		printf("\nFirst Mount VFS\n ");
		return 0;
	}

	if(path == NULL)
		return 0;
	
	else if(name == NULL)
		return 0;

	else if(infile == NULL)
		return 0;
	
	
	int status = create(path,name,infile);   
	if(status==1)
	{
	    puts("\naddfile_SUCCESS");
     
	}
	else
	{
	    puts("\naddfile_FAILURE <err no.>");	
	}
	return status;	
}

/*
 */
int validate_list_file(char *filepath,char *outputpath)
{
	if(IS_VFS_MOUNTED==0)
	{
		printf("\nFirst Mount VFS\n ");
		return 0;
	}

	if(filepath == NULL)
		return 0;
	
	else if(outputpath == NULL)
		return 0;
 
	int status = list_file(filepath,outputpath);
	if(status==1)
	{
	    puts("\nlistfile_SUCCESS");
    
	}
	else
	{
	    puts("\nlistfile_FAILURE <err no.>");	
	}
	return status;	

}
 /*
 */
int validate_rm_file(char input[])
{
	
	if(IS_VFS_MOUNTED==0)
	{
		printf("\nFirst Mount VFS\n ");
		return 0;
	}

	char temp[100];
	strcpy(temp,input);
	char *file_name = strtok(temp, " ");	
	
	file_name = strtok(NULL, " "); //get name of file

	if(file_name == NULL)//check name of file
	{
		puts("File Name cannot be empty");
		return 0;
	}
	 return rm(file_name);

}

/*
 */
int validate_memory_statistics(char input[])
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
		printf("\nFirst Mount VFS\n ");
		return 0;
	}

	if(path == NULL)
	{
		puts("Please Enter Destination Path");
		return 0;
	}

	if(name == NULL)
	{
        puts("Please Enter New Directory Name");
		return 0;
	}


    int status = makedir_recursive(path,name);;
	if(status==1)
	{
	    puts("\nmakedir_SUCCESS");
	}
	else
	{
	    puts("\nmakedir_FAILURE <err no.>");	
	}
	return status;	

}

/*
 */
int validate_listdir( char *path, int flag, char *output_file )
{
	if(IS_VFS_MOUNTED==0)
	{
		printf("\nFirst Mount VFS\n ");
		return 0;
	}

	if(path == NULL)//check path
	{
		puts("Path cannot be empty");
        return 0;
	}
	if(flag != 0 && flag!= 1)//check flag
	{
		puts("Invalid Flag");
        return 0;
	}
	 
	//puts(path);
    FILE *fout = fopen(output_file,"a");
    if(fout==NULL)
    {
        fout = fopen(output_file,"w");     
    }
    if(fout==NULL)
    {
        puts("Cannot open outputfile");
        return 0;
    }
    fprintf(fout,"\n\nListing Files under path: %s:\n",path);
    int status = list_dir (path,flag,fout);
    fprintf(fout,"\n..done.");
    fclose(fout);
	if(status==1)
	{
	    puts("\nlistdir_SUCCESS");
	}
	else
	{
	    puts("\nlistdir_FAILURE <err no.>");	
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
    	puts("Path cannot be empty");
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
		printf("\nFirst Mount VFS\n ");
		return 0;
	}

	if(file_name == NULL)//check path
	{
		puts("Path cannot be empty");
        return 0;
	}
	if(outputfile==NULL)//check flag
	{
		puts("Invalid output  file");
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
	    puts("\nsearchfile_FAILURE <err no.>");	
	}
	return status;	


	
}

/*

*/
int validate_copy_dir(char input[])
{

}
/*

*/
int validate_delete_dir(char *path)
{
	if(IS_VFS_MOUNTED==0)
	{
		printf("\nFirst Mount VFS\n ");
		return 0;
	}

	if(path != NULL)//check path
	{

        return delete_dir(path);
	}
    else
    {
    	puts("Dir name cannot be empty");
        return 0;
    }
}
/*

*/
int validate_move_dir(char *src,char *dest)
{
    if(IS_VFS_MOUNTED==0)
	{
		printf("\nFirst Mount VFS\n ");
		return 0;
	}

	if(src == NULL)//check path
	{
	    puts("Path cannot be empty");
	    return 0;
	}
	if(dest==NULL)//check flag
	{
		puts("DEST PAth cannot be empty");
		return 0;
	}
	return move_dir (src,dest);

}



/*--------EOF--------*/
