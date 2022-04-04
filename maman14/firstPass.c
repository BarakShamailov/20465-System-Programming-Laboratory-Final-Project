#include "header.h"
/*This file contains the first pass function*/

/*The first pass function will go throgh each line.
It will first check for a label, then check for a directive.
if there's no directive, it will check for a command. It will add errors
to the errors list if found, and will go through the entire input file*/

void firstPass(FILE * f, int * IC, int * DC)
{

    char line[MAX_LINE];
    /*Line counter*/
    int lineCounter = 0;
    /*Line fetching loop*/
    while (fgets(line, MAX_LINE, f))
    {

        /*pointer to token*/
        char * token = NULL;
        /*pointer to label*/
        char * label  = NULL;
        /*directive flag*/
        int directive = 0;
        /*get first token*/
        token = strtok(line, " \t\n");

        /*increment line counter*/
        lineCounter++;

        /*if line is empty, go to next line*/
        if (!token)
        {
            continue;
        }
        /*if line is comment, go to next line*/
        if (isComment(token))
        {

            continue;
        }
        /*when we have label*/
        if ((label = getLabel(token)))
        {
            /*If getLabel returned error string*/
            if (!strcmp(label, "$"))
            {
                /*Add error*/
                addError("illegal label-> ",lineCounter,token);
                /*set label to null*/
                label = NULL;
                continue;
            }
            else if (!strcmp(label, "@"))
            {
                addError("reserved label-> ",lineCounter,token);
                continue;
            }
            /*Get next token.*/
            token = strtok(NULL, " \t\n");
            /*if next token is null, add error and go to next line*/
            if (!token)
            {
                addError("Must add something next to label-> ",lineCounter,label);
                continue;
            }
        }
        /*check if it is start of directive*/
        if (isDirective(token)) {
            /*get the type of directive*/
            directive = getDirective(token);

            /*if getDirective is not directive add error*/
            if (directive == DIRECTIVE_ERROR)
            {
                /*add error*/
                addError("Invalid directive -> ", lineCounter,token);
                continue;
            }
            /*if the directive is .asciz*/
            else if (directive == DIRECTIVE_STRING)
            {
                /*if we have label before*/

                if(label) {
                    /*we add the label to the symbol list*/
                    addSymbol(label, *DC, FALSE, FALSE, TRUE, FALSE, lineCounter);
                    if (!(token = strtok(NULL, " \n"))) {
                        /*if we dont have parameter we add error*/
                        addError(".asciz directive must have a string parameter", lineCounter, token);
                        continue;
                    }
                    if (isValidString(token, lineCounter)) {
                        addString(token, lineCounter, DC);

                        /*if there's a next token*/
                        if ((token = strtok(NULL, " \t\n"))) {
                            /*add error and go to next line*/
                            addError("asciz directive can only take one parameter", lineCounter, NULL);
                            continue;
                        }

                    }
                }else {

                        /*get next token*/
                        if (!(token = strtok(NULL, " \n"))) {
                            /*if we dont have parameter we add error*/
                            addError(".asciz directive must have a string parameter", lineCounter, token);
                            continue;
                        }
                        /*check if the string parameter is valid and add it to data list*/
                        if (isValidString(token, lineCounter)) {
                            addString(token, lineCounter, DC);

                            /*if there's a next token*/
                            if ((token = strtok(NULL, " \t\n"))) {
                                /*add error and go to next line*/
                                addError("asciz directive can only take one parameter", lineCounter, NULL);
                                continue;
                            }
                            /*if next token is null*/
                        }
                    }

            }
            /*if we have .dh directive*/
            else if (directive == DIRECTIVE_DH)
            {

                /*if we have a label*/
                if (label) {
                    /*check if we have parameter, if not we add error*/
                    if (!(token = strtok(NULL, "\n"))) {
                        addError(".dh directive must have a parameter", lineCounter, token);
                        continue;
                    }
                    /*add the label to the symbol list*/
                    addSymbol(label, *DC, FALSE, FALSE, TRUE, FALSE, lineCounter);
                    /*check if we have legal commas*/
                    if (checkCommas(token, lineCounter)) {
                        /*check if the parameter is legal*/
                        checkNumbers(token, lineCounter, FALSE, TRUE, FALSE, DC);
                        continue;
                    }

                }
                else
                {
                /*get next token*/
                if (!(token = strtok(NULL,"\n"))) {
                    addError("dh directive must have a parameter", lineCounter, NULL);
                    continue;
                }
                /*check if the commas are illegal*/
                if(checkCommas(token,lineCounter))
                    {

                        /*check the number parameter*/
                        checkNumbers(token,lineCounter,FALSE,TRUE,FALSE,DC);
                        continue;
                    }

                }

            }
            else if (directive == DIRECTIVE_DW)
            {
                /*if we have a label, print a warning*/
                if (label) {
                    /*check if we have parameter*/
                    if (!(token = strtok(NULL,"\n")))
                    {
                        addError("dw directive must have a  parameter",lineCounter,NULL);
                        continue;
                    }
                    /*add the label to the symbol list*/
                    addSymbol(label, *DC, FALSE, FALSE, TRUE, FALSE, lineCounter);
                    /*check the commas*/
                    if (checkCommas(token,lineCounter)) {
                          /*check the parameters*/
                        checkNumbers(token, lineCounter, FALSE, FALSE, TRUE, DC);
                        continue;
                    }

                }
                else{
                /*get next token , and check if we have parameter*/
                if (!(token = strtok(NULL,"\n")))
                {
                    addError("dw directive must have a  parameter ",lineCounter,NULL);
                    continue;
                }

                   /*check the commas*/
                    if(checkCommas(token,lineCounter))
                    {
                        /*check the parameters*/
                        checkNumbers(token,lineCounter,FALSE,FALSE,TRUE,DC);
                        continue;
                    }

               }
            }
            else if (directive == DIRECTIVE_DB)
            {

                /*if we have a label, print a warning*/
                if (label)
                {
                    /*check if there are parameters*/
                    if (!(token = strtok(NULL,"\n")))
                    {
                        addError("db directive must have a parameter ",lineCounter,NULL);
                        continue;
                    }
                    /*add the label to the symbol list*/
                    addSymbol(label, *DC, FALSE, FALSE, TRUE, FALSE, lineCounter);
                    /*check the commas*/
                    if (checkCommas(token,lineCounter)) {
                        /*check the parameters*/
                        checkNumbers(token, lineCounter, TRUE, FALSE, FALSE, DC);
                        continue;
                    }

                }

                else{
                /*get next token*/
                /*check if there are parameters*/
                if (!(token = strtok(NULL,"\n")))
                {
                    addError("db directive must have a parameter ",lineCounter,NULL);
                    continue;
                }
                /*check the commas*/
                if(checkCommas(token,lineCounter))
                {
                    /*check the parameters*/
                       checkNumbers(token,lineCounter,TRUE,FALSE,FALSE,DC);
                       continue;
                }
             }
            }
            else if (directive == DIRECTIVE_ENTRY)
            {

                /*if we have a label, print a warning*/
                if (label)
                    printf("Warning, line %d: label \"%s\" before entry directive is meaningless\n", lineCounter, label);
                /*get next token , check if we have parameters*/
                if (!(token = strtok(NULL, " \t\n")))
                {
                    addError("Entry directive must have a label parameter" ,lineCounter,NULL);
                    continue;
                }
                /*check if the label is legal*/
                if(isValidLabel(token,lineCounter))
                {
                    /*add the label to the symbol list*/
                    addSymbol(token, 0, FALSE, TRUE,FALSE,FALSE, lineCounter);
                    /*if there's a next token*/
                    if ((token = strtok(NULL, "\n")))
                    {
                        /*add error and go to next line*/
                        addError("Entry directive can only take one parameter ",lineCounter,NULL);
                        continue;
                    }

                }
                else
                {
                    /*go to next line*/
                    continue;
                }
            }
            /*if it is extern directive*/
            else if (directive == DIRECTIVE_EXTERN) {

                /*if there's a label, print a warning*/

                if (label)
                    printf("Warning, line %d: label \"%s\" before extern directive is meaningless\n", lineCounter,
                           label);
                /*if there are parameters if not add error*/
                if (!(token = strtok(NULL, " \t\n"))) {
                    addError("Extern directive must have a label parameter ",lineCounter,NULL);
                    continue;
                }
                /*if the label is legal*/
                if(isValidLabel(token,lineCounter))
                {
                    /*add extern symbol*/
                    addSymbol(token, 0, TRUE, FALSE,FALSE,FALSE, lineCounter);
                    token = strtok(NULL, "\n");
                    /*if there is next token*/
                    if (token) {
                        /*add error and go to next line*/
                        addError("Extern directive can only take one parameter ",lineCounter,NULL);
                        continue;
                    }
                    continue;


                }
                else {
                    /*go to next line*/
                    continue;
                }
            }

        }
        else
        {
            /*Analyze the instruction operation*/
            analyzeOperation(token, lineCounter, IC, label);
            continue;

        }

    }
    /*if the file is empty add error*/
    if (lineCounter == 0)
    {
        addError("ERROR: File is empty.", -1, NULL);
    }

}