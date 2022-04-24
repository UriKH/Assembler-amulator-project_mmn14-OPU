/* [mmn 14 : word.h]:
in this file: The defenition of a word - a memory cell sized 12 bits. 

author: Uri K.H,   Lihi Haim       Date  : 21.3.2021 
ID: 215105321,     313544165       Tutor: Danny Calfon */
#ifndef WORD_H
#define WORD_H

#include <math.h>             /* for pow() */
#define BLANK_ARE ' '
#define UNKNOWN_ARE '?'       /* a flag in the ARE field at the memory image for unused cells (for later use) */
#define ERROR_ARE '0'         /* a flag in the ARE field at the memory image for an expected error, used in functions which */
                              /* are returning a pointer to a 'word' or a 'word' */
#define MIN_WORD_VAL (int)(-pow(2, 11))
#define MAX_WORD_VAL (int)(pow(2, 11) - 1)
/* word: the structure of every memory cell (imginary RAM) */
typedef struct{
     unsigned int wrd :12;    /* value */
     char ARE;                /* memory flag */
} word;

#endif