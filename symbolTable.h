/* [mmn 14 : symbolTable.h]:
In this file: prototypes of functions which are operating on the symbol-table or are related to it. 

author: Uri K.H,   Lihi Haim       Date: 2.4.2021 
ID: 215105321,     313544165       Tutor: Danny Calfon 
*/
#include "bool.h"
#include "symTabTypes.h" /* for the symbol table data types defenition */

boolean pushExtern(char *, int *, int);
boolean pushEntry(char *, int*, int);
boolean checkEntry(char *, int *, int);
boolean addToSymTab(char *, enum attribs, int);
boolean wasDefined(char *, int, boolean);
symbolTable getFromSymTab(char *);
void encPlusIC();
void cleanSymAndExtTab();