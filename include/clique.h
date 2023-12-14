#ifndef LOWERBOUND_INCLUDE_CLIQUE_H
#define LOWERBOUND_INCLUDE_CLIQUE_H

typedef struct Clique{
	unsigned int *vertices;
	unsigned int size;
	Struct Clique *next;
	Struct Clique *prev;


}Clique;

unsigned int calc_clique_cover();

#endif
