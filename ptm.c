#define INELIGIBLE 0
#define READY 1
#define RUNNING 2
#define FINISHED 3

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_LENGTH	128
#define MAX_CHILDREN	5

struct Node {
    int id; // corresponds to line number in graph text file
    char prog[MAX_LENGTH]; // prog + arguments
    char input[MAX_LENGTH]; // filename
    char output[MAX_LENGTH]; // filename
    int children[MAX_CHILDREN]; // children ids
    int num_children; // how many children this node has
    int status; // ineligible/ready/running/finished
    pid_t pid; // Process
} *ptr[10]; // max 10 processes


// main: open textproc.txt
// separate and store each of the commands somewhere
// use : as delimiter to split the commands
void main() {
	FILE *infile;
	infile = fopen("testproc.txt", "r");

	char str[30];
	int count = 1;
	
   	char line [ 128 ]; /* or other suitable maximum line size */
	char *token;
	while ( fgets ( line, sizeof line, infile ) != NULL ) /* read a line */
	{
		// parse childIDs to get int array and no. of children
		ptr[count-1] = (struct Node *) malloc(sizeof(struct Node));
	    	
	    fputs ( line, stdout ); /* write the line */
	    token = strtok(line, ":");
		ptr[count-1]->prog = token;
	    	
	    token = strtok(NULL, ":");
		ptr[count-1]->children = token;
	    	
	    token = strtok(NULL, ":");
		ptr[count-1]->input = token;
	    	
    	token = strtok(NULL, "\n");
    	ptr[count-1]->output = token;
	    
	    	
	    	
	    	
	    ptr[count-1]->id = count;
	    	
	};
	
	for (int i=0; i<count; i++) {
	    printf("%s\n", ptr[i]->progName);
	    printf("%s\n", ptr[i]->childIDs);
	    printf("%s\n", ptr[i]->inputFile);
	    printf("%s\n", ptr[i]->outputFile);
	}
	
	fclose ( infile );
	
	/*
	// starting processes
	pid = fork();
	
	if (pid >= 0){ //forking successful
	
	    if (pid == 0) { // child process
	        printf("Child process\n");
	    }
	    
	    else { // parent process
	        printf("Parent process\n");
	    }
	    
	}
	else { printf("Forking failed~\n");
    */
	
	
};



// create a process // parallel processing
// follow the requirements

// output to sink.txt
