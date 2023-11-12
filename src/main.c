#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"

unsigned int total_amt_vertices; // number of vertices in the graph
unsigned int total_amt_edges; // number of edges in the graph
Vertex *vertex;

void delete_vertex(unsigned int v, bool put_in_vc);

int maximum_degree; // Holds the value for the highest degree found in the graph
/*
 * INFO: an array of size maximum_degree that holds the index of the first
 * vertex that represents a degree
 *
 * EXAMPLE: degree_list[i] holds value x if vertex x is the first in the list of
 * all vertices of degree i, "the list" is constructed through the arrays
 * 'degree_list_prev' and 'degree_list_next'
 *
 * MODIFICATION: content can be altered after initialisation
 */
int *degree_list; 

/*
 * stack of deletions we executed to be able to reverse the graph to a previous
 * condition, can be at most total_amt_vertices long as we can't delete more
 * vertices than that
 */
int *deletion_stack;
int stack_top = -1; // index of the "topmost" / "most recently added" stack entry

/*
 * calculates lower_bound as per lecture slides
 */
unsigned int calc_lower_bound(){
    unsigned int result = 0;
    if( maximum_degree > 0 ) { // don't divide by 0 when edges == max_degree == 0
		result = (total_amt_edges + (maximum_degree-1))/maximum_degree; // weird term rounds up
	}
    return result;
}

/*
 * calculates upper_bound as per lecture slides
 */
unsigned int calc_upper_bound(){
	unsigned int upper_bound = 0;
	if (maximum_degree == 0) { // recursion hook, all edges removed
		return upper_bound;
	}
	// find max degree vertex v, calculate on G-{v}
	unsigned int v = degree_list[maximum_degree]; 
	delete_vertex(v,1);
	upper_bound = 1 + calc_upper_bound(); // update size of vc by +1 for v
    return upper_bound;
}

/*
 * INFO: add a vertex to it's corresponding degree list and update maxmimum_degree
 *
 * EXAMPLE: adds vertex x to degree list i if it is of degree i
 */
void add_to_degree_list(unsigned int v) {
    unsigned int current_degree = vertex[v].degree;
	 //put v to front of list
    unsigned int first = degree_list[current_degree];
    degree_list[current_degree] = v;

    if(first != -1) vertex[first].degree_list_prev = v;
    vertex[v].degree_list_next = first;
    vertex[v].degree_list_prev = -1;
}

/*
 * INFO: remove a vertex from its degree list (used when it needs to be put
 * into a different degree list for example) and link its predecessor and successor
 * in that list to each other if they exist
 *
 * EXAMPLE: removes vertex v from degree_list[x] if v was of degree x
 */
void remove_from_degree_list(unsigned int v){
    unsigned int current_degree = vertex[v].degree;
	//update list head if needed
    if (degree_list[current_degree] == v) {
		degree_list[current_degree] = vertex[v].degree_list_next;
    }
    int next = vertex[v].degree_list_next;
    int prev = vertex[v].degree_list_prev;
	// connect prev and next if needed
    if (next != -1) vertex[next].degree_list_prev = prev;
    if (prev != -1) vertex[prev].degree_list_next = next;
	//reset v, as it is currently in no list
    vertex[v].degree_list_next = -1;
    vertex[v].degree_list_prev = -1;
}

/*
 * INFO: mark a vertex as "deleted and used in vertex_cover" or "just deleted",
 * put it on the deletion_stack, 
 * remove it from its degree list,
 * update the degrees and degree lists of all its neighbours, 
 * reduce maximum_degree if a neighbour was the only one with that degree
 */
void delete_vertex(unsigned int v, bool put_in_vc){
	// add v to deletion stack
	stack_top++;
	deletion_stack[stack_top] = v;

	// update used status
	if (put_in_vc) { 
		vertex[v].used = 2; // v gets deleted and put in v cover
	} else {
        vertex[v].used = 1; // v gets deleted but not put in v cover
	}

	// we only want "active" vertices in the degree lists
	remove_from_degree_list(v);
	total_amt_edges -= vertex[v].degree;
    // check if v was max_degree, unless v had degree 0 (occurs in instance 2 1 \n 1 2)
    if(maximum_degree > 0 && vertex[v].degree == maximum_degree && degree_list[maximum_degree] == -1){
        // find new max degree
        while(degree_list[maximum_degree] == -1){
            maximum_degree--;
        }
    }

	/*
	 * remove all "active" neighbours of v from their degree lists, update their
	 * degrees and add them back to the new correct list
	 */
	for (int i = 0, j = 0; j < vertex[v].degree; i++) {
		int w = vertex[v].adj[i];
        if (vertex[w].used == 0) {
			remove_from_degree_list(w);

            //  decrease maximum_degree if needed
			if (vertex[w].degree == maximum_degree && degree_list[maximum_degree] == -1) {
				maximum_degree--;
			}
			vertex[w].degree--;

			add_to_degree_list(w);
            j++;
        }
    }
}

/*
 * INFO: mark a vertex as "untouched"
 * remove it from the deletion_stack, 
 * add it to its corresponding degree list,
 * update the degrees and degree lists of all its neighbours, 
 */
void reverse_deletion(int stack_checkpoint){
	// as long as we didn't revert back to the desired state
    while (stack_top > stack_checkpoint) { 
		// get the most recently deleted vertex
        unsigned int v = deletion_stack[stack_top];
        vertex[v].used = 0; // set its status to "untouched"
        add_to_degree_list(v); // add it to its corresponding degree list
		total_amt_edges += vertex[v].degree;

		// I reallocated the maximum_degree update into add_to_degree_list //L: i think either both in lists function or neither, since we cant do it for the other, lets keep it here
		// if v has the highest degree now
		if (vertex[v].degree > maximum_degree) {
            maximum_degree = vertex[v].degree;
        }

		//update neighbors, only for active ones, as the other are deeper in the deletion_stack

        for (int i = 0, j = 0; j < vertex[v].degree; i++) {
            unsigned int w = vertex[v].adj[i];
            if (vertex[w].used == 0) {
                //increase degree and add to correct list
				remove_from_degree_list(w);
				vertex[w].degree++;
				add_to_degree_list(w);
                j++;
                //check if w has maximum_degree
                if (vertex[w].degree > maximum_degree) {
                    maximum_degree = vertex[w].degree;
                }
            }
        }
        stack_top--;
    }
}

unsigned int data_reduction() {
    //TODO: more reduction rules

    //degree 1 rule
    if (maximum_degree == 0) { // In case there is only a degree 0 list
		return 0;
    }
    unsigned int k = 0;

    //degree 2-rule
    if(maximum_degree >= 2){
        unsigned int v = degree_list[2];
        while(v != -1){
            //get the two neighbors
            unsigned int neighbors[2];
            for(int i = 0, j = 0; j < 2; i++){
                unsigned int w = vertex[v].adj[i];
                if(vertex[w].used == 0){
                    neighbors[j] = w;
                    j++;
                }
            }
            //get the one with lower degree
            int x = 0;
            if(vertex[neighbors[1]].degree < vertex[neighbors[0]].degree) x = 1;

            //check if they are adjacent
            char are_adj = 0;
            for(int i = 0, j = 0; j < vertex[neighbors[x]].degree; i++){
                unsigned int w = vertex[neighbors[x]].adj[i];
                if(vertex[w].used == 0){
                    if(w == neighbors[(x +1)%2]){
                        are_adj = 1;
                        break;
                    }
                    j++;
                }
            }
            if(are_adj){ //triangle rule
                delete_vertex(v, 0);
                delete_vertex(neighbors[0], 1);
                delete_vertex(neighbors[1], 1);
                k += 2;
                v = degree_list[2];
            }else{//TODO: forward deg-2 rule, substancially more work required
                unsigned int next = vertex[v].degree_list_next;
                v = next;
            }
        }
    }

    while (degree_list[1] != -1) {
        unsigned int v = degree_list[1];
        for (int i = 0, j = 0; j < 1; i++) {
            int w = vertex[v].adj[i];
            if (vertex[w].used == 0) {
                delete_vertex(v, 0);
                delete_vertex(w, 1);
                j++;
                k++;
            }
        }
    }
    return k;
}

/*
 * INFO: Implement the main vc_branch algorithm as shown in the lecture slides
 */
bool vc_branch(int k, int stack_checkpoint){
    //TODO: reduce data
    k -= data_reduction();
    if (k < 0) { // Recursion hook, no vertex cover was found
        return false;
    }
	// graph is edgless => vertex_cover of size k
	// TODO: IMO this could be maximum_degree == 0, there is no situation where
	// it should be negative
    if (maximum_degree <= 0) { 
        return true;
    }
	// we want to find a vc with k vertices but need more -> impossible
	if (k < calc_lower_bound()) return false;


	// pick a vertex of maximum degree (this is an attempt at an heuristic)
    unsigned int v = degree_list[maximum_degree]; 
    int old_stack_top = stack_top;
	delete_vertex(v, 1);
	if (vc_branch(k - 1, stack_top)) {
        return true; // it makes a vertex cover of size k with our deleted one
    }

	//no solution of size k with v
    reverse_deletion(old_stack_top);
    int degree_v = vertex[v].degree;//need the current degree, because after deleting the neighborhood deg[v] = 0
	/* 
	 * delete neighbors and add to vc
	 *
	 * ! note the usage of i and j here: i iterates over all neighbours of v 
	 * but the iteration stops as soon as all still "active" neighbours have 
	 * been deleted which is handled by j and the degree[v]
	 */ 
    for (int i = 0, j = 0; j < degree_v; i++) {
		unsigned int w = vertex[v].adj[i];
        if (vertex[w].used == 0) {
            delete_vertex(w, 1);
            j++;
        }
    }

	/*
	 * check with all neighnors in vc
	 */ 
    if (vc_branch(k - degree_v, stack_top)) {
        return true; // it makes a vertex cover of size k with our deleted ones
    }

	// else we were out of luck with vertex covers for size k in this subgraph
    return false; 
}

/*
 * INFO: Parse input and initialize everything, calculate solution, print solution
 */
int main(int argc, char **argv){
    //////////////////////////////////////////////////////////////////////////
    //// parse input and initialize everything
    //////////////////////////////////////////////////////////////////////////
    char* line = NULL;
    size_t len = 0;
    getline(&line, &len, stdin);

    while (line[0] == '#') getline(&line, &len, stdin); // ignore comment lines

    // init edge and vertex count
    char *ptr = strtok(line, " ");
    total_amt_vertices = strtol(ptr, (char**) NULL, 10);
    ptr = strtok(NULL, " ");
    total_amt_edges = strtol(ptr, (char**) NULL, 10);

	/*
   	 * init all arrays and their pointers (we use static arrays so that they 
	 * will be allocated in proximity to each other in memory and also because 
	 * we can and dont have to worry about freeing memory, we still use
	 * global pointers so we can easily reference the arrays contents from 
	 * within functions)
	 */
    vertex = malloc(total_amt_vertices * sizeof(Vertex));
    /*
    int *adj_[total_amt_vertices];
    adj = adj_;

    char used_[total_amt_vertices];
    used = used_;

    unsigned int degree_[total_amt_vertices];
    degree = degree_;

    int degree_list_next_[total_amt_vertices];
	int degree_list_prev_[total_amt_vertices];
    degree_list_next = degree_list_next_;
    degree_list_prev = degree_list_prev_;
	*/
    //int deletion_stack_[total_amt_vertices];
    deletion_stack = malloc(total_amt_vertices * sizeof(int));

	/*
	 * initialisation of arrays contents with standard data where we can and
	 * need to
 	 */
    maximum_degree = 0;
    for (int i = 0; i < total_amt_vertices; i++) {
        vertex[i].adj = NULL;
        vertex[i].used = 0;
		deletion_stack[i] = -1; // not necessary but makes debugging easier
        vertex[i].used = 0;
        vertex[i].degree_list_prev = -1; // -1 symbolises NULL
        vertex[i].degree_list_next = -1;
    }

	// parse edges to update vertex degrees and fill their adjacency lists
    int counter_edges = 0;
    while (counter_edges < total_amt_edges) {
        getline(&line, &len, stdin);
        if (line[0] != '#') { // skip comments
							  
            // get vertices v and w of edge
            ptr = strtok(line, " ");
            unsigned int v = (unsigned int) strtol(ptr, (char **) NULL, 10) - 1;
            ptr = strtok(NULL, " ");
            unsigned int w = (unsigned int) strtol(ptr, (char **) NULL, 10) - 1;

            // increase degree for both
            vertex[v].degree++;
            vertex[w].degree++;

            // add to each others adjacency lists
            vertex[v].adj = realloc(vertex[v].adj, sizeof(int) * vertex[v].degree);
            vertex[v].adj[vertex[v].degree - 1] = w;
            vertex[w].adj = realloc(vertex[w].adj, sizeof(int) * vertex[w].degree);
            vertex[w].adj[vertex[w].degree - 1] = v;

			// update maximum_degree if necessary
            if (vertex[v].degree > maximum_degree) {
                maximum_degree = vertex[v].degree;
			}
            if (vertex[w].degree > maximum_degree) {
                maximum_degree =vertex[w].degree;
            }

            counter_edges++;
        }
    }

    // initialize degree_list array and its pointer
    //int degree_list_[maximum_degree + 1]; // +1 because we have vertices with 0 adj
    // malloc is safer!
    degree_list = malloc(sizeof(Vertex) * (maximum_degree + 1));
	// set first elements of all degree lists to -1 (symbolises NULL)
    for (int i = 0; i <= maximum_degree; i++) degree_list[i] = -1;
	// add each vertex to its corresponding degree list
    for (int i = 0; i < total_amt_vertices; i++) {
        if(vertex[i].degree > 0) add_to_degree_list(i);// 0-list has no effect
    }

    //////////////////////////////////////////////////////////////////////////
    //// calculate solution 
    //////////////////////////////////////////////////////////////////////////
	data_reduction();
    stack_top = -1;//first reduction is never reversed
				   
    unsigned int lower_bound = calc_lower_bound();
    unsigned int upper_bound = calc_upper_bound();

	
	if (lower_bound < upper_bound) { 
		/*
		 * search for lowest vertex_cover of size k
		 * on if l_b < u_b we can still search, if we get into the while body
		 * again and they are equal, we need to do the last iteration to set
		 * the used's right
		 */
		int M = lower_bound + (upper_bound-lower_bound)/2;
		while (lower_bound != upper_bound){
			reverse_deletion(-1);
			if(vc_branch(M,-1)) {
				upper_bound = M;
			} else {
				lower_bound = M+1;
			}
			M = lower_bound + (upper_bound-lower_bound)/2;
		}
		reverse_deletion(-1);
		vc_branch(lower_bound, -1);
	}

    //////////////////////////////////////////////////////////////////////////
    //// print solution
    //////////////////////////////////////////////////////////////////////////
    for (int i = 0; i < total_amt_vertices; i++) {
        if (vertex[i].used == 2) {
            printf("%d\n", i+1);
        }
    }

    return 0;
}
