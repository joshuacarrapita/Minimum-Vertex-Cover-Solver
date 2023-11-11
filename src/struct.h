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


#endif //VERTEXCOVER_STRUCT_H
