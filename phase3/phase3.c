#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "phase3_funcs.h"

/**************************************************
Ana Cecilia Sánchez Ramos
SYS PROG - CSCI 3334.01

PHASE 3.
Implement the second pass of a two-pass assembler:
Read from the intermediate file (pass 1), write to 
listing and object files.

Missing: object code + error msgs
**************************************************/
int main(){
	
	build_optab();
	pass1("source.asm");
  	symtab_to_file();
	pass2();

	return 0;
}