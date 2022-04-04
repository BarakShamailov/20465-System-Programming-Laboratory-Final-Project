#include "header.h"

/******************************************************************************************/
/*this array handle the all reserved words*/
char *resrvedWords[27] = {"nor", "or", "and", "add",
                          "sub", "move", "mvhi", "mvlo", "addi",
                          "subi", "andi", "ori", "nori", "bne",
                          "beq", "blt", "bgt", "lb", "sb", "lw", "sw", "lh",
                          "sh", "jmp", "la", "call", "stop"};
/******************************************************************************************/
/*check if the token is valid label parameter
if there are errors we add them 
@param line is the number of the line
@param token is the string of the label
@return 1 if there is no error ,else 0 
*/
int isValidLabel(char *token, int line)
{
    char *ptr;
    ptr = token;
    /*check first if the token is reserved word*/
    if (isRegister(token) || isOperation(token))
    {
        addError("its reserved label or register ", line, token);
        return FALSE;
    }
    /*check if the first char is alpha*/
    if (!(isalpha(*ptr)))
    {
        addError("Illegal label ", line, ptr);
        return FALSE;
    }
    ptr++;
    while ((*ptr) != '\0')
    {
        /*check if the rest chars are digits or chars*/
        if (!(isalpha(*ptr)) && !(isdigit(*ptr)))
        {
            addError("Illegal label ", line, ptr);
            return FALSE;
        }
        ptr++;
    }
    return TRUE;
}

/******************************************************************************************/
/*check if the token is valid label before the command 
if we found error we mark it with sign
@param token is the label to be check
@return sign of type the error
*/
char *getLabel(char *token)
{

    char *ptr = token;
    /*if last char is ':'*/
    if (token[strlen(token) - 1] == ':')
    {
        /*if first letter is valid*/
        if (isalpha(*token))
        {
            /*set null terminator instead of ':'*/
            token[strlen(token) - 1] = 0;

            /*If there's a char which is not a digit or a letter, return an error flag*/
            while ((*ptr) != '\0')
            {
                if (!isalpha(*ptr))
                {
                    if (!isdigit(*ptr))
                    {
                        return "$";
                    }
                }
                ptr++;
            }
            /*If the token is an operation, return reserved word flag*/
            /*If the token is a register, return reserved word flag*/
            if (isOperation(token))
            {
                return "@";
            }
            if (isRegister(token))
            {
                return "@";
            }
            return token;
        }
        /*if first letter is invalid, return error flag*/
        else
        {
            return "$";
        }
    }
    /*if last letter is not ':', return null*/
    else
    {
        return NULL;
    }
}
/******************************************************************************************/
/*check if the parameter is register and convert it to number
@param token is the register
@param line is the number of line
@return number if the convertion succeeded,else -1
*/
int checkRegisters(char *token, int line)
{

    int i = 0;
    char numReg[3];
    int num1 = 0;
    /*check if the token is register*/
    if (isRegister(token))
    {
        int j = 1;
        while (token[i] != '\0')
        {
            numReg[i] = token[j];
            i++;
            j++;
        }
        /*convert the number of register to int*/
        num1 = atoi(numReg);
        return num1;
    }
    else
    {
        addError("illegal paramater, only registers parameters ($0 - $31)", line, token);
        return -1;
    }
}
/******************************************************************************************/
/*check if the string is valid
@param token is the string to be check
@param line is the number of line
@return 1 if the string is legal ,else 0
*/
int isValidString(char *token, int line)
{
    char *ptr = token;
    /*check if the first cher and the last cher are double quotes*/
    if (token[0] != '"' || token[strlen(token) - 1] != '"')
    {
        addError("Invalid string missing double quotes", line, token);
        return 0;
    }

    ptr++;
    while ((*ptr) != '"')
    {
        /*check if the rest chars are can be print */
        if (!(isprint(*ptr)))
        {
            addError("Invalid string ", line, token);
            break;
        }
        else
        {
            ptr++;
        }
    }
    return 1;
}

/******************************************************************************************/
/*check if the commas is llegal
 * for directive parameters 
 @param token the all parameters of the directive
 @param the number of the line
 @return 1 if the commas legal , else 0
 */
int checkCommas(char *token, int line)
{

    int i = 0;
    unsigned int len = strlen(token), j;
    int counterCommas = 0;
    int counterNum = 0;
    j = len - 1; /*the last index of the token*/
    /*we run in loop from the end until the start of the token
     * we ignore from white space if we found ',' we add error */
    while (j >= 0)
    {
        if (token[j] != ' ' && token[j] != ',')
        {
            break;
        }
        if (token[j] == ' ')
        {
            j--;
        }
        if (token[j] == ',')
        {
            addError("illegal comma", line, NULL);
        }

        j--;
    }

    /*we run until the end of the token and count the number of commas and the parameters*/
    while (i < len)
    {

        if (token[i] == ' ')
        {
            i++;
        }
        /*count the number parameters*/
        if (isdigit(token[i]))
        {
            while (TRUE)
            {
                if (token[i] == ',' || token[i] == ' ')
                {
                    break;
                }

                i++;
            }
            counterNum++;
        }

        if (token[i] == ',')
        {
            /*count the comma and check errors*/
            counterCommas++;
            if (counterNum > counterCommas)
            {
                addError("missing comma", line, token);
                return FALSE;
            }
            if (counterCommas == 1 && counterNum == 0)
            {
                addError("illegal comma ", line, NULL);

                return FALSE;
            }

            if (counterCommas - counterNum == 1)
            {
                addError("consective commas ", line, NULL);

                return FALSE;
            }
        }
        i++;
    }
    if (counterNum - counterCommas > 1)
    {
        addError("missing comma ", line, NULL);
        return FALSE;
    }

    return TRUE;
}
/******************************************************************************************/
/*check if we have memory
@param ptr a pointer to allocation
@return if not succeeded EXIT the program*/
void checkAllocation(void *ptr)
{
    if (!ptr)
    {
        printf("The program has quit due to a memory allocation error.\n");
        exit(0);
    }
}
/*****************************************************************************************************/
/*This function determines if we received a directive.
it will return an enum int of the proper directive, or an error flag
if there's no such directive. if the string doesn't start with ".",
it will return an enum defined as NONE, meaning it is not a directive*/
int getDirective(char *token)
{
    if (!strcmp(token, STR_DIRECTIVE_DH))
    {
        return DIRECTIVE_DH;
    }
    else if (!strcmp(token, STR_DIRECTIVE_DW))
    {
        return DIRECTIVE_DW;
    }
    else if (!strcmp(token, STR_DIRECTIVE_DB))
    {
        return DIRECTIVE_DB;
    }
    else if (!strcmp(token, STR_DIRECTIVE_ASCIZ))
    {
        return DIRECTIVE_STRING;
    }
    else if (!strcmp(token, STR_DIRECTIVE_ENTRY))
    {
        return DIRECTIVE_ENTRY;
    }
    else if (!strcmp(token, STR_DIRECTIVE_EXTERN))
    {
        return DIRECTIVE_EXTERN;
    }
    /*if string is unknown*/
    else
    {
        /*return error*/
        return DIRECTIVE_ERROR;
    }
}

/*****************************************************************************************************/
/*check if the token is start of directive*/
int isDirective(char *token)
{
    char *ptr = token;
    /*if first char is '.'*/
    if (*ptr == '.')
    {

        return TRUE;
    }
    /*if this isn't a directive*/
    else
    {
        return FALSE;
    }
}

/***********************************************************************************************/
/*check if the line is commit*/
int isComment(char *token)
{
    if (*token == ';')
        return 1;
    else
        return 0;
}

/***********************************************************************************************/
/*check if the token is operation word*/
int isOperation(char *token)
{
    int i;

    for (i = 0; i < 27; i++)
    {
        if (!strcmp(token, resrvedWords[i]))
        {
            return 1;
        }
    }
    return 0;
}

