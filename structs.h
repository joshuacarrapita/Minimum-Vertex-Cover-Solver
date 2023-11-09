#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct Vert {
	unsigned int num;
	struct Vert** adj;
	char used;
	unsigned int count;
} Vert;

typedef struct Element {
	struct Element* prev;
	struct Element* next;
	struct Vert* data;
} Element;
#endif
