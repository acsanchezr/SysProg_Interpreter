#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "commands.h"

//*FUNCTION-> break up string input*/
void breakLine(char *str, char *comm, char *param1, char *param2, int *n_words) {	

	int str_ct = 0, comm_ct = 0, p1_ct = 0, p2_ct = 0;

	memset(&comm[0],0,sizeof(comm));
	memset(&param1[0],0,sizeof(param1));
	memset(&param2[0],0,sizeof(param2));
	*n_words = 0;

	while(str[str_ct] == ' '){		//check for spaces before command
		str_ct++;
	}

	while(str[str_ct] != '\0' && str[str_ct] != ' ') {		//find command
		comm[comm_ct] = str[str_ct];
		str_ct++;
		comm_ct++;

		if(str[str_ct] == '\0' || str[str_ct] == ' '){		//null-terminate it	
			comm[comm_ct] = '\0';			
			comm_ct++;
			(*n_words)++;
		}
	}

	while(str[str_ct] == ' '){
		str_ct++;
	}

// find 1st parameter + store it
	if(str[str_ct] != '\0' && str[str_ct] !=' '){			
		while(str[str_ct] != '\0' && str[str_ct]!=' '){
			param1[p1_ct] = str[str_ct];
			p1_ct++;
			str_ct++;

			if(str[str_ct] == '\0' || str[str_ct] == ' '){
				param1[p1_ct] = '\0';
				p1_ct++;
				(*n_words)++;
			}
		}
	}
	else { param1[0]='\0'; } 
	

	while(str[str_ct] == ' '){
		str_ct++;
	}

//find 2nd parameter + store it
	if(str[str_ct] != '\0' && str[str_ct] !=' ') {		
		while(str[str_ct] != '\0' && str[str_ct] != ' '){
			param2[p2_ct] = str[str_ct];

			p2_ct++;
			str_ct++;

			if(str[str_ct] == '\0' || str[str_ct] == ' '){
				param2[p2_ct] = '\0';
				p2_ct++;
				(*n_words)++;
			}
		}
	}
	else{ param2[0]='\0'; }
}


/**************************************************************************************************/
//*FUNCTION-> check user input*/
void check(char *comm, char *param1, char *param2){
	char * commands[8];		//create array to store commands

	char* input;	
	int cmp;
	int index;

//❤--------- commands ---------❤
	commands[0] = "load";
	commands[1] = "execute";
	commands[2] = "debug";
	commands[3] = "dump";
	commands[4] = "help";
	commands[5] = "assemble"; 
	commands[6] = "directory";
//❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤❤

	for(int i = 0 ; i < 8; i++){		/*loop through array to find command*/
		cmp=strcmp(comm,commands[i]);
		 if(cmp == 0) {
    		input = comm;
    		index = i;
    		break;
	    }
	}

	/*check each case*/
	if(index == 0){	//load
		if(param1[0] != '\0' && param2[0] == '\0'){
			load();
		}
		else if(param1[0] == '\0'){
			printf("You're missing parameters, check \"help\" for more info\n");
		}
		else{
			printf("Too many parameters! check \"help\" for more info\n");
		}
	}
	else if(index == 1 || index == 2){		//execute or debug
		if(param1[0] == '\0' && param2[0] == '\0'){
			if(index == 1){	//execute
				execute();
			}
			else{		//index = 2 -> debug 
				debug();
			}
		}
		else{
			printf("No need for parameters! check \"help\" for more info\n");
		}
	}
	else if(index == 3){	//dump
		if(param1[0] != '\0' && param2[0] != '\0'){
			dump();
		}
		else if(param1[0] == '\0' || (param1[0] !='\0' && param2[0] =='\0')){
			printf("You're missing parameters, check \"help\" for more info\n");
		}
		else{
			printf("Too many parameters! check \"help\" for more info\n");
		}
	}
	else if(index == 4){	//help
		if(param1[0] == '\0' && param2[0] == '\0'){
			menu();
		}
		else{
			printf("No need for parameters! check \"help\" for more info\n");
		}
	}
	else if(index == 5){	//assemble
		if(param1[0] != '\0' && param2[0] == '\0'){
			assemble(param1);
		}
		else if(param1[0] == '\0'){
			printf("You're missing parameters, check \"help\" for more info\n");
		}
		else{
			printf("Too many parameters! check \"help\" for more info\n");
		}
	}

	else if(index == 6) {	//directory	
		if(param1[0]=='\0' && param2[0] == '\0'){
			system("ls");
		}
		else{
			printf("No need for parameters! check \"help\" for more info\n");
		}
	}

	else{
		printf("Sorry, not a valid command!!! check \"help\" for more info\n");
	}
}
