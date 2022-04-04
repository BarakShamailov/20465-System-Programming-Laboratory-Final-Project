#ifndef _STRUCTS_H
#define _STRUCTS_H

/*this struct will storage all the operand from R group*/
typedef struct r_type {

    unsigned int opcode: 6;
    unsigned int rs: 5;
    unsigned int rt: 5;
    unsigned int rd: 5;
    unsigned int funct: 5;
    unsigned int unused: 6;


}  r_type;
/*this struct will storage all the operand from I group*/
typedef struct i_type {

    int immed: 16;

    unsigned int rt: 5;
    unsigned int rs: 5;

    unsigned int opcode: 6;

}  i_type;

/*this struct will storage all the operand from J group*/
typedef struct j_type {

    unsigned int address: 25;
    unsigned int reg: 1;
    unsigned int opcode: 6;

}  j_type;


/*this union hold the 3 types of the word list*/
typedef union wordHolder
{
    /*firsWord type*/
    r_type R;
    /*Immediate word type*/
    i_type I;
    /*Index word type*/
    j_type J;

} wordHolder;

/*this struct storage error node*/
typedef struct error
{
    /*The error itself*/
    char * str_error;
    /*The line where the error was found*/
    int line;
    /*The string where the error was found*/
    char * str;
    /*Pointer to next*/
    struct error * next;
} error;


/*this struct will storage db directive*/
typedef struct db
{
    /*address of data word*/

    /*Value*/
    char value;
    /*pointer to next*/

} db;
/*this struct will storage dw directive*/
typedef struct dw
{
    /*address of data word*/

    /*Value*/
    long value;
    /*pointer to next*/

} dw;

/*this struct will storage dh directive*/
typedef struct dh
{
    /*address of data word*/

    /*Value*/
    short int value;
    /*pointer to next*/

} dh;
/*this struct will storage asciz directive*/
typedef struct asciz
{
    /*address of data word*/

    /*Value*/
    char value;
    int isNull;



} asciz;
/*this union hold the all type of data*/
typedef union dataHolder
{
    /*firsWord type*/
    dh  dhData;
    dw  dwData;
    db  dbData;
    asciz  ascizData;

} dataHolder;
/*this struct storage data node and it's address*/
typedef struct data
{
    /*the type of the data node (dw,db,dh,asciz)*/
    int type;
    /*union of the data types*/
    dataHolder dataCell;
    /*the address value of the node*/
    long address ;
    /*pointer to the next node*/
    struct data * next;
}data;

/*this struct will storage the word node and address ,line,if we have missing label*/
typedef struct wordNode
{
    /*The word itself*/
    wordHolder holder;
    /*The address assigned to it*/
    long address;
    /*Address sign method - which word is it?*/
    int method;
    /*Which line in input?*/
    int line;
    /*Missing label*/
    char * missingLabel;
    /*Label of external*/
    char * externalLabel;
    /*Is it external?*/
    int isExternal;
    /*Pointer to next*/
    struct wordNode * next;
} wordNode;

/*this struct is a node of symbol list*/
typedef struct symbol
{
    /*Label of the symbol*/
    char * label;
    /*Address of the symbol*/
    long address;
    /*Is it external?*/
    int external;
    /*is it entry*/
    int entry;
    /*is it data?*/
    int data;
    /*is it an operation?*/
    int operation;
    /*pointer to next*/
    struct symbol * next;
} symbol;

/*this struct storage the all instruction in our assembler in array*/
typedef struct opcode
{
    /*The operation string*/
    char * str;
    /*The opcode*/
    int code;
    /*The funct*/
    int funct;
    /*The group*/
    char * group;
} opcode;

/*This file contains function used for file handling*/
/*those structs for print in LSB*/
typedef struct i_print_little_endian {

    unsigned int opcode: 6;

    unsigned int rs:5;
    unsigned int rt:5;
    int immed: 16;


}  i_print_little_endian;

typedef struct j_print_little_endian {

    unsigned int opcode: 6;

    unsigned int reg: 1;
    unsigned int address: 25;


}  j_print_little_endian;
#endif


