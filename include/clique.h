#ifndef LOWERBOUND_INCLUDE_CLIQUE_H
#define LOWERBOUND_INCLUDE_CLIQUE_H

typedef struct Clique{
	unsigned int *vertices;
	unsigned int size;
	Struct Clique *next;
	Struct Clique *prev;
} Clique;

void new_clique(unsigned int vertex);

void remove_from_clique_lists(Clique *clique);

void add_to_clique_lists(Clique *clique);

void add_to_clique(Clique clique, unsigned int vertex);

char in_clique(Clique clique, unsigned int vertex);

unsigned int calc_clique_cover();

#endif
