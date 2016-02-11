#define INELIGIBLE 0
#define READY 1
#define RUNNING 2
#define FINISHED 3

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_LENGTH	128
#define MAX_CHILDREN	5

typedef struct node {
    int id; // corresponds to line number in graph text file
    char prog[MAX_LENGTH]; // prog + arguments
    char input[MAX_LENGTH]; // filename
    char output[MAX_LENGTH]; // filename
    int children[MAX_CHILDREN]; // children ids
    int num_children; // how many children this node has
    int status; // ineligible/ready/running/finished
    pid_t pid; // Process
} node_t;


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
	printf("%s\n", progName);
	char *childIDs;
	childIDs = strtok(NULL, ":");
	printf("%s\n", childIDs);
	char *inputFile;
	inputFile = strtok(NULL, ":");
	printf("%s\n", inputFile);
	char *outputFile;
	outputFile = strtok(NULL, "\n");
	printf("%s\n", outputFile);

	// parse childIDs to get int array and no. of children
	char 

	node_t *data;
	data = (node_t *) malloc(sizeof(node_t));
	data->id = count;
	data->prog = progName;
	data->input = inputFile;
	data->output = outputFile;	
	data->children = childIDs;
		

	};

	fclose ( infile );
	
};



// create a process // parallel processing
// follow the requirements

// output to sink.txt
