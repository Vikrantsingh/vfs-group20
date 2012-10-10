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
	char temp[10000],*ch,*name,*data,*path ;	
	int i=0;
	strcpy(temp,input);
	ch = strtok(temp," ");	

	path = strtok(NULL," "); //check path of file

	if(path == NULL)
		return 0;
	
	name = strtok(NULL," "); //check name of file

	if(name == NULL)
	{
	        ////////////////////now consider 2nd parameter as name and use defaultpath
	    //name=path;
	    //strcpy(default_path,PRESENT_WORKING_DIRECTORY);
    	//path=default_path;
	    
	    return 0;
    }
	printf("\nName of File=%s",name);

	int from =strlen("create ") +strlen(path)+1+ strlen (name) + 1; //1 for space
	int length = strlen(input) - from;
	data = sub_string(input,from,length);
	
	puts("\ndata");
	puts(path);
	puts(name);
	puts(data);
	
	return create(path,name,data);
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

	/*char temp[100];
	strcpy(temp,input);
	char *file_name = strtok(temp, " ");	
	
	file_name = strtok(NULL, " "); //get name of file

	if(file_name == NULL)//check name of file
	{
		puts("File Name cannot be empty");
		return 0;
	}
	 return listfile("root",file_name);
	 */
	
	 
	char temp[1000],default_path[1000],*file_name;
	strcpy(temp,input);
	char *path = strtok(temp, " ");	
	
	path = strtok(NULL, " "); //get path
	if(path == NULL)//check path	
	{
	    puts("Too few parameter");
	    return -1;
	}

    file_name = strtok(NULL, " "); //get name of file	
	if(file_name == NULL)//check path
	{
        strcpy(file_name,path);
		//puts("Path cannot be empty");
		strcpy(default_path,PRESENT_WORKING_DIRECTORY);
    	path=default_path;
        
		//return 0;
	}
	//puts(path);

	 return listfile(path,file_name);



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

	char temp[1000],default_path[1000];
	strcpy(temp,input);
	char *path = strtok(temp, " ");	
	
	path = strtok(NULL, " "); //get path

	if(path == NULL)//check path
	{
		//puts("Path cannot be empty");
		strcpy(default_path,PRESENT_WORKING_DIRECTORY);
    	path=default_path;

		//return 0;
	}
	//puts(path);
	 return list (path);


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
int validate_mkdir(char input[])
{

	if(IS_VFS_MOUNTED==0)
	{
		printf("\nFirst Mount VFS\n ");
		return 0;
	}
	//puts("in validate mkdir");

	//token separation
	char temp[1000],*ch,*name,*path,default_path[1000];	
	int i=0;
	
	strcpy(temp,input);
	
	ch = strtok(temp, " ");	//return command
	
	path = strtok(NULL, " "); //check name args
	
	if(path == NULL)
	{
		puts("Please Enter Destination Path");
		return 0;
	}

	name = strtok(NULL, " "); //check name of new directory

	if(name == NULL)
	{
	
        ////////////////////now consider 2nd parameter as name and use defaultpath
	    name=path;
	    strcpy(default_path,PRESENT_WORKING_DIRECTORY);
    	path=default_path;
	    
	
		//puts("Please Enter New Directory Name");
		//return 0;
	}

// 	puts(name);
// 	puts(path);
	
	
	return makedir(path,name);
	

}

/*
 */
int validate_listdir(char input[])
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

	if(path == NULL)//check path
	{
	//	puts("Path cannot be empty");
		
		strcpy(default_path,PRESENT_WORKING_DIRECTORY);
    	path=default_path;
    //return 0;
	}

	//puts(path);
	return listdir (path,"ls-li");


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
int validate_find(char input[])
{
	if(IS_VFS_MOUNTED==0)
	{
		printf("\nFirst Mount VFS\n ");
		return 0;
	}


	char temp[1000];
	strcpy(temp,input);
	char *name = strtok(temp, " ");	
	
	name= strtok(NULL, " "); //get path

	if(name != NULL)//check path
	{

        return find_file(name);
	}
    else
    {
    	puts("File name cannot be empty");
        return -1;
    }
	//puts(path);
	
}


/*--------EOF--------*/
