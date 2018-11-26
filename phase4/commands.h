#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "phase3_funcs.h"


/************protoypes**************/
void menu(); //<--- used for "help" as well
void load();
void execute();
void debug();
void dump();
void assemble(char*);


/************definitions**************/
void menu(){
	printf("load		<filename> \nexecute\ndebug\ndump		<start>		<end>\nhelp\nassemble	<filename>\ndirectory\nexit\n");
}

void load(){
	printf("You've reached the load command\n");
}

void execute(){

	unsigned long PCloc = 0;

	SICRun(&PCloc,0);


	printf("You've reached the execute command\n");
}

void debug(){
	printf("You've reached the debug command \n");
}

void dump(){
	printf("You've reached the dump command\n");
}

void assemble(char * src_file){
	build_optab();
	pass1(src_file);
  	symtab_to_file();
	pass2();
}