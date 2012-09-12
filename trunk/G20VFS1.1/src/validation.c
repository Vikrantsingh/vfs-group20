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
int validate_create_vfs(char input[])
{
	if(IS_VFS_MOUNTED==1)
	{
		printf(" %s VFS is alreay mounted, unmount it first\n",METADATA.label);
		return 0;
	}

	char temp[10000],*ch,*name,*size;	
	int i=0;
	strcpy(temp,input);
	ch = strtok(temp, " ");	
	
	name = strtok(NULL, " "); //check name args
	if(name == NULL)
		return 0;
	if(strlen(name)>MAX_LENGTH_OF_NAME)
		{
			puts("File Name too Large");
			return 0;
		}	

	size = strtok(NULL, " "); //check size args
	if(size == NULL)
		return 0;

	else ///check size is int/alphanumeric
	{
  		for(i=0;i<strlen(size);i++)
			if(isalpha(size[i]))
				return 0;	
	}
	if( strtok(NULL, " ") != NULL)  //any extra arguments
		return 0;	

	if(atol(size)>MAX_VFS_SIZE) ////checking size limits
	{
		puts("VFS Size too large");	
		return 0;
	}
	
	
	return create_vfs(name,atol(size));	

//	return 1;

}

/*
 */
int validate_mount(char input[])
{
	if(IS_VFS_MOUNTED==1)
	{
		printf(" %s VFS is alreay mounted, unmount it first\n",METADATA.label);
		return 0;
	}
	char temp[10000],*ch,*name;	
	int i=0;
	strcpy(temp,input);
	ch = strtok(temp, " ");	
	
	name = strtok(NULL, " "); //check name args
	if(name == NULL)
		return 0;
	
	if( strtok(NULL, " ") != NULL)  //any extra arguments
		return 0;	
	

	IS_VFS_MOUNTED = mount_vfs(name);
	return IS_VFS_MOUNTED;
}

/*
 */
int validate_unmount(char input[])
{
	if(IS_VFS_MOUNTED==0)
	{
		printf("\nFirst Mount VFS\n ");
		return 0;
	}
	char temp[10000],*ch;	
	int i=0;
	strcpy(temp,input);
	ch = strtok(temp, " ");	
	
	if( strtok(NULL, " ") != NULL)  //any extra arguments
		return 0;	
	

		
	IS_VFS_MOUNTED=0;
	return unmount_vfs();
}

/*
 */
int validate_create_file(char input[])
{
	if(IS_VFS_MOUNTED==0)
	{
		printf("\nFirst Mount VFS\n ");
		return 0;
	}
	char temp[10000],*ch,*name,*data;	
	int i=0;
	strcpy(temp,input);
	ch = strtok(temp, " ");	
	
	name = strtok(NULL, " "); //check name of file

	if(name == NULL)
		return 0;

	printf("\nName of File=%s",name);

	int from =strlen("create ") + strlen (name);
	int length = strlen(input) - from;
	data = sub_string(input,from,length);
	//data=strstr(input," ");//remove create comm

//	data=strstr(data," "); //remove filename
//
 //	data=strstr(data," ");
	puts("\ndata");
	puts(data);
	//if( strtok(NULL, " ") != NULL)  //any extra arguments
	//	return 0;	
	
	return create("/",name,data);
	//return create("/","xyz.txt","Hello This is our second sample data ");
}

/*
 */
int validate_list_file(char input[])
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
	 return listfile("/",file_name);


}
/*
 */
int validate_list(char input[])
{
	if(IS_VFS_MOUNTED==0)
	{
		printf("\nFirst Mount VFS\n ");
		return 0;
	}

	 return list("/");


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


/*--------EOF--------*/
