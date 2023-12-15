#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
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
	deletion_stack_top++;
	deletion_stack[deletion_stack_top] = v;

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
    if(current_maximum_degree > 0 && vertex[v].degree == current_maximum_degree && degree_list[current_maximum_degree] == -1){
        // find new max degree
        while(degree_list[current_maximum_degree] == -1){
            current_maximum_degree--;
        }
    }

	/*
	 * remove all "active" neighbours of v from their degree lists, update their
	 * degrees and add them back to the new correct list
	 */
	for (int i = 0;i < vertex[v].adj_size; i++) {
		int w = vertex[v].adj[i];
        if (vertex[w].used == 0) {
			remove_from_degree_list(w);

            //  decrease current_maximum_degree if needed
			if (vertex[w].degree == current_maximum_degree && degree_list[current_maximum_degree] == -1) {
				current_maximum_degree--;
			}
			vertex[w].degree--;
			vertex[w].modified = 1;
			add_to_degree_list(w);
        }
    }
}

void reverse_deletion(int stack_checkpoint){
	// as long as we didn't revert back to the desired state
    while (deletion_stack_top > stack_checkpoint) { 
		// get the most recently deleted vertex
        unsigned int v = deletion_stack[deletion_stack_top];
        vertex[v].used = 0; // set its status to "untouched"
		
        add_to_degree_list(v); // add it to its corresponding degree list
		current_amt_edges += vertex[v].degree;
		current_amt_vertices++;

		// I reallocated the current_maximum_degree update into add_to_degree_list //L: i think either both in lists function or neither, since we cant do it for the other, lets keep it here
		// if v has the highest degree now
		if (vertex[v].degree > current_maximum_degree) {
            current_maximum_degree = vertex[v].degree;
        }

		//update neighbors, only for active ones, as the other are deeper in the deletion_stack

        for (int i = 0; i < vertex[v].adj_size; i++) {
            unsigned int w = vertex[v].adj[i];
            if (vertex[w].used == 0) {
                //increase degree and add to correct list
				remove_from_degree_list(w);
				vertex[w].degree++;
				vertex[w].modified = 1;
				add_to_degree_list(w);

                //check if w has current_maximum_degree
                if (vertex[w].degree > current_maximum_degree) {
                    current_maximum_degree = vertex[w].degree;
                }
            }
        }
        deletion_stack_top--;
    }
}

int is_dominated(unsigned int v){
	if(!vertex[v].modified){
		return -1; //if it has not been modified it cant suddenly be dominated
	}
	for(int k = 0; k < vertex[v].adj_size; k++){
		unsigned int w = vertex[v].adj[k];

		if(vertex[w].used == 0 && vertex[w].degree >= vertex[v].degree){
			int j = 0;
			int i = 0;
			int covered = 0;
			while(j < vertex[w].adj_size && i < vertex[v].adj_size){
				if(vertex[vertex[v].adj[i]].used != 0 || vertex[v].adj[i] == w){//skip deleted verteces v or vertex w			
					i++;
					continue; 
				}
				if(vertex[vertex[w].adj[j]].used != 0 || vertex[w].adj[j] == v){//skip deleted verteces w or vertex v			
					j++; 
					continue;
				}		 
				if(vertex[v].adj[i] == vertex[w].adj[j]){
					covered++;
					i++;
					j++;
				}else{
					j++;
				}
			}

			if(vertex[v].degree == covered + 1){
				return w;
			}
		}
	}
	vertex[v].modified = 0; //if w is not modified we dont have to check again
	return -1;
}	

void insertion_sort(unsigned int *array, int left, int right){
	for (int i = left + 1; i < right; i++) { 
        unsigned int temp = array[i]; 
        int j = i - 1; 
        while (j >= left && array[j] > temp) { 
            array[j + 1] = array[j]; 
            j--; 
        } 
        array[j + 1] = temp; 
    } 	
}

void sort(unsigned int *array, unsigned int size){
	//int min_array_size = 42;
	//break array into subarrays of size 42 and sort them via insertionsort
	//for(int i = 0; i < vertex[v].adj_size; i += min_array_size){
		int right = size;
		//if(i < right) rightborder = i;
		insertion_sort(array ,0,right);
	//}

	//for(int i = min_array_size; i < j
	//mergesort abondend, does it really make a difference when array size is under 100 ? 
}	

void merge_vertices(unsigned int merge_into, unsigned int other, unsigned int triggerer){
	//decrease current_amt_edgesy
	current_amt_edges -= (vertex[merge_into].degree + vertex[other].degree );
	remove_from_degree_list(merge_into);
	remove_from_degree_list(other);
	remove_from_degree_list(triggerer);
	if(current_maximum_degree > 0 && degree_list[current_maximum_degree] == -1 && (vertex[merge_into].degree == current_maximum_degree 
			||	vertex[other].degree == current_maximum_degree 
			||	vertex[triggerer].degree == current_maximum_degree)) {
		// find new max degree
		while(degree_list[current_maximum_degree] == -1){
			current_maximum_degree--;
		}
	}
	
	//copy old adj list
	int *old_adj = vertex[merge_into].adj;

	//TODO: make new adj_list
	int *new_adj = (int *) malloc(sizeof(int)*(vertex[merge_into].degree + vertex[other].degree -2));
	
	
	int i = 0, j = 0, new_index = 0;
	while(i < vertex[merge_into].adj_size && j < vertex[other].adj_size){

		unsigned int v = vertex[merge_into].adj[i];
		unsigned int w = vertex[other].adj[j];
		if(v == triggerer || vertex[v].used != 0){ //skipp deleted vertices or if trigerrer 
			i++;
			continue;
		}
		if(w == triggerer || vertex[w].used != 0){
			j++;	
			continue;
		}
		if(v <= w ){
			new_adj[new_index] = v;
			i++;
			new_index++;
			if(v == w){
				remove_from_degree_list(v);
				vertex[v].degree--;
				add_to_degree_list(v);
				// check if v was max_degree, unless v had degree 0 (occurs in instance 2 1 \n 1 2)
				if(current_maximum_degree > 0 && vertex[v].degree+1 == current_maximum_degree && degree_list[current_maximum_degree] == -1){
					// find new max degree
					while(degree_list[current_maximum_degree] == -1){
						current_maximum_degree--;
					}
				}
				j++;
			}
		} else {
			new_adj[new_index] = w;

			vertex[w].adj_size++;	
			vertex[w].adj = realloc(vertex[w].adj, sizeof(int) * vertex[w].adj_size);
			vertex[w].adj[vertex[w].adj_size -1] = merge_into;
			sort(vertex[w].adj, vertex[w].adj_size);		

			j++;	
			new_index++;
		}	

	}	
	
	while(i < vertex[merge_into].adj_size){
		unsigned int v = vertex[merge_into].adj[i];
		if(vertex[v].used == 0 && v != triggerer){
			new_adj[new_index] = v;
			new_index++;
		}
		i++;
	}

	while(j < vertex[other].adj_size){
		unsigned int w = vertex[other].adj[j];
		if(vertex[w].used == 0 && w != triggerer){
			new_adj[new_index] = w;

			vertex[w].adj_size++;	
			vertex[w].adj = realloc(vertex[w].adj, sizeof(int) * vertex[w].adj_size);
			vertex[w].adj[vertex[w].adj_size -1] = merge_into;
			sort(vertex[w].adj, vertex[w].adj_size);		

			new_index++;
		}	

		j++;	
	}

	if(new_index > current_maximum_degree){
		   if(new_index > initial_maximum_degree){ 
				   degree_list = realloc(degree_list, sizeof(int) * (new_index + 1));
				   for(int i = current_maximum_degree + 1; i <= new_index; i++){
						   degree_list[i] = -1;
				   }
		   }
		   current_maximum_degree = new_index;
	}       
	
	new_adj = realloc(new_adj, sizeof(int) * new_index);

	vertex[merge_into].degree = new_index;
	vertex[merge_into].adj_size = new_index;

	add_to_degree_list(merge_into);

	vertex[merge_into].adj = new_adj;
	sort(vertex[merge_into].adj, vertex[merge_into].adj_size);	

	// make new Merge instance
	Merge *merge = (Merge *) malloc(sizeof(Merge));
	merge->merge_into = merge_into;
	merge->other = other;
	merge->triggerer = triggerer;
	merge->old_adj = old_adj;
	
	//put new Merge instance into merge_stack and into deletion_stack
	merge_stack_top++;
	merge_stack[merge_stack_top] = merge;

	//deletion_stack_top++; //without this at first and only merge in first reduction
	//deletion_stack[deletion_stack_top] = -1;

	//deleted other vertices 
	vertex[other].used = 1;

	vertex[triggerer].used = 1;

	//update edges and vertices
	current_amt_vertices -= 2;

	current_amt_edges += new_index;
}	

