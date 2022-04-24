/* [mmn 14 : bool.h]:
In this file: Defenition of the two boolean types: true and false + an error constant (ERROR).

author: Uri K.H,   Lihi Haim       Date: 2.4.2021 
ID: 215105321,     313544165       Tutor: Danny Calfon 
*/
#ifndef BOOL_H
#define BOOL_H
typedef enum {FALSE, TRUE, ERROR = -1 } boolean;
/* The ERROR constant represent a situation when an unexpected error acurred */
#endif