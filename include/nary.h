/*
	File: narray.h
	Description: Maintain N-array for File Descriptor
	 
*/

/*
*/
struct nary* create_nary(long int add,struct nary* parent);  

/*
*/

struct nary* get_parent_nary(char **dir_struct,int total_dir_in_path);


/*
*/
int isChild(struct nary *parent, struct nary *child);
int delete_node_from_nary();

int delete_subtree_from_nary(struct nary *node);
/*
*/

int display_nary(struct nary *node,FILE *fout);


int display_nary_c(struct nary *node);
int update_path_in_subtree(struct nary *node);
/*
 */
int search_file_in_nary(char input[]);

/**/
int write_back_nary(struct nary *node,FILE *fp); 

/*--------EOF--------*/

