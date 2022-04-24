/* [mmn 14 : util.c]:
in this file: utility function which are used in many of the files.

author: Uri K.H,   Lihi Haim       Date: 21.3.2021 
ID: 215105321,     313544165       Tutor: Danny Calfon */

#include <stdio.h>
#include <ctype.h>
#include "util.h"

/* isBlank(): returns if the a string from a certain index is full of spaces 
 parameters: str - the string
             int - the starting index for checking 
*/
boolean isBlank(char *str, int ind){
    for (; isspace(str[ind]) && str[ind] != '\0'; ind++)
        ;
    if (str[ind] == '\0')
        return TRUE;
    return FALSE;
}

/* jumpSpaces(): jumps over the spaces in a string with a pointer to a certain index in it.
 parameters: str - the string
             int - the pointer to the index
*/
void jumpSpaces(char *str, int *ind){
    char curr;
    while ((curr = str[*ind]) != '\0' && isspace(curr))
        (*ind)++;
    return;
}

/* isThereComma(): Jumps over the spaces and commas in a line from a file with a pointer to a certain index in it.
 The function returns TRUE if there in only one comma within the spaces.
 else - returned FALSE. If there are only spaces are between the starting index util the end of the string 
 TRUE will be returned.
 parameters: line - the string
             int - the pointer to the starting index
             lineCnt - the number of the line in the file
*/
boolean isThereComma(char *line, int *ind, int lineCnt){
    int commas = 0;
    char curr;
    while ((curr = line[*ind]) != '\0' && (isspace(curr) || curr == ',')){
        if (curr == ',')
            commas++;
        /*if (commas > 1){
            printf("error [line %d]: between parameters must seperate one comma only\n", lineCnt);
            return FALSE;
        }*/
        (*ind)++;
    }
    if (commas > 1){ /* there has been more than one comma in the space */
        printf("error [line %d]: between parameters must seperate one comma only\n", lineCnt);
        return FALSE;
    }
    if (curr == '\0' && commas >= 1){ /* there has been more than one comma after the last word of the string */
        printf("error [line %d]: extranous comma after the last operand in the line\n", lineCnt);
        return FALSE;
    }
    else if (curr != '\0' && commas == 0){ /* there is no comma between the words of the string */
        printf("error [line %d]: between parameters must seperate a comma\n", lineCnt);
        return FALSE;
    }
    return TRUE;
}

/* isAlloc(): checks if a pointer has been allocated properly (not NULL) and returns the answer. 
 parameters: p - the pointer 
 */
boolean isAlloc(void *p){
    if (!p){
        printf("[fatal error : memory allocation failed, could not continue the compilation process]\n");
        return FALSE;
    }
    return TRUE;
}
