/* [mmn 14 : dataHandeling.c]:
In this file: functions for directive a statment and related memory handeling and managing.

author: Uri K.H,   Lihi Haim       Date: 2.4.2021 
ID: 215105321,     313544165       Tutor: Danny Calfon */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "util.h"
#include "word.h"
#include "label.h"

#define MAX_DEC_LEN 5

static int DC;
static word *dataImage;

static word *scanDataParams(char *, int *, char *, int);
word getDecWord(char *, int, boolean); /* from instruction handeling.h */

/* pushData(): the function get as parameters the current line we're reading,
 the pointer to the current character, the line's length, the type of the current word, and an allocation flag 
 (if flag is TRUE then the line only be check but the data shall not be pushed to the Data-Image).
 the function and pushes the relevent words to memory image (according to the allocation flag). */
boolean pushData(char *line, int *lInd, char *type, int lineCnt, boolean allocate){
    int i = 0;
    word *scaned;
    
    jumpSpaces(line, lInd);
    while(line[*lInd] != '\0'){
        if(!(scaned = scanDataParams(line, lInd, type, lineCnt))) /* scanning data from the line */
            return ERROR;
        else if(scaned->ARE == ERROR_ARE){ /* error occurred while scanning */
            free(scaned);
            return FALSE;
        }
        if(!isThereComma(line, lInd, lineCnt)){ /* searching for a comma between parameters */
            free(scaned);
            return FALSE;
        }
        i = 0;
        while(scaned[i].ARE != BLANK_ARE && allocate){
            if(DC == 0){ /* pushing the data to the data image */
                dataImage = malloc(sizeof(word));
                DC++;
            }
            else
                dataImage = realloc(dataImage, ++DC * sizeof(word));
            if(!isAlloc(dataImage)){
                free(scaned);
                return ERROR;
            }
            (dataImage)[DC-1] = scaned[i]; /* adding scaned argument to the Data Image array */
            i++;
        }
        free(scaned);
    }
    return TRUE;
}
    
/* scanDataParams(): the function get as parameters the current line we're reading,
 the pointer to the current word, the line's length and the type of the current word.
 and reads the parameters from the line and returns an array of words with the parameters */
static word *scanDataParams(char *line, int *lInd, char *type, int lineCnt){
    char curr;
    int size = 1;
    word *dataForPush, *error = malloc(sizeof(word));
    error->ARE = ERROR_ARE; /* returned if error has been occurred */

    if (!isAlloc(error))
        return NULL;
    /* There must be parameters after .data or .string directives */
    if (line[*lInd] == '\0'){
        printf("error [line %d]: There must be parameters after the directives \".data\" or \".stirng\"\n", lineCnt);
        return error;
    }

    /* data type is decimal numbers */
    if (strcmp(type, "data") == 0){
        char *temp = readWord(line, lInd);
        word tempDecWord = getDecWord(temp, lineCnt, TRUE);
        free(temp);

        dataForPush = calloc(2, sizeof(word));
        if(!isAlloc(dataForPush)){
            free(error);
            return NULL;
        }
        if(tempDecWord.ARE == ERROR_ARE){
            free(dataForPush);
            return error;
        }
        dataForPush[0].wrd = tempDecWord.wrd;
        dataForPush[0].ARE = tempDecWord.ARE;
        dataForPush[1].ARE = BLANK_ARE;
        free(error);
        return dataForPush;
    }
    else if (strcmp(type, "string") != 0)
        return error;
    
    /* data type is a string (characters) */
    dataForPush = calloc(size, sizeof(word));
    if (line[*lInd] != '\"'){
        printf("error [line %d]: the argument of the directive \".string\" must start and end with a quotation mark\n", lineCnt);
        free(dataForPush);
        return error;
    }
    (*lInd)++; /* the beginning of the string (after the '"') */
    size = 2;
    while ((curr = line[*lInd]) != '\0' && curr != '\"'){
        if (curr == '\\' && line[*lInd + 1] == '\"'){ /* reached '\"' character */
            dataForPush = realloc(dataForPush, ++size * sizeof(word));
            if (!isAlloc(dataForPush)){
                free(error);
                return NULL;
            }
            dataForPush[size - 3].wrd = '\"';
            (*lInd) += 2;
            continue;
        }
        dataForPush = realloc(dataForPush, ++size * sizeof(word));
        if (!isAlloc(dataForPush)){
            free(error);
            return NULL;
        }
        dataForPush[size - 3].wrd = curr;
        dataForPush[size - 3].ARE = 'A';
        (*lInd)++;
    }
    if (curr == '\0' || curr != '\"'){ /* string ended unaturaly at the end of the array and not by: " */
        printf("error [line %d]: There must be \" at the end of a string\n", lineCnt);
        free(dataForPush);
        return error;
    }
    (*lInd)++;
    if (!isBlank(line, *lInd)){
        printf("error [line %d]: the directive \".string\" gets one string only\n", lineCnt);
        free(dataForPush);
        return error;
    }
    dataForPush[size - 2].wrd = '\0';
    dataForPush[size - 2].ARE = 'A';
    dataForPush[size - 1].ARE = BLANK_ARE; /* simbolising the */
    free(error);
    return dataForPush;
}

/* cleanDC(): the function frees all DATA from data image and sets DC counter to 0 */
void cleanDC(){
    free(dataImage);
    dataImage = NULL;
    DC = 0;
    return;
}

/* getDC(): the fuction fetches the value of the DATA counter */
int getDC(){
    return DC;
}

/* pushDataToFile(): the function get as a parameter the file we want to write to
 and copies everything from the Data Image array to it */
void pushDataToFile(FILE *fd){
    int i;
    for(i = 0; i < DC; i++)
        fprintf(fd, "%04d %03X %c\n", getIC() + i, dataImage[i].wrd & 0xFFF, dataImage[i].ARE);
    return;
}