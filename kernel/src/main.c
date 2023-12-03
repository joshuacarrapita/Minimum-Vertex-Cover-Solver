#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "main.h"
#include "graph.h"
#include "io.h"

unsigned int total_amt_vertices;
unsigned int total_amt_edges;
							  
unsigned int current_amt_vertices;
unsigned int current_amt_edges;

unsigned int upper_bound;
unsigned int highest_remaining_k;

Vertex *vertex;


int maximum_degree; // holds the value for the highest degree found in the graph
int *degree_list; 

int *deletion_stack;

int stack_top = -1; // index of the "topmost" / "most recently added" stack entry

time_t start_time;
bool timeout;

unsigned int calc_lower_bound(){
    unsigned int result = 0;
    if( maximum_degree > 0 ) { // don't divide by 0 when edges == max_degree == 0
		result = (current_amt_edges + (maximum_degree-1))/maximum_degree; // weird term rounds up
	}
    return result;
}

unsigned int calc_upper_bound(){
	unsigned int result = 0;
	if (maximum_degree == 0) { // recursion hook, all edges removed
		return result;
	}
	// find max degree vertex v, calculate on G-{v}
	unsigned int v = degree_list[maximum_degree]; 
	delete_vertex(v,1);
	result = 1 + calc_upper_bound(); // update size of vc by +1 for v
    return result;
}

unsigned int data_reduction() {
    //TODO: more reduction rules
	bool done = false;
	bool found = false;
    unsigned int k = 0;

	while(!done){
		found = false;
		//degree 0 rule 
		if (maximum_degree == 0) { // In case there is only a degree 0 list
			return k;
		}
		//degree 1 rule
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
			found = true;
		}

		//stop if not enough time.. 
		time_t current_time = time(NULL);
		if(current_time - start_time > 117) {
			timeout = true;
			return k;
		}

		if (found) continue;

		//degree 2-rule
		if(maximum_degree >= 2){
			unsigned int v = degree_list[2];
			while(v != -1 && !found){
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
					found = true;
				} else {//TODO: forward deg-2 rule, substancially more work required
					unsigned int next = vertex[v].degree_list_next;
					v = next;
				}
			}
		}
		if (found) continue;

		//domination-rule
		for(int i = 2; i <= maximum_degree; i++){
			int current_vertex = degree_list[i];
			while(current_vertex != -1){
				int dominator = is_dominated(current_vertex);
				if(dominator > -1){
					delete_vertex(dominator, 1);
					found = 1;
					k++;
					break;
				}else{
					current_vertex = vertex[current_vertex].degree_list_next;
				}
			}
			if (found) break;
		}
		if (!found) done = true;
	}
    return k;
}

bool vc_branch(int k){
	if(timeout){
		return false;	
	}else{	
		time_t current_time = time(NULL);
		if(current_time - start_time > 117){
			timeout = true;
			return false;
		}
	}
    //TODO: reduce data
    k -= data_reduction();
    if (k < 0) { // Recursion hook, no vertex cover was found
        return false;
    }
	// graph is edgless => vertex_cover of size k
	// TODO: IMO this could be maximum_degree == 0, there is no situation where
	// it should be negative
    if (maximum_degree <= 0) { 
		highest_remaining_k = k;
        return true;
    }
	// we want to find a vc with k vertices but need more -> impossible
	if (k < calc_lower_bound()) return false;


	// pick a vertex of maximum degree (this is an attempt at an heuristic)
    unsigned int v = degree_list[maximum_degree]; 
    int stack_checkpoint = stack_top;
	delete_vertex(v, 1);
	if (vc_branch(k - 1)) {
        return true; // it makes a vertex cover of size k with our deleted one
    }
	

	if(timeout ) return false;
	//no solution of size k with v
    reverse_deletion(stack_checkpoint);
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
    if (vc_branch(k - degree_v)) {
        return true; // it makes a vertex cover of size k with our deleted ones
    }

	// else we were out of luck with vertex covers for size k in this subgraph
    return false; 
}

void initialize_globals(){
	maximum_degree = 0;

	parseinput();

	current_amt_edges = total_amt_edges;
	current_amt_vertices = 0;

	// alloc deletion_stack mem and init with standard content
	deletion_stack = malloc(total_amt_vertices * sizeof(int));
	for (int i = 0; i < total_amt_vertices; i++) deletion_stack[i] = -1;

	/*
	 * alloc degree_list array and init with content
	 * -> set first elements of all degree lists to -1 (symbolises NULL)
	 * -> add each vertex to its corresponding degree list
	 */
	degree_list = malloc(sizeof(int) * (maximum_degree + 1));
	for (int i = 0; i <= maximum_degree; i++) degree_list[i] = -1;
	for (int i = 0; i < total_amt_vertices; i++) {
		vertex[i].adj_size = vertex[i].degree;
		if(vertex[i].degree > 0){
			add_to_degree_list(i); // 0-list has no effect
			current_amt_vertices++;
		}
		if(vertex[i].degree > 1) sort(i);
	}
}

int main(void){
	//start timer
	timeout = false;
	start_time = time(NULL);	
	
    //////////////////////////////////////////////////////////////////////////
    //// parse input and initialize everything
    //////////////////////////////////////////////////////////////////////////
	initialize_globals();

    //////////////////////////////////////////////////////////////////////////
    //// calculate solution 
    //////////////////////////////////////////////////////////////////////////
    unsigned int difference = data_reduction();


	if(!timeout){
		stack_top = -1;//first reduction is never reversed
		unsigned int lower_bound = calc_lower_bound();
		upper_bound = calc_upper_bound();
		
		if (lower_bound < upper_bound ) { 
			/*
			 * search for lowest vertex_cover of size k
			 * on if l_b < u_b we can still search, if we get into the while body
			 * again and they are equal, we need to do the last iteration to set
			 * the used's right
			 */
			int k = lower_bound + (upper_bound-lower_bound)/2;
			while (!timeout && lower_bound != upper_bound){
				reverse_deletion(-1);
			
				if(vc_branch(k)) {
					upper_bound = k - highest_remaining_k;
				} else {
					lower_bound = k+1;
				}
				k = lower_bound + (upper_bound-lower_bound)/2;
			}
			reverse_deletion(-1);
			vc_branch(lower_bound);
		}
		if(!timeout) difference += lower_bound;
	}
    //////////////////////////////////////////////////////////////////////////
    //// print solution
    //////////////////////////////////////////////////////////////////////////
	if(current_amt_edges == 0) current_amt_vertices = 0;
	print_reduction_solution();
    print_solution_graph(difference);
	
    return 0;
}
