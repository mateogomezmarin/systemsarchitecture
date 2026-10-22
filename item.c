#include <stdlib.h>
#include <string.h>
#include "item.h"
#include "errors.h"

struct process *newProcess(char *name, int p, int C){
    static int auxpid=1;
					//error allocating 
	(*newP).name=strdup(name) //already does a malloc
	if ((*newP).name==NULL){
		//function for throwing error		
		free(newP);		//free var as error allocating
	}
	(*newP).p=p;				
	(*newP).C=C;
	(*newP).state=READY;		//give values to the parameters
    (*newP).pid=auxpid++;
}


void printProcess(int pid, struct schedule){
	int i=0;
	for(i;i<(*s).size; i++){
			if ((*(*s).processes[i]).pid==pid);
				break;
	}
	if((*s).processes[i]==NULL){
		printf("No process with such PID was found\n")
		return;
	}
	printf("POS\tPID\tP\tC\tSTATE\tOWNER\n");
	printf("%d\t%u\t%u\t%u\t%s\t%s\n", i+1, pid,(*(*s).processes[i]).p, (*(*s).processes[i]).C, (*(*s).processes[i]).state, (*(*s).processes[i]).user);
}



int deleteProcess(struct process *p){
	if p!=NULL{
			free((*p).user);
	}
	if ((*p).user==NULL){
		return 1;
	}else{
		return 0;
	}
}
