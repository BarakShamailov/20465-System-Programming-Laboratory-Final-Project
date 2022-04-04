#include "header.h"
/*This function update the empty parameters we cant fill in the first pass*/
void secondPass(int IC) {
    /*Temporary word node*/
    wordNode *tmpWordNode = NULL;

    /*Temporary symbol node*/
    symbol *tmpSymbol = NULL;
    while ((tmpWordNode = getMissingLabel()))
    {
        /*Search for a symbol in the symbols list with the same address*/
        tmpSymbol = searchSymbol(tmpWordNode->missingLabel);
        /*If no such symbol was found, add an error*/
        if(!tmpSymbol)
        {
            char *tmpString;
            tmpString = malloc(sizeof(char)*MAX_LINE);
            strcpy(tmpString, "Label \"");
            strcat(tmpString, tmpWordNode->missingLabel);
            strcat(tmpString, "\" was not declared");
            addError(tmpString, tmpWordNode->line, NULL);
        }
            /*If the symbol was found*/
        else
        {
            /*If it's external, change the word accordingly*/
            if (tmpSymbol->external)
            {
                checkSymbol(tmpWordNode->missingLabel,tmpWordNode->method,tmpWordNode->line);
                tmpWordNode->holder.J.address = 0;
                tmpWordNode->isExternal = TRUE;
                tmpWordNode->externalLabel = tmpSymbol->label;
            }
            else
            {

                /*got the address*/
                long address = checkSymbol(tmpWordNode->missingLabel,tmpWordNode->method,tmpWordNode->line);
                /*update the value*/
                if(address > 0) {
                    if (tmpWordNode->method == ADDRESS)
                     {
                       tmpWordNode->holder.J.address = address;
                     }
                    if (tmpWordNode->method == IMMED)
                    {     
                        /*check if the value is out of range*/
                         if(address - tmpWordNode->address < INT_MIN || address - tmpWordNode->address > INT_MAX)
                         {
                             addError("the value of immed is out of range",tmpWordNode->line,NULL);
                         }
                         else
                         {
                         tmpWordNode->holder.I.immed = address - tmpWordNode->address;
                         }
                    }

                }

            }

        }
        tmpWordNode->missingLabel = NULL;
    }

}
