#include <stdio.h>
#include <string.h>

int main(void){
    ////////////////////////////////////////////////////////////////////////////////////////
    //read input
    ///////////////////////////////////////////////////////////////////////////////////////////////////

	//prepare line reading
	char* line = NULL;
	size_t len = 0;

    //skip to our comments
    do {
        getline(&line, &len, stdin);
    } while (strcmp(line, "#STARTReductionSolution\n") != 0) ;
	getline(&line, &len, stdin);

    //print every vertex, because were are not merging yet 
    while(strcmp(line, "#ENDReductionSolution\n") != 0){
        if(line[0] != '\0') line++;
        printf("%s", line);
        getline(&line, &len, stdin);
    }

    //skip to heuristik solution
    while(strcmp(line, "#StartSolution\n") != 0) getline(&line, &len, stdin);

    while(getline(&line, &len, stdin) != -1){
        printf("%s", line);
    }
    
}
