/* [mmn 14 : filesPrivate.h]:
In this file: function prototypes for building output-files functions. 

author: Uri K.H,   Lihi Haim       Date: 2.4.2021 
ID: 215105321,     313544165       Tutor: Danny Calfon 
*/
#include <stdio.h>
#include "bool.h"

/* from symbolTable.c */
void pushEntryToFile(FILE *);
void pushExternToFile(FILE *);
boolean entryExist();
boolean externalExist();
void pushInstsToFile(FILE *);   /* from instructionHandeling.c */
void pushDataToFile(FILE *);    /* from dataHandeling.c */