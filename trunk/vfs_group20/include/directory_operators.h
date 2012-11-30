/*

	File: directory_operators.h
	Description:Functions for Managing Directory-Subdirectory and their content
	 
*/



/* To create a new directory whose name should be
<dir_name> in path specified by <parent_path> where '/' is considered as 'root'
directory.
*/
int make_dir(char parent_path[],char dir_name[]);


/*
To remove a file or a directory as indicated by < name>
*/

int delete_dir(char name[]);

/*
To move a sub-tree
<source_dir_name> to <dest_dir_name> directory.
*/

int move_dir(char src_dir_path[],char dest_dir_path[]);

/*
List all the contents according to <flag> of the current directory
i.e. specified by <dir_name>.
*/
int list_dir(char dir_path[],int flag, FILE *fout );

/*
       separate path with / ,and return it via call by reference way
 */
//int separate_path(char  **dir_struct,char parent_path[],int *total_dir_in_path);
int separate_path(char dir_struct[MAX_SUB_DIRECTORY_LEVEL][MAX_LENGTH_OF_NAME],char parent_path[],int *total_dir_in_path);
 

