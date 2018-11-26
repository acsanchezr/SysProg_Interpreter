#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "command_funcs.c"

/****************/
//Cecy Sánchez
//CSCI3334.01
//Phase #2
/***************/

int main(){

	char ans[100]; // string that stores the whole line
	char comm[10], param1[10], param2[20];	//command + parameters
	int ans_len, n;	//string length + number of parameters
	int tru = 1;

	printf("Welcome!\n");
	printf("Here's the available commands:\n\n");

	menu();

	while(1){
		printf("\nPlease enter a command + parameters: ");

		fgets(ans, 100, stdin);	/* read in a line */
	    ans_len = strlen(ans) - 1;		 /* get rid of trailing newline character */
	    if (ans[ans_len] == '\n'){
		        ans[ans_len] = '\0';
	    }

	    breakLine(ans, comm, param1, param2, &n);
	    tru = strcmp(comm, "exit");

	    if(tru == 0 && param1[0] == '\0' && param2[0] == '\0'){
				exit(0);
	    }

	    else if (tru == 0 && param1[0] != '\0'){
		    printf("No need for parameters! check \"help\" for more info\n");
	    }

	    else{
		    check(comm,param1,param2);
		}
	}
	return 0;
}




