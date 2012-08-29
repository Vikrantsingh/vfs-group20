/*
	File: file_operators.h
	Description: Function for Manipulating Files and their data in VFS.
	 
*/


/*
Create a new file named <file_name> with
<data> as content. in path specified by <dest_dir_name>.
*/
int create(char dest_dir[],char file_name[],char data[]);

/*
View the contents of a file specified by <file_path>.
*/
int listfile(char file_path[]);

/*
Update/Edit the contents of a file specified by <file_path>
with <data> according to <flag>.
*/
int update(char file_path[],char data[]);

/*
Remove a file specified by <file_path>.
*/
int rm(char file_path[]);

/*
Rename <source_file_path> to
<dest_file_path> file.
*/
int mv(char src[],char dest[]);

/*
To copy source directory/file to
destination specified.
*/
int copy(char src[],char dest[]);

/*
Export the file from the file system into the hard disk
*/
int Export(char file_path[]);

/*--------EOF------*/
