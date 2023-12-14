#include <stdlib.h>
#include "main.h"
#include "graph.h"
#include "clique.h"

Clique **clique_lists;
unsigned int number_of_cliques = 0;

void new_clique(unsigned int vertex){
	//create new Clique with only vertex in it	
	Clique *new_clique = malloc(sizeof(Clique));

	new_clique->vertices = malloc(sizeof(int));
	new_clique->vertices[0] = vertex;

	new_clique->size = 1;
	new_clique->next = NULL;
	new_clique->prev = NULL;
	
	add_to_clique_list(new_clique);
	number_of_cliques++;	
}

void remove_from_clique_list(Clique *clique){
	//TODO: remove from current clique list
}

void add_to_clique_list(Clique *clique){
	//TODO: add to clique_list of the right size
}

void add_to_clique(Clique clique, unsigned int vertex){
	remove_from_clique_list(clique);
	clique->vertices = realloc(clique->vertices, sizeof(int) * ++clique->size);
}

char in_clique(Clique clique, unsigned int vertex){
	//TODO: check if vertex is in clique
	return 0;
}

unsigned int calc_clique_cover(){
	unsigned int biggest_clique = 0;	
	clique_lists = malloc(current_maximum_degree * sizeof(clique *));

	for(int i = 0; i < current_maximum_degree; i++){
		clique_lists[i] = NULL;
	}

	for(int i = current_maximum_degree; i > 0; i--){// check for all vertices descending in degree
		unsigned int current_vertex = degree_list[i];
		while(current_vertex != -1){
			for(j = i; j > 0; j++){//check for vertice for all cliques in descending size
				char found_clique = 0;
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
