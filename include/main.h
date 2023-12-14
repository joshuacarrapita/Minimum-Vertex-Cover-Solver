#ifndef KERNEL_INCLUDE_MAIN_H
#define KERNEL_INCLUDE_MAIN_H

#include "graph.h"

//////////////////////////////////////////////////////////////////////////
//// global variables used in every file
//////////////////////////////////////////////////////////////////////////
extern unsigned int total_amt_vertices;
extern unsigned int total_amt_edges;
							  
extern unsigned int current_amt_vertices;
extern unsigned int current_amt_edges;

extern unsigned int upper_bound;
extern unsigned int lower_bound;
extern unsigned int highest_remaining_k;
extern Vertex *vertex;

#define MAX 100
#define NO_REVERSE -2
#define MERGE 1
#define NO_MERGE 0

extern int initial_maximum_degree;
extern int current_maximum_degree; // holds the value for the highest degree found in the graph

/*
 * INFO: an array of size current_maximum_degree+1 that holds the index of the first
 * vertex that represents a degree
 *
 * EXAMPLE: degree_list[i] holds value x if vertex x is the first in the list of
 * all vertices of degree i, "the list" is constructed through the arrays
 * 'degree_list_prev' and 'degree_list_next'
 *
 * MODIFICATION: content can be altered after initialisation
 */
extern int *degree_list; 

/*
 * stack of deletions we executed to be able to reverse the graph to a previous
 * condition, can be at most total_amt_vertices long as we can't delete more
 * vertices than that
 */
extern int *deletion_stack;
extern int deletion_stack_top; // index of the "topmost" /
							   // "most recently added" stack entry
					  
extern Merge **merge_stack;
extern int merge_stack_top;

//////////////////////////////////////////////////////////////////////////
//// functions
//////////////////////////////////////////////////////////////////////////
unsigned int calc_lower_bound();

unsigned int calc_upper_bound(int reverse_point);

unsigned int data_reduction(bool should_merge);

bool vc_branch(int k);

/*
 * Parses input and initialises global variables with it.
 */
void initialize_globals();

#endif
