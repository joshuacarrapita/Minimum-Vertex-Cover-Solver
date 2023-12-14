#ifndef KERNEL_INCLUDE_GRAPH_H 
#define KERNEL_INCLUDE_GRAPH_H 

typedef struct Vertex{
    int *adj; //adjency list of vertex
	int adj_size; //currentsize of adj aray
    char used;//is vertex untouched(0), deleted(1) or in vc (2)
	char modified; //is vertex modified by last reduction
    unsigned int degree; //the current degree of vertex
    unsigned int degree_list_prev;//pointer to prev element in degree_list
    unsigned int degree_list_next;//pointer to next element in degree_list
}Vertex;

typedef struct  Merge{
	unsigned int merge_into;
	unsigned int other;
	unsigned int triggerer;
	int *old_adj;
}Merge;

//all functions with info

/*
 * INFO: add a vertex to it's corresponding degree list and update maxmimum_degree
 *
 * EXAMPLE: adds vertex x to degree list i if it is of degree i
 */
void add_to_degree_list(unsigned int v);


/*
 * INFO: remove a vertex from its degree list (used when it needs to be put
 * into a different degree list for example) and link its predecessor and successor
 * in that list to each other if they exist
 *
 * EXAMPLE: removes vertex v from degree_list[x] if v was of degree x
 */

void remove_from_degree_list(unsigned int v);

/*
 * INFO: mark a vertex as "deleted and used in vertex_cover" or "just deleted",
 * put it on the deletion_stack, 
 * remove it from its degree list,
 * update the degrees and degree lists of all its neighbours, 
 * reduce current_           if a neighbour was the only one with that degree
 */

void delete_vertex(unsigned int v, bool put_in_vc);


/*
 * INFO: mark a vertex as "untouched"
 * remove it from the deletion_stack, 
 * add it to its corresponding degree list,
 * update the degrees and degree lists of all its neighbours, 
 */

void reverse_deletion(int stack_checkpoint);

int is_dominated(unsigned int w);

void sort(unsigned int v);

void merge_vertices(unsigned int merge_into, unsigned int other, unsigned int triggerer);



#endif 
