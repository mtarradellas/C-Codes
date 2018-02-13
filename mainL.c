#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "censusADT.h"

#define MAX_LINE 80
#define MAX_ASCII 128
#define OCCUPIED_VALUE 1
#define UNOCCUPIED_VALUE 2

//Deletes line from buffer.
#define DELLINE(c) while(((c)=getchar())!='\n' && (c)!=EOF)

// The first value is the occupied and the second is the desoccupied.
#define UNINDX(a, b) ((b)>0?(double)(b)/((a)+(b)):(b))

#define STATUS_MAX 3
#define ABS(c) ((c)>0?(c):(-(c))) 

#define ECONOMICSTATUS 0
#define HOMEID 1
#define DEPT 2
#define PROV 3
#define ERROR 4

#define PATH_COUNTRY "./country.csv"
#define PATH_PROVINCE "./province.csv"
#define PATH_DEPARTMENT "./department.csv"

/*
Reads a line from stdin and returns as argument the economic status value and it stores
both the dept name followed by a '\0' and the province name followed by '\0' in the
corresponding strings.
	
It returns 1 if it managed to do all of this accordingly.

In case of an invalid line it returns 0.
*/
int getLine(int * status, char * dept, char * prov);
/*
Stores the census into 3 files.
Returns 0 if the files could not be created or if there was an error while copying.
Returns 1 if everything was stored correctly.

The country file contains the total population of the country as well as the
unemployment index separated by commas.

The province file is an alfabetically ordered file in which each line corresponds
to a province and has the province name, the total population of the province and
the unemployment index of the province separated by commas.

The department file is an alfabetically ordered file, first by province name, then
by department name, in which each line corresponds to a department and has the
province name the department is included in, the department name, the total 
population of the department and the unemployment index of the department separated
by commas.
*/
int storesFiles(censusADT c, char *pathCountry, char * pathProvince, char * pathDepartment);

//Auxiliary function used in storesFiles.
int storeCountry(censusADT c, FILE * fileCountry);

//Auxiliary function used in storesFiles.
int storeProvincesAndDeptartment(censusADT c, FILE * fileProvince, FILE * fileDepartment);

int main(void){
	
	censusADT c = newCensus(OCCUPIED_VALUE, UNOCCUPIED_VALUE);
	
	if(c==NULL){
		fprintf(stderr, "Memory Error: Census was not created.\nTerminating program\n");
		
		return 1;
	}
	//lineError and memError are equal to 1 when there were no errors and 0 when there were.
	int status, x, lineError=1, memError=1, storeError;
	long totalLineErrors=0, totalMemErrors = 0, line = 0;
	/*
	5: the minimum space used by the status, home id, the other string (dept or prov) and
	commas (6) minus the space for the '0' which terminates the string (1).
	*/
	char * dept = malloc(MAX_LINE - 5);
	char * prov = malloc(MAX_LINE - 5);
	

	while((x=getchar())!=EOF){
		
		line++;
		ungetc(x, stdin);

		lineError = getLine(&status, dept, prov);

		if (lineError==0){
			fprintf(stderr, "Line Error: Line %ld was ignored.\n",line);
			totalLineErrors++;
		}

		else{
			memError = addElem(c, status, dept, prov);

			if (memError==0){
				fprintf(stderr, "Memory Error: Line %ld was ignored.\n", line);
				totalMemErrors++;
			}
		}
		
	}
	if(totalLineErrors>0 || totalMemErrors>0){
		fprintf(stderr, "Total Line Errors: %ld\nTotal Memory Errors: %ld\n", totalLineErrors, totalMemErrors);
		fprintf(stderr, "Total lines ignored: %ld\n", totalMemErrors+totalLineErrors);
	}
	

	storeError = storesFiles(c, PATH_COUNTRY, PATH_PROVINCE, PATH_DEPARTMENT);
	if (storeError == 0){
		
		fprintf(stderr, "Storing Error\n");
	}

	freeCensus(c);
	free(dept);
	free(prov);

	return 0;
}



int getLine(int * status, char * dept, char * prov){

	int lineSize=0,state=ECONOMICSTATUS, lineError=1, c;
	/*
	These flags validate that there is at least a character in each string and that
	there is a HomeID; they are used in case HOMEID, DEPT and PROV.
	*/
	int flagDept=0, flagProv=0, flagHomeID=0;
	*status = 0;

	while(lineSize < MAX_LINE && (c=getchar())!='\n' && c!=EOF){
		switch(state){

			/*
			Builds status number until it reads a comma, then switches to HOMEID.

			It switches to ERROR if the first character read is a comma, status number
			exceeds its maximun value,or any character is invalid.
			*/
			case ECONOMICSTATUS:
				if (isdigit(c)){
					*status = (*status)*10 + c - '0';
					if (*status > STATUS_MAX){
						state = ERROR;
					}
				}
				else if (c==','&& lineSize>0){
					state=HOMEID;
				}
				else{
					state=ERROR;
				}
				break;

			/*
			Switches to ERROR if HOME ID is not composed of digits or if it reads
			a comma before any digit.

			Switches to DEPT if it reads a comma after digits.
			*/
			case HOMEID:
				if (isdigit(c))
					flagHomeID=1;
				else if (c==',' && flagHomeID)
					state=DEPT;
				else {
					state=ERROR;
				}
				break;						
			
			/*
			Copies to string dept only valid characters until it reads a comma,
			then finishes the string with a 0 and switches to case PROV.

			If flagDept is 0 nothing was copied yet, so if it reads a comma 
			before copying anything, or character is not valid, switches to ERROR.
			*/
			case DEPT:
				if(c!=',' && c<=MAX_ASCII){
					*dept = c;
					dept++;
					flagDept=1;
				}
				else if (c==',' && flagDept){
					*dept=0;
					state=PROV;
				}
				else {
					state=ERROR;
				}
				break;

			/*
			Copies to string prov only valid characters until it reads a comma,
			then finishes the string with a 0.
			*/
			case PROV:
				if(c!=',' && c<MAX_ASCII){
					*prov = c;
					prov++;
					flagProv=1;
				}
				else {
					state=ERROR;
				}
				break;

			case ERROR:
				lineError=0;
				break;
		}
		lineSize++;
	}

	if (flagProv==0)
		lineError = 0;

	else if (state==PROV)
			*prov=0;

	//If the line reading was terminated because it was too long it is skipped.
	if(c!='\n' && c!=EOF){

			lineError=0;

		DELLINE(c);
	}
	return lineError;
}

int storesFiles(censusADT c, char *pathCountry, char * pathProvince, char * pathDepartment){
	FILE * fileCountry = fopen(pathCountry, "w");
	FILE * fileProvince = fopen(pathProvince, "w");
	FILE * fileDepartment = fopen(pathDepartment, "w");

	int flag1, flag2=1;

	if(fileCountry == NULL || fileProvince == NULL || fileDepartment == NULL){
		if(fileCountry != NULL)
			fclose(fileCountry);
		if(fileProvince != NULL)
			fclose(fileProvince);
		if(fileDepartment != NULL)
			fclose(fileDepartment);
		return 0;
	}

	flag1 = storeCountry(c, fileCountry);
	flag2 = storeProvincesAndDeptartment(c, fileProvince, fileDepartment);

	fclose(fileCountry);
	fclose(fileProvince);
	fclose(fileDepartment);

	return flag1*flag2;
}

int storeCountry(censusADT c, FILE * fileCountry){
	int errorStatus;
	long totalOc;
	long totalUo;
	long total;
	
	total = totalsCensus(c, &totalUo, &totalOc);

	errorStatus = fprintf(fileCountry, "%ld,%.2f\n", total, UNINDX(totalOc, totalUo));

	return errorStatus>0;
}

int storeProvincesAndDeptartment(censusADT c, FILE * fileProvince, FILE * fileDepartment){
	int errorStatus;
	long totalOc, totalUo,total;
	char dept[MAX_LINE - 5];
	char prov[MAX_LINE - 5];

	toBeginP(c);

	while ((getProv(c, prov, &totalUo, &totalOc, &total)) != 0 ){

		toBeginD(c);
		while ((getDept(c, dept, &totalUo, &totalOc, &total)) != 0){

			errorStatus=fprintf(fileDepartment, "%s,%s,%ld,%.2f\n", prov, dept, total, UNINDX(totalOc, totalUo));
			nextDept (c);	
		}
		errorStatus=fprintf(fileProvince, "%s,%ld,%.2f\n", prov, total, UNINDX(totalOc, totalUo));
		nextProv(c);
	}
	return errorStatus>0;

}
