//
// Created by levent on 11/11/23.
//

#ifndef VERTEXCOVER_STRUCT_H
#define VERTEXCOVER_STRUCT_H

typedef struct Vertex{
    int *adj; //adjency list of vertex
    char used;//is vertex untouched(0), deleted(1) or in vc (2)
    unsigned int degree; //the current degree of vertex
    unsigned int degree_list_prev;//pointer to prev element in degree_list
    unsigned int degree_list_next;//pointer to next element in degree_list
}Vertex;
//global variables used in every file

unsigned int total_amt_vertices; // number of vertices in the graph
unsigned int total_amt_edges; // number of edges in the graph
unsigned int upper_bound;
unsigned int highest_remaining_k;
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


#endif //VERTEXCOVER_STRUCT_H
