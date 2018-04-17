#ifndef PCS_H
#define PCS_H

//structures: 

struct col{
	int count;
	int id;
	struct col *left, *right;
	struct node *up, *head;
};

struct node{
	int val;
	int row;
	int col;
	int id;
	struct node *left, *right;
	struct node *up_n; struct col *up_c;
	struct node *down_n; struct col *down_c;
};

//prototypes de fonctions:

struct col *allocate_col(int n); 
struct node *allocate_node(int i, int v, int r, int c);
void create_col(struct col* array[324]); 
void create_row(struct col* array[324]); 

#endif 
