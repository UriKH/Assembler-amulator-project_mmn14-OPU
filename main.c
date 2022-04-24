/* [mmn 14 : ]:
in this file: 

author: Uri K.H,   Lihi Haim       Date: 21.3.2021 
ID: 215105321,     313544165       Tutor: Danny Calfon */

#include <stdio.h>
#include "bool.h"

boolean compile(char *);
/* main(): This is the main function of the program, it sends the program to compile
 prints out error messages in case compiling failed and terminates the program if needed */
int main(int argc, char *argv[]){
     int i = 1;
     boolean compiled;

     while(i < argc){
          compiled = TRUE;
          if(!(compiled = compile(argv[i])))
               printf("could not compile the Assembly file: \"%s.as\"\n", argv[i]);
          else if(compiled == ERROR){
               printf("hardware unexpected error: memory related error acurred while running the program\n");
               printf("the program has been terminated\n");
               i++;
               break;
          }
          i++;
     }
     if(i == 1)
          printf("no file names has been inserted in the command line\n");
     return 0;
}
