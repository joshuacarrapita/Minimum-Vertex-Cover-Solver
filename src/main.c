#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "main.h"
#include "graph.h"
#include "io.h"
#include "bpm.h"

unsigned int total_amt_vertices;
unsigned int total_amt_edges;
							  
unsigned int current_amt_vertices;
unsigned int current_amt_edges;

unsigned int lower_bound;
unsigned int upper_bound;
unsigned int highest_remaining_k;

Vertex *vertex;

int initial_maximum_degree;
int current_maximum_degree; 
int *degree_list; 

int *deletion_stack;
int deletion_stack_top; //index of the "topmost" /
						//"most recently added" stack entry
				
Merge **merge_stack;
int merge_stack_top;

time_t start_time;
bool timeout;

unsigned int greedy_clique_cover(){

}

unsigned int calc_lower_bound(){
    unsigned int result = 0;
    result = greedy_clique_cover();
    if( current_maximum_degree > 0 ) { // don't divide by 0 when edges == max_degree == 0
		result = (current_amt_edges + (current_maximum_degree-1))/current_maximum_degree; // weird term rounds up
	}
    return result;
}

unsigned int calc_upper_bound(int reverse_point){
	unsigned int result = 0;
	if (current_maximum_degree == 0) { // recursion hook, all edges removed
		return result;
	}
	// find max degree vertex v, calculate on G-{v}
	unsigned int v = degree_list[current_maximum_degree]; 
	delete_vertex(v,1);
	result = 1 + calc_upper_bound(NO_REVERSE); // update size of vc by +1 for v
	if (reverse_point != NO_REVERSE) {
		reverse_deletion(reverse_point);
	}
    return result;
}

unsigned int do_lp(){
    unsigned int difference = 0;
    
    int* left = malloc(sizeof(int) * total_amt_vertices);
    int* right = malloc(sizeof(int) * total_amt_vertices);

    bpm(left, right);
    char* reach_r = malloc(sizeof(char) * total_amt_vertices);
    char* reach_l = malloc(sizeof(char) * total_amt_vertices);
    for (unsigned int i = 0; i < total_amt_vertices; i++) {
        reach_r[i] = 0;
        reach_l[i] = 0;
    }

    unsigned int* queue_r = malloc(sizeof(unsigned int) * total_amt_vertices);
    unsigned int* queue_l = malloc(sizeof(unsigned int) * total_amt_vertices);
    int r_curr = 0;
    int l_curr = 0;
    int r_max = 0;
    int l_max = 0;
    char t_reached = 0;

    for (unsigned int i = 0; i < total_amt_vertices; i++) {
        if (left[i] <= -1) {
            queue_l[l_max] = i;
            l_max++;
            reach_l[i] = 1;
        }
    }
    while (l_curr < l_max || r_curr < r_max) {
        while (l_curr < l_max) {
            Vertex v = vertex[queue_l[l_curr]];
            l_curr++;
            for (unsigned int i = 0; i < v.adj_size; i++) {
                if (!reach_r[v.adj[i]]) {
                    reach_r[v.adj[i]] = 1;
                    queue_r[r_max] = v.adj[i];
                    r_max++;
                }
            }
        }
        while (r_curr < r_max) {
            unsigned int v = queue_r[r_curr];
            r_curr++;
            if (left[v] >= 0) {
                if (!reach_l[left[v]]) {
                    reach_l[left[v]] = 1;
                    queue_l[l_max] = left[v];
                    l_max++;
                }
            }else{
				if(!t_reached) {
					for (unsigned int i=0; i<total_amt_vertices; i++) {
						if (right[i] >= 0 && !reach_r[i]) {
							reach_r[i] = 1;
							queue_r[r_max] = i;
							r_max++;
						}
					}
				}
				t_reached = 1;
			}	
        }
    }
    
    for (unsigned int i = 0; i < total_amt_vertices; i++) {
        if (reach_l[i] && !reach_r[i]) {
            delete_vertex(i, 0);
        }
        if (!reach_l[i] && reach_r[i]) {
            delete_vertex(i, 1);
            difference++;
        }
    }   

    free(left);
    free(right);
    free(reach_r);
    free(reach_l);
    free(queue_r);
    free(queue_l);

	return difference;
}	

unsigned int data_reduction(bool should_merge) {
    //TODO: more reduction rules
	bool done = false;
	bool found = false;
    unsigned int k = 0;

	while(!done){
		found = false;
		//degree 0 rule 
		if (current_maximum_degree == 0) { // In case there is only a degree 0 list
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
		if(current_time - start_time > 118) {
			timeout = true;
			return k;
		}
		if (found) continue;

		//degree 2-rule
		if(current_maximum_degree >= 2){
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
				} else {
					//TODO: forward deg-2 rule, substancially more work required
					//TODO: Handle lower_bound and merging
					if(should_merge){
						merge_vertices(neighbors[(x +1)% 2], neighbors[x], v);	
						found = true;
						k++;
					}
					unsigned int next = vertex[v].degree_list_next;
					v = next;
				}
			}
		}
		if (found) continue;

		//domination-rule
		for(int i = 2; i <= current_maximum_degree; i++){
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
		if(current_time - start_time > 118){
			timeout = true;
			return false;
		}
	}
    //TODO: reduce data
    k -= data_reduction(NO_MERGE);
    if (k < 0) { // Recursion hook, no vertex cover was found
        return false;
    }
	// graph is edgless => vertex_cover of size k
	// TODO: IMO this could be current_maximum_degree == 0, there is no situation where
	// it should be negative
    if (current_maximum_degree <= 0) { 
		highest_remaining_k = k;
        return true;
    }
	// we want to find a vc with k vertices but need more -> impossible
	if (k < calc_lower_bound()) return false;


	// pick a vertex of maximum degree (this is an attempt at an heuristic)
    unsigned int v = degree_list[current_maximum_degree]; 
    int stack_checkpoint = deletion_stack_top;
	delete_vertex(v, 1);
	if (vc_branch(k - 1)) {
        return true; // it makes a vertex cover of size k with our deleted one
    }
	

	if(timeout) return false;
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
	 * check with all neighbours in vc
	 */ 
    if (vc_branch(k - degree_v)) {
        return true; // it makes a vertex cover of size k with our deleted ones
    }

	// else we were out of luck with vertex covers for size k in this subgraph
    return false; 
}

void initialize_globals(){
	current_maximum_degree = 0;

	parse_input();

	initial_maximum_degree = current_maximum_degree;
	current_amt_edges = total_amt_edges;
	current_amt_vertices = total_amt_vertices;

	// alloc deletion_stack mem and init with standard content
	deletion_stack = malloc(total_amt_vertices * sizeof(int));
	for (int i = 0; i < total_amt_vertices; i++) deletion_stack[i] = -1;
	deletion_stack_top = -1;
	
	merge_stack = malloc(total_amt_vertices * sizeof(Merge *));
	merge_stack_top = -1;
	/*
	 * alloc degree_list array and init with content
	 * -> set first elements of all degree lists to -1 (symbolises NULL)
	 * -> add each vertex to its corresponding degree list
	 */
	degree_list = malloc(sizeof(int) * (current_maximum_degree + 1));
	for (int i = 0; i <= current_maximum_degree; i++) degree_list[i] = -1;
	for (int i = 0; i < total_amt_vertices; i++) {
		vertex[i].adj_size = vertex[i].degree;
		if(vertex[i].degree > 0){
			add_to_degree_list(i); // 0-list has no effect
			current_amt_vertices++;
		}
		if(vertex[i].degree > 1) sort(vertex[i].adj, vertex[i].adj_size);
	}

	lower_bound = calc_lower_bound();
	upper_bound = calc_upper_bound(deletion_stack_top);
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
    unsigned int init_lower_bound = do_lp();
	current_amt_vertices = 0;
	for(int i = 0; i < total_amt_vertices; i++){
		if(vertex[i].used == 0 && vertex[i].degree > 0) current_amt_vertices++;
	}
	init_lower_bound += data_reduction(MERGE);
	deletion_stack_top = -1;//first reduction is never reversed
	if (init_lower_bound == total_amt_vertices) {
		lower_bound = init_lower_bound;
		upper_bound = init_lower_bound;
	} else {
		lower_bound = calc_lower_bound();
		upper_bound = calc_upper_bound(NO_REVERSE);
	}

	if(!timeout){
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
				} else if (!timeout) {
					lower_bound = k+1;
				}
				k = lower_bound + (upper_bound-lower_bound)/2;
			}
			reverse_deletion(-1);
			vc_branch(lower_bound);
		}
	}

	lower_bound += init_lower_bound;

    //////////////////////////////////////////////////////////////////////////
    //// print solution
    //////////////////////////////////////////////////////////////////////////
	print_lower_bound();
	
    return 0;
}
