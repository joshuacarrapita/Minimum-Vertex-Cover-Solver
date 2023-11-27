#include <stdio.h>
#include <string.h>

int main(void){
    ////////////////////////////////////////////////////////////////////////////////////////
    //read input
    ///////////////////////////////////////////////////////////////////////////////////////////////////

	//prepare line reading
	char* line = "";
	size_t len = 0;

    //skip to our comments
    while (strncmp(line, "#STARTReductionSolution", 23) != 0) getline(&line, &len, stdin);

	//go to line after #STARTReduct...
	getline(&line, &len, stdin);

    //print every vertex, because were are not merging yet 
    while(strncmp(line, "#ENDReductionSolution", 21) != 0){
        if(line[0] != '\0') line++;
        printf("%s", line);
        getline(&line, &len, stdin);
    }

    //skip to heuristik solution
    while(strncmp(line, "#SolutionSize", 13) != 0) getline(&line, &len, stdin);

    while(getline(&line, &len, stdin) != -1) printf("%s", line);
    
}
