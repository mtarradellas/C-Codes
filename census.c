#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#include "censusADT.h"


/*
Holds a pointer to the next deptCDT, making a NULL terminated and alfabeticallly
ordered list.

Each contains its name in a string and data regarding the amount of citizens
according to economic status. 
*/

struct deptCDT{
	char * name;
	long totalOc, totalUo, total;
	struct deptCDT * next;
};

/*
Holds a pointer to the first element of a list of the departments in that province,
an iterative pointer to that list which makes going through it faster and is used 
in toBegin and next functions.

Holds a pointer to the next provCDT, making a NULL terminated and alfabeticallly
ordered list.

Each contains its name and the data regarding citizens and their status.
*/
struct provCDT{
	char * name;
	long totalOc, totalUo, total;
	struct deptCDT * firstD;
	struct deptCDT * lastD;
	struct provCDT * next;
};

/*
Holds a pointer to the first element of a list of the provinces,an iterative
pointer to that list which makes going through it faster and is used in toBegin
and next functions.

The variables ocStatus and uoStatus contain the values corresponding to the
economic status indicators for occupied and unoccuied citizens.

Contains data regarding citizens and their status.
	
*/
struct censusCDT{
	long totalOc, totalUo, total;
	int ocStatus, uoStatus;
	struct provCDT * firstP;
	struct provCDT * lastP;
};


//Adds to the census, province and dept economic counters.
static void statusCounter(censusADT c, int status);

//Adds a department to the list of departments in a province recursively.
static struct deptCDT * addDeptRec(struct deptCDT * d, int status, char* dept, censusADT c, int * flag);

//Adds a province to the list of provinces in the census recursively.
static struct provCDT * addProvRec(struct provCDT * p, char* prov, censusADT c, int * flag);

//Frees memory used by departments recursively.
static void freeDeptRec(struct deptCDT * d);

//Frees memory used by provinces recursively.
static void freeProvRec(struct provCDT * p);


//Creates a new census.
censusADT newCensus(int ocStatus, int uoStatus){

	censusADT c = malloc(sizeof(*c));

	if(c!=NULL){

		c->total = 0;
		c->totalOc = 0;
		c->totalUo = 0;	
		c->firstP = NULL;
		c->lastP = NULL;
		c->ocStatus = ocStatus;
		c->uoStatus = uoStatus;		
	}

	return c;
}

/*
Adds a department to its corresponding list in the list of provinces.

If the province does not exist, it adds the province to the list of provinces
and creates a new list for the department and adds it.

Uses the statusCounter function.

In order to make this function faster, it first checks if the new element is located
after the last added (iter), and if it's not then it runs through the list from the begining. 
*/

int addElem(censusADT c, int status, char *dept, char *prov){
	int fail =0;
	if((c->lastP)!= NULL && strcmp(c->lastP->name, prov) <= 0){
		addProvRec(c->lastP, prov, c, &fail);
	}
	else 
		c->firstP = addProvRec(c->firstP, prov, c, &fail);
	
	if(fail == 0){
		if(c->lastP->lastD!= NULL && strcmp(c->lastP->lastD->name, dept) <= 0)
			addDeptRec(c->lastP->lastD, status, dept, c, &fail);
		else
			c->lastP->firstD = addDeptRec(c->lastP->lastD, status, dept, c, &fail);
	}

	return !fail;
	
}

static struct deptCDT * addDeptRec(struct deptCDT * d, int status, char* dept, censusADT c, int * flag){
	int x;
	if(d==NULL || (x=strcmp(d->name, dept))>0){
		struct deptCDT * aux = malloc(sizeof(*aux));
		if(aux == NULL){
			*flag =1;
			return d;
		}

		aux->name = malloc(strlen(dept)+1);
		if(aux->name == NULL){
			*flag =1;
			free(aux);
			return d;
		}

		aux->total=0;
		aux->totalOc=0;
		aux->totalUo=0;
		aux->next = d;

		c->lastP->lastD = aux;
		strcpy(c->lastP->lastD->name, dept);
		statusCounter(c, status);
		
		return aux;
	}
	if(x == 0){
		c->lastP->lastD = d;
		statusCounter(c, status);
		return d;
	}

	d->next = addDeptRec(d->next, status, dept, c, flag);
	return d;

}

static struct provCDT * addProvRec(struct provCDT * p, char * prov, censusADT c, int * flag){
	int x;
	if(p==NULL || (x=strcmp(p->name, prov))>0){
		struct provCDT * aux = malloc(sizeof(struct provCDT));
		if(aux==NULL){
			*flag =1;
			return p;
		}

		aux->name = malloc(strlen(prov)+1);
		if(aux->name == NULL){
			*flag =1;
			free(aux);
			return p;
		}

		aux->total=0;
		aux->totalOc=0;
		aux->totalUo=0;
		aux->next = p;
		c->lastP = aux;

		strcpy(c->lastP->name, prov);
		aux->firstD = NULL;
		aux->lastD = NULL;
		return aux;
	}
	if(x==0){
		c->lastP = p;
		return p;
	}
	p->next = addProvRec(p->next, prov, c, flag);
	return p;
}

static void statusCounter(censusADT c, int status){
	if(status == c->ocStatus){
			c->totalOc++;
			c->lastP->totalOc++;
			c->lastP->lastD->totalOc++;	
	}
	else if(status == c->uoStatus){
		c->totalUo++;	
		c->lastP->totalUo++;	
		c->lastP->lastD->totalUo++;	
	}
	c->total++; 
	c->lastP->total++;
	c->lastP->lastD->total++;
}

//Frees memory used by the census.
void freeCensus(censusADT c){
	freeProvRec(c->firstP);
	free(c);
	return;
}

static void freeProvRec(struct provCDT * p){
	if(p==NULL)
		return;
	freeProvRec(p->next);
	freeDeptRec(p->firstD);
	free(p->name);
	free(p);
	return;
}

static void freeDeptRec(struct deptCDT * d){
	if (d==NULL)
		return;
	freeDeptRec(d->next);
	free(d->name);
	free(d);
	return;
}

//Sets the province iter at the begining of the list of provinces.
void toBeginP(censusADT c){

	c->lastP = c->firstP;
	return;
}

/*
Sets the department iter corresponding to the last province at
the begining of the list of departments.
*/
void toBeginD(censusADT c){

	if (c->lastP != NULL){
		c->lastP->lastD = c->lastP->firstD;
	}
	
	return;
}

//Returns 0 if there where no more provinces in the list, 1 otherwise.
int getProv(censusADT c, char * nameP, long * totUo, long * totOc, long * total){

	if (c->lastP != NULL){

		strcpy(nameP,c->lastP->name);
		*totOc = c->lastP->totalOc;
		*totUo = c->lastP->totalUo;
		*total = c->lastP->total;
		return 1;
	}
	return 0;
}

int nextProv (censusADT c){

	if (c->lastP != NULL ){
		c->lastP = c->lastP->next;
		return 1;
	}
	return 0;
}

//Returns 0 if there where no more departments in the list, 1 otherwise.
int getDept(censusADT c, char * nameD, long * totUo, long * totOc, long * total){

	if (c->lastP != NULL){
		if (c->lastP->lastD != NULL){

			strcpy(nameD,c->lastP->lastD->name);
			*totOc = c->lastP->lastD->totalOc;
			*totUo = c->lastP->lastD->totalUo;
			*total = c->lastP->lastD->total;

			return 1;
		}
	}
	return 0;
}

int nextDept (censusADT c){

	if (c->lastP != NULL ){
		if (c->lastP->lastD != NULL){
			c->lastP->lastD = c->lastP->lastD->next;
			return 1;
		}
	}
	return 0;
}

/*
Retuns the total population of the country.
Returns by argument the total occupied and unoccupied population.
*/
long totalsCensus (censusADT c, long * totUo, long * totOc){
	
	*totUo = c->totalUo;
	*totOc = c->totalOc;
	return c->total; 
}
