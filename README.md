###SIC/XE ASSEMBLER PROJECT###
## By Omar H. Khalil ##


Project Introduction:
Project.cpp reads SIC/XE code from input.txt and does the pass1 and pass2 for the code and
generates the Symbol table, Location Counter, Object Code, and HTE record for the code. The
results are printed in OUTPUT.txt. The Project is completely built using C++17.


Project Implementation:
1- the function GetUp() loads instructions and registers data from instructions.txt and inserts them in a map
 and inserts other SIC/XE directives in sets.
2- The function pass1() starts reading the Input.txt file and builds location counter and returns a 6 string vectors
that include: Location Counter(in hexadecimal format), labels, instruction, references, symobal table (takes 2 vectors),
program length.
3- the function pass1_2() calls function pass1 and then starts generating the object for each line. The algorithm first generates
a binary string that represents the object code in binary format, and then starts manipulating the string based on the details of 
the instruction. Finally, it converts the binary string to hexadecimal format. It appends to the object code vector to 6 orginal 
vectors from pass1 and returns all of them.
4- the function HTE() uses data from pass1_2() to build the HTE record and prints in OUTPUT.txt.
5- the function pass_and_print() calls the function pass1_2() and prints the data from them. The function then
calls HTE().


Example Input/Output:
check files Input.txt and OUTPUT.txt


How to Run:
1- Download files: Project.cpp, Input.txt, OUTPUT.txt, and Instructions.txt.
2- Write your SIC/XE code in Input.txt and save.
3- Run Project.cpp.
4- Open OUTPUT.txt to view results.

