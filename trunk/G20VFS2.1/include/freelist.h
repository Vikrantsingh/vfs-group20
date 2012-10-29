/*
	File: freelist.h
	Description: Maintain Track of Available Free Tracks
	 
*/

/*
	Creating block to store data by removing  it from freelist
*/
long int  get_new_node();


/*
	Deleting a block by adding it to free list.
*/
int delete_node(long int);

/*
	Display Free List
*/

int display_freelist();
