#include <stdlib.h>
#include <stdbool.h>
#include "main.h"
#include "graph.h"

void add_to_degree_list(unsigned int v) {
    unsigned int current_degree = vertex[v].degree;
	 //put v to front of list
    unsigned int first = degree_list[current_degree];
    degree_list[current_degree] = v;

    if(first != -1) vertex[first].degree_list_prev = v;
    vertex[v].degree_list_next = first;
    vertex[v].degree_list_prev = -1;
}

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
	current_amt_edges -= vertex[v].degree;
	current_amt_vertices -= 1;

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

void reverse_deletion(int stack_checkpoint){
	// as long as we didn't revert back to the desired state
    while (stack_top > stack_checkpoint) { 
		// get the most recently deleted vertex
        unsigned int v = deletion_stack[stack_top];
        vertex[v].used = 0; // set its status to "untouched"
        add_to_degree_list(v); // add it to its corresponding degree list
		current_amt_edges += vertex[v].degree;
		current_amt_vertices += 1;

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

char dominates(unsigned int v){
	for(int k = 0; k < vertex[v].adj_size; k++){
		unsigned int w = vertex[v].adj[k];
		if(vertex[w].used == 0 && vertex[v].degree >= vertex[w].degree){
			if(vertex[w].degree < 2){
				return 1;
			}
			int j = 0;
			int covered = 0;
			for(int i = 0; i < vertex[v].adj_size; i++){
				if(vertex[v].adj[i] == vertex[w].adj[j] && vertex[vertex[v].adj[i]].used == 0 ){
					j++;
					if(vertex[vertex[w].adj[j]].used == 0 && vertex[w].adj[j] != v){
						covered++;
					}
				}
				if(covered = vertex[w].degree == covered){
					return 1;
				}
			}
		}
	}
	return 0;
}	

void insertion_sort(unsigned int v, int left, int right){
	for (int i = left + 1; i < right; i++) { 
        unsigned int temp = vertex[v].adj[i]; 
        int j = i - 1; 
        while (j >= left && vertex[v].adj[j] > temp) { 
            vertex[v].adj[j + 1] = vertex[v].adj[j]; 
            j--; 
        } 
        vertex[v].adj[j + 1] = temp; 
    } 	
}

void sort(unsigned int v){
	//int min_array_size = 42;
	//break array into subarrays of size 42 and sort them via insertionsort
	//for(int i = 0; i < vertex[v].adj_size; i += min_array_size){
		int right = vertex[v].adj_size;
		//if(i < right) rightborder = i;
		insertion_sort(v,0,right);
	//}

	//for(int i = min_array_size; i < j
	//mergesort abondend, does it really make a difference when array size is under 100 ? 
}	

