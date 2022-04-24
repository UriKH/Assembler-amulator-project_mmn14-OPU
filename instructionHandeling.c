/* [mmn 14 : instructionHandeling.c]:
In this file: functions for instruction statment and related memory handeling and managing.

author: Uri K.H,   Lihi Haim       Date: 2.4.2021 
ID: 215105321,     313544165       Tutor: Danny Calfon */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "label.h"
#include "line.h"
#include "util.h"
#include "word.h"
#include "buildWord.h"
#include "symTabTypes.h"

boolean wasDefined(char *, int, boolean);   /* from symbolTable.c */
boolean addToExtTab(char *, int);           /* from symbolTable.c */
symbolTable getFromSymTab(char *);          /* from symbolTable.c */
boolean definedAs(char *, enum attribs);    /* from symbolTable.c */


/* memory macros */
#define REGISTERS 8     /* the number of general register the imagenry computer's CPU has */
#define INSTRUCTIONS 16 /* the amount of instructions in the imagenary computer's assembly language */

#define MVAM 3          /* Maximum Valid Addressing Method options - for any of the instruction commands */
#define MAX_WORDS 3     /* maximum words to build from an instruction command (operation, source operand, target operand) */

#define MAKE_STRING(str) (#str)                  /* makes a string */
#define REGISTER_NAME(reg) (MAKE_STRING(r##reg)) /* makes a register name (string starts with 'r' and then a number) */
#define freeArr(arr, size)         \
    {                              \
        int i;                     \
        for (i = 0; i < size; i++) \
            free(arr[i]);          \
        free(arr);                 \
    }

typedef struct{
    char *name;
    int opcode;
    int funct;
    int SVAMs[MVAM]; /* Source operand Valid Addressing Methods */
    int TVAMs[MVAM]; /* Target operand Valid Addressing Methods */
    int ops;         /* operands */
} inst;

static inst instsList[INSTRUCTIONS] = {
    /* the two operand instructions */
    {"mov", 0, 0, {0, 1, 3}, {1, 3, -1}, 2},
    {"cmp", 1, 0, {0, 1, 3}, {0, 1, 3}, 2},
    {"add", 2, 10, {0, 1, 3}, {1, 3, -1}, 2},
    {"sub", 2, 11, {0, 1, 3}, {1, 3, -1}, 2},
    {"lea", 4, 0, {1, -1}, {1, 3, -1}, 2},
    /* the one operand instructions */
    {"clr", 5, 10, {-1}, {1, 3, -1}, 1},
    {"not", 5, 11, {-1}, {1, 3, -1}, 1},
    {"inc", 5, 12, {-1}, {1, 3, -1}, 1},
    {"dec", 5, 13, {-1}, {1, 3, -1}, 1},
    {"jmp", 9, 10, {-1}, {1, 2, -1}, 1},
    {"bne", 9, 11, {-1}, {1, 2, -1}, 1},
    {"jsr", 9, 12, {-1}, {1, 2, -1}, 1},
    {"red", 12, 0, {-1}, {1, 3, -1}, 1},
    {"prn", 13, 0, {-1}, {0, 1, 3}, 1},
    /* the zero operand instructions */
    {"rts", 14, 0, {-1}, {-1}, 0},
    {"stop", 15, 0, {-1}, {-1}, 0}};

enum {  immidiate,
        direct,
        relative,
        registerDirect };
static int IC; /* Instructions-Counter, Data-Counter (index) */
static word *instImage;
static int blankInd;

static int JumpToBlankCell();
static word *scanInstParams(char *, int *, int);
static boolean isValidOpsAmount(int, int, int);
static boolean isValidSAM(int, int, int);
static boolean isValidTAM(int, int, int);
static boolean isValidOperand(char *, int, int);
static int getOperationIndex(char *, int);
static int getAddressingMethod(char *, int);

/* pushInstFirst(): This function is the first transition of the file's code.
 it's intention is to count the memory size taken by the instructions, and set's the instruction counter accordingly
 the function get as parameters the current line, a pointer to current word in line and the length of the line 
 and returns TRUE in case of success in pushing the instructions to the suitable array */
boolean pushInstFirst(char *line, int *lInd, int lineCnt, boolean allocate){
    int temp = IC, i/*, tempInd = *lInd*/;
    word *toPush;
    jumpSpaces(line, lInd);
    toPush = scanInstParams(line, lInd, lineCnt);
    if (toPush == NULL)
        return ERROR;
    if (toPush->ARE == ERROR_ARE){
        free(toPush);
        return FALSE;
    }
    if(allocate){
        if (temp == OS_MEM) /* pushing the data to the data image */
            instImage = malloc(sizeof(word));  
        if(IC - OS_MEM > 1)
            instImage = realloc(instImage, (IC - OS_MEM) * sizeof(word));
        if (!isAlloc(instImage)){
            free(toPush);
            return ERROR;
        }
        for (i = 0; i < IC - temp; i++)
            instImage[temp+i-OS_MEM] = toPush[i];
    }
    free(toPush);
    return TRUE;
}

/* pushInstSecond(): This function is the second transition of the file's code.
 it's intention is to complete the coding into machine code using the addresses and data from symbol table
 the function gets as parameters the current line, a pointer to current word in line and the length of the line 
 and returns TRUE in case of success in inserting the proper data from symbol table and setting the A,R,E feature*/
boolean pushInstSecond(char *line, int *lInd, int lineCnt){
    word tempWord;
    char *operand;
    symbolTable symTabRow;
    int AM, blankIC; /* addressing method type and index of blank memory cell needed to be filled */
    
    if(!(operand = readWord(line, lInd)))
        return ERROR;
    if(strcmp(operand, "\0") == 0){
        free(operand);
        return TRUE;
    }
    switch((AM = getAddressingMethod(operand, lineCnt))){
    case direct:
    case relative:
        blankIC = JumpToBlankCell(); /* jumping to the nearest blank cell */
        if(AM == relative)
            operand++;
        if(blankIC == -1){
            if(AM == relative)
                operand--;
            free(operand);
            return FALSE;
        }
        symTabRow = getFromSymTab(operand); /* getting a symbol table line */
        if (strcmp(symTabRow.symbol, "\0") != 0){
            if (definedAs(operand, EXT)){
                if(AM == relative){
                    printf("error [line %d]: the relative addressing method can be used only on defeined-in-file labels (not defined by .extern)\n", lineCnt);
                    operand--;
                    free(operand);
                    return FALSE;
                }
                instImage[blankIC].ARE = 'E';
                if(!addToExtTab(operand, blankIC)){
                    free(operand);
                    return FALSE;
                }
            }
            else
                instImage[blankIC].ARE = 'R';
            if(AM == relative){
                instImage[blankIC].wrd = -1 * (blankIC - symTabRow.address + OS_MEM);
                instImage[blankIC].ARE = 'A';
            }
            else
                instImage[blankIC].wrd = symTabRow.address;
        }
        else{
            printf("error [line %d]: label operand \"%s\" was not defined\n", lineCnt, operand);
            if(AM == relative)
                operand--;
            free(operand);
            return FALSE;
        }
        break;
    case registerDirect:
        blankIC = JumpToBlankCell(); 
        if(blankIC == -1){
            free(operand);
            return FALSE;
        }
        tempWord = buildRegWord(operand); /* building a register word */
        instImage[blankIC].ARE = 'A';
        instImage[blankIC].wrd = tempWord.wrd;
        free(operand);
        return TRUE;
    case -1: /* error */
        free(operand);
        return FALSE;
    }

    if(AM == relative)
        operand--;
    free(operand);
    return TRUE;
}

/* JumpToBlankCell(): the static function fetches a pointer to the next empty cell in
 the instruction array */
static int JumpToBlankCell(){
    for (; blankInd < IC-OS_MEM; blankInd++){
        if(instImage[blankInd].ARE == UNKNOWN_ARE)
            return blankInd;
    }
    return -1;
}

/* getDecWord(): the function gets as parameters a string, the number of the line and a 
 flag for knowing if the string represents an operand of an instruction or a directive command. 
 The function checks if the string is a valid decimal number and if it is the decimal will be converted to 
 and it will be returned. */
word getDecWord(char *toWord, int lineCnt, boolean isPure){
    int i = 0, decimal;
    char *temp = toWord, curr;
    word finalWrd, error;
    error.ARE = ERROR_ARE;

    if(!isPure)
        temp = toWord +1;
    curr = temp[i];
    if (curr == '+' || curr == '-'){
        i++;
        if(isspace(temp[i]) || temp[i] == ',' || temp[i] == '\0'){
            printf("error [line %d]: after a sign -/+ must come a decimal number\n", lineCnt);
            return error;
        }
    }
    for (; i < strlen(temp); i++){
        curr = temp[i];
        if(curr == '.'){
            printf("error [line %d]: cannot hold uninteger numbers\n", lineCnt);
            return error;
        }
        if (!isdigit(curr)){
            if(isPure)
                printf("error [line %d]: arguments for the directive \".data\" can be writen only in numericl characters\n", lineCnt);
            else
                printf("error [line %d]: in the immediate addressing method can be writen only the numericl characters\n", lineCnt);
            printf("\t\t (and '+' or '-' in the start of the number)\n");
            return error;
        }
    }
    decimal = atoi(temp);
    if (decimal < MIN_WORD_VAL || decimal > MAX_WORD_VAL){
        printf("error [line %d]: decimal numbers can be handled only in range: %d to %d\n", lineCnt, MIN_WORD_VAL, MAX_WORD_VAL);
        return error;
    }
    finalWrd.wrd = decimal;
    finalWrd.ARE = 'A';
    return finalWrd;
}

/* scanInstParams(): the function gets as parameters the current line,
 a pointer to current word in line and the number of the line 
 for parameters for DATA and STRING instructions and returns an array of the parameters as words */
static word *scanInstParams(char *line, int *lInd, int lineCnt){
    int opAmnt = 1, operationInd;   /* amount of operands (including the command), index of the command in the instructions array */
    int SAM = 0, TAM = 0;           /* Source and Target Addressing Methods */
    char **operands = calloc(MAX_WORDS, sizeof(char *)), *tempOperand = NULL;
    word *memory = calloc(MAX_WORDS, sizeof(word));
    word *error = malloc(sizeof(word)), unknown;
    boolean commaErr = FALSE;       /* error while searching for a comma */

    if(!isAlloc(error) || !isAlloc(memory) || !isAlloc(operands)){
        free(error);
        free(memory);
        free(operands);
        return NULL;
    }
    error->ARE = ERROR_ARE;     /* returned if error has been occurred */
    unknown.ARE = UNKNOWN_ARE;  /* for symbolizing an empty memory cell */
    unknown.wrd = 0;

    if ((operands[0] = readWord(line, lInd)) == NULL){ /* reading the operation name */
        freeArr(operands, MAX_WORDS);
        free(memory);
        free(error);
        return NULL;
    }
    else if (strcmp(operands[0], "\0") == 0){ /* could not read a word */
        printf("error [line %d]: after the end of a label must come a directive or an instruction\n", lineCnt);
        freeArr(operands, MAX_WORDS);
        free(memory);
        return error;
    }
    /* there is no possible situation in which the operands[0] is equal to "\0" -> cheking in compiler.c */
    if ((operationInd = getOperationIndex(operands[0], lineCnt)) == -1){ /* getting the operation index at the instructions table */
        freeArr(operands, MAX_WORDS);
        free(memory);
        return error;
    }
    jumpSpaces(line, lInd);
    while ((tempOperand = readWord(line, lInd)) != NULL){ /* reading a temporary operand from the line */
        if(strcmp(tempOperand, "\0") == 0)
            break;
        if(strcmp(tempOperand, "%") == 0 || strcmp(tempOperand, "#") == 0){
            printf("error [line %d]: after an addressing method sign must come an operand (without spaces in between)\n", lineCnt);
            free(memory);
            freeArr(operands, MAX_WORDS);
            free(tempOperand);
            return error;
        }
        if (!isThereComma(line, lInd, lineCnt))
            commaErr = TRUE;
        if(opAmnt < MAX_WORDS)
            operands[opAmnt] = tempOperand;
        else if(opAmnt >=  MAX_WORDS){
            if(!isBlank(line, *lInd)){
                printf("error [line %d]: extranous text after end of command\n", lineCnt);
                freeArr(operands, MAX_WORDS);
                free(tempOperand);
                free(memory);
                return error;
            }
            free(tempOperand);
        }
        opAmnt++;
    }
    IC += opAmnt;
    if (tempOperand == NULL){ /* could not read the last operand or there are more than 2 operands */
        freeArr(operands, MAX_WORDS);
        free(error);
        free(memory);
        return NULL;
    }
    if (!isValidOpsAmount(operationInd, opAmnt-1, lineCnt) || commaErr){ /* the operands amount does not correspond to the needed amount of operands */
        freeArr(operands, MAX_WORDS);
        free(memory);
        return error;
    }
    else if (opAmnt-1 == 1){ /* if instruction has only one operand */
        TAM = getAddressingMethod(operands[1], lineCnt); /* target operand addressing method */
        if(TAM == -1){
            freeArr(operands, MAX_WORDS);
            free(memory);
            return error;
        }
        if (isValidTAM(operationInd, TAM, lineCnt)){
            if(!isValidOperand(operands[1], TAM, lineCnt)){
                freeArr(operands, MAX_WORDS);
                free(memory);
                return error;
            }
            if (TAM == immidiate){
                memory[1] = getDecWord(operands[1], lineCnt, FALSE);
                if(memory[1].ARE == ERROR_ARE){ /* error while converting the immediate operand to a word */
                    freeArr(operands, MAX_WORDS);
                    free(memory);
                    return error;
                }
            }
            else
                memory[1] = unknown;
        }
        else{
            freeArr(operands, MAX_WORDS);
            free(memory);
            return error;
        }
    }
    if (opAmnt-1 < 2) { /* if operand amount is 0 or 1 */
        memory[0] = buildMainWord(instsList[operationInd].opcode, instsList[operationInd].funct, SAM, TAM); /* building the main word */
        freeArr(operands, MAX_WORDS);
        free(error);
        return memory;
    }
    /* if instruction has two operand */
    SAM = getAddressingMethod(operands[1], lineCnt); /* source operand addressing method */
    TAM = getAddressingMethod(operands[2], lineCnt); /* target operand addressing method */
    if(TAM == -1 || SAM == -1){
            freeArr(operands, MAX_WORDS);
            free(memory);
            return error;
        }
    if (isValidSAM(operationInd, SAM, lineCnt) && isValidTAM(operationInd, TAM, lineCnt)){
        if(!isValidOperand(operands[1], SAM, lineCnt) || !isValidOperand(operands[2], TAM, lineCnt)){
            freeArr(operands, MAX_WORDS)
            free(memory);
            return error;
        }
        if (SAM == immidiate)
            memory[1] = getDecWord(operands[1], lineCnt, FALSE);
        else
            memory[1] = unknown;
        if (TAM == immidiate)
            memory[2] = getDecWord(operands[2], lineCnt, FALSE);
        else
            memory[2] = unknown; 
        if(memory[1].ARE == ERROR_ARE || memory[2].ARE == ERROR_ARE){ /* error while converting the immediate operand to a word */
            freeArr(operands, MAX_WORDS);
            free(memory);
            return error;
        }
    }
    else{
        freeArr(operands, MAX_WORDS);
        free(memory);
        return error;
    }
    memory[0] = buildMainWord(instsList[operationInd].opcode, instsList[operationInd].funct, SAM, TAM);
    freeArr(operands, MAX_WORDS);
    free(error);
    return memory;
}

/* isValidOpsAmount(): the function get as parameters the operation's identification number and
 amount of opernd's in the line of the current oparation, and the number of the line,
 to check if the line of the oparation is valid- and returns true or false accordingly 
 accompanied by an error message if needed*/
static boolean isValidOpsAmount(int operationInd, int opAmount, int lineCnt){
    if (instsList[operationInd].ops == opAmount)
        return TRUE;
    printf("error [line %d]: wrong number of operands for the operation \"%s\"\n", lineCnt, instsList[operationInd].name);
    printf("\t\t this operation demandes exactly %d operands\n", instsList[operationInd].ops);
    if(opAmount > 2)
        printf("\t\t [note : to an instruction command can be sent at most: a source and a target operand]\n");
    return FALSE;
}

/* isValidSAM(): the function get as parameters the operation's identification number and
 Addressing Method's number, and the number of the line,
 to check if the Addressing Method's number of the source operator is valid
 and if this part of the the oparation is valid- and returns true or false accordingly */
static boolean isValidSAM(int operandInd, int AM, int lineCnt){
    int i;
    for (i = 0; (instsList[operandInd].SVAMs)[i] != -1 && i < MVAM; i++){
        if ((instsList[operandInd].SVAMs)[i] == AM)
            return TRUE;
    }
    printf("error [line %d]: the addressing method for the source operand is not valid\n", lineCnt);
    return FALSE;
}

/* isValidTAM(): the function get as parameters the operation's identification number and
 Addressing Method's number, and the number of the line,
 to check if the Addressing Method's number of the target operator is valid
 and if this part of the oparation is valid- and returns true or false accordingly */
static boolean isValidTAM(int operandInd, int AM, int lineCnt){
    int i;
    for (i = 0; (instsList[operandInd].TVAMs)[i] != -1 && i < MVAM; i++){
        if ((instsList[operandInd].TVAMs)[i] == AM)
            return TRUE;
    }
    printf("error [line %d]: the addressing method for the target operand is not valid\n", lineCnt);
    return FALSE;
}

/* isValidOperand(): the function get as parameters an opernd and it's Addressing Method's number,
 and the number of the line, to check if the Addressing Method's number of the operator is valid
 and returns true or false accordingly */
static boolean isValidOperand(char *operand, int AM, int lineCnt){
    switch (AM){
    case relative:{
        char *temp = operand + 1;
        if(!isIlegalName(temp, lineCnt) && isValidLabel(temp, lineCnt, FALSE))
            return TRUE;
        return FALSE;
    }
    case direct:
        if (!isIlegalName(operand, lineCnt) && isValidLabel(operand, lineCnt, FALSE))
            return TRUE;
        return FALSE;
    default:
        return TRUE;
    }
}

/* getOperationIndex(): the function get as parameters an oparation name and it's number (at the input file),
 to check if the operation is valid and to fetch it's identification number
 or if it's not valid- return error */
static int getOperationIndex(char *operation, int lineCnt){
    int i;
    for (i = 0; i < INSTRUCTIONS; i++){
        if (strcmp(instsList[i].name, operation) == 0)
            return i;
    }
    printf("error [line %d]: invalid operation (instruction command) \"%s\" name\n", lineCnt, operation);
    return -1;
}

/* getAddressingMethod(): the function get as parameters an operand name and it's number (at the input file),
 to check if the operand is valid and to fetch it's addressing method identification number
 or if it's not valid- returns error */
static int getAddressingMethod(char *operand, int lineCnt){
    switch (operand[0]){
    case '#':
        if (strlen(operand) > 1)
            return immidiate;
        printf("error [line %d]: after the addressing method sign '#' must come a decimal number (with no spaces between)\n", lineCnt);
        break;
    case '%':
        if (strlen(operand) > 1)
            return relative;
        printf("error [line %d]: after the addressing method sign - precentage, must come a label (with no spaces between)\n", lineCnt);
        break;
    case 'r':
    {
        int i = 1;
        while(!isspace(operand[i]) && operand[i] != ',' && operand[i] != '\0'){
            if(!isdigit(operand[i]))
                return direct;
            i++;
        }
        if(atoi(operand+1) >= REGISTERS){
            printf("error [line %d]: non existing register (register are in range r0 - r7)\n", lineCnt);
            return -1;
        }
        for (i = 0; i < REGISTERS; i++){
            if(i == atoi(operand+1))
                return registerDirect;
        }        
    }
    default:
        return direct;
    }
    return -1;
}

/* cleaICn(): the function get as a parameter the boolean flag that indicates
 whether to free all Instruction that are stored and if it's TRUE releases them
 and sets IC counter to it's original value*/
void cleanIC(){
    free(instImage);
    instImage = NULL;
    IC = OS_MEM;
    blankInd = 0;
    return;
}

/* isIlegalName(): the function gets as parameters a label and it's number (at the input file),
 to check if the label is valid and return true or false accordingly */
boolean isIlegalName(char *label, int lineCnt){
    int i = 1;
    if(getDirCom(label, lineCnt, FALSE) != -1){
        printf("error [line %d]: label should not be named as a directive\n", lineCnt);
        return TRUE;
    }
    if(label[0] == 'r'){
        while(!isspace(label[i]) && label[i] != '\0'){
            if(!isdigit(label[i]))
                break;
            i++;
        }
        if(atoi(label+1) >= 0 && atoi(label+1) <= REGISTERS-1 && isdigit(label[strlen(label)-1])){
            printf("error [line %d]: label should not be named as a register\n", lineCnt);
            return TRUE;       
        }
    }

    for (i = 0; i < INSTRUCTIONS; i++){
        if (strcmp(instsList[i].name, label) == 0){
            printf("error [line %d]: label must not be named as an instruction\n", lineCnt);
            return TRUE;
        }   
    }
    return FALSE;
}

/* getIC(): the fuction fetches the value of the INSTRUCTION counter*/
int getIC(){
    return IC;
}

/* pushDataToFile(): the function get as a parameter the file we want to write to
 and writes everything from ISTRUCTION's  array*/
void pushInstsToFile(FILE *fd){
    int i;
    for (i = 0; i < IC - OS_MEM; i++)
        fprintf(fd, "%04d %03X %c\n", i + OS_MEM, instImage[i].wrd & 0xFFF, instImage[i].ARE);
    return;
}