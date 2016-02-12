#define INELIGIBLE 0
#define READY 1
#define RUNNING 2
#define FINISHED 3

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>


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
}; // max 10 processes


// main: open textproc.txt
// separate and store individual commands in struct array
// use : as delimiter to split the commands (use strtok)
void main() {
	FILE *infile;
	infile = fopen("testproc.txt", "r");

	char str[30];
	int count = 1, i, j, k;
	
	char *ptr;
   	char line [ 128 ]; // or other suitable maximum line siz
	struct Node nodesArray[10];
	char *token;
	while ( fgets ( line, sizeof line, infile ) != NULL ) // read a line
	{
	    	// fputs ( line, stdout ); // Print out commands

	    	token = strtok(line, ":");
		strcpy(nodesArray[count-1].prog, token);
	    	
	    	token = strtok(NULL, ":");
		if (strcmp(token, "none") == 0){
			nodesArray[count-1].children[0] = -1; // childless
			nodesArray[count-1].num_children = 0;
		}
		else {
			j=0; int len, num;
			// if more than one child, continue storing
			while (sscanf(token, "%d%n", &num, &len) == 1) {
				nodesArray[count-1].children[j] = num;
				token += len;
				j++;
			}
			nodesArray[count-1].num_children = j;
		}
	    	token = strtok(NULL, ":");
		strcpy(nodesArray[count-1].input, token); // store inputFile string
	    	token = strtok(NULL, "\n");
	    	strcpy(nodesArray[count-1].output, token); // store outputFile string
	    
	    	nodesArray[count-1].id = count; // Node Number = Node ID    	

		if((count-1) == 0){ nodesArray[count-1].status = 1;} // READY (1st node)
		else {nodesArray[count-1].status = 0; } // INELIGIBLE (other nodes)

		count++;
	    	
	};
	
	// Verify no. of commands
	printf("No. of commands: %d\n", count-1);
	
	// Print content in each node (for verificaton purposes)
	/* for (i=0; i<count-1; i++) {
		printf("%s\n", nodesArray[i].prog);
		if (nodesArray[i].children[0] == -1 ) {
			printf("%i\n", nodesArray[i].children[0]);
		}
		else {
			int k;
			for (k=0; k<MAX_CHILDREN; k++) {
				if (nodesArray[i].children[k] != 0 ) {
					printf("%i\n", nodesArray[i].children[k]);
				}
			}
		}
		printf("%s\n", nodesArray[i].input);
		printf("%s\n\n", nodesArray[i].output);
	}*/
	
	fclose ( infile );
	


	
	// starting processes

	
	pid_t pid;
	int numLeftToProcess = count;
	int nextNode = 1;
	while (numLeftToProcess > 0) {

		// get PID for parent node
		nodesArray[nextNode-1].pid = getpid();

		for (j=0; j<nodesArray[nextNode-1].num_children; j++) { // loop through all children nodes and run them

			nextNode++;

			nodesArray[i].children[j] = fork(); // save child PID in parent
			pid = nodesArray[i].children[j];
			
			if (pid >= 0){

			    if (pid == 0) {
				numLeftToProcess--;
				nodesArray[i].status = 2; // STATUS: RUNNING
				printf("Child process status: %d\n", nodesArray[i].status);
				nodesArray[i].pid = getpid();
				printf("childPID: %d\n", nodesArray[i].pid);
			
				// Redirection
				int status;
				if (strcmp(nodesArray[i].input, "stdin") == 0) {
					if (strcmp(nodesArray[i].output, "stdout") == 0) {
						status = dup2(1, 0); // (1) stdin to stdout
					}
					else { // use output file
						int fd_out = open(nodesArray[i].output, O_WRONLY ); // writing only
						status = dup2(fd_out, 0); // (2) stdin to output file
					}
				}
				else { // use input file
					int fd_in = open(nodesArray[i].input, O_RDONLY ); // reading only
					if (strcmp(nodesArray[i].output, "stdout") == 0) {
						status = dup2(1, fd_in); // (3) input file to stdout
					}
					else { // use output file
						int fd_out = open(nodesArray[i].output, O_WRONLY ); // writing only
						status = dup2(fd_out, fd_in); // (4) input file to output file
					}
				}

				if (status == -1) {
					perror("dup2(): ");
				}
			
				nodesArray[i].status = 3; // STATUS: FINISHED
				printf("Child process status: %d\nExiting child process", nodesArray[i].status);
				_exit(0);
			    }

			    else { // parent process
				printf("Parent process\n");
				// verify node statuses
				for(k=0; k<count-1; k++){
					printf("Node %d status: %d || ", nodesArray[k].id, nodesArray[k].status);
				}
				printf("\n");
				// wait for all children to finish processing
				waitpid(pid, NULL, 0);
			    }

			}

			else { printf("Forking failed~\n"); }

		}
	};

		
		
	

	/*
	for(i=0; i<count-1; i++){
		
		// fork() for node 0
		if(i==0){
			if(nodesArray[i].status == READY){
				pid = fork();
				if (pid >= 0){ //forking successful
				    if (pid == 0) { // child process
				        nodesArray[i].status = RUNNING;
				        printf("Child process status: %d\n", nodesArray[i].status);
				        nodesArray[i].pid = pid;
				        printf("childPID: %d\n", pid);
				        
				        //TODO: execute program for this node
				        
				        nodesArray[i].status = FINISHED;
				        printf("Child process status: %d\n", nodesArray[i].status);
				    }
				    else { // parent process
				        printf("Parent process\n");
				        waitpid(-1, NULL, 0);
				        printf("Back to parent process\n");
				    }
				}
				else { printf("Forking failed~\n");}
			}
		}
		

	}*/
	
	
	
    
	
	
};



// create a process // parallel processing
// follow the requirements

// output to sink.txt
