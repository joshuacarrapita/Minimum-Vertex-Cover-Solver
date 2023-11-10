//
// Created by levent on 11/9/23.
//
#include <stdio.h>
#include <stdbool.h>


unsigned int vertices; //number of vertices
unsigned int edges;//number of edges

int **adj; //adjacency list for each vertex

unsigned char *used;//is vertex used in vertex_cover (1), is it deleted(-1) or untouched(0)
unsigned int *neighborhood_sizes;//array of the degree of each vertex

int *degree_list;//start of list for each degree
int *degree_list_prev, *degree_list_next; //prev and next counter in degree_list, simulated as ints, not real pointer

int *command_stack;//stack of command
int stack_top; //current size of stack i.e. where the last new entry of stack is


void delete_vertex(int vertex, bool put_in_vc){
    //TODO: set used[vertex], decrease degree of every adjacent and untouched vertex
}

void reverse_commands(int stack_checkpoint){
    //TODO: reverse commad_stack up until stack_top == stack_checkpoint
}

void parse_graph(){
    //TODO: parse graph and init all the global arrays and variables
}


bool vc_branch(int k, int stack_checkpoint){
    return false;
    //TODO: check if we have a vertex_cover, otherwise branch recursively
}


int main(int argc, char **argv){
    parse_graph();
    //


    return 0;
}