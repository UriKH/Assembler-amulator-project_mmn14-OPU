/* [mmn 14 : files.c]:
In this file: functions for manging files.

author: Uri K.H,   Lihi Haim       Date: 2.4.2021 
ID: 215105321,     313544165       Tutor: Danny Calfon */

#include <stdlib.h>
#include <string.h>
#include "files.h"
#include "util.h"
#include "filesPrivate.h"

#define RAM_SIZE 4096

/* openf(): Opens a file with a given name in a given status and returns the pointer to the file. */
FILE *openf(char *fileName, char *status){
     FILE *fd = fopen(fileName, status);
     if(!fd){
          printf("error : could not open file: \"%s\"\n",fileName);
          return NULL;
     }
     return fd;
}

/* readLine(): Reads a line (length 82 characters) from a given file pointer - fd, 
 to a dinamic array and returns the array. */
char *readLine(FILE *fd, int len, int lineCnt){
     char *line = calloc(len, sizeof(char));
     int i;
     fgets(line,len,fd);
     for(i = 0; line[i] != '\n' && line[i] != '\0' ; i++)
          ;
     if(line[i] == '\0' && !feof(fd)){
          printf("error [line %d]: line is too long, maximum line length is %d\n", lineCnt, len-2);
          free(line);
          return NULL;
     }
     return line;
}

/* buildOutputFiles(): the function get as a parameter the name we want for a new file
 creats the file and pushes the data into the file. */
boolean buildOutputFiles(char *fileName){
     char *tempFileName = malloc(strlen(fileName) + MAX_SUFIX_LEN + 1);
     FILE *fd = openf(addSufix(tempFileName, ".ob", fileName), "w");
     if(getIC() + getDC() >= RAM_SIZE){
          printf("error: file is too long\n");
          free(tempFileName);
          return FALSE;
     }
     if(!fd){
          free(tempFileName);
          return FALSE;
     }
     fprintf(fd, "\t%d %d\n", getIC()-OS_MEM, getDC());
     pushInstsToFile(fd);
     pushDataToFile(fd);
     fclose(fd);
     
     if(entryExist()){
          fd = openf((addSufix(tempFileName, ".ent", fileName)), "w");
          if(!fd){
               free(tempFileName);
               return FALSE;
          }
          pushEntryToFile(fd);
          fclose(fd);
     }
     if(externalExist()){
          fd = openf((addSufix(tempFileName, ".ext", fileName)), "w");
          if(!fd){
               free(tempFileName);
               return FALSE;
          }
          pushExternToFile(fd);
          fclose(fd);
     }
     free(tempFileName);
     return TRUE;
}

/* buildOutputFiles(): the function get as a parameter the destination global array,
 an extension to be added to name and combines them and pushes them to the destination array
 without harming 'name' itself and returns the full string's address */
char *addSufix(char *dest, char *suffix, char *name){
     if(!dest || !suffix || !name)
          return NULL;
     strcpy(dest, name);
     strcat(dest, suffix);
     return dest;
}