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

	while ( fgets ( line, sizeof line, infile ) != NULL ) /* read a line */
	{
    	fputs ( line, stdout ); /* write the line */
    	char *progName;
    	progName = strtok(line, ":");
    	
    	char *childIDs;
    	childIDs = strtok(NULL, ":");
    	
    	char *inputFile;
    	inputFile = strtok(NULL, ":");
    	
    	char *outputFile;
    	outputFile = strtok(NULL, "\n");
    	
    
    	// parse childIDs to get int array and no. of children
    	
    	ptr[count-1] = (struct Node *) malloc(sizeof(struct Node));
    	ptr[count-1]->id = count;
    	ptr[count-1]->prog = progName;
    	ptr[count-1]->input = inputFile;
    	ptr[count-1]->output = outputFile;	
    	ptr[count-1]->children = childIDs;
    	
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
