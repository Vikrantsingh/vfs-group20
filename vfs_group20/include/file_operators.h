/*
	File: file_operators.h
	Description: Function for Manipulating Files and their data in VFS.
	 
*/


/*
Create a new file named <file_name> with
<data> as content. in path specified by <dest_dir_name>.
*/
int create(char dest_dir[],char file_name[],char infile[]);

/*
View the contents of a file specified by <file_path>.
*/
int list_file(char file_path[],char outputfile[]) ;

/*
list all files in current directory

int list(char file_path[]);
*/
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
int copy_file(char src[],char dest[]);

/*
Export the file from the file system into the hard disk
*/
int export_file(char file_path[],char outputfile[]);


/*
	Get Free node from freelist and write data to it
*/
long int add_data_blocks(char [],long int );
/*
	Read a single data blocks at given location.
*/
struct block read_data_blocks(long int );
/*
	Display data to Screen.
*/
int display_data_blocks(struct block);
/*
	Search file and return FD location
*/
//long int search_File(char file_path[]);
int find_file(char name[], char output_file[]);

/*--------EOF------*/
