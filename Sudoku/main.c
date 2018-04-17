# include <stdio.h>
# include <stdlib.h>
# include "PCS.h"
# include "sudoku.h"

int main(int argv, char **argc){
	solve_sudoku(argc[1]); 
	printf("\nToutes les solutions ont été affichées \n\n");
	return 0; 
}
