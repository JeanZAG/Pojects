#include <stdio.h>
#include <stdlib.h>

/*
la résolution du sudoku par le biais des Dancing links nous a demander des créer une matrice de résolution de contraintes
dans cette matrice il y a 729 lignes car 9*9*9 possibilités
et 324 colonnes qui correspondent aux 324 contraintes qu'il y a dans une grille de sudoku 9*9
En effet il y a 4 contrainte correspondant à:
    -toutes les cases doivent être remplies (81)
    -chaques lignes doit avoir une seul 1, 1 seul 2, etc. (81 contraintes ici aussi)
    -de meme pour les colonnes
    -de meme pour les blocs
on a bien : 81*4=324
cette matrice est remplie de 1 et de 0 mais toute l'information est contenue dans les 1
on crée donc des listes doublement chaînés pour représenté les liens entres les lignes et les colonnes 
ceci permet de prendre moins de place en mémoire
Nous avons choisi de pouvoir acceder a chaque colonne sans parcourir les colonnes précédentes en créant un tableau de taille 324
dont chaque case pointe vers une unique colonne
ensuite grâce 
    



*/
int row_checker[9][9];
int col_checker[9][9];
int block_checker[9][9];
int sol[81];

struct node{
	int val;
	int row;
	int col;
	int id;
	struct node *left, *right;
	struct node *up_n; struct col *up_c;
	struct node *down_n; struct col *down_c;
};


struct col{
	int count;
	int id;
	struct col *left, *right;
	struct node *up, *head;
};


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

void create_col(struct col* array[324]){
	int i;
	array[0]=allocate_col(1);
	array[0]->left=NULL;
	for(i=1; i<324; i++){
		array[i]=allocate_col(i+1);
		(*array[i-1]).right=array[i];
		(*array[i]).left=array[i-1];
	}
	array[323]->right=NULL;
	return;
}

/*
create_row() est une fonction qui doit être appelée après create_col()
elle crée 9 noeuds par colonne et relie ces noeuds a leur voisins respectifs 
les voisins sont connu grace au cond i (1<=i<=4)  
les noeuds sont donc crée 4 par 4
on doit donc faire attention a la position des noeud dans les colonnes
*/
void create_row(struct col* array[324]){
	int r=1, v=1, c=1;
	int cond1, cond2, cond3, cond4;
	int cmt=0;
	while (cmt<729){     //729 lignes : 9x9x9=729
		cmt=cmt+1;
		cond1=(r-1)*9+c-1; 
		cond2=81+(r-1)*9+v-1; 
		cond3=162+(c-1)*9+v-1;
		cond4=243+(((r-1)/3)*3+((c-1)/3))*9+v-1; 
		struct node *n1=allocate_node(cond1+1,v,r,c);  //creation des 4 noeuds
		struct node *n2=allocate_node(cond2+1,v,r,c);
		struct node *n3=allocate_node(cond3+1,v,r,c);
		struct node *n4=allocate_node(cond4+1,v,r,c);
		n1->right=n2; n2->left=n1;						//on relie les 4noeuds a leurs voisins
		n2->right=n3; n3->left=n2;
		n3->right=n4; n4->left=n3;
		n4->right=n1; n1->left=n4;
		if(array[cond1]->head==NULL){        //si la colonne n'a pas de 'head'
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
			if (array[cond1]->count>=8){		//si c'est le denrier noeud
				n1->up_n=x1;
				x1->down_n=n1;
				array[cond1]->up=n1;
				n1->down_c=array[cond1];
				array[cond1]->count++;
			}
			else{							//pour les noeuds centraux
				x1->down_n=n1;
				n1->up_n=x1;
				array[cond1]->count++;
			}
		}
		if((*array[cond2]).head==NULL){  		//meme travail mais pour la colonne cond2
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
		if((*array[cond3]).head==NULL){		//meme travail mais pour la colonne cond3
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
		if((*array[cond4]).head==NULL){				//meme travail mais pour la colonne cond4
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



/*
remove_row() supprime tout les éléments voisins au noeud entré en paramètre 
la supression doit réaffecter les voisins du haut et du bas correctement pour ne pas avoir une matrice biaisée
ces réafectations sont différentes si le noeud a supprimé est seul,en tête de colonne,en queue, ou au centre 
*/

void remove_row( struct node* x,struct col* array[324]){
	struct node *tmp=x->right; struct node *rem;
	while(tmp!=x){
		if(tmp->down_n==NULL && tmp->up_n!=NULL){   //si tmp est le dernier
			tmp->up_n->down_n=NULL;
			tmp->up_n->down_c=tmp->down_c;
			tmp->down_c->up=tmp->up_n;
			rem=tmp;
			array[(tmp->id)-1]->count--;
			tmp=tmp->right;
			free(rem);
			rem=NULL;
		}
		else if(tmp->up_n==NULL && tmp->down_n!=NULL){  // si x->right est la tête 
            		tmp->up_c->head=tmp->down_n;
			tmp->down_n->up_n=NULL;
			tmp->down_n->up_c=tmp->up_c;
			rem=tmp;
			array[(tmp->id)-1]->count--;
			tmp=tmp->right;
			free(rem);
			rem=NULL;
		}
       		else if(tmp->up_n!=NULL && tmp->down_n!=NULL){ //si x->right est un noeud central
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



/*
remove_col() supprime tout les noeuds présent dans la colonne
avant de les supprimer il appel la fonction remove_row()
qui supprime tout ses voisins
une fois que le noeud a été isolé il est ensuite suprimé dans remove_col()
*/

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
	x->up_n=NULL;   			//on a trouver le bon noeud
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




/*
la fonction delete() permet de supprimer tout les noeud qui correspondent à un noeud
ce noeud a été précédament sauvegarder dans la variable globale sol[]
il nous reste donc à supprimer tout les noeuds qui sont redondant ou qui lui sont incompatible 
c'est à dire dans le cas d'un sudoku 
on doit supprimer 4 colonnes qu'on peut atteindre directement car on a mis la matrice dans un tableau
chacunes des colonnes est totalement supprimée
et pour chaque noeud présent dasn la colonne on supprime aussi sa ligne 
la fonction est longue car il faut connecter les colonnes
*/

void delete(int r, int c, int v,struct col* array[324]){
	int cond1; int cond2; int cond3; int cond4;   //condi permet d'acceder à la colonne i 
	cond1=(r-1)*9+c-1;							//c'est semblable à une fonction de hashage
	cond2=81+(r-1)*9+v-1;						
	cond3=162+(c-1)*9+v-1;
	cond4=243+(((r-1)/3)*3+((c-1)/3))*9+v-1;
	if(array[cond1]!=NULL){					    
        	remove_col(cond1, v, r, c,array);
        	if (array[cond1]->left==NULL && array[cond1]->right==NULL){   //cas ou la matrice est vide 
		}
        	else if (array[cond1]->left==NULL){							//cas ou on est la door
            		array[cond1]->right->left=NULL;
		}
        	else if (array[cond1]->left!=NULL && array[cond1]->right!=NULL){  //cas ou on est une colonne quelqu'onc
            		array[cond1]->right->left=array[cond1]->left;
            		array[cond1]->left->right=array[cond1]->right;
        	}
        	else {													// cas ou on est la dernière colonne
            		array[cond1]->left->right=NULL;
    		}
        	free(array[cond1]->head);
        	array[cond1]->head=NULL;
        	free(array[cond1]);
        	array[cond1]=NULL;
	}
    	if(array[cond2]!=NULL){     					//idem pour la deuxième colonne
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
	if(array[cond3]!=NULL){  						//idem pour la troisième colonne
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
    	if(array[cond4]!=NULL){						//idem pour la quatrième colonne
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


/*
check_file() vérifie que la grille de sudoku est bien formée et respecte les contraintes
ainsi elle évite des calculs inutils ou incohérent
*/


void check_file (const char *input){ 
	int col=1; int row=1; int c;int block; // block represent the block's position
	FILE *in=fopen(input, "r");
	if (!in){
		printf ("Failed while opening the file"); //this error appear when the file is empty
		exit(0);
	}
	while ((c=fgetc(in))!=EOF){
		if (col>10 || row>10){
			printf("dimention du fichier.txt non adapté"); //this error appear when the file has an oversize
			fclose(in);
			exit(0); }
		if ((char)c!='\n'){
			if (c-48<0 || c-48>9) { //transformation de char en int
				printf("le fichier.txt ne doit contenir que des chiffres \n"); //si le fichier contient des lettres ou des symbols
				fclose(in);
				exit(0);
			}
			if (col!=10 && c!=48){ //on verifie que chaque colonne ligne et block contient au plus 9 entiers différents  
				block=(3*((row-1)/3)+((col-1)/3))+1;
				if (row_checker[row-1][c-49]==1){
					printf("contraintes du sudoku non respectées (erreur sur les lignes)\n"); 
					fclose(in);
					exit(0);
				}
				row_checker[row-1][c-49]=1;
				if (col_checker[col-1][c-49]==1){
					printf("contraintes du sudoku non respectées (erreur sur les colonnes)\n");
					fclose(in);
					exit(0);
				}
				col_checker[col-1][c-49]=1;
				if (block_checker[block-1][c-49]==1){
					printf("contraintes du sudoku non respectées (erreur sur les blocks)\n");
					fclose(in);
					exit(0);
				}
				block_checker[block-1][c-49]=1;
			}
			col=col+1;
		}
		else {
			if (col!=10){
				printf("dimention du fichier.txt non adapté\n"); //cette erreur apparait quand la ligne contient moi de 9 nombres
				fclose(in);
				exit(0);
			}
			col=1;
			row=row+1;
			}
	}
	if (row!= 10 || col != 1){
		printf ("dimention du fichier.txt non adapté \n"); //fichier de taille incorrecte
		fclose(in);
		exit(0);
	}
	fclose(in);
	return;
}
/*
read_file() est appelée dans solve_sudoku() elle supprime les éléments dans le tableau array
des qu'elle rencontre un entier différent de 0 (=case vide)
check_file() est appelé en début de fonction pour éviter des calculs inutils
*/

void read_file (const char *input,struct col* array[324]){
	int val; int col=1; int row=1; int c;     //col et row sont des compteurs qui permettent de retrouver 
	check_file(input);							//ce qu'il faut effacer
	FILE *in=fopen(input, "r");
	while ((c=fgetc(in))!=EOF){
		if ((c-48!=0) && (c!=10) && (col<10) &&(row<10)){
			val= c-48;
			sol[(row-1)*9+col-1]=val;         //stockage de la valeurs dans la variable globale sol[]
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

/*
print_array() imprime la variable globale sol[]
elle imprime un block 9x9 d'entier correspondant à une solution du sudoku
elle est appelée quand sudoku() trouve une solution
*/

void print_array(int sol[81]){
	int i;
	printf("\n");
	for (i=0; i<81; i++){
		if ((i)%9==0){		//retour a la ligne toutes les 9 impressions
			printf("\n");
		}
		printf("%d", sol[i]);
	}
	printf("\n\n");       
	return;
}

/*
door_declaration() repère la première case du tableau qui n'est pas NULL
si alors door=0 c'est que les tableau est entièrement vide 
elle dit donc a sudoku() quand afficher une solution
*/

int door_declaration(struct col *tab[324]){
	int i=1;
	while(tab[i-1]==NULL && i<81){
    		i++;
    		if(i==81 && tab[i-1]==NULL){ //dans ce cas la matrice est vide
        		i=0;
			return i;
		}
	}
	return i;
}


/*
min_matrix() permet de repérer la case du tableau ayant le moins de noeud
ceci permet d'avoir moins d'appel dans le while du sudoku et donc d'accelerer un peu l'algorithme
de plus elle est vitale pour permettre de repérer les case n'ayant pas de noeuds (qui ont donc un count=0)
et permet alors a sudoku() de repérer les "branches mortes" 
*/

int min_matrix(struct col *tab[324]){
    int door=door_declaration(tab);
	if (door==0){
		return 0;
	}
	int min=door;
	struct col *x=tab[door-1];
	while(x->right!=NULL){           //cherche le plus petit éléments 
		if((tab[min-1]->count)>=(x->count)){ 
			min=x->id;														
		}
		x=x->right;
	}
	return min;
}

/*
fonction appelée dans le cas ou sudoku ne trouve pas de solution
la sous matrice qui étais alors crée devient obsolète
le tableau tab_aux[] se supprimera tout seul car il a été appelé sur la pile d'execution
mais les structure copiée est enregistrée sur le tas grâce a malloc()
il faut donc libérer manuellement cet espace pour optimiser la mémoire
*/
void delete_array(struct col *tab[324]){
	int i;struct node *x;
	for(i=0;i<324;i++){
		if(tab[i]==NULL||tab[i]->count==0){  //si sol[i]==0 il n'y a rien a supprimer
			continue;}
		else{
			x=tab[i]->head;
			while(x->down_n!=NULL){
				struct node * tmp=x;
				x=x->down_n;
				free(tmp);
			}
			free(x);   					//supression du dernier noeud
		}
	}
}
	
/*
delete_copy_array() prend un tableau vierge: tab_aux de taille 324
cré 324 pointeurs à des struct col qui pointeront chacun a 9 struct node
(un peu comme si on avais rentré une grille de sudoku vide et qu'on avais appelé read_file() dessus)
ensuite elle lit le tableau sol[] et des qu'elle trouve un nombre différent de 0 elle effectue les opérations d'élimination nécessaires
la seul différence avec le tab[] précédament initialisé dans sudoku() est que ce tableau correspond a la deletion de x
Il contient donc forcément une structure plus petite que celle de tab[]
*/
void delete_copy_array(struct col *tab_aux[324]){
	create_col(tab_aux);
	create_row(tab_aux);
	int i;
	for(i=0;i<81;i++){
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
		print_array(sol);      //on a une solution donc on l'affiche
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

int main(){
	struct col* array[324];
    	create_col(array);
    	create_row(array);
	struct col *tab_aux[324];
	delete_copy_array(tab_aux);
	read_file("multisol.txt",tab_aux);	
	sudoku(tab_aux);
    return 0;
}

