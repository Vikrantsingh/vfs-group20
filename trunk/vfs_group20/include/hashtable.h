/*
	File: hashtable.h
	Description: Maintain HashTable for File Descriptor
	 
*/

/*
*/
void hash(char name[],int *a,int *b);

void insert_in_hashtable(char name[],char path[]);

void find_in_hashtable(char search_name[],FILE *fout);


void display_hashtable();



/*--------EOF--------*/
