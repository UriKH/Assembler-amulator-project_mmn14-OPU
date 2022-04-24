/* [mmn 14 : ]:
in this file: functions which are operating on statements such as editing, validatingת analyzing etc.


author: Uri K.H,   Lihi Haim       Date: 21.3.2021 
ID: 215105321,     313544165       Tutor: Danny Calfon */

#include <stdio.h>
#include <ctype.h>
#include "bool.h"
#include "statmentType.h"

#define DIRECTIVE -1
#define LINE_LEN 81

static boolean isBlankLine(char *);
static void delComments(char *);
static int jumpLabel(char *,int);

/* getSenType(): This function gets a line Assembly code and returns which sentence type is the line:
 comment, blank (spaces), order or directive */
statType getStatType(char *line){
     int i = 0;
     if(isBlankLine(line) == TRUE) /* if the line is a blank one */
          return blank;
     delComments(line);
     if(isBlankLine(line) == TRUE)
          return comment;
     while(line[i] != '\0' && isspace(line[i]))
          i++;
     if(jumpLabel(line, i) == DIRECTIVE){ /* jumps over the optional label in the start of the sentence */
          return directive;
     }
     return instruction;
}

/* isBlank(): This function return if the only characters in a characters array are spaces 
 (tabs, enters, vertical tabs etc.) */
static boolean isBlankLine(char *line){
     int i = 0;
     while(line[i] != '\0'){
          if(!isspace(line[i]))
               return FALSE;
          i++;
     }
     return TRUE;
}

/* delComments(): This function deletes all the comments (in the form " ;comment ") 
 from a character array. The comment is from the character ';' untill the end of the line. */
static void delComments(char *line){
     int i = 0;
     while(line[i] != '\0'){
          if(line[i] == ';')
               break;
          i++;
     }
     line[i] = '\0';
     return;
}

/* jumpLabel(): Jumps to the index after the label (label is in the form " :lable ") */
static int jumpLabel(char *line, int i){
     char curr; 
     boolean labelExist = FALSE;
     
     while(!isspace(curr = line[i]) && curr != '\0'){
          if(curr == ':')
               labelExist = TRUE;
          i++;
     }
     if(!labelExist)
          i = 0;
     while(line[i] != '\0' ){
          if(isspace(line[i]))
               i++;
          else
               break;
          
     }
     while((curr = line[i]) != '\0'){
          if(isspace(curr))
               break;
          if(curr == '.') /* no label */
               return DIRECTIVE;
          i++;
     }
     return 0;
}
