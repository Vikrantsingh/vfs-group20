/*
	File: ui.h
	Description:User interaction
	 
*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"../include/structure.h"
#include"../include/file_system.h"
#include"../include/directory_operators.h"
#include"../include/file_operators.h"
#include"../include/validation.h"

#define MAX_INPUT_COMMAND_SIZE 10000
/*
   Read User Inputs
*/
int get_command()
{
	char input[MAX_INPUT_COMMAND_SIZE];
	while(1)
	{
		//printf("%c[%d;%d;%dm Hello",27,1,33,40);

		//printf("%c[%d;%dmHello World%c[%dm\n",27,1,31,27,0);
		
		do{
		
			if(IS_VFS_MOUNTED==0)
				printf("\nVFS-G20 # ");
			else
				printf("\nVFS-G20 @ %s : ~ %s # ",METADATA.label,PRESENT_WORKING_DIRECTORY);

			gets(input);
			//fgets(input,MAX_INPUT_COMMAND_SIZE,stdin);
			//scanf("%[^\n]s",input);
			//fflush(stdout);
			//puts(input);

		}while(strlen(input)==0);
		

		if(strcmp(input,"exit")==0)
			break;

		else if(strcmp(input,"man")==0)
			man();
			
        else
        {
		    if(identify_command(input))
			    {
				//do nothing...since task is completed by validate...command
		    	}

    		else
	    	printf("%s - command not available",input);
         }	
		
	}
	puts("\nbye...");
}

/*
	Check User Input is write or wrong
*/

int identify_command(char input[])
{

	char *ch, temp [MAX_INPUT_COMMAND_SIZE];
	int i=0;
	strcpy(temp,input);
	ch = strtok(temp, " ");
	if(strcmp(ch,"create_vfs")==0)
	{
		process_command(1,input);	
	}
	
	else if(strcmp(ch,"mount")==0)
	{
		process_command(2,input);	
	}
	else if(strcmp(ch,"create")==0)
	{
		process_command(3,input);	
	}
	else if(strcmp(ch,"listfile")==0)
	{
		process_command(4,input);	
	}
	else if(strcmp(ch,"list")==0)
	{
		process_command(7,input);	
	}
	else if(strcmp(ch,"rm")==0)
	{
		process_command(8,input);	
	}
	else if(strcmp(ch,"freelist")==0)
	{
		process_command(9,input);	
	}
	else if(strcmp(ch,"mkdir")==0)
	{
		process_command(10,input);	
	}
	else if(strcmp(ch,"listdir")==0)
	{
		process_command(11,input);	
	}
	else if(strcmp(ch,"cd")==0)
	{
		process_command(12,input);	
	}
	else if(strcmp(ch,"find")==0)
	{
		process_command(13,input);	
	}
	else if(strcmp(ch,"pwd")==0)
	{
		puts(PRESENT_WORKING_DIRECTORY);	
	}
	else if(strcmp(ch,"unmount")==0)
	{
		process_command(5,input);	
	}
	else if(strcmp(ch,"mem")==0)
	{
		process_command(6,input);	
	}
	
	/*while (ch != NULL && i<10) 
	{
		i++;
		printf("\n\n%s\n", ch);
		printf("%s\n", temp);
		printf("%s\n", input);
		ch = strtok(NULL, " ");
	}
*/
	else 		
		return 0;

	return 1;


	
}

/*
	Preform action on user input
*/

int process_command(int i,char input[]){

	switch(i)
	{
		case 1 :

			if(validate_create_vfs(input)==1)
			{
				
				//create_vfs(name,size);
				puts("VFS Created");

			}
			else
			{
				puts("\nInvalid use of create_vfs: create_vfs <path/filename:without space> <size:longint>");
			}
			
		
			break;
		case 2 :
			if(validate_mount(input)==1)
			{
				
				//create_vfs(name,size);
				puts("\nVFS Mounted.");

			}
			else
			{
				puts("Invalid use of mount:\nmount <path/filename:without space>");
			}

			break;
		case 3 :
			if(validate_create_file(input)==1)
			{
				
				//create_vfs(name,size);
				puts("\nFile Created.");

			}
			else
			{
				puts("\nInvalid use of create:create <path> <filename:without space> <data>");
			}

			break;
		case 4 :
			if(validate_list_file(input)==1)
			{
				
				//create_vfs(name,size);
				//puts("\nData Displayed.");

			}
			else
			{
				puts("\nInvalid use of filelist:filelist <path/filename:without space>");
			}


			break;
		case 5 :
			if(validate_unmount(input)==1)
			{
				
				//create_vfs(name,size);
				puts("\nVFS Unmounted");

			}
			else
			{
				puts("\nInvalid use of unmount:unmount");				
			}
			break;

		case 6 :
			if(validate_memory_statistics(input)==1)
			{
				
				//create_vfs(name,size);
				puts("\nVFS Memory Statistics");

			}
			else
			{
				puts("\nInvalid use of mem:mem");				
			}
			break;
		case 7 :
			if(validate_list(input)==1)
			{
				
				//create_vfs(name,size);
				puts("\nFile Listed.");

			}
			else
			{
				puts("\nInvalid use of list:list <path/filename:without space>");
			}


			break;
		case 8 :
			if(validate_rm_file(input)==1)
			{
				
				//create_vfs(name,size);
				puts("\nFile Deleted.");

			}
			else
			{
				puts("\nInvalid use of rm:rm <path/filename:without space>");
			}


			break;

		case 9 :
			if( validate_freelist()==1)
			{
				
				//create_vfs(name,size);
				puts("\nFile Deleted.");

			}
			else
			{
				puts("\nInvalid use of freelist:freelist");
			}


			break;


		case 10 :
			if( validate_mkdir(input)==1)
			{
				
				 
				puts("\nDirectory Created.");

			}
			else
			{
				puts("\nInvalid use of mkdir:mkdir <path>");
			}


			break;



		case 11 :
			if( validate_listdir(input)==1)
			{
				
		 		puts("\nDirectory Listed.");

			}
			else
			{
				puts("\nInvalid use of listdir:listdir <path>");
			}


			break;


		case 12 :
			if( validate_change_dir(input)==1)
			{
				
		 		puts("\nDirectory Changed.");

			}
			else
			{
				puts("\nInvalid use of cd: <path>");
			}


			break;



		case 13 :
			if( validate_find(input)==1)
			{
				
		 		puts("\nFiles Listed.");

			}
			else
			{
				puts("\nInvalid use of find: <filename>");
			}


			break;



		default :
			puts("\nSorry, This Command not available..");

			break;


	
	}
	return 1;

}


/*
	String Tokens
*/
int string_tokens()
{


}



/*
	Help File
*/
int man()
{
	puts("\n=============================================");
	puts("\nVFS - G20 : User Command Manual");
	puts("\n=============================================");
	puts("man - for help.");
	puts("create_vfs <vfs name> <size> : To create New VFS.");
	puts("mount <vfs name> : To mount vfs.");
	puts("create <filpath> <filename> <data> : To create a file in mounted VFS.");
	puts("mkdir <path> <dir name> : To create new directory in given path.");
	puts("listfile <path> : To display content of file.");
	puts("list <path> : To list all files in given path.");
	puts("find <filename> : Searching files.");
	puts("rm <path> : To delete file in given path.");
	puts("listdir <path> : To display n-ary for given path.");
	puts("unmount : To unmount current mounted VFS.");
	puts("exit - to terminate application.");

}


