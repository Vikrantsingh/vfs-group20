/*
	File: bst_hash_chain.h
	Description: Maintain Binary Search Tree for Chaining in Hashtable
	 
*/

/*
*/

int insert_into_bst_hash(int a,int b, struct hashtable *h);

int delete_from_bst_hash(struct hashtable *h,char path[],char name[]);

int search_bst_hash(struct hashtable *h,char name[],FILE *fout);

int display_bst_all_hash(struct hashtable *h);



/*--------EOF--------*/
