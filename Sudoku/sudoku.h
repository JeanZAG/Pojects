#ifndef SUDOKU_H
#define SUDOKU_H

#include "PCS.h"

//variables globales: 

int row_checker[9][9];
int col_checker[9][9];
int block_checker[9][9];
int sol[81];

//protypes des fonctions: 

void remove_row( struct node* x,struct col* array[324]); 
void remove_col(int i, int v, int r, int c,struct col* array[324]);
void delete(int r, int c, int v,struct col* array[324]);
void check_file (const char *input);
void read_file (const char *input,struct col* array[324]);
void print_array(int sol[81]);
int door_declaration(struct col *tab[324]);
int min_matrix(struct col *tab[324]);
void delete_array(struct col *tab[324]);
void delete_copy_array(struct col *tab_aux[324]);
void sudoku(struct col *tab[324]); 
void solve_sudoku(const char *input); 

#endif
