//
// Created by levent on 11/9/23.
//
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

unsigned int vertices; //number of vertices
unsigned int edges;//number of edges

int **adj; //adjacency list for each vertex

char *used;//is vertex used in vertex_cover (2), is it deleted(1) or untouched(0)
unsigned int *degree;//array of the degree of each vertex, this is altered

int *degree_list;//start of list for each degree
int *degree_list_prev, *degree_list_next; //prev and next counter in degree_list, simulated as ints, not real pointer
int maximum_degree;

int *command_stack;//stack of command
int stack_top = -1; //current size of stack i.e. where the last new entry of stack is

int calc_lower_bound(){
    return 0;//TODO: calculate lower bound
}

void data_reduction(){
    //TODO: reduce data :-)
}

int calc_upper_bound(){
    return vertices;//TODO: calculate upper bound
}

void add_to_list(unsigned int vertex){
    unsigned int current_degree = degree[vertex];
    if (degree_list[current_degree] == -1){
        degree_list[current_degree] = vertex;
    }else{
        unsigned int first = degree_list[current_degree];
        degree_list[current_degree] = vertex;
        degree_list_prev[first] = vertex;
        degree_list_next[vertex] = first;
        degree_list_prev[vertex] = -1;
    }
}

void remove_from_list(unsigned int vertex){
    unsigned int current_degree = degree[vertex];
    if(degree_list[current_degree] == vertex){
        degree_list[current_degree] = degree_list_next[vertex];
    }
    int next = degree_list_next[vertex];
    int prev = degree_list_prev[vertex];
    if(next != -1){degree_list_prev[next] = prev;}
    if(prev != -1){degree_list_next[prev] = next;}
    degree_list_next[vertex] = -1;
    degree_list_prev[vertex] = -1;
}

void delete_vertex(int vertex, bool put_in_vc){
    //TODO: set used[vertex], decrease degree of every adjacent and untouched vertex
}

void reverse_commands(int stack_checkpoint){
    //TODO: reverse command_stack up until stack_top == stack_checkpoint




}

bool vc_branch(int k, int stack_checkpoint){
    //TODO: reduce data
    if(k < 0){
        return false;
    }
    if(maximum_degree <= 0){
        return true;
    }
    unsigned int v = degree_list[maximum_degree];
    delete_vertex(v, 1);
    if(vc_branch(k - 1, stack_top)){
        return true;
    }
    reverse_commands(stack_checkpoint);
    for(int i = 0, j = 0; j < degree[v];i++){
        if(used[adj[v][i]] == 0){
            delete_vertex(used[adj[v][i]], 1);
            j++;
        }
    }
    if(vc_branch(k - degree[v], stack_top)){
        return true;
    }
    return false;
}

int main(int argc, char **argv){
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////parse input and initialize everything
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    char* line = NULL;
    size_t len = 0;
    getline(&line, &len, stdin);

    while (line[0] == '#'){ getline(&line, &len, stdin);}					// ignore comment lines

    //init edge and vertex size in first line
    char *ptr = strtok(line, " ");
    vertices = strtol(ptr, (char**) NULL, 10);
    ptr = strtok(NULL, " ");
    edges = strtol(ptr, (char**) NULL, 10);

    //init all arrays
    int *adj_[vertices];
    adj = adj_;

    char used_[vertices];
    used = used_;

    unsigned int degree_[vertices];
    degree = degree_;

    int degree_list_next_[vertices], degree_list_prev_[vertices];
    degree_list_next = degree_list_next_;
    degree_list_prev = degree_list_prev_;

    int command_stack_[vertices +1];//+1 because when
    command_stack = command_stack_;

    for (int i = 0; i < vertices; i++){
        adj[i] = NULL; // FREE LATER
        used[i] = 0;
        degree[i] = 0;
        degree_list_prev[i] = -1;// -1 is equivalent to NULL in this case
        degree_list_next[i] = -1;
    }

    int counter_edges = 0;
    maximum_degree = 0;
    while(counter_edges < edges) {
        getline(&line, &len, stdin);
        if (line[0] != '#') {
            //get vertices of edge
            ptr = strtok(line, " ");
            unsigned int v = (unsigned int) strtol(ptr, (char **) NULL, 10) - 1;
            ptr = strtok(NULL, " ");
            unsigned int w = (unsigned int) strtol(ptr, (char **) NULL, 10) - 1;
            //increase degree for both
            degree[v]++;
            degree[w]++;
            //add to adj[vertex] for both
            adj[v] = realloc(adj[v], sizeof(int) * degree[v]);
            adj[v][degree[v] - 1] = w;
            adj[w] = realloc(adj[w], sizeof(int) * degree[w]);
            adj[w][degree[w] - 1] = v;


            if(degree[v] > maximum_degree && degree[v] > degree[w]){
                maximum_degree = degree[v];
            }else if(degree[w] > maximum_degree){
                maximum_degree = degree[w];
            }

            counter_edges++;
        }
    }
    //initialize degree_list
    int degree_list_[maximum_degree + 1];//+1 because we have vertices with 0 adj
    degree_list = degree_list_;
    for(int i = 0; i < vertices; i++){
        add_to_list(i);
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////
    ////Calculating Solution here
    //////////////////////////////////////////////////////////////////////////////////////////////////
    //TODO: simple upper lower bound
    int k = calc_lower_bound();
    int upper_bound = calc_upper_bound();
    while(!vc_branch(k, -1) && k < upper_bound){
        reverse_commands(-1);//set back to init
        k++;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////print Solution
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    for(int i = 0; i < vertices; i++){
        if(used[i] == 1){
            printf("%d\n", i +1);
        }
    }
    return 0;
}