    #include<stdio.h>
    #include<stdlib.h>
    #include <string.h>

//************** O P C O D E  T A B L E *************//
    typedef struct node {
		char opcode[5];
		char mnemonic[5];
    	struct node *next;
    }node;

    typedef struct linkedList {	//stores a list at each index
    	struct node *head;    	
    	struct node *tail;
    }linkedList;

    linkedList *hasht = NULL;
    int max = 26;	      
     
	int hashFunction(char* opcode){	//returns index where we'll store item
		unsigned int wordVal = 0;
		for(int i = 0; i < strlen(opcode); i++){
			wordVal = 37 * wordVal + (opcode[i] - 48);	//convert opcode -> number(key)
		}
		return wordVal % max;
	}

    //functions
    void init_hasht();
    void insert(char* opc, char* mnem);
    int lookup_opcode(char* str);

    void insert(char* opc, char* mnem)
    {
		int index = hashFunction(opc);
    	node *list = hasht[index].head;
     	
     	node *item = malloc(sizeof(struct node));	//item to be added
    	strcpy(item->opcode,opc);
    	strcpy(item->mnemonic,mnem);
    	item->next = NULL;

    	if (list == NULL) 	//linked list is empty
            {
    		hasht[index].head = item;
    		hasht[index].tail = item;
    	}

        else {	//add to end of list
    			hasht[index].tail->next = item;
    			hasht[index].tail = item;
    	}
    }

    int lookup_opcode(char* str){
    	int index = hashFunction(str);
    	node *temp = hasht[index].head;

    	while(temp!=NULL){
			if(strcmp(str,temp->opcode) == 0){
				return 0;
			}
			temp = temp->next;
		}
		return 1;
    }

    void init_hasht(){	//initialize hash symbol_table
       	int i = 0;
    	for (i = 0; i < max; i++)
            {
    		hasht[i].head = NULL;
    		hasht[i].tail = NULL;
    	}
    }


/**************************************************************************************************/


//************** S Y M B O L  T A B L E *************//
    typedef struct symtab_node {
		char label[7];
		char address[7];
    	struct symtab_node *next;
    }symtab_node;

    typedef struct symtab_linkedList {	//stores a list at each index
    	struct symtab_node *head;    	
    	struct symtab_node *tail;
    }symtab_linkedList;

    symtab_linkedList *symbol_table = NULL;
    int symtab_max = 501;	      
     
	int symtab_hashFunction(char* label){	//returns index where we'll store item
		unsigned int wordVal = 0;
		for(int i = 0; i < strlen(label); i++){
			wordVal = 37 * wordVal + (label[i] - 48);	//convert opcode -> number(key)
		}
		return wordVal % symtab_max;
	}

    //functions
    void build_symtab();
    void symtab_insert(char* lbl, char* addr);
    void symtab_display();

    void symtab_insert(char* lbl, char* addr)
    {
		int index = symtab_hashFunction(lbl);
    	symtab_node *list = symbol_table[index].head;
     	
     	symtab_node *item = malloc(sizeof(struct symtab_node));	//item to be added
    	strcpy(item->label,lbl);
    	strcpy(item->address,addr);
    	item->next = NULL;

    	if (list == NULL) {	//linked list is empty
    		symbol_table[index].head = item;
    		symbol_table[index].tail = item;
    	}

        else {	//add to end of list
    			symbol_table[index].tail->next = item;
    			symbol_table[index].tail = item;

    	}
    }

    void build_symtab(){	//initialize hash symbol_table
       	int i = 0;
    	for (i = 0; i < symtab_max; i++) {
    		symbol_table[i].head = NULL;
    		symbol_table[i].tail = NULL;
    	}
    }

    int lookup_symbol(char* str){
    	int index = symtab_hashFunction(str);
    	symtab_node *temp = symbol_table[index].head;

    	while(temp!=NULL){
			if(strcmp(str,temp->label) == 0){
				return 0;
			}
			temp = temp->next;
		}
		return 1;
    }

    void symtab_to_file(){
	    FILE * file_ptr;
		if((file_ptr = fopen("symtab_file.txt","w")) == NULL){
					printf("can't open intermediate file\n");	//file can't be opened
		}else{
			fprintf(file_ptr,"%s | %s \n\n","LABEL", "ADDRESS");	//<---write to file

			for (int i = 0; i < symtab_max; i++) {
			    symtab_node *temp = symbol_table[i].head;
			    if (temp != NULL) {
			   		while (temp != NULL) {
			   			fprintf(file_ptr,"%s \t %s \n",temp->label, temp->address);	//<---write to file
			   			temp = temp->next;
			   		}
				}
			}
		}
		fclose(file_ptr);
    }