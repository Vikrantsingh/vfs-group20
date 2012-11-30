/*
	File: bst_full_path.h
	Description: Maintain Binary Search Tree for Chaining in Hashtable
	 
*/
 
/*
*/
 
struct bst* insert_into_bst_path(struct bst *temp,struct bst *file_desp);

int delete_from_bst_path(char name[]); 

struct bst* search_bst_path(struct bst *b,char path[]);

int display_bst_all_path(struct bst *b);

int update_bst_full_path(char oldpath[],char newpath[]);

int reset_bst_path();
int delete_all_bst_path(struct bst *node);
int build_bst(struct nary* node);

/*--------EOF--------*/ 
