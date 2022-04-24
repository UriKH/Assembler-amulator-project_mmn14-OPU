CC = gcc
CFLAGS = -Wall -pedantic -ansi
BUILDFLAGS = -c -g

main: main.o compiler.o buildWord.o dataHandeling.o files.o instructionHandeling.o label.o line.o statmentType.o symbolTable.o util.o
	$(CC) $(CFLAGS) main.o compiler.o buildWord.o dataHandeling.o files.o instructionHandeling.o label.o line.o statmentType.o symbolTable.o util.o -o main
main.o: main.c bool.h
	$(CC) $(BUILDFLAGS) $(CFLAGS) main.c -o main.o
compiler.o: compiler.c files.h line.h label.h util.h instructionHandeling.h dataHandeling.h symbolTable.h statmentType.h
	$(CC) $(BUILDFLAGS) $(CFLAGS) compiler.c -o compiler.o
buildWord.o: buildWord.c buildWord.h 
	$(CC) $(BUILDFLAGS) $(CFLAGS) buildWord.c -o buildWord.o
dataHandeling.o: dataHandeling.c util.h word.h label.h
	$(CC) $(BUILDFLAGS) $(CFLAGS) dataHandeling.c -o dataHandeling.o -lm
files.o: files.c files.h util.h  filesPrivate.h
	$(CC) $(BUILDFLAGS) $(CFLAGS) files.c -o files.o
instructionHandeling.o: instructionHandeling.c instructionHandeling.h label.h line.h util.h word.h buildWord.h symTabTypes.h 
	$(CC) $(BUILDFLAGS) $(CFLAGS) instructionHandeling.c -o instructionHandeling.o -lm
label.o: label.c label.h util.h
	$(CC) $(BUILDFLAGS) $(CFLAGS) label.c -o label.o
line.o: line.c line.h
	$(CC) $(BUILDFLAGS) $(CFLAGS) line.c -o line.o
statmentType.o: statmentType.c bool.h statmentType.h
	$(CC) $(BUILDFLAGS) $(CFLAGS) statmentType.c -o statmentType.o
symbolTable.o: symbolTable.c label.h util.h  symbolTable.h
	$(CC) $(BUILDFLAGS) $(CFLAGS) symbolTable.c -o symbolTable.o
util.o: util.c util.h
	$(CC) $(BUILDFLAGS) $(CFLAGS) -ansi util.c -o util.o

clean:
	rm *.o