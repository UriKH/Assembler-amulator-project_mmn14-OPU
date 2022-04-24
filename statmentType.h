/* [mmn 14 : statmentType.h]:
In this file: function prototype for getting the assembly line type (statment).

author: Uri K.H,   Lihi Haim       Date: 2.4.2021 
ID: 215105321,     313544165       Tutor: Danny Calfon 
*/
typedef enum {blank = 1, comment, directive, instruction} statType; /* the different statment types */
statType getStatType(char *);