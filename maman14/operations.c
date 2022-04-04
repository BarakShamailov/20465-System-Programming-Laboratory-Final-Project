#include "header.h"

/*array of all the commands and their sign,opcode and funct*/
static opcode opCodes[27] = {
    {"add", 0, 1, "R"}, {"sub", 0, 2, "R"}, {"and", 0, 3, "R"}, {"or", 0, 4, "R"}, {"nor", 0, 5, "R"}, {"move", 1, 1, "R"}, {"mvhi", 1, 2, "R"}, {"mvlo", 1, 3, "R"},
     {"addi", 10, 0, "I"}, {"subi", 11, 0, "I"}, {"andi", 12, 0, "I"}, {"ori", 13, 0, "I"}, {"nori", 14, 0, "I"}, {"bne", 15, 0, "I"}, {"beq", 16, 0, "I"}, {"blt", 17, 0, "I"}, {"bgt", 18, 0, "I"}, {"lb", 19, 0, "I"}, {"sb", 20, 0, "I"}, {"lw", 21, 0, "I"}, {"sw", 22, 0, "I"}, {"lh", 23, 0, "I"}, {"sh", 24, 0, "I"},
     {"jmp", 30, 0, "J"}, {"la", 31, 0, "J"}, {"call", 32, 0, "J"}, {"stop", 63, 0, "J"}};
     
/*array of all the registers*/
static char *registers[NUM_OF_REGISTERS] =
    {"$0", "$1", "$2", "$3", "$4", "$5", "$6", "$7",
     "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15",
     "$16", "$17", "$18", "$19", "$20", "$21", "$22", "$23",
     "$24", "$25", "$26", "$27", "$28", "$29", "$30", "$31"};

/*This function will analyze every operation , check the parameters and commas if all ok we create a new word node
@param token -  a string with all the parameters of the operand.
@param line - the number of line.
@param IC - intersuction counter.
@param label - if we have label before the operand*/
void analyzeOperation(char *token, int line, int *IC, char *label)
{
    int i = 0;
    int increment_value_IC = 0;
    wordNode *first = NULL;
    wordNode *second = NULL;
    wordNode *third = NULL;
    /*if there is label before we add it to the symbol list*/
    if (label)
    {
        addSymbol(label, *IC, FALSE, FALSE, FALSE, TRUE, line);
    }
    /*we run in loop and check if the token is same operand in the array opcodes */
    for (i = 0; i < OPCODES_AMOUNT; i++)
    {
        /*check if the perand we got is in our assembler */
        if (!strcmp(token, opCodes[i].str))
        {
            /**************************               R group           **********************************************/
            if (!(strcmp(opCodes[i].group, "R")))
            {
                /*operands add sub and or nor*/

                if (opCodes[i].code == 0)
                {
                    /*if there is any parameter*/
                    if (!(token = strtok(NULL, "\n")))
                    {
                        addError("missing parameters , must be parameters after the operand ", line, NULL);
                        i = OPCODES_AMOUNT;
                    }
                    else
                    {
                        /*check the commas*/
                        if (checkCommasOperation(token, line))
                        {
                            /*start analyze if we got legal parameters and the amount of them*/
                            int reg1 = 0; /*rs*/
                            int reg2;     /*rt*/
                            int reg3;     /*rd*/
                            char *ptr = token;
                            /*check if there are parameters*/
                            if (!(ptr = strtok(token, " ,")))
                            {
                                addError("missing parameter , must be 3 parameters ", line, NULL);
                                return;
                            }
                            /*check if the register is legal and convert him to int*/
                            reg1 = checkRegisters(ptr, line);
                            /*check if there are parameters*/
                            if (!(ptr = strtok(NULL, " ,")))
                            {
                                addError("missing parameter , must be 3 parameters ", line, NULL);
                                return;
                            }
                            /*check if the register is legal and convert him to int*/
                            reg2 = checkRegisters(ptr, line);

                            if (!(ptr = strtok(NULL, " ,")))
                            {
                                addError("missing parameter , must be 3 parameters ", line, NULL);
                                return;
                            }
                            /*check if the register is legal and convert him to int*/
                            reg3 = checkRegisters(ptr, line);
                            /*check if there are parameters*/
                            if ((ptr = strtok(NULL, " ,")))
                            {
                                addError("extra parameter , must be 3 parameters ", line, NULL);
                                return;
                            }
                            /*after the all checks we can to add */

                            first = malloc(sizeof(wordNode));
                            checkAllocation(first);
                            /*Set all of first word's parameters*/
                            first->method = REG;
                            first->isExternal = FALSE;
                            first->missingLabel = NULL;
                            first->externalLabel = NULL;
                            first->line = line;
                            first->holder.R.unused = 0;
                            first->holder.R.funct = opCodes[i].funct;
                            first->holder.R.rd = reg3;
                            first->holder.R.rt = reg2;
                            first->holder.R.rs = reg1;
                            first->holder.R.opcode = opCodes[i].code;
                            first->next = NULL;
                            /*Add 4 to the increment value*/
                            increment_value_IC += 4;
                        }
                        else
                        {
                            return;
                        }
                    }
                }
                else
                {
                    /*operands move mvhi mvlo*/

                    if (!(token = strtok(NULL, "\n")))
                    {
                        addError("missing parameters after the operand ", line, NULL);
                        i = OPCODES_AMOUNT;
                    }
                    else
                    {
                        /*check commas*/
                        if (checkCommas(token, line))
                        {
                            /*start analyze if we got legal parameters and the amount of them*/
                            int reg1;
                            int reg2;
                            char *ptr = token;
                            /*check if there are parameters*/
                            if (!(ptr = strtok(token, " ,")))
                            {
                                addError("missing parameter , must be 2 parameters ", line, NULL);
                                return;
                            }
                            /*check if the register is legal and convert him to int*/
                            reg1 = checkRegisters(ptr, line);
                            /*check if there are parameters*/
                            if (!(ptr = strtok(NULL, " ,")))
                            {
                                addError("missing parameter , must be 2 parameters ", line, NULL);
                                return;
                            }
                            /*check if the register is legal and convert him to int*/
                            reg2 = checkRegisters(ptr, line);
                            /*check if there are parameters*/
                            if ((ptr = strtok(NULL, " ,")))
                            {
                                addError("extra parameter , must be 2 parameters ", line, NULL);
                                return;
                            }

                            first = malloc(sizeof(wordNode));
                            checkAllocation(first);
                            /*Set all of first word's parameters*/
                            first->method = REG;
                            first->isExternal = FALSE;
                            first->missingLabel = NULL;
                            first->externalLabel = NULL;
                            first->line = line;
                            first->holder.R.unused = 0;
                            first->holder.R.funct = opCodes[i].funct;
                            first->holder.R.rd = reg2;
                            first->holder.R.rt = 0;
                            first->holder.R.rs = reg1;
                            first->holder.R.opcode = opCodes[i].code;
                            first->next = NULL;
                            /*Add 4 to the increment value*/
                            increment_value_IC += 4;
                        }
                        else
                        {
                            return;
                        }
                    }
                }
            }
            /**************************               I group           **********************************************/
            else if (!(strcmp(opCodes[i].group, "I")))
            {
                if (opCodes[i].code >= 10 && opCodes[i].code <= 14)
                {

                    /*operands addi subi andi ori nori*/
                    if (!(token = strtok(NULL, "\n")))
                    {
                        addError("missing parameters , must be parameters after the operand ", line, NULL);
                        i = OPCODES_AMOUNT;
                    }
                    else
                    {
                        /*check if the commas legal*/
                        if (checkCommasOperation(token, line))
                        {
                            /*start analyze if we got legal parameters and the amount of them*/
                            int reg1 = 0;
                            long int immed;
                            int reg2;
                            char *tmp = NULL;
                            char *ptr = token;
                            /*if we dont have parameters*/
                            if (!(ptr = strtok(token, " ,")))
                            {
                                addError("missing parameter , must be 3 parameters ", line, NULL);
                                return;
                            }

                            /*check if the parameter is register*/
                            reg1 = checkRegisters(ptr, line);

                            if (!(ptr = strtok(NULL, " ,")))
                            {
                                addError("missing parameter , must be 3 parameters ", line, NULL);
                                return;
                            }
                            /*convert from string to int the immed parameter*/
                            immed = strtol(ptr, &tmp, 10);
                            /*if the number out of range*/
                            if (immed > INT_MAX || immed < INT_MIN)
                            {
                                addError("Out of range number ", line, NULL);
                                return;
                            }
                            if (immed == 0)
                            {
                                addError("illegal value for immed ,only number parameter ", line, NULL);
                                return;
                            }
                            /*check if there are parameters*/
                            if (!(ptr = strtok(NULL, " ,")))
                            {
                                addError("missing parameter , must be 3 parameters ", line, NULL);
                                return;
                            }
                            /*check if the parameter is register*/
                            reg2 = checkRegisters(ptr, line);
                            /*check if there are parameters*/
                            if ((ptr = strtok(NULL, " ,")))
                            {
                                addError("extra parameter , must be 3 parameters ", line, NULL);
                                return;
                            }
                            /*add node to the word list*/
                            second = malloc(sizeof(wordNode));
                            checkAllocation(second);
                            /*Set all of second word's parameters*/
                            second->method = IMMED;
                            second->isExternal = FALSE;
                            second->missingLabel = NULL;
                            second->externalLabel = NULL;
                            second->line = line;
                            second->holder.I.immed = (int)immed;
                            second->holder.I.rt = reg2;
                            second->holder.I.rs = reg1;
                            second->holder.I.opcode = opCodes[i].code;
                            second->next = NULL;
                            /*Add 4 to the increment value*/
                            increment_value_IC += 4;
                        }
                        else
                        {
                            return;
                        }
                    }
                }
                if (opCodes[i].code >= 15 && opCodes[i].code <= 18)
                {

                    /***** bne beq blt bgt    ******/
                    if (!(token = strtok(NULL, "\n")))
                    {
                        addError("missing parameters , must be parameters after the operand ", line, NULL);
                        i = OPCODES_AMOUNT;
                    }
                    else
                    {
                        if (checkCommasOperation(token, line))
                        {
                            /*start analyze if we got legal parameters and the amount of them*/
                            char *tmpString;
                            int reg1 = 0;
                            int reg2;
                            char *ptr = token;
                            /*check if there are parameters*/
                            if (!(ptr = strtok(token, " ,")))
                            {
                                addError("missing parameter , must be 3 parameters\n", line, NULL);
                                return;
                            }

                            /*check if the parameter is register*/
                            reg1 = checkRegisters(ptr, line);

                            /*check if there are parameters*/
                            if (!(ptr = strtok(NULL, " ,")))
                            {
                                addError("missing parameter , must be 3 parameters\n", line, NULL);
                                return;
                            }
                            /*check if the parameter is register*/
                            reg2 = checkRegisters(ptr, line);

                            /*check if there are parameters*/
                            if (!(ptr = strtok(NULL, " ,")))
                            {
                                addError("missing parameter , must be 3 parameters ", line, NULL);
                                return;
                            }
                            /*check if the parameter is legal label*/
                            if (!(isValidLabel(ptr, line)))
                            {
                                addError("illegal label parameter for immed value ", line, NULL);
                            }
                            tmpString = ptr;

                            /*check if there are parameters*/
                            if ((ptr = strtok(NULL, " ,")))
                            {
                                addError("extra parameter , must be 3 parameters ", line, NULL);
                                return;
                            }

                            /*add node the word list*/
                            second = malloc(sizeof(wordNode));
                            checkAllocation(second);
                            /*Set all of second word's parameters*/
                            second->method = IMMED;
                            second->isExternal = FALSE;
                            second->missingLabel = malloc(sizeof(char) * MAX_LINE);
                            second->externalLabel = NULL;
                            strcpy(second->missingLabel, tmpString);
                            second->line = line;
                            second->holder.I.immed = 0; /*we dont know yet the value*/
                            second->holder.I.rt = reg2;
                            second->holder.I.rs = reg1;
                            second->holder.I.opcode = opCodes[i].code;
                            second->next = NULL;
                            /*Add 4 to the increment value*/
                            increment_value_IC += 4;
                        }
                        else
                        {
                            return;
                        }
                    }
                }
                if (opCodes[i].code >= 19 && opCodes[i].code <= 24)
                {
                    /************ lb , sb , lw , sw , lh ,sh  ***************/
                    if (!(token = strtok(NULL, "\n")))
                    {
                        addError("missing parameters , must be parameters after the operand ", line, NULL);
                        i = OPCODES_AMOUNT;
                    }
                    else
                    {
                        /*start analyze if we got legal parameters and the amount of them*/
                        if (checkCommasOperation(token, line))
                        {

                            int reg1 = 0;
                            long int immed;
                            int reg2;
                            char *tmp = NULL;
                            char *ptr = token;
                            /*check if there are parameters*/
                            if (!(ptr = strtok(token, " ,")))
                            {
                                addError("missing parameter , must be 3 parameters ", line, NULL);
                                return;
                            }

                            reg1 = checkRegisters(ptr, line);

                            /*check if there are parameters*/
                            if (!(ptr = strtok(NULL, " ,")))
                            {
                                addError("missing parameter , must be 3 parameters ", line, NULL);
                                return;
                            }
                            /*we convert the number to int from string*/
                            immed = strtol(ptr, &tmp, 10);
                            ;
                            if (immed > INT_MAX || immed < INT_MIN)
                            {
                                addError("Out of range number ", line, NULL);
                            }
                            if (immed == 0)
                            {
                                addError("illegal value for immed ,only number parameter ", line, NULL);
                                return;
                            }

                            /*check if there are parameters*/
                            if (!(ptr = strtok(NULL, " ,")))
                            {
                                addError("missing parameter , must be 3 parameters ", line, NULL);
                                return;
                            }

                            reg2 = checkRegisters(ptr, line);
                            /*check if there are parameters*/
                            if ((ptr = strtok(NULL, " ,")))
                            {
                                addError("extra parameter , must be 3 parameters ", line, NULL);
                                return;
                            }

                            second = malloc(sizeof(wordNode));
                            checkAllocation(second);
                            /*Set all of second word's parameters*/
                            second->method = IMMED;
                            second->isExternal = FALSE;
                            second->missingLabel = NULL;
                            second->externalLabel = NULL;
                            second->line = line;
                            second->holder.I.immed = (int)immed;
                            /*we dont know yet the value of address*/
                            second->holder.I.rt = reg2;
                            second->holder.I.rs = reg1;
                            second->holder.I.opcode = opCodes[i].code;
                            second->next = NULL;
                            /*Add 4 to the increment value*/
                            increment_value_IC += 4;
                        }
                        else
                        {
                            return;
                        }
                    }
                }
            }
            /**************************               J group           **********************************************/
            else if (!(strcmp(opCodes[i].group, "J")))
            {
                /************  jmp ***************/
                if (opCodes[i].code == 30)
                {
                    /*start analyze if we got legal parameters and the amount of them*/
                    char *check = NULL;

                    if (!(token = strtok(NULL, " \n")))
                    {
                        addError("missing parameters , must be parameters after the operand ", line, NULL);
                        i = OPCODES_AMOUNT;
                    }
                    else
                    {

                        char *ptr = token;
                        /*check if the parameter is register or label*/
                        if (!(isRegister(token)) && !isValidLabel(token, line))
                        {
                            addError("Illegal parameter to jmp operand ,should be label or register ", line, NULL);
                            return;
                        }

                        check = strtok(NULL, " ");
                        /*check if there are parameters*/
                        if (check)
                        {
                            addError("extra parameter should be one ", line, NULL);
                            return;
                        }
                        if (isRegister(ptr))
                        {
                            /*if the parameter is register we add new node to the wordlist */

                            third = malloc(sizeof(wordNode));
                            checkAllocation(third);
                            /*Set all of third word's parameters*/
                            third->method = ADDRESS;
                            third->isExternal = FALSE;
                            third->missingLabel = NULL;
                            third->externalLabel = NULL;
                            third->line = line;
                            third->holder.J.address = checkRegisters(ptr, line); /*we dont know yet the value*/
                            third->holder.J.reg = 1;
                            third->holder.J.opcode = opCodes[i].code;
                            third->next = NULL;
                            /*Add 4 to the increment value*/
                            increment_value_IC += 4;
                        }
                        else
                        {
                            /*if the parameter is label we add new node to the wordlist */
                            third = malloc(sizeof(wordNode));
                            checkAllocation(third);
                            /*Set all of third word's parameters*/
                            third->method = ADDRESS;
                            third->isExternal = FALSE;
                            third->missingLabel = malloc(sizeof(char) * MAX_LINE);
                            third->externalLabel = NULL;
                            strcpy(third->missingLabel, ptr);
                            third->line = line;
                            /*we dont know yet the value of the address*/
                            third->holder.J.reg = 0;
                            third->holder.J.opcode = opCodes[i].code;
                            third->next = NULL;
                            /*Add 4 to the increment value*/
                            increment_value_IC += 4;
                        }
                    }
                }
                else if (opCodes[i].code == 31)
                {
                    /************  la ***************/
                    /*check if there are parameters*/
                    if (!(token = strtok(NULL, " \n")))
                    {
                        addError("missing parameters , must be parameter after the operand ", line, NULL);
                        i = OPCODES_AMOUNT;
                    }
                    else
                    {

                        char *check;
                        char *ptr = token;
                        /*start analyze if we got legal parameters and the amount of them*/
                        if (!isValidLabel(token, line))
                        {
                            addError("Illegal parameter to jmp operand ,should be label ", line, NULL);
                            return;
                        }
                        /*check if there are parameters*/
                        check = strtok(NULL, " ");

                        if (check)
                        {
                            addError("extra parameter should be one ", line, NULL);
                            return;
                        }
                        third = malloc(sizeof(wordNode));
                        checkAllocation(third);
                        /*Set all of third word's parameters*/
                        third->method = ADDRESS;
                        third->isExternal = FALSE;
                        third->missingLabel = malloc(sizeof(char) * MAX_LINE);
                        third->externalLabel = NULL;
                        strcpy(third->missingLabel, ptr);
                        third->line = line;
                        /*we dont know yet the value of the address*/
                        third->holder.J.reg = 0;
                        third->holder.J.opcode = opCodes[i].code;
                        third->next = NULL;
                        /*Add 4 to the increment value*/
                        increment_value_IC += 4;
                    }
                }
                else if (opCodes[i].code == 32)
                {
                    /************  call ***************/
                    /*start analyze if we got legal parameters and the amount of them*/
                    if (!(token = strtok(NULL, " \n")))
                    {
                        /*check if there are parameters*/
                        addError("missing parameters , must be parameter after the operand ", line, NULL);
                        i = OPCODES_AMOUNT;
                    }
                    else
                    {

                        char *check;
                        char *ptr = token;
                        if (!isValidLabel(token, line))
                        {
                            addError("Illegal parameter to jmp operand ,should be label ", line, NULL);
                            return;
                        }
                        /*check if there are parameters*/
                        check = strtok(NULL, " ");

                        if (check)
                        {
                            addError("extra parameter should be one ", line, NULL);
                            return;
                        }

                        /*add a new node to the wordlist*/
                        third = malloc(sizeof(wordNode));
                        checkAllocation(third);
                        /*Set all of third word's parameters*/
                        third->method = ADDRESS;
                        third->isExternal = FALSE;
                        third->missingLabel = malloc(sizeof(char) * MAX_LINE);
                        third->externalLabel = NULL;
                        strcpy(third->missingLabel, ptr);
                        third->line = line;
                        third->holder.J.reg = 0;
                        third->holder.J.opcode = opCodes[i].code;
                        third->next = NULL;
                        /*Add 4 to the increment value*/
                        increment_value_IC += 4;
                    }
                }
                else if (opCodes[i].code == 63)
                {
                    /************  stop ***************/
                    /*if we have parameter add error*/
                    if ((token = strtok(NULL, " \t")))
                    {
                        addError("extra parameter ", line, NULL);
                        return;
                    }
                    else
                    {
                        /*add new node to wordlist*/
                        third = malloc(sizeof(wordNode));
                        checkAllocation(third);
                        /*Set all of third word's parameters*/
                        third->method = ADDRESS;
                        third->isExternal = FALSE;
                        third->missingLabel = NULL;
                        third->externalLabel = NULL;
                        third->line = line;
                        third->holder.J.address = 0; /*we dont know yet the value*/
                        third->holder.J.reg = 0;
                        third->holder.J.opcode = opCodes[i].code;
                        third->next = NULL;
                        /*Add 4 to the increment value*/
                        increment_value_IC += 4;
                    }
                }
                else
                {
                    /*if we got invalid operand*/
                    addError("illegal instruction ", line, NULL);
                    return;
                }
            }
        }
    }

    /*Add each word (as needed) to the words list*/
    if (first)
        addWordNode(first);
    if (second)
        addWordNode(second);
    if (third)
        addWordNode(third);
    /*Increment the IC as needed*/
    *IC += increment_value_IC;
}

/*******************************************************************************************/
/*check if the token is register*/
int isRegister(char *token)
{
    int i;
    if (strlen(token) == 2 || strlen(token) == 3)
    {
        /*if the first char is $*/
        if (token[0] == '$')
        {
            /*check if the token is register*/
            if (isdigit(token[1]))
            {
                for (i = 0; i < NUM_OF_REGISTERS; i++)
                {
                    if (!strcmp(token, registers[i]))
                    {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

/*******************************************************************************************************/
/*this function check if the commas are legal for operands with 3 parameters*/
int checkCommasOperation(char *token, int line)
{
    int counterComma = 0;
    int counterParameters = 0;
    int i = 0;
    unsigned int len = strlen(token), j;
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

    /*we run untill the end of the token and count the parameters and commas*/
    while (i < len)
    {

        if (token[i] == ' ')
        {
            i++;
        }
        /*count the parameters*/
        if (token[i] != ' ' && token[i] != '\t' && token[i] != ',')
        {
            while (TRUE)
            {

                if (token[i] == ' ' || token[i] == '\t' || token[i] == ',')
                {
                    break;
                }

                i++;
            }
            counterParameters++;
        }

        if (token[i] == ',')
        {
            /*count commas and start check errors*/
            counterComma++;
            if (counterParameters > counterComma)
            {
                addError("missing comma ", line, NULL);
                return FALSE;
            }

            if (counterComma - counterParameters == 1)
            {
                addError("consective commas", line, NULL);

                return FALSE;
            }
            if (counterComma == 1 && counterParameters == 0)
            {
                addError("illegal comma ", line, NULL);

                return FALSE;
            }
        }

        i++;
    }

    if (counterParameters != 3 && counterComma != 2)
    {
        addError("missing comma", line, NULL);
        return FALSE;
    }

    return TRUE;
}
