#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include "phase2_funcs.h"


//*find the opcode from the intermediate file*/
char* find_opcode(char *line){
	static char opcode[6];
	int tab_ct = 0;	//<---- amount of tabs (max = 2)
	int i = 0;	//<---- moving	
	int op_ct = 0;	//<---- opcode counter

	while(tab_ct != 2){		//<--- count 2 tabs (#tabs before opcode)
		if(line[i] == '\t'){
			tab_ct++;
		}
		i++;
	}
	while(line[i] != '\t'){	//<--- put opcode in string
		opcode[op_ct] = line[i];
		op_ct++;
		i++;
	}
	opcode[op_ct] = '\0'; //<--- null-terminate string
	return opcode;
}

/**************************************************************************************************/
//*find the operand from the intermediate file -> used to create object code
char* find_operand(char *line){
	static char operand[10];
	int tab_ct = 0;	//<---- amount of tabs (max = 3)
	int i = 0;	//<---- moving	
	int op_ct = 0;	//<---- operand counter

	while(tab_ct != 3){		//<--- count 3 tabs (#tabs before opcode)
		if(line[i] == '\t'){
			tab_ct++;
		}
		i++;
	}
	while(line[i] != '\t'){	//<--- put opcode in string
		operand[op_ct] = line[i];
		op_ct++;
		i++;
	}
	operand[op_ct] = '\0'; //<--- null-terminate string
	return operand;
}

/**************************************************************************************************/
//*write line to listing file
void w_listing_file(FILE* output_ptr, char* line, char* opcode, char* mnemonic, char* address){
	char obj_code[20];
	int i = 0;

	if(strcmp(opcode,"START") == 0 || strcmp(opcode, "END") == 0){
		fprintf(output_ptr,"\t%s\n", line);
	}
	else if(strcmp(opcode,"WORD") != 0 && strcmp(opcode,"RESW") != 0 && strcmp(opcode,"RESB") != 0){
		for(i; i < strlen(mnemonic); i++){
			obj_code[i] = mnemonic[i];
		}
		obj_code[i] = '\0';

		if(strcmp(opcode,"RSUB") == 0){
			strcat(obj_code,"0000");
		}
		else{
			strcat(obj_code,address);
		}

		fprintf(output_ptr,"%s\t%s\n",obj_code, line);
	}
	else{
		fprintf(output_ptr,"\t%s\n", line);	//testing purposes
	}
}

/************************************************************************************************/
//*get object code for BYTE
void getmnem_BYTE(char* operand, char* mnemonic){
	int m_ct= 0;

	if(operand[0] == 'C'){
		char con_mnem[5];

		for(int i = 0 ; i < strlen(operand); i++){	//get rid of apostrophes + C
			if(operand[i] != '\'' && operand[i] != 'C'){
				con_mnem[m_ct] = operand[i];
				m_ct++;
			}
		}

	    for(int i = 0; i<strlen(con_mnem); i++){	//get hex vals of constants
	        sprintf(mnemonic+i*2, "%02X", con_mnem[i]);
	    }
	}

	else{
		for(int i = 0 ; i < strlen(operand); i ++){	//if X
			if(operand[i] != 'X' && operand[i] != '\''){
				mnemonic[m_ct] = operand[i];
				m_ct++;
			}
		}
	}

}

/************************************************************************************************/
void pass2(){

	FILE * file_ptr;
	file_ptr = fopen("intermediate_file.txt","r");	//<--- read from intermediate file

	FILE * output_ptr;
	output_ptr = fopen("listing_file.txt","w");

	char line[80];
	char opcode[7], mnemonic[7];
	char operand[10], address[7];
	char obj_code[20];

	if(file_ptr == NULL){
		printf("can't reach file\n");
	}
	else{
		fgets(line,80,file_ptr);	//<--- get headers to ignore them

		while(!feof(file_ptr)){

			memset(&opcode[0],0,sizeof(opcode));
			memset(&opcode[0],0,sizeof(mnemonic));
			memset(&operand[0],0,sizeof(operand));
			memset(&address[0],0,sizeof(address));
			memset(&obj_code[0],0,sizeof(obj_code));

			fgets(line,80,file_ptr);			
			strcpy(opcode,find_opcode(line));	//<--- get opcode

			if(strcmp(opcode,"RSUB")==0){
				strcpy(mnemonic,mnem_value(opcode)); //<--- get mnemonic
			}
			else if(strcmp(opcode,"START") != 0 && strcmp(opcode,"END") != 0 && strcmp(opcode,"RSUB") != 0 && 
				strcmp(opcode, "BYTE") != 0 && strcmp(opcode,"WORD") != 0 && strcmp(opcode,"RESW") != 0 && strcmp(opcode,"RESB") != 0){

				strcpy(operand,find_operand(line));		//<---get operand
				
				if(strcmp(operand,"BUFFER,X") == 0){	//exception
					strcpy(operand, "BUFFER");
				}

				strcpy(address,address_val(operand));
				strcpy(mnemonic,mnem_value(opcode)); //<--- get mnemonic
			}
			else{
				operand[0] = '\0';

				/*check if BYTE or WORD*/
				if(strcmp(opcode,"BYTE") == 0){
					strcpy(operand,find_operand(line));		//<---get operand & call function to get object code
					getmnem_BYTE(operand,mnemonic);
				}
				else if(strcmp(opcode,"WORD") == 0){
					/*call function to get objcode*/
				}

			}
		    w_listing_file(output_ptr,line, opcode, mnemonic, address);
		}
	}
	fclose(output_ptr);
	fclose(file_ptr);
}