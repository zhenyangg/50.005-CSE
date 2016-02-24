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
	    
	    	nodesArray[count-1].id = count-1; // Node Number = Node ID    	

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
	

	for(j=0; j<count-1; j++){
		nodesArray[j].pid = -1;
	}

	
	// starting processes
	pid_t pid;
	int numLeftToProcess = count -1;
	int currNode = 0;
	int toExecuteNow [MAX_CHILDREN]; // to determine which node can run in the current round
	while (numLeftToProcess > 0) {

		// show current node statuses
		printf("\nparentPID: %d\n", getpid());
		for(j=0; j<count-1; j++){
			printf("\nNode %d status: %d || pid: %d", nodesArray[j].id, nodesArray[j].status, nodesArray[j].pid);
		}
		printf("\n");

		// Select nodes to run
		for(k=0; k<count-1; k++){
			if (nodesArray[k].status == 1) { toExecuteNow[k] = 1;} // STATUS: READY (1)
			else { toExecuteNow[k] = 0; } // STATUS: INELIGIBLE (1) / FINISHED (4)
		}

		// run 'READY' nodes
		for(k=0; k<count-1; k++){
			
			if (toExecuteNow[k] == 1) { // STATUS: READY

				pid = fork(); // fork
				nodesArray[k].pid = pid;

				if (pid >= 0){

				    if (pid == 0) {
					
					//nodesArray[k].status = 2; // STATUS: RUNNING
					nodesArray[k].pid = getpid();
					printf("'***'Executing Node %d now...\n", nodesArray[k].id);
					printf("Child status: RUNNING // Node %d (%d) \n", nodesArray[k].id, getpid());
					
					// Redirection
					int status;
					if (strcmp(nodesArray[k].input, "stdin") == 0) {
						if (strcmp(nodesArray[k].output, "stdout") == 0) {
							status = dup2(1, 0); // (1) stdin to stdout
						}
						else { // use output file
							int fd_out = open(nodesArray[k].output, O_WRONLY ); // writing only
							status = dup2(fd_out, 0); // (2) stdin to output file
						}
					}
					else { // use input file
						int fd_in = open(nodesArray[k].input, O_RDONLY ); // reading only
						if (strcmp(nodesArray[k].output, "stdout") == 0) {
							status = dup2(1, fd_in); // (3) input file to stdout
						}
						else { // use output file
							int fd_out = open(nodesArray[k].output, O_WRONLY ); // writing only
							status = dup2(fd_out, fd_in); // (4) input file to output file
						}
					}
					if (status == -1) {
						perror("dup2(): ");
					}
					
					_exit(0);
				    }

				    else { // parent process
					printf("Waiting for child to finish...\n");
			
					// wait for all children to finish processing
					waitpid(pid, NULL, 0);
					printf("Child status: FINISHED // Node %d (%d) \n", nodesArray[k].id, pid);

					// post-processing
					numLeftToProcess--;
					nodesArray[k].status = 3; // STATUS: FINISHED
					
					// set children of current Node to 'READY'
					for (i=1; i<=nodesArray[k].num_children; i++) {
						nodesArray[k+i].status = 1;
					}

				    }
				
				}

				else { printf("Forking failed~\n"); }
				
				
			}
		
		}
		//numLeftToProcess--;
		
		//printf("currNode: %d\n", currNode);

		// get PID for parent node
		nodesArray[currNode].pid = getpid();
		//printf("parentPID: %d\n", nodesArray[currNode].pid);

		
		//currNode += nodesArray[currNode].num_children;
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