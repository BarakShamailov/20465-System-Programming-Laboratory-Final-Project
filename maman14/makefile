assembler: firstPass.o main.o methodsFirstPass.o symbolsList.o operations.o dataList.o wordsList.o secondPass.o errorHandler.o files.o
	gcc -g -Wall -ansi -pedantic -m32 firstPass.o main.o methodsFirstPass.o symbolsList.o operations.o dataList.o wordsList.o secondPass.o errorHandler.o files.o -o assembler

main.o: main.c
	gcc -c -Wall -ansi -pedantic -m32 main.c -o main.o

firstPass.o: firstPass.c
	gcc -c -Wall -ansi -pedantic -m32 firstPass.c -o firstPass.o

methodsFirstPass.o: methodsFirstPass.c
	gcc -c -Wall -ansi -pedantic -m32 methodsFirstPass.c -o methodsFirstPass.o

symbolsList.o: symbolsList.c
	gcc -c -Wall -ansi -pedantic -m32 symbolsList.c -o symbolsList.o

operations.o: operations.c
	gcc -c -Wall -ansi -pedantic -m32 operations.c -o operations.o

dataList.o: dataList.c
	gcc -c -Wall -ansi -pedantic -m32 dataList.c -o dataList.o

wordsList.o: wordsList.c
	gcc -c -Wall -ansi -pedantic -m32 wordsList.c -o wordsList.o

secondPass.o: secondPass.c
	gcc -c -Wall -ansi -pedantic -m32 secondPass.c -o secondPass.o

errorHandler.o: errorHandler.c
	gcc -c -Wall -ansi -pedantic -m32 errorHandler.c -o errorHandler.o

files.o: files.c
	gcc -c -Wall -ansi -pedantic -m32 files.c -o files.o
