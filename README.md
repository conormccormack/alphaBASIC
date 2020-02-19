# alphaBASIC
alphaBASIC is a robust interpreter for the BASIC programming language written in C++ by Conor McCormack in Fall 2019. alphaBASIC is able to parse and execute control flow commands, variable assignments and referencing, and binary arithemtic including multi-dimensional arrays.

Defintions, instructions, and a sample program are all provided below. More BASIC programs are provided in the "input" directory. Enjoy!

### Execution Instructions:
Compile interpreter using make.

Include name of BASIC input file as command line argument which can be a .txt, .in, or any similar file type.  
```
basic_interpreter myInput.txt
```

### Some Definitions:
**\<AEXPR\>** is an arithmetic expression. This can be the name of a variable, a constant integer such as 45, a binary arithemtic operation (e.g. \<AEXPR\> + \<AEXPR\>, or an array with an evaluated index (e.g. MYARR[\<AEXPR\>])

**\<BEXPR\>** is a boolean expression. This is an expression of the format (\<AEXPR\> \<  \<AEXPR\>), and will evaluate to either **True** or **False**. Less than \<, Greater than \>, and equal to \= comparisons are all supported.

**\<LINEJ>** Is a line jump. This should be the number of a line that exists in the BASIC program, which the interpreter will use to execute the **IF**, **GOSUB**, and **GOTO** commands. 

*Note*: Each line in the input BASIC program requires a line number preceeding the actual command. This is used for the **IF**, **GOSUB**, and **GOTO** commands. These lines do not necessarily have to be in consecutive order, but should be monotonically increasing without repeated line numbers.

### Supported Commands:
**PRINT \<AEXPR>**: prints the value of the arithmetic expression followed by a newline.  
**LET \<VAR> \<AEXPR>**: evaluates \<AEXPR> and stores the resulting value into the variable \<VAR>, overwriting the previous value if there was one.  
**LET \<VAR> \[\<AEXPR1>] \<AEXPR2>**: evaluates \<AEXPR2> and stores the resulting value into the array \<VAR>, at the position obtained from evaluating \<AEXPR1>.  
**GOTO \<LINEJ>: Jump to the line \<LINEJ>**. Forgets which line it came from and just resumes processing at the given line \<LINEJ>. If there is no line numbered \<LINEJ> in the program, the program will print an error and terminate.  
**IF \<BEXPR> THEN \<LINEJ>**: If \<BEXPR\> is true, then the program jumps to the line \<LINEJ> (as in the case of GOTO). Otherwise, it will just advance to the next line. As for GOTO, if the target line does not exist, the program will output an error. If the condition evaluates to false and the target line does not exist, then no error is reported as the program will safely continue running.  
**GOSUB \<LINEJ>**: Jump to the line \<LINEJ>, and remember where you came from. As for GOTO/IF, if the line \<LINEJ> does not exist, then an error is printed (including the current line number and non-existing target line), and the program terminates.  
**RETURN**: Go back to the line immediately after the line whence the most recent GOSUB jump started.   
**END**: Terminate the execution of the program.  

## Example Program:
BASIC program: 
```
1 LET I 0
2 LET SIZE 10
3 LET A[I] (10 - I)
4 PRINT A[I]
5 LET I (I + 1)
6 IF I < SIZE THEN 3
7 GOSUB 11
8 END
11 LET SIZE 10
12 LET I 0
13 LET J I
14 IF A[J] < A[I] THEN 21
15 LET J (J + 1)
16 IF J < SIZE THEN 14
17 PRINT A[I]
18 LET I (I + 1)
19 IF I < SIZE THEN 13
20 RETURN
21 LET TEMP A[I]
22 LET A[I] A[J]
23 LET A[J] TEMP
24 GOTO 15
```

Output:
```
10
9
8
7
6
5
4
3
2
1
1
2
3
4
5
6
7
8
9
10
```
