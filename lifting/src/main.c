#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "main.h"

void memcheck (void* result) {
	if (!result) {
		printf("Error at line %d.", __LINE__);
		perror("A pointer was unexpectedly NULL");
		exit(1);
	}
}

#define MAX 100
int main(void){
    ///////////////////////////////////////////////////////////////////////////
    //read input
    ///////////////////////////////////////////////////////////////////////////

	//prepare line reading
	char line[MAX]; 
	char *line_ptr = line;

	//prepare in_vc
	//skip leading comments
    while (strncmp(line, "#InputGraph", 11) != 0 && line_ptr){
		line_ptr = fgets(line, MAX, stdin);
	}
	//get graph size
	line_ptr = fgets(line, MAX, stdin);
	unsigned int total_amt_vertices = (unsigned int) strtol(strtok(line, " "), 
			(char**) NULL, 10);
	//alloc 
	bool* in_vc = (bool *) calloc(total_amt_vertices,sizeof(bool));
	memcheck(in_vc);

	//handle reduction results
	//skip to reduction results	
    while (strncmp(line, "#STARTReductionResult", 21) != 0 && line_ptr){
		line_ptr = fgets(line, MAX, stdin);
	}
	line_ptr = fgets(line, MAX, stdin);

	//set in_vc according to reduction results
	while(strncmp(line, "#ENDReductionResult", 19) != 0){
		line_ptr++; //skip '#'
		in_vc[(unsigned int) strtol(line_ptr, (char **) NULL, 10)-1] = 1;
		line_ptr = fgets(line, MAX, stdin);
    }

	//handle merge info pt. 1
	//skip to merge info
	while (strncmp(line, "#STARTMerge", 11) != 0 && line_ptr){
		line_ptr = fgets(line, MAX, stdin);
	}
	//read and store merge info
	line_ptr = fgets(line, MAX, stdin);
	line_ptr++; //skip '#'
	unsigned int total_amt_merges = (unsigned int) strtol(line_ptr, (char **) NULL, 10);
	int** merge_history_list  = (int **) calloc(total_amt_merges,sizeof(int*));
	if (total_amt_merges) memcheck(merge_history_list);
	line_ptr = fgets(line, MAX, stdin);
	for (int i = 0; i < total_amt_merges; i++) {
		line_ptr++; //skip '#'
		char* token = strtok(line_ptr,",");
		int* merge = (int *) calloc(1,sizeof(int));
		memcheck(merge);
		int token_count = 1;
		while(token) {
			memcheck(merge = realloc(merge,token_count*sizeof(int)));
			merge[token_count-1] = (unsigned int) strtol(token, (char **) NULL, 10)-1;
			token = strtok(NULL,",");
			token_count++;
		}
		merge_history_list[i] = merge;
		line_ptr = fgets(line, MAX, stdin);
    }

	//handle heuristik info
    //skip to heuristik solution
    while(strncmp(line, "#SolutionSize", 13) != 0){
		line_ptr = fgets(line, MAX, stdin);
	}
	//set in_vc according to heuristik solution
	line_ptr = fgets(line, MAX, stdin);
    while(line_ptr){
		in_vc[(unsigned int) strtol(line_ptr, (char **) NULL, 10)-1] = 1;
		line_ptr = fgets(line, MAX, stdin);
   	} 

	//handle merge info pt. 2
	//save merge info
	for (int i = total_amt_merges-1; i >= 0; i--){
		if(in_vc[merge_history_list[i][0]]){
			in_vc[merge_history_list[i][1]]=1;
		} else {
			in_vc[merge_history_list[i][2]]=1;
		}
	}

    ///////////////////////////////////////////////////////////////////////////
    //print output
    ///////////////////////////////////////////////////////////////////////////

	for (int i = 0; i < total_amt_vertices; i++) {
		if (in_vc[i]) printf("%d\n",i+1);
	}
}

