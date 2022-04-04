#ifndef HEADER_H
#define HEADER_H
/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "defines.h"
#include "structs.h"

/*second pass function*/
void secondPass(int IC);


/*files.c functions*/
FILE * openFile(char* filename, char * mode, char * extension);
void exportFiles(int IC, int DC, char * name);
void importBinary(int method, wordNode * p , FILE *object);


/*errorHandler.c functions*/
void addError(char * err, int line, char * str);
void printErrors(char * filename);
void freeErrors();
int isError();
int getMaxLine();


/*operations.c functions*/
void analyzeOperation(char * token, int line, int *IC, char * label);
int checkCommasOperation(char * token,int line);
int isRegister(char * token);


/*symbols list functions*/
symbol * searchSymbol(char * label);
long checkSymbol(char * label,int method,int line);
void freeSymbols();
void updateSymbols(int IC);
void addSymbol(char * label, int address, int external, int  entry,int data,int operation, int line);
int isSameSymbol(char * label , int address, int external, int  entry,int data,int operation, int line);
symbol * getHeadSymbol();


/*dataList.c functions*/
void addNumber(char * number,int db ,int dw ,int dh ,int line , int *DC);
void checkNumbers(char * token, int line,int db,int dh ,int dw, int  *DC);
void freeData();
void addString(char * string, int line, int *DC);
void addChar(char c );
int countData();
void updateDataAddresses(int IC);
data * getHeadData();


/*wordsList.c functions*/
void addWordNode(wordNode * node);
void freeWords();
wordNode * getMissingLabel();
wordNode * getHeadWord();
int countWords();
void updateAddresses();


/*firstPass.c functions*/
void firstPass(FILE * f, int * counter, int * DC);
char * getLabel(char * token);
int isComment(char * token);
int isOperation(char * token);
int isDirective(char * token);
int getDirective(char * token);
void checkAllocation(void * ptr);
int checkCommas(char * token , int line);
int checkRegisters(char * token , int line);
int isValidString(char * token,int line);
int isValidLabel(char * token, int line);


#endif
