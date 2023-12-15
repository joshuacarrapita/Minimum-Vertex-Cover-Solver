#include <stdlib.h>
#include <stdbool.h>
#include "main.h"
#include "graph.h"
#include "clique.h"

unsigned int number_of_cliques;

void new_clique(unsigned int v){
	//create new Clique with only vertex in it	
	Clique *new_clique = calloc(1,sizeof(Clique));

	new_clique->vertices = calloc(1,sizeof(int));
	new_clique->vertices[0] = v;

	new_clique->size = 1;
	
	vertex[v].clique = new_clique;
	number_of_cliques++;	
}

void add_to_clique(Clique *clique, unsigned int v){
	clique->vertices = realloc(clique->vertices, sizeof(int) * ++clique->size);
	clique->vertices[clique->size-1] = v;

	sort(clique->vertices, clique->size);

	vertex[v].clique = clique;
}

void reset_cliques(){
	for(int i = 0; i < total_amt_vertices; i++){
		if(vertex[i].clique){
			Clique *clique = vertex[i].clique;
			for(int j = 0; j < clique->size; j++)	{
				unsigned int w = clique->vertices[j];
				vertex[w].clique = NULL;
			}
			free(clique);
		}
	}
	number_of_cliques = 0;
}

unsigned int calc_clique_cover(){
	reset_cliques();
	for(int i = current_maximum_degree; i > 0; i--){// check for all vertices descending in degree
		int current_vertex = degree_list[i];
		
		while(current_vertex != -1){
			Clique **clique_array = NULL; 
			int clique_array_size = 0;

			for(int j = vertex[current_vertex].adj_size - 1; j > 0; j--){
				unsigned int w = vertex[current_vertex].adj[j];
				if(vertex[w].used != 0) continue; 

				Clique *clique = vertex[w].clique;
				//if clique is null
				if(!clique){
					if(vertex[w].degree < vertex[current_vertex].degree){
						break;
					}else{
						continue;
					}
				} 
				//w is in clique
				if(!clique->counter){
					clique_array = realloc(clique_array, sizeof(Clique *) * ++clique_array_size);
					clique_array[clique_array_size - 1] = clique;
				}
				clique->counter++;
			}
			Clique *max_size_clique = NULL;
			for(int j = 0; j < clique_array_size; j++){
				Clique *clique = clique_array[j];
				if(clique->counter == clique->size && (!max_size_clique || clique->size > max_size_clique->size)){
					max_size_clique = clique;
				}
				clique->counter = 0;
			}

			if(!max_size_clique){
				new_clique(current_vertex);
			}else{
				add_to_clique(max_size_clique, current_vertex);
			}
			free(clique_array);
			current_vertex = vertex[current_vertex].degree_list_next;
		}
	}

	//TODO: calcutlate result or maybe delete all vertices that are not in single cliques...	
		
	return current_amt_vertices - number_of_cliques;
}

