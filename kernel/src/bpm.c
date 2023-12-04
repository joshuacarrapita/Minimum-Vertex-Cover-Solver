#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include "main.h"
#include "graph.h"

int* left;
int* right;
int* dist;
//unsigned short* count;
//int** conn;
int dist_nil;

char helper2(int curr) {
    Vertex v = vertex[curr];
	for (int i = 0; i < v.adj_size; i++) {
		int next = v.adj[i];
		if (left[next] >= 0 && dist[left[next]] == dist[curr] + 1) {
			if (helper2(left[next])) {
				left[next] = curr;
				right[curr] = next;
				return 1;
			}
		} else if (left[next] == -2 && dist_nil == dist[curr] + 1) {
			left[next] = curr;
			right[curr] = next;
			return 1;
		}
	}
	dist[curr] = INT_MAX;
	return 0;
}

void bpm(int* l, int* r) {
    left = l;
    right = r;
	for (int i=0; i<total_amt_vertices; i++) {
        right[i] = -2;
	    left[i] = -2;
    }

	/*while (edges > 0) {
		getline(&line, &len, stdin);
		if (line[0] != '#') {
			ptr = strtok(line, " ");
			int l = (int) strtol(&ptr[1], (char**) NULL, 10) - 1;
			ptr = strtok(NULL, "#");
			int r = (int) strtol(&ptr[1], (char**) NULL, 10) - 1;

			if (right[r] == -1) {
				right[r] = -2;
				conn[r] = malloc(sizeof(int));
				conn[r][0] = l;
				count[r] = 1;
			} else {
				conn[r] = realloc(conn[r], sizeof(int) * (count[r] + 1));
				if (l < conn[r][count[r]-1]) {
					for (int i=0; i<count[r]; i++) {
						if (conn[r][i] > l) {
							for (int j=count[r]; j>i; j--) conn[r][j] = conn[r][j-1];
							conn[r][i] = l;
							break;
						}
					}
				} else conn[r][count[r]] = l;
				count[r]++;
			}
			if (left[l] <= -1) left[l] = -2;
			if ((left[l] <= -2) && (right[r] <= -2)) {
				left[l] = r;
				right[r] = l;
			}
			edges--;
		}
	}*/
	int* r_free = malloc(sizeof(int) * total_amt_vertices);
	int end_free = 0;
	for (int i = 0; i < total_amt_vertices; i++){
		if (right[i] <= -2) {
			r_free[end_free] = i;
			end_free++;
		}
		
	}
	dist = malloc(sizeof(int) * total_amt_vertices);

	int* queue = malloc(sizeof(int) * (total_amt_vertices+1));

	while (1) {
		int start = 0;
		int end = 0;
		for (int i = 0; i < total_amt_vertices; i++) {
			if (right[i] <= -2) {
				dist[i] = 0;
				queue[end] = i;
				end++;
			} else dist[i] = INT_MAX;
		}
		dist_nil = INT_MAX;
		while (start < end) {
			int curr = queue[start];
			start++;
			if (curr >= 0 && dist_nil > dist[curr]){
				for (int i = 0; i < vertex[curr].adj_size; i++) {
					int next = left[vertex[curr].adj[i]];
					if (next <= -2 && dist_nil == INT_MAX) {
						dist_nil = dist[curr] + 1;
						queue[end] = -2;
						end++;
					} else if (dist[next] == INT_MAX) {
						dist[next] = dist[curr] + 1;
						queue[end] = next;
						end++;
					}
				}
			}
		}
		if (dist_nil == INT_MAX) break;

		for (int i=0; i<total_amt_vertices; i++) {
			if (right[i] == -2) {
				helper2(i);
			}
		}
	}

    free(dist);
    free(r_free);
    free(queue);
}
