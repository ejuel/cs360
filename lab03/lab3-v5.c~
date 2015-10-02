/************ Lab3.c file ***************/
// Ezekiel Juel 11258126
// man ls | grep ls
// man loads
// grep searches for ls
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // <-- used for most of the execution functions
#include <wait.h>
//#include <sys/types.h>


#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

char command[64], input[128], str[128];
char line[128];       /* user input line */
char string[128];
int i = 0;
int d=0;

int main(int argc, char** argv, char **envp){
	char args[64][64];
	char **myargv = args; // used to pass all arguments to next function for execute
	int ID = 0;

	char *SystemID = strtok(envp[15], "/:"); // this grabs the hardware ID
	SystemID = strtok(NULL, "/:");
	ID = 0;
	while(ID != -1){
		printf("%s@%s " COLOR_YELLOW "%s $ " COLOR_RESET, envp[10]+5, SystemID, get_current_dir_name());
		fgets(line, 128, stdin);
		if(line[0] != '\n'){
			strcpy(string, line);    
			char *token = strtok(string, " \n");
			if(token!=NULL){
				strcpy(command, token);
			}
			if(0 == strcmp(token, "exit")){
				ID = -1;
				return 1;
				exit(EXIT_SUCCESS);
			}
			i = 0;
			while(token != NULL){
				myargv[i] = token;
				i++;
				token = strtok(NULL, " \n");
			}
			myargv[i]=NULL;
			myargv[i+1]=NULL;
			execute(myargv,envp);
		}
	}
	return 1;
}

printARGV(char **argv){
	int d = 0;
	while(argv[d] /*&& argv[d] != "|"*/){ // it's ok to print the "|" when reading the line
		printf("%s ", argv[d]);
		d++;
	}
}
execute(char **myargv,char **envp){
	pid_t pid;
	int fd[2]; // used for read and write of pipe function
	int status;
	int pipeFound = 0;
	char args[64][64];
	char **nextARGV = args;
	char localARGS[64][64];
	char **localARGV = localARGS;
	i = 0;
	while(myargv[i] && strcmp(myargv[i], "|")){ // checks that myargv exists and looks for pipe "\"
		localARGV[i] = myargv[i];		    // localARGV will be used locally and the nextARGV will be passed on if a pipe is encountered.
		printf("## localARGV[%d] = '%s'\n", i, localARGV[i]);
		i++;
	}
	if(myargv[i] && strcmp(myargv[i], "|") == 0){
		printf("## myargv[%d] = '%s'\n", i, myargv[i]);
		pipeFound = 1;
	}
	else{
		pipeFound = 0;
	}
		
	i+= 1;
	d = 0;
	while(myargv[i]){
		nextARGV[d] = myargv[i];
		i++;
		d++;
	}
	char path[32][32] = {   "/usr/local/sbin/ ","/usr/local/bin/", "/usr/sbin/","/usr/bin/","/sbin/","/usr/games/","/usr/local/games","/bin/"};
	if(localARGV[0] && 0 == strcmp(localARGV[0], "cd")){
		if(NULL == localARGV[1]){
			chdir("/");
			printf("chdir\n");
			return 1;
		}
		else if(chdir(localARGV[1]) == -1){
			printf("error: chdir gave unknown error\n");
		}
	}
	status = 1; // for some reason code would only execute once unless this was changed
	for(i = 0; i < 2 && status != 0; i++){
		pid = fork();
		if(pid != 0){
			printf("parent sh PROC %d forks a child process %d\n", getpid(), pid);
		}
		if(pid == -1){
			printf("error: fork failed\n");
			return 0;
			exit(EXIT_FAILURE);
		}
		else if(pid == 0){ // child execution
			printf("PROC %d: line=", getpid());
			printARGV(localARGV);
			printf("\n");
			int pipeCheck = 0;
			d = 0;
			printf("PROC %d do_command: line=", getpid());
			printARGV(localARGV);
			printf("(NOTE: Implemented execvpe instead of execve)\n");
			if(pipeFound == 1 && nextARGV[0]){
				printf("has pipe: head=");
				printARGV(localARGV);
				printf(" tail=");
				printARGV(nextARGV);
				printf("\n");
				
// This code will execute a pipe
				pipe(fd);
				runsource(fd, localARGV, envp); // runs source function
				rundest(fd, nextARGV, envp);  // runs destination function
				close(fd[0]); 
				close(fd[1]); // this is important! close both file descriptors on the pipe
				i = 0;
				while(localARGV[i]){
					localARGV[i] = NULL;
					i++;
				}
				i = 0;
				while(nextARGV[i]){
					nextARGV[i] = NULL;
					i++;
				}
				//exit(1);
				

				// won't use this VVV once pipe function works
				//execute(nextARGV,envp);
				// remove this ^^^^
			}
			else{
				d = 0;
				status = execvpe(localARGV[0],localARGV,envp);
			}
			//return status;
			exit(status);
		}
		else{
			printf("parent sh PROC %d waits\n", getpid());
			if(waitpid(pid, &status, 0) < 0){
				printf("error: waitpid failed,");
				return 0;
				exit(EXIT_FAILURE);
			}
			printARGV(localARGV);
			printf("\n");
			if(WIFEXITED(status)){
				if(status != 65280){ // this suggests error occured
					printf("child sh PROC %d died: exit status = %04d\n", pid, status);
				}
				else{
					printf("good child status code: %04d\n", status);
				}
				break;
			}
			return 0;
			exit(EXIT_FAILURE);
		}

	}
}


runsource(int pfd[],char **myargv,char **envp) /* run the first part of the pipeline, cmd1 */ 
{ int pid; /* we don't use the process ID here, but you may wnat to print it for debugging */ 
	switch (pid = fork()) { 
		case 0: /* child */ 
			dup2(pfd[1], 1); /* this end of the pipe becomes the standard output */ 
			close(pfd[0]); /* this process don't need the other end */ 
			execvpe(myargv[0], myargv, envp); /* run the command */ 
			perror(myargv[0]); /* it failed! */ 

		default: /* parent does nothing */ 
			break; 

		case -1: 
			perror("fork"); 
			exit(1); 
	} 
} 

rundest(int pfd[],char **myargv,char **envp) /* run the second part of the pipeline, cmd2 */ 
{ 
	int pid; 
	switch (pid = fork()) { 
		case 0: /* child */ 
			dup2(pfd[0], 0); /* this end of the pipe becomes the standard input */ 
			close(pfd[1]); /* this process doesn't need the other end */ 
			execvpe(myargv[0], myargv, envp); /* run the command */ 
			perror(myargv[0]); /* it failed! */ 

		default: /* parent does nothing */ 
			break; 
		case -1: 
			perror("fork"); 
			exit(1); 
	}
}
