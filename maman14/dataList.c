#include "header.h"

static data *head = NULL;

/*This function check every parameter if is legal
 * and send him addNumer function.
 @param token its a string of all the parameters
 @param line the number of the line
 @param db if its db type
 @param dw if its dw type
 @param dh if its dh type
 @param DC the data counter */
void checkNumbers(char *token, int line, int db, int dh, int dw, int *DC)
{
    int i = 1;
    char *ptr = token;
    /*split every parameter by ','*/
    ptr = strtok(token, " ,\n");

    while (TRUE)
    {

        /*check if the first char is '+' or '-' or digit , if not add error*/
        if (ptr[0] == '+' || ptr[0] == '-' || isdigit(ptr[0]))
        {

            while (i < strlen(ptr))
            {

                if (!(isdigit(ptr[i])))
                {
                    addError("illegal paramater ", line, ptr);
                    return;
                }
                i++;
            }
        }
        else
        {
            addError("illegal paramater ", line, ptr);
            return;
        }
        /*if the check is pass we convert to the paraneter to int*/
        addNumber(ptr, db, dw, dh, line, DC);
        /*if we dont have more parameters we came to the end of the token and stop*/
        if (!(ptr = strtok(NULL, " ,\n")))
        {

            break;
        }
    }
}

/************************************************************************************************************/
/*This function get a number parameter and convert him from string to long int by strtol.
* we check if the parameter in the range values if yes we create a data node.
@param number -  the number parameter in string type
@param db if its db type
@param dw if its dw type
@param dh if its dh type
@param line the number of the line
@param DC the data counter */
void addNumber(char *number, int db, int dw, int dh, int line, int *DC)
{
    char *ptr = NULL;
    long int tmpNum;
    data *tmp = NULL;
    /*convert by strtol from string to int*/
    tmpNum = strtol(number, &ptr, 10);
    /*if it is not succeed we add error*/
    if (*ptr != '\0')
    {
        /*add an error and exit function*/
        addError("Invalid number ", line, NULL);
        return;
    }
    if (db)
    {
        /*if the number is in the range */
        if (tmpNum < SCHAR_MIN || tmpNum > SCHAR_MAX)
        {
            addError("The Number is out of range ", line, NULL);
            return;
        }
        tmp = malloc(sizeof(data));
        checkAllocation(tmp);
        /*put the type of the data*/
        tmp->type = DIRECTIVE_DB;
        tmp->dataCell.dbData.value = (char)tmpNum;

        tmp->next = NULL;
        /*if the list is empty*/
        if (!head)
        {
            /*make head point to temp*/
            head = tmp;
        }
        /*if list is not empty*/
        else
        {
            /*pointer to head*/
            data *p = head;
            /*make p point to last node*/
            while (p->next)
                p = p->next;
            /*make tmp the last node*/
            p->next = tmp;
        }
        (*DC)++; /*add 1 to dc*/
    }
    if (dw)
    {
        /*if the number is in the range */
        if (tmpNum < INT_MIN || tmpNum > INT_MAX)
        {
            addError("The Number is out of range ", line, NULL);
            return;
        }
        /*make a new node of data*/
        tmp = malloc(sizeof(data));
        checkAllocation(tmp);
        /*put the type of the data*/
        tmp->type = DIRECTIVE_DW;
        tmp->dataCell.dwData.value = (long)tmpNum;

        tmp->next = NULL;
        /*if the list is empty*/
        if (!head)
        {
            /*make head point to temp*/
            head = tmp;
        }
        /*if list is not empty*/
        else
        {
            /*pointer to head*/
            data *p = head;
            /*make p point to last node*/
            while (p->next)
                p = p->next;
            /*make tmp the last node*/
            p->next = tmp;
        }

        (*DC) += 4; /*add 4 to dc*/
    }
    if (dh)
    {
        /*if the number is in the range */
        if (tmpNum < SHRT_MIN || tmpNum > SHRT_MAX)
        {
            addError("The Number is out of range ", line, NULL);
            return;
        }
        tmp = malloc(sizeof(data));
        checkAllocation(tmp);
        /*put the type of the data*/
        tmp->type = DIRECTIVE_DH;
        tmp->dataCell.dhData.value = (short int)tmpNum;

        tmp->next = NULL;
        /*if the list is empty*/
        if (!head)
        {
            /*make head point to temp*/
            head = tmp;
        }
        /*if list is not empty*/
        else
        {
            /*pointer to head*/
            data *p = head;
            /*make p point to last node*/
            while (p->next)
                p = p->next;
            /*make tmp the last node*/
            p->next = tmp;
        }
        (*DC) += 2; /*add 2 to dc*/
    }
}
/************************************************************************************************************/
/*This function update the address of every data node*/
void updateDataAddresses(int IC)
{

    /*pointer to head*/
    data *ptr;
    ptr = head;
    /*if head is null, return*/
    if (!ptr)
    {
        return;
    }
    /*go through every node and assign it an address*/
    while (ptr)
    {
        if (ptr->type == DIRECTIVE_DB)
        {
            ptr->address = IC;
            IC++;
        }
        if (ptr->type == DIRECTIVE_DH)
        {
            ptr->address = IC;
            IC += 2;
        }
        if (ptr->type == DIRECTIVE_DW)
        {
            ptr->address = IC;
            IC += 4;
        }
        if (ptr->type == DIRECTIVE_STRING)
        {
            ptr->address = IC;
            IC++;
        }
        ptr = ptr->next;
    }
}
/************************************************************************************************************/

/*This function free each data node*/
void freeData()
{
    /*temp pointer to head*/
    data *ptr = head;

    /*if head is null, return*/
    if (!ptr)
        return;
    /*free each data node*/
    while (ptr)
    {

        data *tmp = ptr;

        ptr = ptr->next;
        free(tmp);
    }
    /*make the head pointer null*/
    head = NULL;
}
/*****************************************************************************************************************/
/*return pointer to head. used when exporting files*/
data *getHeadData()
{
    data *tmp = head;
    return tmp;
}
/************************************************************************************************************/
/*add every char from the string to data node*/
void addChar(char c)
{

    /*temporary data node*/

    data *tmp = malloc(sizeof(data));
    checkAllocation(tmp);
    /*put the type of the data*/
    tmp->type = DIRECTIVE_STRING;
    /*Assign node with character and set next to NULL*/
    tmp->dataCell.ascizData.value = c;
    tmp->next = NULL;
    /*if list is empty*/
    if (!head)
    {
        /*make head point to temp*/
        head = tmp;
    }
    /*if list is not empty*/
    else
    {
        /*pointer to head*/
        data *p = head;
        /*make p point to last node*/
        while (p->next)
            p = p->next;
        /*make tmp the last node*/
        p->next = tmp;
    }
}
/*****************************************************************************************************************/

/*This function got the string and add it to data list , every char is data node*/
void addString(char *string, int line, int *DC)
{

    string[strlen(string) - 1] = '\0';
    string++;
    /*we run in loop until the end of the string*/
    while (*string != '\0')
    {
        /*add the character*/
        addChar(*string);

        /*point string to next char*/
        string++;
        /*increment data count*/
        (*DC)++;
    }
    /*add null terminator*/
    addChar(0);
    /*increment data count*/
    (*DC)++;
}

/*****************************************************************************************************************/

/*This functions counts the number of nodes in the data list*/
int countData()
{
    /*Temp counter variable*/
    int counter = 0;
    /*pointer to head*/
    data *ptr = head;
    /*if head is null, return counter (0)*/
    if (!ptr)
        return counter;

    /*go through the list and count each node according to his type*/
    while (ptr)
    {
        if (ptr->type == DIRECTIVE_DB)
        {
            counter++;
        }
        if (ptr->type == DIRECTIVE_DH)
        {
            counter += 2;
        }
        if (ptr->type == DIRECTIVE_DW)
        {
            counter += 4;
        }
        if (ptr->type == DIRECTIVE_STRING)
        {
            counter++;
        }

        ptr = ptr->next;
    }
    /*Return the number of nodes*/
    return counter;
}
