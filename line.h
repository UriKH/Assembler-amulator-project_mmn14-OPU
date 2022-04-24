/* [mmn 14 : line.h]:
In this file: function prototype for getting the directive command type of an directive statment in
Assembly language + an enum for the directive types.

author: Uri K.H,   Lihi Haim       Date: 2.4.2021 
ID: 215105321,     313544165       Tutor: Danny Calfon 
*/
#include "bool.h"

enum cmds {data, string, entry, external};  /* the directive commands types, matching: .data, .string, .entry, .extern */
                                            /* mathing the array in the function getDirCom() */
int getDirCom(char *, int, boolean);