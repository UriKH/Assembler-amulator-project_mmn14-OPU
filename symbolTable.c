/* [mmn 14 : symbolTable.c]:
in this file: functions which are operating on and with the symbol-table.  

author: Uri K.H,   Lihi Haim       Date: 2.4.2021 
ID: 215105321,     313544165       Tutor: Danny Calfon */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "label.h"
#include "util.h"
#include "symbolTable.h"

typedef struct{
    int address;
    char name[MAX_LABEL_LEN + 1];
} extTable;

static symbolTable *symTab;
static int symSize;
static extTable *externals;
static int extSize;

static boolean addAttribToSymTab(char *, enum attribs);
boolean definedAs(char *, enum attribs);

/* pushExtern(): the function gets as parameters a line and a pointer to a external variable
 in the line and the line's number,
 and pushes it if it's a valid external variable to symbol-table */
boolean pushExtern(char *line, int *lInd, int lineCnt){
    char *label;
    boolean added;

    jumpSpaces(line, lInd);
    label = readWord(line, lInd);
    if (!isValidLabel(label, lineCnt, FALSE)){
        free(label);
        return FALSE;
    }
    else if(!(added = addToSymTab(label, EXT, lineCnt))){
        free(label);
        return FALSE;
    }
    if (added == ERROR){
        free(label);
        return ERROR;
    }
    if(!isBlank(line, *lInd)){
        printf("error [line %d]: extranous text after end of command\n", lineCnt);
        free(label);
        return FALSE;
    }
    free(label);
    return TRUE;
}
/* pushEntry(): the function gets as parameters a line and a pointer to a entry variable
 in the line and the line's number,
 and pushes it if it's a valid entry variable to symbol-table */
boolean pushEntry(char *line, int* lInd, int lineCnt){
    char *operand, *operation;
    boolean added, isExt;

    jumpSpaces(line, lInd);
    if (!(operation = readWord(line, lInd)))
        return ERROR;
    if(strcmp(operation, ".entry") == 0){
        free(operation);
        jumpSpaces(line, lInd);
        if (!(operand = readWord(line, lInd)))
            return ERROR;
        if((isExt = definedAs(operand, EXT)) == TRUE){
            printf("error [line %d]: label \"%s\" cannot be named \".extern\" and \".entry\" at the same time\n", lineCnt, operand);
            free(operand);
            return FALSE;
        }
        else if(isExt == ERROR){
            printf("error [line %d]: label \"%s\" was ignored due to the defenition in an .extern or .entry line\n", lineCnt, operand);
            free(operand);
            return FALSE;
        }
        if(!(added = addAttribToSymTab(operand, ENT))){
            free(operand);
            return FALSE;
        }
        else if(added == ERROR){
            free(operand);
            return ERROR;
        }
        free(operand);
        return TRUE;
    }
    else if(strcmp(operation, ".string") == 0 || strcmp(operation, ".data") == 0 || strcmp(operation, ".extern") == 0){
        free(operation);
        *lInd = -1;
        return TRUE;
    }
    free(operation);
    return TRUE;
}

/* checkEntry(): the function gets as parameters a line, the pointer to an entry variable in line and the line's number,
 to check if the entry variable is valid and return true or false accordingly */
boolean checkEntry(char *line, int *lInd, int lineCnt){
    char *operand;
    jumpSpaces(line, lInd);
    if (!(operand = readWord(line, lInd)))
        return ERROR;
    if(!isValidLabel(operand, lineCnt, FALSE)){
        free(operand);
        return FALSE;
    }
    if(!isBlank(line, *lInd)){
        printf("error [line %d]: extranous text after end of command\n", lineCnt);
        free(operand);
        return FALSE;
    }
    free(operand);
    return TRUE;
}

/* addToSymTab(): the function gets as parameters a symbol name, directive type
 and the line and the line's number,
 and pushes it if it's a valid variable to symbol-table
 in case of success TRUE will be returned and False in case of invalidation */
boolean addToSymTab(char *name, enum attribs attrib, int lineCnt){
    int tabInd;
    if(wasDefined(name, lineCnt, FALSE)){
        if(attrib != EXT && definedAs(name, EXT)){
            printf("error [line %d]: external label \"%s\" cannot be redeclared as local\n", lineCnt, name);
            return FALSE;
        }
        else if(attrib == EXT && (definedAs(name, DAT) || definedAs(name, COD))){
            printf("error [line %d]: local label \"%s\" cannot be redeclared as external\n", lineCnt, name);
            return FALSE;
        }
        else if(attrib == EXT && definedAs(name, EXT))
            return TRUE;
    }
    if (isIlegalName(name, lineCnt) || wasDefined(name, lineCnt, TRUE))
        return FALSE;
    if (symSize == 0){
        symTab = malloc(sizeof(symbolTable));
        symSize++;
    }
    else
        symTab = realloc(symTab, ++symSize * sizeof(symbolTable));
    if (!isAlloc(symTab))
        return ERROR;
    
    tabInd = symSize-1;
    strcpy(symTab[tabInd].symbol, name);
    symTab[tabInd].attribute1 = attrib;
    symTab[tabInd].attribute2 = 0;
    if (attrib == DAT)
        symTab[tabInd].address = getDC();
    else if (attrib == COD)
        symTab[tabInd].address = getIC();
    else if (attrib == EXT)
        symTab[tabInd].address = 0;
    return TRUE;
}

/* addToExtTab(): the function gets as parameters the name of the external variable and the address
 in the memory on which it is suposse to be writen. And then pushes it to the externals table.  */
boolean addToExtTab(char *name, int add){
    if (extSize == 0){
        externals = malloc(sizeof(extTable));
        extSize++;
    }
    else
        externals = realloc(externals, ++extSize * sizeof(extTable));
    if (!isAlloc(externals))
        return ERROR;
    externals[extSize-1].address = add + OS_MEM;
    strcpy(externals[extSize-1].name, name);
    return TRUE;
}

/* addAttribToSymTab(): the function gets as parameters a symbol name to search for in symbol-table
 and the directive type to add to it,
 and pushes the directive type if it's valid to symbol-table
 in case of success TRUE will be returned and False in case of invalidation */
static boolean addAttribToSymTab(char *sym, enum attribs attrib){
    int i;
    for (i = 0; i < symSize; i++){
        if (strcmp(symTab[i].symbol, sym) == 0){
            symTab[i].attribute2 = attrib;
            return TRUE;
        }
    }
    return FALSE;
}

/* encPlusIC(): the function increases the variables by IC in symbol-table to be at the end of the file in the right address */

void encPlusIC(){
    int i;
    for(i = 0; i < symSize; i++){
        if(symTab[i].attribute1 == DAT)
            symTab[i].address += getIC();
    }
}

/* wasdefined(): the function gets as parameters a symbol name 
 and the line's number to search for in symbol-table and a boolean flag
 and the function checks if the symbol is already in the symbol table and return true or false(accompanied by error message) accordingly  */
boolean wasDefined(char *sym, int lineCnt, boolean printErr/*, int pass*/){   
    int i;
    for (i = 0; i < symSize/*((pass == 1)?(symSize-1):(symSize))*/; i++){
        if (strcmp(symTab[i].symbol, sym) == 0){
            if(printErr)
                printf("error [line %d]: label \"%s\" has been already defined\n", lineCnt, sym);
            return TRUE;
        }
    }
    return FALSE;
}

/* definedAs(): the function gets as parameters a symbol name 
 and a directive type to search for in symbol-table 
 and the checks if the symbol with the matching directive is already in the symbol table
 and return true or false accordingly  */
boolean definedAs(char *sym, enum attribs attrib){
    int i;
    boolean wasFound = FALSE;
    for (i = 0; i < symSize; i++){
        if (strcmp(symTab[i].symbol, sym) == 0){
            wasFound = TRUE;
            if (symTab[i].attribute1 == attrib)
                return TRUE;
        }
    }
    if (!wasFound)
        return ERROR;
    return FALSE;
}

/* getFromSymTab(): searches for a matching symbol name in the symbol-table for a name
 and returns the row of the symbol. If the symbol is not defined in the symbol-table 
 a row will be returned with a symbol name is a NULL pointer.
 parameters: sym - the symbol name to search for 
 */
symbolTable getFromSymTab(char *sym){
    int i = 0;
    symbolTable error;
    strcpy(error.symbol, "\0");

    while (i < symSize){
        if (strcmp(symTab[i].symbol, sym) == 0)
            return symTab[i];
        i++;
    }
    return error; /* (never used) */
}

/* entryExist(): returns if there is an entry variable in the symbol-table */
boolean entryExist(){
    int i;
    for (i = 0; i < symSize; i++){
        if(symTab[i].attribute2 == ENT)
            return TRUE;
    }
    return FALSE;
}

/* externalExist(): returns if there is an external variable in the symbol-table */
boolean externalExist(){
    int i;
    if(externals == NULL) /* there is no useage of external variables in the assembly code */
        return FALSE;
    for (i = 0; i < symSize; i++){
        if (symTab[i].attribute1 == EXT)
            return TRUE;
    }
    return FALSE;
}

/* pushEntryToFile(): the function gets as parameter a file
 and pushes the Enrty values from symboltable to the file*/
void pushEntryToFile(FILE *fd){
    int i;
    for(i = 0; i < symSize; i++){
        if (symTab[i].attribute2 == ENT)
            fprintf(fd, "%s %d\n", symTab[i].symbol, symTab[i].address);
    }
    return;
}

/* pushExternToFile(): the function gets as parameter a file
 and pushes the Extern values from symboltable to the file*/
void pushExternToFile(FILE *fd){
    int i;
    for(i = 0; i < extSize; i++)
        fprintf(fd, "%s %d\n", externals[i].name, externals[i].address);
    return;
}

/* cleanSymAndExtTab(): frees symbol-table and resets the table's pointer*/
void cleanSymAndExtTab(){
    free(symTab);
    free(externals);
    externals = NULL;
    symTab = NULL;
    symSize = 0;
    extSize = 0;
    return;
}