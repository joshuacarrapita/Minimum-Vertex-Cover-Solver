#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include"structs.h"

//unsigned int** conn;												// Adjazenzliste
//unsigned int* count;												// Anzahl Nachbarn
unsigned int verts;												// Knotenanzahl
unsigned int edges;												// Kantenanzahl - ACHTUNG NICHT VERWENDEN
//int* used;
Vert* vlist;													// Liste aller Knoten

/*char helper(int k, unsigned int pos) {
	// k ist gesuchte Größe des VC; pos ist aktuelle Position in der Liste die wir bearbeiten
	//printf("%uhelper, k:%i, pos: %i\n", conn[4][0], k, pos);
	//for (int i=0; i<verts; i++) printf("%i ", used[i]);
	//printf("\n%u\n", conn[4][0]);
	if (k < 0) return 0;
	//printf("hi1\n");
	while (1) {
		if (pos >= verts) return 1;
		if (used[pos] == 0) break;
		pos++;
	}
	//printf("hi2, %u, %u\n", used[0], used[1]);
	//for (int i=0; i<count[pos]; i++) if (ptr[i] == 0) break;
	used[pos] = 1;
	if (helper(k-1, pos+1)) return 1;
	//printf("%u\n", used[pos]);
	used[pos] = -1;
	//printf("hä?\n");
	//unsigned int* ptr = conn[pos];
	//printf("%u\n", used[6]);
	for (int i=0; i<count[pos]; i++) {
		//printf("hi\n");
		if (used[conn[pos][i]] < 0) return 0;
		//printf("hi1: %u > %u\n", conn[4][0], pos);
		if (conn[pos][i] > pos && used[conn[pos][i]] == 0) {
			//printf("hi2\n");
			k--;
			used[conn[pos][i]] = 1;
		}
	}
	//printf("end, %u, %u\n", used[1], used[6]);
	if (helper(k, pos+1)) return 1;
	used[pos] = 0;
	for (int i=0; i<count[pos]; i++) if (conn[pos][i] > pos) used[conn[pos][i]] = 0;
	return 0;

}*/

int main(void) {
	// Ab hier: einlesen der Daten
	char* line = NULL;
	size_t len = 0;
	getline(&line, &len, stdin);

	while (line[0] == '#') getline(&line, &len, stdin);							// commend lines

	char* ptr = strtok(line, " ");
	verts = strtol(line, (char**) NULL, 10);
	ptr = strtok(NULL, " ");
	edges = strtol(ptr, (char**) NULL, 10);

	// Initialisieren der Knotenliste
	vlist = malloc(sizeof(Vert) * verts);
	for (unsigned int i=0; i<verts; i++) {
		vlist[i].count = 0;
		vlist[i].used = 0;
		vlist[i].num = i + 1;
	}

	while (edges > 0) {
		getline(&line, &len, stdin);
		if (line[0] != '#') {
			ptr = strtok(line, " ");
			int v1 = (unsigned int) strtol(ptr, (char**) NULL, 10) - 1;
			ptr = strtok(NULL, " ");
			int v2 = (unsigned int) strtol(ptr, (char**) NULL, 10) - 1;
			
			// alte Version, aktuell nicht benötigt. Falls doch bleibt das erstmal
			/*count[v1]++;
			count[v2]++;
			if (count[v1] == 1) conn[v1] = malloc(sizeof(unsigned int)); else conn[v1] = realloc(conn[v1], sizeof(unsigned int) * count[v1]);
			if (count[v2] == 1) conn[v2] = malloc(sizeof(unsigned int)); else conn[v2] = realloc(conn[v2], sizeof(unsigned int) * count[v2]);
			conn[v1][count[v1]-1] = v2;
			conn[v2][count[v2]-1] = v1;*/

			Vert* ptr1 = &vlist[v1];
			Vert* ptr2 = &vlist[v2];
			
			ptr1->count++;
			ptr1->adj = realloc(ptr1->adj, sizeof(Vert*) * ptr1->count);
			ptr1->adj[ptr1->count - 1] = ptr2;
			
			ptr2->count++;
			ptr2->adj = realloc(ptr2->adj, sizeof(Vert*) * ptr2->count);
			ptr2->adj[ptr2->count - 1] = ptr1;

			edges--;
		}
	}

	// Ab hier: Berechnung
	//printf("5: c%u, a%u; 9: c%u, a%u\n", count[4], conn[4][0], count[8], conn[8][0]);
	/*int k = 0;
	while(1) {
		//printf("%i %u\n", k, conn[4][0]);
		for (unsigned int i=0; i<verts; i++) used[i] = 0;
		//printf("%u\n", conn[4][0]);
		if (helper(k, 0)) break;
		k++;
	}

	// Ab hier: Ausgabe
	for (unsigned int i=0; i<verts; i++) if (used[i] == 1) printf("%u\n", i+1);*/

	// Zum testen, kommt bald wieder weg
	printf("Knotenzahl: %u\n", verts);
	for (unsigned int i=0; i<verts; i++) {
		printf("Knoten %u, Kanten:", vlist[i].num);
		for (unsigned int j=0; j<vlist[i].count; j++) printf(" %u", vlist[i].adj[j]->num);
		printf("\n");
	}

    	return 0;
}
