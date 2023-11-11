#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

unsigned int total_amt_vertices; // number of vertices in the graph
unsigned int total_amt_edges; // number of edges in the graph

/*
 * INFO: a list that contains pointers to each vertex' adjacency list
 * 
 * EXAMPLE: adj[i] contains the int* that points to the adjacency list of
 * vertex i
 *
 * MODIFICATION: content not to be altered after initialisation
 */
int **adj; 

/*
 * INFO: an array that holds information for each vertex whether it is:
 * deleted and used in vertex cover (2), just deleted(1), or untouched(0)
 *
 * EXAMPLE: used[i] is set to 0/1/2 according to the rules just mentioned if 
 * vertex i is of status (0), (1), or (2)
 *
 * MODIFICATION: content can be altered after initalisation
 */
char *used; 

/*
 * INFO: an array that holds the degree of each vertex (as unsigned int)
 *
 * EXAMPLE: degree[i] is set to x if vertex i has degree x, means x "undeleted"
 * neighbours
 *
 * MODIFICATION: content can be altered after initialisation
 */
unsigned int *degree; 

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
 * INFO: arrays of size total_amt_vertices that hold the following and previous 
 * vertices in the list of vertices of degree d
 *
 * EXAMPLE: degree_list_next[i] holds value x if vertex x is the following vertex
 * to vertex i in the degree_list that corresponds to vertex i's degree
 *
 * MODIFICATION: content can be altered after initialisation
 */
int *degree_list_prev, *degree_list_next; 

/*
 * stack of deletions we executed to be able to reverse the graph to a previous
 * condition, can be at most total_amt_vertices long as we can't delete more
 * vertices than that
 */
int *deletion_stack;
int stack_top = -1; // index of the "topmost" / "most recently added" stack entry

int calc_lower_bound(){
	//TODO: calculate lower bound
    return 0; 
}

int calc_upper_bound(){
	//TODO: calculate upper bound
    return total_amt_vertices; 
}

void data_reduction(){
    //TODO: reduce data :-)
}

/*
 * INFO: add a vertex to it's corresponding degree list and update maxmimum_degree
 *
 * EXAMPLE: adds vertex x to degree list i if it is of degree i
 */
void add_to_degree_list(unsigned int vertex) {
    unsigned int current_degree = degree[vertex];
	// check if list is empty
    if (degree_list[current_degree] == -1) { 
        degree_list[current_degree] = vertex;
    } else {
        //put vertex to front of list
        unsigned int first = degree_list[current_degree];
        degree_list[current_degree] = vertex;

		degree_list_prev[first] = vertex;
        degree_list_next[vertex] = first; 
        degree_list_prev[vertex] = -1;
    }

	// update maxmimum_degree //L: took it out here, since it is not needed in initialization
}

/*
 * INFO: remove a vertex from its degree list (used when it needs to be put
 * into a different degree list for example) and link its predecessor and successor
 * in that list to each other if they exist
 *
 * EXAMPLE: removes vertex v from degree_list[x] if v was of degree x
 */
void remove_from_degree_list(unsigned int vertex){
    unsigned int current_degree = degree[vertex];
	//update list head if needed
    if (degree_list[current_degree] == vertex) {
		degree_list[current_degree] = degree_list_next[vertex];
    }
    int next = degree_list_next[vertex];
    int prev = degree_list_prev[vertex];
	// connect prev and next if needed
    if (next != -1) degree_list_prev[next] = prev;
    if (prev != -1) degree_list_next[prev] = next;
	//reset vertex, as it is currently in no list
    degree_list_next[vertex] = -1;
    degree_list_prev[vertex] = -1;
}

/*
 * INFO: mark a vertex as "deleted and used in vertex_cover" or "just deleted",
 * put it on the deletion_stack, 
 * remove it from its degree list,
 * update the degrees and degree lists of all its neighbours, 
 * reduce maximum_degree if a neighbour was the only one with that degree
 */
void delete_vertex(int vertex, bool put_in_vc){
	// add vertex to deletion stack
	stack_top++;
	deletion_stack[stack_top] = vertex;

	// update used status
	if (put_in_vc) { 
		used[vertex] = 2; // vertex gets deleted and put in vertex cover
	} else {
		used[vertex] = 1; // vertex gets deleted but not put in vertex cover
	}

	// we only want "active" vertices in the degree lists
	remove_from_degree_list(vertex);
    //check if v was max_degree
    if(degree[vertex] == maximum_degree && degree_list[maximum_degree] == -1){
        //find new max degree
        while(degree_list[maximum_degree] == -1){
            maximum_degree--;
        }
    }

	/*
	 * remove all "active" neighbours of vertex from their degree lists, update their 
	 * degrees and add them back to the new correct list
	 */
	for (int i = 0, j = 0; j < degree[vertex]; i++) {
		int w = adj[vertex][i];
        if (used[w] == 0) {
			remove_from_degree_list(w);

            //  decrease maximum_degree if needed
			if (degree[w] == maximum_degree && degree_list[maximum_degree] == -1) {
				maximum_degree--;
			}
			degree[w]--;

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
        used[v] = 0; // set its status to "untouched"
        add_to_degree_list(v); // add it to its corresponding degree list

		// I reallocated the maximum_degree update into add_to_degree_list //L: i think either both in lists function or neither, since we cant do it for the other, lets keep it here
		// if v has the highest degree now
		if (degree[v] > maximum_degree) {
            maximum_degree = degree[v];
        }

		/*
		 * update neighbors
		 * 
		 * only for active ones, as the other are deeper in the deletion_stack
		 */
        for (int i = 0, j = 0; j < degree[v]; i++) {
            unsigned int w = adj[v][i];
            if (used[w] == 0) {
                //increase degree and add to correct list
				remove_from_degree_list(w);
				degree[w]++;
				add_to_degree_list(w);
                j++;

		 		 // !! I refactored this into add_to_degree_list, there is a todo for it

                if (degree[w] > maximum_degree) {
                    maximum_degree = degree[w];
                }
            }
        }

        stack_top--;
    }
}

/*
 * INFO: Implement the main vc_branch algorithm as shown in the lecture slides
 */
bool vc_branch(int k, int stack_checkpoint){
    //TODO: reduce data
    if (k < 0) { // Recursion hook, no vertex cover was found
        return false;
    }
	// graph is edgless => vertex_cover of size k
    if (maximum_degree <= 0) { 
        return true;
    }
	
	// pick a vertex of maximum degree (this is an attempt at an heuristic)

    unsigned int v = degree_list[maximum_degree]; 
	delete_vertex(v, 2);
	if (vc_branch(k - 1, stack_top)) {
        return true; // it makes a vertex cover of size k with our deleted one
    }

	//no solution of size k with v
    reverse_deletion(stack_checkpoint);
    int degree_v = degree[v];//need the current degree, because after deleting the neighborhood deg[v] = 0
	/* 
	 * delete neighbors and add to vc
	 *
	 * ! note the usage of i and j here: i iterates over all neighbours of v 
	 * but the iteration stops as soon as all still "active" neighbours have 
	 * been deleted which is handled by j and the degree[v]
	 */ 
    for (int i = 0, j = 0; j < degree_v; i++) {
		unsigned int w = adj[v][i];
        if (used[w] == 0) {
            delete_vertex(w, 2);
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

    int deletion_stack_[total_amt_vertices]; 
    deletion_stack = deletion_stack_;

	/*
	 * initialisation of arrays contents with standard data where we can and
	 * need to
 	 */
    maximum_degree = 0;
    for (int i = 0; i < total_amt_vertices; i++) {
        adj[i] = NULL;
        used[i] = 0;
		deletion_stack[i] = -1; // not necessary but makes debugging easier
        degree[i] = 0;
        degree_list_prev[i] = -1; // -1 symbolises NULL
        degree_list_next[i] = -1;
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
            degree[v]++;
            degree[w]++;

            // add to each others adjacency lists
            adj[v] = realloc(adj[v], sizeof(int) * degree[v]);
            adj[v][degree[v] - 1] = w;
            adj[w] = realloc(adj[w], sizeof(int) * degree[w]);
            adj[w][degree[w] - 1] = v;

			// update maximum_degree if necessary
            if (degree[v] > maximum_degree) {
                maximum_degree = degree[v];
			}
            if (degree[w] > maximum_degree) {
                maximum_degree = degree[w];
            }

            counter_edges++;
        }
    }

    // initialize degree_list array and its pointer
    int degree_list_[maximum_degree + 1]; // +1 because we have vertices with 0 adj
    degree_list = degree_list_;
	// set first elements of all degree lists to -1 (symbolises NULL)
    for (int i = 0; i <= maximum_degree; i++) degree_list[i] = -1;
	// add each vertex to its corresponding degree list
    for (int i = 0; i < total_amt_vertices; i++) add_to_degree_list(i);

    //////////////////////////////////////////////////////////////////////////
    //// calculate solution 
    //////////////////////////////////////////////////////////////////////////

    int k = calc_lower_bound();
    int upper_bound = calc_upper_bound();

    //search for lowest vertex_cover of size k
    while (!vc_branch(k, -1) && k < upper_bound) {
        reverse_deletion(-1); // revert commands executed in prev. vc_branch call
        k++;
        //printf("k = %d", k);
    }

    //////////////////////////////////////////////////////////////////////////
    //// print solution
    //////////////////////////////////////////////////////////////////////////
    for (int i = 0; i < total_amt_vertices; i++) {
        if (used[i] == 2) {
            printf("%d\n", i+1);
        }
    }

    return 0;
}
