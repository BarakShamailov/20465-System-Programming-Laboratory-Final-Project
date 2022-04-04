#include "header.h"


static symbol * head = NULL;

/*This function adds a symbol to the symbols list*/
void addSymbol(char * label, int address, int external, int  entry,int data,int operation, int line)
{
    /*Temp sybol*/
    symbol * tmp = NULL;
    /*This function will check if such symbol already exists*/
    isSameSymbol(label,address,external,entry,data,operation,line);
    /*Allocate memory for new symbol*/
    tmp = malloc(sizeof(symbol));
    /*Check if allocation succeeded*/
    checkAllocation(tmp);
    /*Allocate memory for symbol's label*/
    tmp->label = malloc(sizeof(char)*MAX_LINE);
    /*Check if allocation succeeded*/
    checkAllocation(tmp->label);
    /*Copy the label from the input to this symbol*/
    strcpy(tmp->label, label);
    /*Copy value of DC*/
    tmp->address = address;
    /*Copy external flag*/
    tmp->external = external;
    tmp->data = data;
    tmp->entry = entry;
    /*Copy operation tab*/
    tmp->operation = operation;
    /*set next to NULL*/
    tmp->next = NULL;
    /*if list is empty*/
    if (!head)
    {
        /*make head point to temp*/
        head = tmp;
    }
        /*if list isn't empty*/
    else
    {
        /*create pointer to head*/
        symbol * p = head;
        /*get to the last node*/
        while (p->next)
            p = p->next;
        /*set last node's next to tmp*/
        p->next = tmp;
    }
}
/***********************************************************************************************/
/*This function will update the symbol's addresses to match with the IC counter,
after the first pass*/
void updateSymbols(int IC)
{
    symbol * ptr = head;
    while (ptr)
    {
        if (ptr->data == TRUE)
        {
            ptr->address += IC;
        }

        ptr = ptr->next;
    }
}

/***********************************************************************************************/
/*This function cleans up the symbol list*/
void freeSymbols()
{
    symbol * ptr = head;
    if (!ptr)
        return;

    while (ptr)
    {
        symbol * tmp = ptr;
        ptr = ptr->next;
        free(tmp->label);
        free(tmp);
    }
    head = NULL;
}
/***********************************************************************************************/
/*this function check if there are two label with same name and check errors*/
int isSameSymbol(char * label , int address, int external, int  entry,int data,int operation, int line )
{
    symbol * ptr = head;

    while(ptr)
    {
        if (!strcmp(ptr->label, label))
        {
            if((ptr->entry == TRUE && external == TRUE )|| (ptr->external == TRUE && entry == TRUE))
            {
                addError("cant be entry and external label with the same name  ",line,label);
                return 0;
            }
            if(((ptr->operation == TRUE || ptr->data == TRUE) && external == TRUE ) || (ptr->external == TRUE && (data == TRUE || operation == TRUE)))
            {
                addError("external label , cant appear before operand or data operand ",line,label);
                return 0;
            }
            if(((ptr->data == TRUE || ptr->operation == TRUE) && (external == FALSE && entry == FALSE)) || ((ptr->entry == FALSE && ptr->external == FALSE) && (data == TRUE || operation == TRUE)))
            {
                addError(" The label already exists",line,label);
                return 0;
            }

        }
        ptr=ptr->next;
    }
    return 1;
}

/***********************************************************************************************/

/*This function checks if a symbol with the same name exists
 * and return the address */
long checkSymbol(char * label,int method,int line)
{
    symbol * ptr = head;


    while (ptr)
    {
        if (!strcmp(ptr->label, label))
        {

            if(ptr->external == TRUE && method == IMMED)
            {

                addError("cant use external label parameter for (beq,blt,bne,bgt)",line,label);
            }

            if(ptr->entry == TRUE && method == ADDRESS)
            {
                symbol * tmp = head;
                while(tmp)
                {
                    if (!strcmp(tmp->label, label))
                    {

                        if(  (tmp->data == TRUE && tmp->entry == FALSE )|| ((tmp)->operation == TRUE && tmp->entry == FALSE))
                        {
                            tmp->entry = TRUE;
                            return tmp->address;
                        }
                    }
                    tmp =tmp->next;
                }

            }
            else
            {
                if(method == IMMED)
                {
                    return ptr->address;
                }
            }



        }
        ptr = ptr->next;

    }


    return 0;
}
/***********************************************************************************************/
/*This function will return a symbol with the same label as the string
entered as a parameter. if no such symbol was found, it will return NULL*/
symbol * searchSymbol(char * label)
{
    symbol *ptr = head;
    while (ptr)
    {
        if (!strcmp(ptr->label, label))
        {
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}

/***********************************************************************************************/

symbol * getHeadSymbol()
{
    symbol * tmp = head;
    return tmp;
}


