# 20465-System-Programming-Laboratory-Final-Project
Final project at the course System Programming Laboratory(20465), grade: 100.

The Project is compiler for assembler ,The project is written in the ANSI C standard .
The purpose of this project is to build an assembler for 15-instruction assembly language, 
for an imaginary 32-bit CPU and 32 bits of memory size.
This project is based on the double-pass assembler model.


receive assembly files (.as)

interpret the files according to given rules

output the interpretation, in three files:

* ob. - memory image and instructions (hex)

* ent. - symbol table

* ext. - list of external values

The project was coded and compiled using Ubuntu, but it may run on all Linux versions.

Use makefile to compile the project like this:

```bash
   > make
```

After preparing assembly files with an .as extension, open terminal and pass file names as arguments (without the file extensions) as following:

As for the files file1.as, file2.as, file3.as we will run:

```bash
  > ./assembler file1.as file2.as file3.as
```
The assembler will generate output files with the same filenames and the following extensions:

* .ob - Object file
* .ent - Entries file
* .ext - Externals file

Examples of input and output files can be found under the 'test' folders.
