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
    char input[MAX_LENGTH]; // input filename
    char output[MAX_LENGTH]; // output filename
    int children[MAX_CHILDREN]; // children ids
    int num_children; // how many children this node has
    int status; // ineligible/ready/running/finished
    pid_t pid; // process ID
};


void main() {
	FILE *infile;
	infile = fopen("testcase2.txt", "r");
	if (infile == NULL) { perror("File failed to open. Exiting..."); exit(-1);}

	char str[30];
	int count = 1, i, j, k;
	
	char *ptr;
   	char line [ 128 ]; // or other suitable maximum line siz
	struct Node nodesArray[10];
	char *token;

	/*******************************
	*  --------------------------  *
	*  Read File, Store Variables  *
	*  --------------------------  *
	/******************************/

	while ( fgets ( line, sizeof line, infile ) != NULL )
	{
	    token = strtok(line, ":");
		strcpy(nodesArray[count-1].prog, token);
	    token = strtok(NULL, ":");

	    // node has no child
		if (strcmp(token, "none") == 0){
			nodesArray[count-1].children[0] = -1;
			nodesArray[count-1].num_children = 0;
		}

		// node has children, continue storing
		else {
			j=0; int len, num;
			while (sscanf(token, "%d%n", &num, &len) == 1) {
				nodesArray[count-1].children[j] = num;
				token += len;
				j++;
			}
			nodesArray[count-1].num_children = j;
		}

		// store input and output filenames
	    token = strtok(NULL, ":");
		strcpy(nodesArray[count-1].input, token);
	    token = strtok(NULL, "\n");
	    strcpy(nodesArray[count-1].output, token);
	    
	    // store node ID
	    nodesArray[count-1].id = count-1;

	    // Let 1st node be 'READY'
		if((count-1) == 0){ nodesArray[count-1].status = READY;}
		// and the other nodes be 'INELIGIBLE'
		else {nodesArray[count-1].status = INELIGIBLE; }

		count++;
	    	
	};
	
	// Verify no. of cmds stored == no. cmds in file
	printf("No. of commands: %d\n", count-1);
	
	fclose ( infile );

	// initialize pid of each node to -1
	for(j=0; j<count-1; j++){
		nodesArray[j].pid = -1;
	}
	
	/******************
	*  -------------  *
	*  Run Processes  *
	*  -------------  *
	/*****************/

	pid_t pid;
	int numLeftToProcess = count -1;
	int currNode = 0;
	int toExecuteNow [MAX_CHILDREN]; // to determine which node can run in the current round
	while (numLeftToProcess > 0) {

		// Display node statuses
		printf("\nparentPID: %d\n", getpid());
		for(j=0; j<count-1; j++){
			printf("\nNode %d status: %d || pid: %d", nodesArray[j].id, nodesArray[j].status, nodesArray[j].pid);
		}
		printf("\n");

		// Select nodes to run
		for(k=0; k<count-1; k++){
			if (nodesArray[k].status == READY) { toExecuteNow[k] = READY;}
			else { toExecuteNow[k] = INELIGIBLE; }
		}

		// Loop through all nodes
		// Run all 'READY' nodes
		for(k=0; k<count-1; k++){
			
			if (toExecuteNow[k] == READY) {

				pid = fork();
				
				if (pid >= 0){

					// Child process created
					// Run tagged commands
				    if (pid == 0) {
					
						printf("'***'Executing Node %d now...\n", nodesArray[k].id);
						printf("Child status: RUNNING // Node %d (%d) \n", nodesArray[k].id, getpid());

						printf("\n%s\n", nodesArray[k].prog);

						/****************
						*  -----------  *
						*  Redirection  *
						*  -----------  *
						/***************/

						int status;

							if (strcmp(nodesArray[k].input, "stdin") == 0) {

								// (1) stdin to output file
								if (strcmp(nodesArray[k].output, "stdout") != 0) {
									int fd_out = open(nodesArray[k].output, O_WRONLY );
									status = dup2(fd_out, 1);
									if (status == -1) { perror("dup2() redirection failed. Exiting..."); exit(-1);}
									close(fd_out);

									system(nodesArray[k].prog);
								}
							}
							else {
								int fd_in = open(nodesArray[k].input, O_RDONLY );

								// (2) input file to stdout
								if (strcmp(nodesArray[k].output, "stdout") == 0) {
									status = dup2(STDOUT_FILENO, fd_in);
									if (status == -1) { perror("dup2() redirection failed. Exiting..."); exit(-1);}
									close(fd_in);

									char *temp = malloc(strlen(nodesArray[k].prog)+strlen(" ")+strlen(nodesArray[k].input)+1);//+1 for the zero-terminator
									strcpy(temp, nodesArray[k].prog);
									strcat(temp, " ");
									strcat(temp, nodesArray[k].input);

									system(temp);
								}

								// (3) input file to output file
								else {

									char *temp = malloc(strlen(nodesArray[k].prog)+strlen(" >  ")+strlen(nodesArray[k].input)+strlen(nodesArray[k].output)+1);//+1 for the zero-terminator
									strcpy(temp, nodesArray[k].prog);
									strcat(temp, " ");
									strcat(temp, nodesArray[k].input);
									strcat(temp, " > ");
									strcat(temp, nodesArray[k].output);

									system(temp);
								}

						}

						// if redirection fails
						

						_exit(0);
				    }

				    // Parent process
				    else {
			
						// wait for all children to finish processing
						waitpid(pid, NULL, 0);
						printf("Child status: FINISHED // Node %d (%d) \n", nodesArray[k].id, pid);

						// post-processing
						numLeftToProcess--;
						nodesArray[k].status = FINISHED; // new STATUS: FINISHED
						nodesArray[k].pid=pid;


						// set children of current Node to 'READY'
						for (i=1; i<=nodesArray[k].num_children; i++) {
							nodesArray[k+i].status = READY;
						}

				    }
				
				}

				else { printf("Forking failed~\n"); }
				
				
			}
		
		}

	};
	
};
