#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#define VNAME(value) (#value)
void dummy(int i);
void handle(pid_t pid, int state);
int main(int argc, char *argv[]){

	/* fork a child process */
	pid_t pid,ppid;
	int i,state;
	printf("Parent process id is %d\n",getpid());
	pid=fork();
	/* execute test program */ 
	if (pid==-1){
		perror("fork");
		exit(-1);	
	}else{
	if (pid == 0) {
            
            int i;
            char *arg[argc];
            
            
            for(i=0;i<argc-1;i++){
                arg[i]=argv[i+1];
            }
            arg[argc-1]=NULL;
            
	    	printf("Child process id is %d\n", getpid());
            printf("Child process start to execute test program:\n");
            execve(arg[0],arg,NULL);
	    
	/* wait for child process terminates */
	    
        }else{
			
			signal(SIGCHLD,dummy);
			waitpid(-1,&state,WUNTRACED);
			handle(pid,state);
		}
	    	
	}
	}
	
	
	/* check child process'  termination status */
	


void dummy(int i){
	printf("\n");
}
void handle(pid_t pid, int state){
	int exitStatus,ifExited,ifSignalEd,termSig;
	exitStatus=WEXITSTATUS(state);
	ifSignalEd=WIFSIGNALED(state);
	ifExited=WIFEXITED(state);
	termSig=WTERMSIG(state);
	if(SIGCHLD==17){
		printf("Parent process receive SIGCHLD\n");
	}
	
	printf("WIFEXITED%d,WEXITSTATUS%d,WIFSIGNALED%d,WTERMSIG%d\n",ifExited,exitStatus,ifSignalEd,termSig);
	char name[100];
	char terminationMessage[100];
	if(ifSignalEd)
	{	
		switch(termSig){
			case 1: strcpy(name,"SIGHUP");strcpy(terminationMessage,"hanged up");break;
			case 2: strcpy(name,"SIGINT");strcpy(terminationMessage,"interrupted");break;
			case 3: strcpy(name,"SIGQUIT");strcpy(terminationMessage,"quited");break;
			case 4: strcpy(name,"SIGILL");strcpy(terminationMessage,"received illegal input");break;
			case 5: strcpy(name,"SIGTRAP");strcpy(terminationMessage,"trapped");break;
			case 6: strcpy(name,"SIGABRT");strcpy(terminationMessage,"aborted");break;
			case 7: strcpy(name,"SIGBUS");strcpy(terminationMessage,"accessed memory incorrectly");break;
			case 8: strcpy(name,"SIGFPE");strcpy(terminationMessage,"terminated with a floating point error");break;
			case 9: strcpy(name,"SIGKILL");strcpy(terminationMessage,"terminated by kill");break;
			case 13: strcpy(name,"SIGPIPE");strcpy(terminationMessage,"terminated due to the termination of consumer in pipeline");break;
			case 14: strcpy(name,"SIGALARM");strcpy(terminationMessage,"termianted with an alarm signal");break;
			case 15: strcpy(name,"SIGTERM");strcpy(terminationMessage,"terminated");break;
			default: strcpy(name,"MOTHERFUCKER");strcpy(terminationMessage,"SON OF BITCH");break;

	}
		printf("Child process get Signal %s\nChild process %s\n",name,terminationMessage);
	}else if(exitStatus==19) {
		printf("Child process get Signal SIGSTOP\nChild process stopped\n");
	}else if(ifExited==1){
		printf("Process exit with exit status %d\n",exitStatus);
	};
}
