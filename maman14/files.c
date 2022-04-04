#include "header.h"

/*openInputFile function - used to open files.
  takes 3 strings - filename, mode and extension.
  returns a pointer to the opened file, or null if failed*/
FILE *openFile(char *filename, char *mode, char *extension)
{
    /*Temp file to be returned*/
    FILE *tmp = NULL;
    /*Full filename string*/
    char full_filename[MAX_FILENAME];
    /*Copy the filename from argv to the full filename string*/
    strcpy(full_filename, filename);
    /*Add the .as extension to the full filename.*/
    strcat(full_filename, extension);
    /*Open the file. if fopen return null, exit the function and print error to screen*/
    if (!(tmp = fopen(full_filename, mode)))
    {
        printf("Can't open file %s.\n", full_filename);
        return NULL;
    }
    /*Return pointer to the file*/
    return tmp;
}
void exportFiles(int IC, int DC, char *name)
{
    /*FILE pointers*/

    FILE *object = NULL;
    FILE *entries = NULL;
    FILE *externs = NULL;
    /*Word holder*/

    /*Extern flag*/
    int nExtern = FALSE;

    long addressOb;
    int counter = 0, i;
    /*Pointer to wordNode*/
    wordNode *p = NULL;
    /*Pointer to entry*/
    symbol *pe = NULL;
    /*Pointer to data*/
    data *pd = NULL;
    /*open an object file in write mode*/
    object = openFile(name, "w", OB_EXTENSION);
    /*assign wordNode pointer with wordNode head*/
    p = getHeadWord();
    /*print to the file ob the number of data and instruction*/
    fprintf(object, "\t%d %d\n", countWords(), countData());
    /**start export the word list**/
    while (p)
    {
        /*according to word type we send to function import binary
         * the function print the word in little endian*/
        if (p->method == IMMED)
        {
            importBinary(IMMED, p, object);
        }
        if (p->method == ADDRESS)
        {
            importBinary(ADDRESS, p, object);
        }
        if (p->method == REG)
        {
            importBinary(REG, p, object);
        }

        if (p->isExternal == TRUE)
        {
            nExtern = TRUE;
        }
        p = p->next;
    }
    /**start export the data list**/
    pd = getHeadData();
    /*find the value of the head address*/
    if (pd)
    {
        if (pd->address != 0)
        {
            addressOb = pd->address;
        }
        if (pd->address != 0)
        {
            addressOb = pd->address;
        }
        if (pd->address != 0)
        {
            addressOb = pd->address;
        }
        if (pd->address != 0)
        {
            addressOb = pd->address;
        }
        fprintf(object, "%04ld ", addressOb);
        while (pd)
        {

            /*print in little endian all the data list*/
            /*the counter is for to print in new line*/
            if (pd->type == DIRECTIVE_DH)
            {

                short int dhBinary = 0;
                dhBinary = pd->dataCell.dhData.value;

                for (i = 0; i < 2; i++)
                {
                    fprintf(object, "%X", (dhBinary & 0xF0) >> 4); /* high nibble */
                    fprintf(object, "%X ", dhBinary & 0x0F);
                    /* low nibble */
                    counter++;
                     /*if counter = 4 we print in new line*/
                    if (counter == 4)
                    {
                        counter = 0;
                        fprintf(object, "\n");
                        addressOb += 4;
                        fprintf(object, "%04ld ", addressOb);
                    }
                    dhBinary >>= 8;
                }
            }
            if (pd->type == DIRECTIVE_DW)
            {
                long dwBinary = 0;
                dwBinary = pd->dataCell.dwData.value;

                for (i = 0; i < 4; i++)
                {
                    fprintf(object, "%lX", (dwBinary & 0xF0) >> 4); /* high nibble */
                    fprintf(object, "%lX ", dwBinary & 0x0F);
                    /* low nibble */
                    counter++;
                     /*if counter = 4 we print in new line*/
                    if (counter == 4)
                    {
                        counter = 0;
                        fprintf(object, "\n");
                        addressOb += 4;
                        fprintf(object, "%04ld ", addressOb);
                    }
                    /*finished 1 byte, now shift right 8 bit to get the next byte */
                    dwBinary >>= 8;
                }
            }
            if (pd->type == DIRECTIVE_DB)
            {
                char dbBinary = 0;
                dbBinary = pd->dataCell.dbData.value;

                for (i = 0; i < 1; i++)
                {
                    fprintf(object, "%X", (dbBinary & 0xF0) >> 4); /* high nibble */
                    fprintf(object, "%X ", dbBinary & 0x0F);
                    /* low nibble */

                    counter++;
                     /*if counter = 4 we print in new line*/
                    if (counter == 4)
                    {
                        counter = 0;
                        fprintf(object, "\n");
                        addressOb += 4;
                        fprintf(object, "%04ld ", addressOb);
                    }
                }
            }
            if (pd->type == DIRECTIVE_STRING)
            {

                char ascizBinary = 0;
                ascizBinary = pd->dataCell.ascizData.value;

                for (i = 0; i < 1; i++)
                {
                    fprintf(object, "%X", (ascizBinary & 0xF0) >> 4); /* high nibble */
                    fprintf(object, "%X ", ascizBinary & 0x0F);
                    /* low nibble */
                    counter++;
                    /*if counter = 4 we print in new line*/
                    if (counter == 4)
                    {
                        counter = 0;
                        fprintf(object, "\n");
                        addressOb += 4;
                        fprintf(object, "%04ld ", addressOb);
                    }
                }
            }
            pd = pd->next;
        }
    }
    /*************   start export ent file    ***************************/
    pe = getHeadSymbol();
    /*If the list is not empty*/
    if (pe)
    {
        /*open a file in write mode for entries*/
        entries = openFile(name, "w", ENT_EXTENSION);
        /*go through the entry list and write the labels and addresses in hexadecimal*/
        while (pe)
        {
            if (pe->entry == TRUE && (pe->operation == TRUE || pe->data == TRUE))
                fprintf(entries, "%s %04ld\n", pe->label, pe->address);
            pe = pe->next;
        }
    }
    /*************   start export ext file    ***************************/
    if (nExtern)
    {
        /*open an extern file in write mode*/
        externs = openFile(name, "w", EXT_EXTENSION);
        /*assign pointer with word list head*/
        p = getHeadWord();
        /*Go through the list*/
        while (p)
        {
            /*if the word is external*/
            if (p->isExternal == TRUE)
            {
                /*print the label and address in hexadecimal*/
                fprintf(externs, "%s %04ld\n", p->externalLabel, p->address);
            }
            p = p->next;
        }
    }
    printf("\nSuccess! files exported.\n");
    /*Clean up*/
    if (object)
        fclose(object);
    if (entries)
        fclose(entries);
    if (externs)
        fclose(externs);
}
/************************************************************************************************************/
/*this function print the word list in little endian to .ob file*/
void importBinary(int method, wordNode *p, FILE *object)
{
    unsigned int my_32bit = 0;
    unsigned int temp;
    int i;
    unsigned short int my_16bit = 0;
    unsigned short int temp16Bit;
    short int immedBit = 0;
    short int tempImmed;
    if (method == IMMED)
    {
        /*we use structs 2 to print in LSB*/
        i_print_little_endian iTypePrint;
        iTypePrint.opcode = p->holder.I.opcode;
        iTypePrint.rs = p->holder.I.rs;
        iTypePrint.rt = p->holder.I.rt;
        iTypePrint.immed = p->holder.I.immed;
        temp16Bit = iTypePrint.opcode;
        my_16bit |= (temp16Bit << 10);

        temp16Bit = iTypePrint.rs;
        my_16bit |= (temp16Bit << 5);

        temp16Bit = iTypePrint.rt;
        my_16bit |= (temp16Bit);

        tempImmed = iTypePrint.immed;
        immedBit |= (tempImmed);

        fprintf(object, "%04ld ", p->address);

        for (i = 0; i < 2; i++)
        {
            fprintf(object, "%X", (immedBit & 0xF0) >> 4); /* high nibble */
            fprintf(object, "%X ", immedBit & 0x0F);
            /* low nibble */
            /*finished 1 byte, now shift right 8 bit to get the next byte */
            immedBit >>= 8;
        }
        for (i = 0; i < 2; i++)
        {
            fprintf(object, "%X", (my_16bit & 0xF0) >> 4); /* high nibble */
            fprintf(object, "%X ", my_16bit & 0x0F);
            /* low nibble */
            /*finished 1 byte, now shift right 8 bit to get the next byte */
            my_16bit >>= 8;
        }

        fprintf(object, "\n");
    }
    if (method == ADDRESS)
    {
        j_print_little_endian j2;
        j2.address = p->holder.J.address;
        j2.opcode = p->holder.J.opcode;
        j2.reg = p->holder.J.reg;
        temp = j2.opcode;
        my_32bit |= (temp << 26); /*opcode is 6 bit*/

        temp = j2.reg;
        my_32bit |= (temp << 25); /*reg is 1 bit*/

        temp = j2.address;
        my_32bit |= (temp); /*the rest bits are address*/

        fprintf(object, "%04ld ", p->address);

        for (i = 0; i < 4; i++)
        {
            fprintf(object, "%X", (my_32bit & 0xF0) >> 4); /* high nibble */
            fprintf(object, "%X ", my_32bit & 0x0F);
            /* low nibble */
            /*finished 1 byte, now shift right 8 bit to get the next byte */
            my_32bit >>= 8;
        }
        fprintf(object, "\n");
    }
    if (method == REG)
    {

        temp = p->holder.R.opcode;
        my_32bit |= (temp << 26); /*opcode is 6 bit*/

        temp = p->holder.R.rs;
        my_32bit |= (temp << 21); /* 26 - (5 bits of rs) = 21*/

        temp = p->holder.R.rt;
        my_32bit |= (temp << 16); /* 21 - (5 bits of rt) = 16*/

        temp = p->holder.R.rd;
        my_32bit |= (temp << 11); /* 16 - (5 bits of rd) = 11*/

        temp = p->holder.R.funct;
        my_32bit |= (temp << 6); /* 11 - (5 bits of funct) = 6*/

        temp = p->holder.R.unused;
        my_32bit |= (temp); /* these are the remaining 6 bits of unused*/

        fprintf(object, "%04ld ", p->address);

        for (i = 0; i < 4; i++)
        {
            fprintf(object, "%X", (my_32bit & 0xF0) >> 4); /* high nibble */
            fprintf(object, "%X ", my_32bit & 0x0F);
            /* low nibble */
            /*finished 1 byte, now shift right 8 bit to get the next byte */
            my_32bit >>= 8;
        }
        fprintf(object, "\n");
    }
}
