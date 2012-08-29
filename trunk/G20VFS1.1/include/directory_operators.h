/*

	File: directory_operators.h
	Description:Functions for Managing Directory-Subdirectory and their content
	 
*/



/* To create a new directory whose name should be
<dir_name> in path specified by <parent_path> where '/' is considered as 'root'
directory.
*/
int mkdir(char parent_path[],char dir_name[]);




/*
To remove a file or a directory as indicated by < name>
*/

int deletedir(char name[]);


/*
To move a sub-tree
<source_dir_name> to <dest_dir_name> directory.
*/

int movedir(char src_dir_name[],char dest_dir_name[]);


/*
List all the contents according to <flag> of the current directory
i.e. specified by <dir_name>.
*/

int listdir(char dir_name[],char flag[] );


