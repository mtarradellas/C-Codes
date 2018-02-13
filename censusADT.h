#ifndef __listADT_h_
#define __listADT_h_

typedef struct censusCDT * censusADT;

//Returns NULL if the new census could not be created due to unavailable memory.
censusADT newCensus(int occupied, int unocuppied);

//Frees the memory used by te census structure.
void freeCensus(censusADT c);

/*
Adds a person to the census. 
Returns 0 if there's no available memory.
Returns 1 if the data was added correctly.
*/
int addElem(censusADT c, int status, char *dept, char *prov); 

//Sets the province iter at the begining of the list of provinces.
void toBeginP(censusADT c);

/*
Sets the department iter corresponding to the last province at
the begining of the list of departments.
*/
void toBeginD(censusADT c);

/*
Returns the next province's name, total of occupied and total of unoccupied by argument.
Returns 0 if there where no more departments in the list, 1 otherwise.
*/
int getProv(censusADT c, char * nameP, long * totUo, long * totOc, long * total);

/*
Returns (in thelist to which the provicnce iter points to the next department's name,
total of occupied and total of unoccupied by argument.
Returns 0 if there where no more departments in the list, 1 otherwise.
*/
int getDept(censusADT c, char * nameD, long * totUo, long * totOc, long * total);

/*
Retuns the total population of the country.
Returns by argument the total occupied and unoccupied population.
*/
long totalsCensus (censusADT c, long * totUo, long * totOc);


int nextProv (censusADT c);
int nextDept (censusADT c);



#endif
