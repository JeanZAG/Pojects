# include <stdio.h>
# include <stdlib.h>
# include "PCS.h"
# include "sudoku.h"

int row_checker[9][9];
int col_checker[9][9];
int block_checker[9][9];
int sol[81];

void remove_row( struct node* x,struct col* array[324]){
	struct node *tmp=x->right; struct node *rem;
	while(tmp!=x){
		if(tmp->down_n==NULL && tmp->up_n!=NULL){   //if tmp is the latest element
			tmp->up_n->down_n=NULL;
			tmp->up_n->down_c=tmp->down_c;
			tmp->down_c->up=tmp->up_n;
			rem=tmp;
			array[(tmp->id)-1]->count--;
			tmp=tmp->right;
			free(rem);
			rem=NULL;
		}
		else if(tmp->up_n==NULL && tmp->down_n!=NULL){  // if x->right is the head
            		tmp->up_c->head=tmp->down_n;
			tmp->down_n->up_n=NULL;
			tmp->down_n->up_c=tmp->up_c;
			rem=tmp;
			array[(tmp->id)-1]->count--;
			tmp=tmp->right;
			free(rem);
			rem=NULL;
		}
       		else if(tmp->up_n!=NULL && tmp->down_n!=NULL){ //if x->right is a central node
            		tmp->up_n->down_n=tmp->down_n;
            		tmp->down_n->up_n=tmp->up_n;
			rem=tmp;
			array[(tmp->id)-1]->count--;
			tmp=tmp->right;
                	free(rem);
                	rem=NULL;
        	}
		else{
			rem=tmp; 
			array[(tmp->id)-1]->count--;
			tmp=tmp->right;
                	free(rem);
		}
   	}
	x->right=x;
	x->left=x;
	return;
}


void remove_col(int i, int v, int r, int c,struct col* array[324]){
	struct node *x = array[i]->head;
	struct node *tmp;
	while((x->val!=v)||(x->col!=c)||(x->row!=r)){
		remove_row(x,array);
		tmp=x;
		x=x->down_n;
		free(tmp);
		array[i]->head=x;
		array[i]->count=(array[i]->count)-1;
	}
	x->up_n=NULL;
	x->up_c=array[i];
	array[i]->head=x;
	array[i]->up=x;
	x->down_c=array[i];
	while(x->down_n!=NULL){
		tmp=x->down_n;
		remove_row( x->down_n,array);
		x->down_n=x->down_n->down_n;
		array[i]->count=(array[i]->count)-1;
		free(tmp);
	}
	x->down_n=NULL;
	return;
}


void delete(int r, int c, int v,struct col* array[324]){
	int cond1; int cond2; int cond3; int cond4;
	cond1=(r-1)*9+c-1;
	cond2=81+(r-1)*9+v-1;
	cond3=162+(c-1)*9+v-1;
	cond4=243+(((r-1)/3)*3+((c-1)/3))*9+v-1;
	if(array[cond1]!=NULL){
        	remove_col(cond1, v, r, c,array);
        	if (array[cond1]->left==NULL && array[cond1]->right==NULL){
		}
        	else if (array[cond1]->left==NULL){
            		array[cond1]->right->left=NULL;
		}
        	else if (array[cond1]->left!=NULL && array[cond1]->right!=NULL){
            		array[cond1]->right->left=array[cond1]->left;
            		array[cond1]->left->right=array[cond1]->right;
        	}
        	else {
            		array[cond1]->left->right=NULL;
    		}
        	free(array[cond1]->head);
        	array[cond1]->head=NULL;
        	free(array[cond1]);
        	array[cond1]=NULL;
	}
    	if(array[cond2]!=NULL){
        	remove_col(cond2, v, r, c,array);
        	if (array[cond2]->left==NULL && array[cond2]->right==NULL){
		}
        	else if (array[cond2]->left==NULL){
            		array[cond2]->right->left=NULL;
		}
        	else if (array[cond2]->left!=NULL && array[cond2]->right!=NULL){
            		array[cond2]->right->left=array[cond2]->left;
            		array[cond2]->left->right=array[cond2]->right;
        	}
        	else {
            		array[cond2]->left->right=NULL;
    		}
        	free(array[cond2]->head);
        	array[cond2]->head=NULL;
        	free(array[cond2]);
        	array[cond2]=NULL;
	}
	if(array[cond3]!=NULL){
        	remove_col(cond3, v, r, c,array);
        	if (array[cond3]->left==NULL && array[cond3]->right==NULL){
		}
        	else if (array[cond3]->left==NULL){
            		array[cond3]->right->left=NULL;
		}
        	else if (array[cond3]->left!=NULL && array[cond3]->right!=NULL){
            		array[cond3]->right->left=array[cond3]->left;
            		array[cond3]->left->right=array[cond3]->right;
        	}
        	else {
            		array[cond3]->left->right=NULL;
    		}
        	free(array[cond3]->head);
        	array[cond3]->head=NULL;
        	free(array[cond3]);
        	array[cond3]=NULL;
	}
    	if(array[cond4]!=NULL){
        	remove_col(cond4, v, r, c,array);
        	if (array[cond4]->left==NULL && array[cond4]->right==NULL){
		}
        	else if (array[cond4]->left==NULL){

            		array[cond4]->right->left=NULL;
		}
        	else if (array[cond4]->left!=NULL && array[cond4]->right!=NULL){
            		array[cond4]->right->left=array[cond4]->left;
            		array[cond4]->left->right=array[cond4]->right;
        	}
        	else {
            		array[cond4]->left->right=NULL;
    		}
        	free(array[cond4]->head);
        	array[cond4]->head=NULL;
        	free(array[cond4]);
        	array[cond4]=NULL;
    	}
	return;
}

void check_file (const char *input){ //this function return 0 if the file is not adapted, and 1 if the file is adapted
	int col=1; int row=1; int c;int block; // block represent the block's position
	FILE *in=fopen(input, "r");
	if (!in){
		printf ("Failed while opening the file"); //this error appear when the file is empty
		exit(0);
	}
	while ((c=fgetc(in))!=EOF){
		if (col>10 || row>10){
			printf("FILE non adapted \n"); //this error appear when the file has an oversize
			fclose(in);
			exit(0); }
		if ((char)c!='\n'){
			if (c-48<0 || c-48>9) { //transformation of char in int
				printf("Error \n"); //this error appear when the the file contains letters or symbols
				fclose(in);
				exit(0);
			}
			if (col!=10 && c!=48){
				block=(3*((row-1)/3)+((col-1)/3))+1;
				if (row_checker[row-1][c-49]==1){
					printf("Error1 \n"); //this error appear when there are two equals numbers on the same row
					fclose(in);
					exit(0);
				}
				row_checker[row-1][c-49]=1;
				if (col_checker[col-1][c-49]==1){
					printf("Error2 \n"); //this error appear when there are two equals numbers on the same column
					fclose(in);
					exit(0);
				}
				col_checker[col-1][c-49]=1;
				if (block_checker[block-1][c-49]==1){
					printf("Error3 \n"); //this error appear when there are two equals numbers on the same block
					fclose(in);
					exit(0);
				}
				block_checker[block-1][c-49]=1;
			}
			col=col+1;
		}
		else {
			if (col!=10){
				printf("FILE non adapted \n"); //this error appear when a row contains less than 9 numbers
				fclose(in);
				exit(0);
			}
			col=1;
			row=row+1;
			}
	}
	if (row!= 10 || col != 1){
		printf ("FILE non adapted \n"); //this error appear when the file has an incorrect size
		fclose(in);
		exit(0);
	}
	fclose(in);
	return;
}

void read_file (const char *input,struct col* array[324]){
	int val; int col=1; int row=1; int c;
	check_file(input);
	FILE *in=fopen(input, "r");
	while ((c=fgetc(in))!=EOF){
		if ((c-48!=0) && (c!=10) && (col<10) &&(row<10)){
			val= c-48;
			sol[(row-1)*9+col-1]=val;
			delete(row, col, val,array);
		}
		if ((char) c!= '\n'){
			col=col+1;
		}
		else {
			col=1;
			row=row+1;
		}
	}
	return;
}

void print_array(int sol[81]){
	int i;
	printf("\n");
	for (i=0; i<81; i++){
		if ((i)%9==0){
			printf("\n");
		}
		printf("%d", sol[i]);
	}
	printf("\n");
	return;
}

int door_declaration(struct col *tab[324]){
	int i=1;
	while(tab[i-1]==NULL && i<81){
    		i++;
    		if(i==81 && tab[i-1]==NULL){ //in this case the matrix is empty 
        		i=0;
			return i;
		}
	}
	return i;
}


int min_matrix(struct col *tab[324]){
    int door=door_declaration(tab);
	if (door==0){
		return 0;
	}
	int min=door;
	struct col *x=tab[door-1];
	while(x->right!=NULL){
		if((tab[min-1]->count)>=(x->count)){ 
			min=x->id;														
		}
		x=x->right;
	}
	return min;
}


void delete_array(struct col *tab[324]){
	int i;struct node *x;
	for(i=0;i<324;i++){
		if(tab[i]==NULL||tab[i]->count==0){
			continue;}
		else{
			x=tab[i]->head;
			while(x->down_n!=NULL){
				struct node * tmp=x;
				x=x->down_n;
				free(tmp);
			}
			free(x);
		}
	}
}



void delete_copy_array(struct col *tab_aux[324]){
	create_col(tab_aux);
	create_row(tab_aux);
	int i=0;
	for(i;i<81;i++){
		if(sol[i]!=0){
		delete((i/9)+1,(i%9)+1,sol[i],tab_aux);
		}
	}
	return;
}

/*
sudoku() est la fonction principal de cette algorithme.
C'est une fonction de recherche en profondeur avec retour sur trace.
Elle est récursive: en fixant un noeud on explore toutes les combinaisons concernant ce noeuds
2 cas sont alors possibles:
	-soit le choix des noeuds mene à une impossibilité et on doit remonter jusqu'à trouver un autre chemin possible
	-soit on arrive a tout supprimer et donc à ecraser la matrice alors on a une solution et on l'affiche.  
*/
void sudoku(struct col *tab[324]){
	int door=door_declaration(tab);
	if(door==0){          	   //la matrice est vide donc on a trouver une solution du sudoku
		print_array(sol); //on a une solution donc on l'affiche
		return;
	}
	int min =min_matrix(tab);
	if(tab[min-1]->count==0){  //les noeuds précédament selectionnés ne mennent pas à une solution
		delete_array(tab);  //on remonte jusqu'a trouver une autre possibilité
		return;     
	}
	struct node *x=tab[min-1]->head;
	while(x->down_n!=NULL || x->down_c!=NULL){  //depth first search 
		if(x->down_n!=NULL){
		struct col *tab_aux[324];
			sol[((x->row)-1)*9+(x->col)-1]=x->val;  //on stock la valeurs dans la variable globale sol[]
			delete_copy_array(tab_aux);  		//on supprime x dans une copie de tab
			sudoku(tab_aux);			//on envoie l'algo sur toutes les sudoku possibles sachant x
			sol[((x->row)-1)*9+(x->col)-1]=0;	//on remonte et on initialise sol[x] à 0
			x=x->down_n;
		}
		else{
			struct col *tab_aux[324];             //meme travail mais pour les noeuds en derniere position 
			sol[((x->row)-1)*9+(x->col)-1]=x->val;
			delete_copy_array(tab_aux);     
			sudoku(tab_aux);
			sol[((x->row)-1)*9+(x->col)-1]=0;
			return;
			}
	}
}

void solve_sudoku(const char *input){
	struct col *tab_aux[324];
	delete_copy_array(tab_aux);
	read_file(input,tab_aux);	
	sudoku(tab_aux);
	return; 
}


