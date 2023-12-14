#include <stdlib.h>
#include <stdbool.h>
#include "main.h"
#include "graph.h"
#include "clique.h"

Clique **clique_lists;
unsigned int clique_lists_size = 0;
unsigned int number_of_cliques = 0;

unsigned int max_clique_size = 0;

void new_clique(unsigned int vertex){
	//create new Clique with only vertex in it	
	Clique *new_clique = calloc(1,sizeof(Clique));

	new_clique->vertices = calloc(1,sizeof(int));
	new_clique->vertices[0] = vertex;

	new_clique->size = 1;
	new_clique->next = NULL;
	new_clique->prev = NULL;
	if(!max_clique_size) max_clique_size++;
	
	add_to_clique_lists(new_clique);
	number_of_cliques++;	
}

void remove_from_clique_lists(Clique *clique){
	//TODO: remove from current clique list
	unsigned int list_index = clique->size-1; // get clique lists index
	// update list head and max_clique_size if needed
	if (clique_lists[list_index] == clique) {
		clique_lists[list_index] = clique->next;
		if (!clique_next && clique->size == max_clique_size) {
			max_clique_size--;
			while (list_index > 0) {
				list_index--;
				max_clique_size--;
				if (clique_lists[list_index]) {
					max_clique_size = list_index+1;
					break;
				}
			}
		}
	}
	// connect prev and next if needed
	Clique* next_clique = clique->next;
	Clique* prev_clique = clique->prev;
	if (next) next_clique->prev = prev;
	if (prev) prev_clique->next = next;
	// reset pointers as clique is currently in no list
	clique->next = NULL;
	clique->prev = NULL;
}

void add_to_clique_lists(Clique *clique){
	//TODO: add to clique_list of the right size	
	if (clique_lists_size < current_maximum_degree) {
		clique_lists = realloc(clique_lists, current_maximum_degree * sizeof(Clique *));
		for (int i = clique_lists_size; i < current_maximum_degree; i++){
			clique_lists[i] = NULL;
		}
		clique_lists_size = current_maximum_degree;
	}
	unsigned int list_index = clique->size-1; // get clique lists index
	Clique* first = clique_lists[list_index]; // get list head
	clique_lists[list_index] = clique; // ours is new head
	if (first) first->prev = clique; // and comes before old head
	clique->next = first;
	clique->prev = NULL;
}

void add_to_clique(Clique clique, unsigned int vertex){
	remove_from_clique_lists(clique);
	clique->vertices = realloc(clique->vertices, sizeof(int) * ++clique->size);
	if(clique->size > max_clique_size){
		max_clique_size = clique->size;
	}
	add_to_clique_lists(clique);
	sort(clique->vertices);
}

char in_clique(Clique clique, unsigned int vertex){
	//TODO: check if vertex is in clique
	char result = 1;	
	unsigned int *adj = vertex[vertex].adj;
	int i = 0, j = 0;
	while(i < vertex[vertex].adj_size && j < clique->size){
		unsigned int w = adj[i];
		if(vertex[w].used != 0){
			i++;
			continue;
		}
		if(clique->vertices[j] == w){
			i++;
			j++;
		}
	}
	if(j < clique->size){
		result = 1;
	}

	return result;
}

unsigned int calc_clique_cover(){
	unsigned int biggest_clique = 0;	
	clique_lists = malloc(current_maximum_degree * sizeof(Clique *));
	clique_lists_size = current_maximum_degree
	for(int i = 0; i < current_maximum_degree; i++){
		clique_lists[i] = NULL;
	}

	for(int i = current_maximum_degree; i > 0; i--){// check for all vertices descending in degree
		unsigned int current_vertex = degree_list[i];
		
		bool found_clique = true;
		while(current_vertex != -1){
			for(int j = i; j > 0; j--){//check for vertice for all cliques in descending size

				Clique *clique = clique_lists[i];		
				while(clique){
					if(in_clique(clique, current_vertex)){
						add_to_clique(clique, current_vertex);
						found_clique = true;
						break;
					}else{ //get next clique
						clique = clique->next; 
					}
				}
				if(found_clique) break;
			}
			if(!found_clique){
				new_clique(current_vertex);
			}
			current_vertex = vertex[current_vertex].degree_list_next;
		}
	}

	//TODO: calcutlate result or maybe delete all vertices that are not in single cliques...	
	unsigned int result = current_amt_vertices - number_of_cliques;

	return result; 
}
