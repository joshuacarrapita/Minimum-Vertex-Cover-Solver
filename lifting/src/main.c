#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX 100
int main(void){
    ////////////////////////////////////////////////////////////////////////////////////////
    //read input
    ///////////////////////////////////////////////////////////////////////////////////////////////////

	//prepare line reading
	char line[MAX]; 
	char *s;
	size_t len = 0;

    //skip to our comments
    while (strncmp(line, "#STARTReductionResult", 21) != 0 && s != NULL){
		s = fgets(line, MAX, stdin);
	}
	//go to line after #STARTReduct...
	s = fgets(line, MAX, stdin);
	while(strncmp(line, "#ENDReductionResult", 19) != 0){
		char *printedline = &line;
		printedline++;
        printf("%s", printedline);
		s = fgets(line, MAX, stdin);
    }

    //skip to heuristik solution
    while(strncmp(line, "#SolutionSize", 13) != 0){
		s = fgets(line, MAX, stdin);
	}
    while(s != NULL){
		s = fgets(line, MAX, stdin);
		printf("%s", line);
   	} 
}
