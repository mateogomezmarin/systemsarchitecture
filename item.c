#include <stdlib.h>
#include <string.h>
#include "item.h"
#include "errors.h"

struct process *newProcces(unsigned int pid, char *name, int p, int C){
    static int auxpid=1;
	struct process *newP=(struct process *)malloc(sizeof(struct process)); //allocate the process
	if (newP)==NULL{
		//function error allocating
		return null;				//error allocating 
	}else{
		(*newP).name=strdup(name)
		if ((*newP).name==NULL){
			//function for throwing error
free(newP);			//free var as error allocating
}
		(*newP).p=p;				
		(*newP).C=C;
		(*newP).state=READY;		//give values to the parameters
        (*newP).pid=auxpid++;
	}
}

