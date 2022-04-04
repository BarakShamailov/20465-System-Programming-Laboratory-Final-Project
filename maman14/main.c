#include "header.h"

/* this function manage the assembler . first we check if we got files if there is not any file we print error ,
 * later we check the type files if they are wrong we print to user error, if all valid we going to first pass update the address where they missing
 * if we got any error we export error file if not we export the ob,ext,ent files
 * */
int main(int argc, char *argv[])
{
    /*check if we got files*/
    if (argc == 1)
    {
        
        printf("\nError: missing files\n");
    }
    else
    {

        int i ;
        for (i=1; i < argc; i++)
        {


            char *fileType;
            char *fileName  = strtok(argv[i], ".");
            /*if we got file without his type*/           
           if(!(fileType = strtok(NULL, ".")))
            {
                printf("\nError: missing type file\n");
                return 0;
            }
            /*if the type file is valid*/
            if (strcmp(fileType, AS_EXTENSION))
            {
                /*when we got invaild file type*/
                printf("\nError: invaild type file - \"%s\" , only .as files.\n", fileName);
            }
            else
            {
                /*start the two passes */
                FILE *f;
                int DC = 0, IC = 100;

                strcat(fileName, ".as");

                if (!(f = fopen(fileName, "r")))
                {
                    printf("\nError : can't read the file %s\n", fileName);


                    return 0;
                }
                else
                {
                    /*start the first pass*/
                    firstPass(f,&IC,&DC);
                    /*update the addresses*/
                    updateSymbols(IC);
                    updateAddresses();
                    updateDataAddresses(IC);
                    /*start the second pass*/
                    secondPass(IC);
               
                    /*when we dont have any error , we export the ob,ent,ext files*/
                    if(!isError())
                    {
                        fileName = strtok(fileName,".");
                        exportFiles(IC,DC,fileName);

                    }
                    /*if we have any error we export file error*/
                    else {
                        printErrors(fileName);
                        /*Free the errors list*/
                        freeErrors();
                        
                    }
                    /*Cleanup*/
                    freeData();
                    freeSymbols();
                    freeWords();

                }

            }
        }
    }
    return 1;
}
