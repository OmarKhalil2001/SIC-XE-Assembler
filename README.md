###SIC/XE ASSEMBLER PROJECT###
## By Omar H. Khalil ##
<br />
<br />
Project Introduction:<br />
Project.cpp reads SIC/XE code from input.txt and does assembler pass 1 and assembler pass 2 and
generates the Symbol table, Location Counter, Object Code, and HTE record. The
results are printed in OUTPUT.txt. The Project is completely built using C++17.
<br />
<br />
Project Implementation:<br />
1- the function GetUp() loads instructions and registers data from instructions.txt and inserts them in a map
 and inserts other SIC/XE directives in sets.<br />
2- The function pass1() starts reading the Input.txt file and builds location counter and symbol table. The function then returns 6 string vectors
that include: Location Counter(in hexadecimal format), labels, instruction, references, symobal table (takes 2 vectors),
program length.<br />
3- the function pass1_2() calls function pass1 and then starts generating the object for each line. The algorithm first generates
a binary string that represents the object code in binary format, and then starts manipulating the string based on the details of 
the instruction. Finally, it converts the binary string to hexadecimal format. It appends to the object code vector to 6 orginal 
vectors from pass1() and returns all of them.<br />
4- the function HTE() uses data from pass1_2() to build the HTE record and prints it in OUTPUT.txt.<br />
5- the function pass_and_print() calls the function pass1_2() and prints the data from them. The function then
calls HTE().<br />
<br />
<br />
Example Input/Output:<br />
check files Input.txt and OUTPUT.txt
<br />
<br />
How to Run:<br />
1- Download files: Project.cpp, Input.txt, OUTPUT.txt, and Instructions.txt.<br />
2- Write your SIC/XE code in Input.txt and save.<br />
3- Run Project.cpp.<br />
4- Open OUTPUT.txt to view results.<br />

