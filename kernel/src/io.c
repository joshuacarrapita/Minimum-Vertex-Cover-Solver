#include "main.h"

void parseinput(){
	// prepare line reading
	char* line = NULL;
	size_t len = 0;
	getline(&line, &len, stdin);
	
 	// ignore comment lines
	while (line[0] == '#') getline(&line, &len, stdin);

	// init total edge and vertex count
	char *ptr = strtok(line, " ");
	total_amt_vertices = strtol(ptr, (char**) NULL, 10);
	ptr = strtok(NULL, " ");
	total_amt_edges = strtol(ptr, (char**) NULL, 10);

	// alloc vertex array and init with standard content
	vertex = malloc(total_amt_vertices * sizeof(Vertex));
	for (int i = 0; i < total_amt_vertices; i++) {
		vertex[i].adj = NULL;
		vertex[i].used = 0;
		vertex[i].used = 0;
		vertex[i].degree_list_prev = -1; // -1 symbolises NULL
		vertex[i].degree_list_next = -1;
	}

	// parse edges to update vertex degrees and fill their adjacency lists
	int counter_edges = 0;
	while (counter_edges < total_amt_edges) {
		getline(&line, &len, stdin);
		if (line[0] != '#') { // skip comments
							  
			// get vertices v and w of edge
			ptr = strtok(line, " ");
			unsigned int v = (unsigned int) strtol(ptr, (char **) NULL, 10) - 1;
			ptr = strtok(NULL, " ");
			unsigned int w = (unsigned int) strtol(ptr, (char **) NULL, 10) - 1;

			// increase degree for both
			vertex[v].degree++;
			vertex[w].degree++;

			// add to each others adjacency lists
			vertex[v].adj = realloc(vertex[v].adj, sizeof(int) * vertex[v].degree);
			vertex[v].adj[vertex[v].degree - 1] = w;
			vertex[w].adj = realloc(vertex[w].adj, sizeof(int) * vertex[w].degree);
			vertex[w].adj[vertex[w].degree - 1] = v;

			// update maximum_degree if necessary
			if (vertex[v].degree > maximum_degree) {
				maximum_degree = vertex[v].degree;
			}
			if (vertex[w].degree > maximum_degree) {
				maximum_degree =vertex[w].degree;
			}

			counter_edges++;
		}
	}
}
