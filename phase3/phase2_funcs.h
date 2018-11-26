#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include "hashtable.h"

void build_optab(){
	hasht = malloc(max * sizeof(linkedList));
	    	init_hasht();
	    	char* op[][2] = {{"ADD", "18"},  {"RSUB", "4C"},
			                {"AND", "58"},  {"STA", "0C"},
			                {"COMP", "28"}, {"STCH", "54"},
			                {"DIV", "24"},  {"STL", "14"},
			                {"J", "3C"},    {"STX", "10"},
			                {"JEQ", "30"},  {"SUB", "1C"},
			                {"JGT", "34"},  {"TD", "E0"},
			                {"JLT", "38"},  {"TIX", "2C"},
			                {"JSUB", "48"}, {"WD", "DC"},
			                {"LDA", "00"},  {"LDCH", "50"},
			                {"LDL", "08"},  {"LDX", "04"},
			                {"MUL", "20"},  {"OR", "44"},
			                {"RD", "D8"}};
	        for(int i=0; i<25; i++){
	        	insert(op[i][0],op[i][1]);
	        }
}
/**************************************************************************************************/

void split_into(char *str, char* label, char* opcode, char* operand) {

	memset(&label[0],0,sizeof(label));
	memset(&opcode[0],0,sizeof(opcode));
	memset(&operand[0],0,sizeof(operand));

	if(str[0] == '.'){	//<---comment
	//	ignore
	}
	else if(str[0] == ' ' || str[0] == '\t' || str[0] == '\n' || str[0] == '\0'){	//no label
		sscanf(str, "%s %s", opcode, operand);
		memset(&label[0],0,sizeof(label));

		if(strcmp(opcode,"RSUB") == 0){
			operand[0] = '\0';
		}
	}
	else{
		  sscanf( str, "%s %s %s", label,opcode,operand);
	}
}

/**************************************************************************************************/

int byte_length(char* operand, char* error){
	int b_len = 0;

	if(operand[0] == 'C'){
		b_len = strlen(operand) - 3;
	}
	else{	//<---if it's an X
		b_len = strlen(operand) - 3;

		if(b_len % 2 != 0){
			strcat(error,"invalid operand! ");
		} else{
			b_len = b_len / 2;
		}
	}
	return b_len;
}

/**************************************************************************************************/

void handle_line(int* loc_hexval, char* locctr, char* label, char* opcode, char* operand, char* error){
//❤--------- check label !!! ---------❤
	if(label[0] == '\0'){}
	else if(lookup_symbol(label) == 0){	//<--- symbol already exists
		strcat(error,"duplicated label! ");
	}else{
		symtab_insert(label, locctr);
	}
//❤--------- check opcode + add to locctr !!! ---------❤
	if(lookup_opcode(opcode)==0 || strcmp(opcode,"WORD") == 0){ //add +3 to locctr
		*loc_hexval = (int)strtol(locctr, NULL, 16);
		*loc_hexval = (*loc_hexval) + 3;
	//	itoa(*loc_hexval,locctr,16);
		sprintf(locctr, "%x", *loc_hexval); //converting loc decimal to hex

	}

	else if(strcmp(opcode,"RESB") == 0){	//add + #OPERAND to locctr

		*loc_hexval = (int)strtol(locctr, NULL, 16);
		int operand_hexval = (int)strtol(operand, NULL, 10);
		*loc_hexval = (*loc_hexval) + operand_hexval;
	//	itoa(*loc_hexval,locctr,16);
		sprintf(locctr, "%x", *loc_hexval); //converting loc decimal to hex
	}

	else if(strcmp(opcode,"RESW") == 0){	//add + 3 * #OPERAND to locctr
		*loc_hexval = (int)strtol(locctr, NULL, 16);
		int operand_hexval = (int)strtol(operand, NULL, 16);
		*loc_hexval = (*loc_hexval) + (3 * operand_hexval);
	//	itoa(*loc_hexval,locctr,16);
		sprintf(locctr, "%x", *loc_hexval); //converting loc decimal to hex
	} 
	else if(strcmp(opcode, "BYTE") == 0){	//find length of constant in bytes & add length to locctr
		if(operand[0] == 'C' || operand[0] == 'X'){
			if(operand[1] == '\'' && operand[strlen(operand)-1] == '\''){
				*loc_hexval = (int)strtol(locctr, NULL, 16);
				*loc_hexval = (*loc_hexval) + (byte_length(operand, error));
				//	itoa(*loc_hexval,locctr,16);
				sprintf(locctr, "%x", *loc_hexval); //converting loc decimal to hex

			}else{
				strcat(error, "invalid operand! ");
			}
		}
		else{
		strcat(error, "invalid operand! ");
		}
	}
	else{
		strcat(error,"invalid opcode! ");
	}
}

/**************************************************************************************************/
void program_length(int* start, char* start_char, int* end, char* end_char, int* loc_hexval){
	*start = (int)strtol(start_char, NULL, 16);
	*end = (int)strtol(end_char, NULL, 16);
	*end = (*loc_hexval)-(*start);
	//itoa(*end, end_char, 16);
	sprintf(end_char, "%x", *end); //converting loc decimal to hex

}

/**************************************************************************************************/


//*----- P A S S  1 -----*
void pass1(char * src_file){
//❤--------- VARIABLES ---------❤
	char locctr[7], st_address[7], end_address[7];
	locctr[0] = '\0', st_address[0] = '\0', end_address[0] = '\0';
	int loc_hexval = 0 ,int_st = 0, int_end = 0;

	char line[80], label[7], opcode[7], operand[10], error[90];
	error[0] = '\0';

	FILE * file_ptr;	//<---- file we read from
	FILE * output_ptr;	//<-----output file

//❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤

	symbol_table = malloc(symtab_max * sizeof(symtab_linkedList));	
	build_symtab();

	if((file_ptr = fopen(src_file,"r")) == NULL){		//source file can't be opened
		printf("can't reach file\n");	
	}
	else{

		if((output_ptr = fopen("intermediate_file.txt","w")) == NULL){		
			printf("can't open intermediate file\n");	//intermediate file can't be opened
		}	
		else{

				fprintf(output_ptr,"%s\t%s\t%s\t%s\t%s\n", "LOCCTR","LABEL","OPCODE","OPERAND","ERRORS");	//intermediate file headers

				fgets(line,80,file_ptr); //store first string in line
				split_into(line, label, opcode, operand);
				
				if(strcmp(opcode,"START") == 0){
				
					strcpy(st_address, operand);	//set starting address + locctr
					strcpy(locctr,operand);	
					loc_hexval = (int)strtol(locctr, NULL, 16);	//<---- hex value to decimal integer
					int_st = (int)strtol(st_address,NULL,16);
					fprintf(output_ptr,"%x\t%s\t%s\t%s\t%s\n", loc_hexval, label, opcode, operand, error);

					fgets(line,80,file_ptr);	//read next line
				}

				while(!feof(file_ptr)){
					memset(&error[0],0,sizeof(error));
					split_into(line, label, opcode, operand);

					if(label[0]=='\0' && opcode[0]=='\0' && operand[0]=='\0'){}
					else if(line[0] != '.'){	//<----- not a comment

						fprintf(output_ptr,"%x\t%s\t%s\t%s\t%s\n", loc_hexval, label, opcode, operand, error);	//<---write to intermediate file

						if(strcmp(opcode,"END") != 0){	

							handle_line(&loc_hexval, locctr, label, opcode, operand, error);
						}

						else{	//<--- +3 to locctr when END is reached
							if(label[0] != '\0'){
								strcat(error,"END does not need a label! ");
							}
							loc_hexval = (int)strtol(locctr, NULL, 16);
							loc_hexval = (loc_hexval) + 3;
						//	itoa(loc_hexval,locctr,16);	
							sprintf(locctr, "%x", loc_hexval); //converting loc decimal to hex*/
						}
					}
				fgets(line,80,file_ptr);
				}
			}
			fclose(output_ptr);
		}
		program_length(&int_st, st_address, &int_end, end_address, &loc_hexval);	//<--- calculate length of program
	
		printf("File has been successfully assembled! \n");
		fclose(file_ptr);
	}