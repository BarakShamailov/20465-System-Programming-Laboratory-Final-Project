#ifndef _DEFINES_H
#define _DEFINES_H

/*max chars in single line including '\0' and '\n'*/
#define MAX_LINE 82
enum boolean
{
    FALSE,
    TRUE
};
/*the all types of operands*/
enum method
{
    IMMED,
    ADDRESS,
    REG
}; 
/*the all types of data */
enum directive
{
    DIRECTIVE_DB,
    DIRECTIVE_DW,
    DIRECTIVE_DH,
    DIRECTIVE_STRING,
    DIRECTIVE_ENTRY,
    DIRECTIVE_EXTERN,
    DIRECTIVE_ERROR
};
/*the types of files we work with them*/

#define AS_EXTENSION "as"
#define TXT_EXTENSION ".txt"
#define OB_EXTENSION ".ob"
#define ENT_EXTENSION ".ent"
#define EXT_EXTENSION ".ext"

/*the range of immed number */
#define MAX_NUM_IMMEDIATE 4095
#define MIN_NUMBER_IMMEDIATE -4096

/*the all directive type*/
#define STR_DIRECTIVE_ASCIZ ".asciz"
#define STR_DIRECTIVE_ENTRY ".entry"
#define STR_DIRECTIVE_EXTERN ".extern"
#define STR_DIRECTIVE_DH ".dh"
#define STR_DIRECTIVE_DB ".db"
#define STR_DIRECTIVE_DW ".dw"

#define NUM_OF_REGISTERS 32 /*the number of all our registers*/
#define OPCODES_AMOUNT 27/*the number of all insteructions*/
#define MAX_RESERVED_WORDS 59/*the number of all our reserved words*/
#define MAX_FILENAME 50/*max name of file*/

#endif
