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
	char *line_ptr = line;

    //skip to our comments
    while (strncmp(line, "#STARTReductionResult", 21) != 0 && line_ptr){
		line_ptr = fgets(line, MAX, stdin);
	}
	//go to line after #STARTReduct...
	line_ptr = fgets(line, MAX, stdin);
	while(strncmp(line, "#ENDReductionResult", 19) != 0){
		char *printedline = line;
		printedline++;
        printf("%s", printedline);
		line_ptr = fgets(line, MAX, stdin);
    }

    //skip to heuristik solution
    while(strncmp(line, "#SolutionSize", 13) != 0){
		line_ptr = fgets(line, MAX, stdin);
	}
    while(line_ptr){
		line_ptr = fgets(line, MAX, stdin);
		printf("%s", line);
   	} 
}
