/* [mmn 14 : files.h]:
In this file: function prototypes for file openning, 
reading from file and for output-file makeing functions.

author: Uri K.H,   Lihi Haim       Date: 2.4.2021 
ID: 215105321,     313544165       Tutor: Danny Calfon 
*/
#include <stdio.h>
#include "bool.h"

#define LINE_LEN 81
#define MAX_SUFIX_LEN 4

FILE *openf(char *, char *);
char *readLine(FILE *, int, int);
char *addSufix(char *, char *, char *);
boolean buildOutputFiles(char *);