/* [mmn 14 : label.h]:
In this file: function prototypes for label reading from a line or for validation of it.

author: Uri K.H,   Lihi Haim       Date: 2.4.2021 
ID: 215105321,     313544165       Tutor: Danny Calfon 
*/
#include "bool.h"

char *readWord(char *, int *);
boolean isValidLabel(char *, int, boolean);
boolean isLabel(char *);