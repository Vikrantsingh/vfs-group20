/*
	File: file_system.h
	Description:Functions for Creating - mounting filesystem
	 
*/


/*
To create a Virtual File System. <vfs_label> is the name
of VFS.
*/
int create_vfs(char[],long int);

/*
	Determine VFS  Property...Memory Utilization
*/
int memory_statistics();
/*
To Mount the VFS path on current directory. This involves
loading the file/directory descriptors into various data structures
*/
int mount_vfs();

/*
To unmount the VFS. It involves saving index information
and tree information back into the hard disk.
*/
int unmount_vfs();

/*--------EOF--------*/
