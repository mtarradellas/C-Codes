#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "getnum.h"
#include "random.h"
#define MINE -1


int menu(void);
int createField ( int *** field, int *** fCopy, int option, int * mines );
void fillFields ( int ** field, int ** fCopy, int size, int mines );
void addMines ( int ** field, int size, int mines);
void addNumbers ( int ** field, int size);
int countMines ( int ** field, int x, int y, int size);
void fill ( int ** fCopy, int size);
int ** createSpace ( int size);
int play ( int ** field, int ** fCopy, int size, int mines);
void printField ( int ** fCopy, int size);
void askPosition ( int * pFil, int * pCol, int ** fCopy, int size );
int controlPosition ( int ** field, int ** fCopy, int size, int pFil, int pCol, int * left);
void open(int ** field, int ** fCopy, int pFil, int pCol, int size, int * left);


int main (void){

	int ** field;
	int ** fCopy;
	int option;
	int won;
	int size;
	int mines;

	printf("\n\n\n~~~~~~~~~~~\nMine Sweeper\n~~~~~~~~~~~~\n\n\n");

	option = menu();
	size = createField(&field, &fCopy, option, &mines);
	won = play(field, fCopy, size, mines);

	if (won)
		printf("\n\n\n         Congratulations!!\n         You Won!!\n\n\n");
	else
		printf("\n\n\n         You Are Dead\n\n\n");
	printf("~ ~ ~ ~ ~ G A M E  O V E R ~ ~ ~ ~ ~\n\n\n");

	return 0;
}


int menu(void){

	int option;

	printf("Select Difficulty:\n\n");
	printf(".1 For 9x9 field, 10 mines.\n");
	printf(".2 For 16x16 field, 40 mines.\n");
	printf(".3 For 22x22 field, 99 mines.\n");

	do{

		option = getint("");

		if (option > 3 || option < 1)
			printf("Invalid option, choose again.\n");

	} while (option > 3 || option < 1);

	return option;
}


int createField ( int *** field, int *** fCopy, int option, int * mines ){

	int size;

	if (option == 1){
		size = 10;
		*mines = 10;
	}
	else if (option == 2){
		size = 17;
		*mines = 40;
	}
	else if (option == 3){
		size = 23;
		*mines = 99;
	}

	*field = createSpace(size);		
	*fCopy = createSpace(size);
	fillFields(*field, *fCopy, size, *mines);

	return size;	
}


void fillFields ( int ** field, int ** fCopy, int size, int mines ){

	addMines(field, size, mines);	
	addNumbers(field, size);	
	fill(fCopy, size);	

	return;
}


void addMines ( int ** field, int size, int mines){

	int i, mineR, mineC, validMine;

	srand(time(0));

	for (i=0; i<mines; i++){

		do{

			mineR = randInt(1, size-1);
			mineC = randInt(1, size-1);
			validMine = (field[mineR][mineC]!=MINE);

			if (validMine)
				field[mineR][mineC] = MINE;

		} while (!validMine);
	}
	return;
}


void addNumbers ( int ** field, int size){

	int i, j;

	for (i=1; i<size; i++){
		for (j=1; j<size; j++){

			if (field[i][j]!=MINE)
				field[i][j] = countMines(field, i, j, size);	
			
		}
	}

	return;
}


int countMines ( int ** field, int x, int y, int size){

	int i, j, count = 0;

	for(i=x-1; i<=x+1; i++){
		for (j=y-1; j<=y+1; j++){

			if (i>0 && j>0 && i<size && j<size && (i!=x || j!=y)){
				if (field[i][j]==MINE)
					count++;
			}
		}
	}

	return count;
}


void fill ( int ** fCopy, int size){

	int i, j;

	fCopy[0][0] = '~';

	for(i=1; i<size; i++){
		fCopy[0][i] = i;
	}

	for(j=1; j<size; j++){
		fCopy[j][0] = j;
	}

	for(i=1; i<size; i++){
		for(j=1; j<size; j++){
			fCopy[i][j] = '*';
		}
	}

	return;
}


int ** createSpace ( int size ){

	int i;

	int ** aux = malloc(size*sizeof(*aux));
	for (i=0; i<size; i++)
		aux[i] = malloc (size*sizeof(*aux[0]));

	return aux;
}


int play ( int ** field, int ** fCopy, int size, int mines){

	int pFil, pCol, boom;
	int left = (size-1)*(size-1) - mines;


	do{
		printField(fCopy, size);
		askPosition(&pFil, &pCol, fCopy, size);
		boom = controlPosition(field, fCopy, size, pFil, pCol, &left);

		if (!left)
			return 1;

		if (boom)
			printField(fCopy, size);
	} while(!boom);

	return 0;
}	


void printField ( int ** fCopy, int size){

	int i, j;

	printf("\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	for (i=0; i<size; i++){
		for (j=0; j<size; j++){
			if((i!=0 || j!=0) && (i==0 || j==0))
				printf("%2d ", fCopy[i][j]);
			else printf("%2c ", fCopy[i][j]);
		}
		putchar('\n');
	}

	return;
}


void askPosition ( int * pFil, int * pCol, int ** fCopy, int size ){

	do{

		printf("\nSelect valid Row:\n");
		*pFil = getint("");
		printf("Select valid Column:\n");
		*pCol = getint("");
	}while ( *pFil<0 || *pCol<0 || *pFil>size || *pCol>size || fCopy[*pFil][*pCol]!='*');

	return;
}


int controlPosition ( int ** field, int ** fCopy, int size, int pFil, int pCol, int * left){

	if (field[pFil][pCol]!=MINE){
		open(field, fCopy, pFil, pCol, size, left);
		return 0;
	}

	return 1;
}


void open(int ** field, int ** fCopy, int pFil, int pCol, int size, int * left){
	
	if (fCopy[pFil][pCol]=='*'){

		if (field[pFil][pCol]==0){
	
				fCopy[pFil][pCol] = ' ';
				(*left)--;
				int i, j;
				for(i=pFil-1; i<=pFil+1; i++){
					for(j=pCol-1; j<=pCol+1; j++){
						if(i>0 && j>0 && i<size && j<size && (i!=pFil || j!=pCol) && field[i][j]!=MINE ){
							open(field, fCopy, i, j, size, left);
						}
					}
				}
			}

		else{

			fCopy[pFil][pCol] = field[pFil][pCol] + '0';
			(*left)--;
		}
	}

	return;
}