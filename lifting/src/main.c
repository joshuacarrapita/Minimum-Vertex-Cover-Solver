#include <stdio.h>
#include <string.h>

int main(void){
    ////////////////////////////////////////////////////////////////////////////////////////
    //read input
    ///////////////////////////////////////////////////////////////////////////////////////////////////

	// prepare line reading
	char* line = NULL;
	size_t len = 0;

    //skipp to our comments
    char* our_solution_start = "#STARTReductionSolution";
    do{
        getline(&line, &len, stdin);
    }while (strcmp(line, our_solution_start)!= 0) ;
	getline(&line, &len, stdin);

    //print every vertice, because were are not merging yet 
    char *our_solution_end = "#ENDReductionSolution";
    while(strcmp(line, our_solution_end) != 0){
        if(line[0] != '\0') line++;
        printf("%s", line);
        getline(&line, &len, stdin);
    }

    //skip to heuristik solution
    char *start_solution = "#StartSolution";
    while(strcmp(line, start_solution) != 0) getline(&line, &len, stdin);
    getline(&line, &len, stdin);

    while(getline(&line, &len, stdin) != -1){
        printf("%s", line);
    }
    
}