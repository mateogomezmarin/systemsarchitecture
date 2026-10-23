#include <stdlib.h>
#include <string.h>
#include "item.h"
#include "errors.h"

struct process newProcess(char *name, int p, int C, int *error){
    *error=SUCCESS;
	struct process newP; //no malloc as the malloc is done by the function add to schedule/create new scheudle
	static int auxpid=1;
	newP.name=strdup(name) //already does a malloc
	if (newP.name==NULL){
		*error=ERROR_ALLOCATING;		
		free(newP);		//free var as error allocating
	}
	newP.p=p;				
	newP.C=C;
	newP.state=READY;		//give values to the parameters
    newP.pid=auxpid++;		
}


void printheader(void){
	printf("POS\tPID\tP\tC\tSTATE\tOWNER\n");
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
	printheader();
	printf("%d\t%u\t%u\t%u\t%s\t%s\n", i+1, pid,(*(*s).processes[i]).p, (*(*s).processes[i]).C, (*(*s).processes[i]).state, (*(*s).processes[i]).user);
}



int deleteProcess(struct process *p, int *error){
	*error=SUCCESS;
	if p!=NULL{
			free((*p).user);
	}
	if ((*p).user==NULL){
		return 1;
	}else{
		*error=ERROR_DELETING;
		return 0;
	}
}
