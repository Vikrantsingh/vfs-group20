/*
	File: validation.h
	Description:validation of user input
	 
*/

/*
*/
int validate_create_vfs(char *name,int size);

/*
 */
int validate_mount(char *name);

/*
 */
int validate_unmount(char *name);

/*
 */
int validate_create_file(char *path,char *name,char *infile);

/*
 */
int validate_list_file(char *filepath,char *outputpath);
/*
 */
int validate_move_file(char *src,char *dest);
/*
*/
int validate_export_file(char *src,char *dest) ;
/*
 */
int validate_memory_statistics();

/*
 */
int validate_rm_file(char file_path[]);

/*

*/
int validate_mkdir(char input[]);
/*

*/
int validate_copy_dir(char input[]);
/*

*/
int validate_delete_dir(char *path);
/*

*/
int validate_move_dir(char *src,char *dest);
/*

*/
int validate_change_dir(char input[]);
/*

*/
int validate_listdir( char *path, int flag, char *output_file );
/*
*/
int validate_find(char *file_name, char *outputfile);
/*
*/
int validate_update_file(char *path,char *fin);

/*--------EOF--------*/
