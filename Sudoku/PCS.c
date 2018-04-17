# include <stdio.h>
# include <stdlib.h>
# include "PCS.h"
# include "sudoku.h"

/*struct col est la structure qui represente l'entete de chaque colonne de la matrice.

struct col{
	int count;  //count represente le nombre de noeud qui sont attachés à la colonne
	int id;  //id represente l'identite de chaque colonne qui est liéé à une contrainte
	struct col *left, *right;  //ces pointeurs permettend de lier les colonnes entre eux
	struct node *up, *head;  //ces pointeur permettent de lier la colonne à la liste de noeuds de la colonne correspondante
};

struct row est la structure qui represente une noued (donc un 1 dans la matrice des contrainte) dans la matrice

struct node{
	int val;  //val represente la valeur qui est comprise entre 1 et 9
	int row;  //row represente la valeur de la ligne qui est comprise entre 1 et 9
	int col;  //col represente la valeur de la colonne qui est comprise entre 1 et 9
	int id;   //id represente l'id de la colonne à laquelle le noeud est attaché
	struct node *left, *right;  //ces pointeurs permet de relier les noeud d'une meme ligne
	struct node *up_n; //ce pointeur permet de relier superiorement un noeud avec un autre noeud 
	struct col *up_c;  //ce pointeur permet de relier superieurement un noeud avec une colonne
	struct node *down_n; //ce pointeur permet de relier inferieurement un noued avec un noeud
	struct col *down_c;  //ce pointeur permet de relier inferieurement un noeud avec une colonne
};
*/

/*struct col() permet de allouer une partie de la memoire grace à la fonction malloc pour contenier un struct col.
Elle initialise la variable count à 0, et tous les pointeurs à NULL
*/
struct col *allocate_col(int n){
	struct col *p= (struct col *) malloc (sizeof(struct col));
	(*p).count=0;
	(*p).id=n;
	(*p).left=NULL;
	(*p).right=NULL;
	(*p).up=NULL;
	(*p).head=NULL;
	return p;
}

/*struct node() permet de allouer une partie de la memoire grace à la fonction malloc pour contenir un struct node.
Elle initialise tous les pointeurs à NULL et il remplie les autres variables avec les parametres correspondants.
*/
struct node *allocate_node(int i, int v, int r, int c){
	struct node *p= (struct node *) malloc (sizeof(struct node));
	(*p).val=v;
	(*p).row=r;
	(*p).col=c;
	(*p).id=i;
	(*p).left=NULL;
	(*p).right=NULL;
	(*p).up_n=NULL;
	(*p).up_c=NULL;
	(*p).down_n=NULL;
	(*p).down_c=NULL;
	return p;
}

/* create_col() permet de creer les 324 struct col qui sont pointés par le tableau passé en parmaètre, 
des bien initaliser les id correspondants et 
de les lier entres eux selon l'ordre des id
*/
void create_col(struct col* array[324]){
	int i;
	array[0]=allocate_col(1);
	array[0]->left=NULL; //On initialise la pointeur de gauche de la premiere struct col à NULL
	for(i=1; i<324; i++){
		array[i]=allocate_col(i+1);
		(*array[i-1]).right=array[i];
		(*array[i]).left=array[i-1];
	}
	array[323]->right=NULL; //On initalise le pointeur de droite de la derniere struct col à NULL
	return;
}

void create_row(struct col* array[324]){
	int r=1, v=1, c=1;
	int cond1, cond2, cond3, cond4;
	int cmt=0;
	while (cmt<729){
		cmt=cmt+1;
		cond1=(r-1)*9+c-1; //first condition
		cond2=81+(r-1)*9+v-1; //second condition
		cond3=162+(c-1)*9+v-1; //third condition
		cond4=243+(((r-1)/3)*3+((c-1)/3))*9+v-1;  //fourth condition
		struct node *n1=allocate_node(cond1+1,v,r,c);
		struct node *n2=allocate_node(cond2+1,v,r,c);
		struct node *n3=allocate_node(cond3+1,v,r,c);
		struct node *n4=allocate_node(cond4+1,v,r,c);
		n1->right=n2; n2->left=n1;
		n2->right=n3; n3->left=n2;
		n3->right=n4; n4->left=n3;
		n4->right=n1; n1->left=n4;
		if(array[cond1]->head==NULL){        //head
			array[cond1]->head=n1;
			n1->up_c=array[cond1];
			array[cond1]->count++;
		}
		else{
			struct node *x1;
			x1=array[cond1]->head;
			while ((x1->down_n!=NULL)){
					x1=x1->down_n;
			}
			if (array[cond1]->count>=8){
				n1->up_n=x1;
				x1->down_n=n1;
				array[cond1]->up=n1;
				n1->down_c=array[cond1];
				array[cond1]->count++;
			}
			else{
				x1->down_n=n1;
				n1->up_n=x1;
				array[cond1]->count++;
			}
		}
		if((*array[cond2]).head==NULL){
			(*array[cond2]).head=n2;
			(*n2).up_c=array[cond2];
			array[cond2]->count++;
		}
		else{
			struct node *x2;
			x2=(*array[cond2]).head;
			while (((*x2).down_n!=NULL)){
					x2=x2->down_n;
			}
			if (array[cond2]->count>=8){
				x2->down_n=n2;
				n2->up_n=x2;
				n2->down_c=array[cond2];
				array[cond2]->up=n2;
				array[cond2]->count++;
			}
			else{
				x2->down_n=n2;
				n2->up_n=x2;
				array[cond2]->count++;
			}
		}
		if((*array[cond3]).head==NULL){
			(*array[cond3]).head=n3;
			(*n3).up_c=array[cond3];
			array[cond3]->count++;
		}
		else{
			struct node *x3;
			x3=(*array[cond3]).head;
			while (((*x3).down_n!=NULL)){
					x3=x3->down_n;
				}
			if (array[cond3]->count>=8){
				x3->down_n=n3;
				n3->up_n=x3;
				n3->down_c=array[cond3];
				array[cond3]->up=n3;
				array[cond3]->count++;
			}
			else{
				x3->down_n=n3;
				n3->up_n=x3;
				array[cond3]->count++;
			}
		}
		if((*array[cond4]).head==NULL){
			(*array[cond4]).head=n4;
			(*n4).up_c=array[cond4];
			array[cond4]->count++;
		}
		else{
			struct node *x4;
			x4=(*array[cond4]).head;
			while (((*x4).down_n!=NULL)){
					x4=x4->down_n;
				}
			if (array[cond4]->count>=8){
				x4->down_n=n4;
				n4->up_n=x4;
				n4->down_c=array[cond4];
				array[cond4]->up=n4;
				array[cond4]->count++;
			}
			else{
				x4->down_n=n4;
				n4->up_n=x4;
				array[cond4]->count++;
			}
		}
		if ((v==9)&&(c==9)){r=r+1; v=1; c=1;}
		else {if (v==9){v=1; c=c+1;}
			else{v=v+1;}}
	}
	return;
}

