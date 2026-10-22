#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <collection.h>

int scheduleCheck=0; // Funtion that needs to be implemented

//Returns 0 if schedule already exists

//1 if need to create schedule already


struct schedule *newShedule(char *name, int p, int C){
	struct schedule *newS=(strcut schedule *)malloc(sizeof(struct schedule));
if (newS==NULL){
return null;
//function error allocating
}
(*newS).processes=(struct processes *)malloc(sizeof(struct process))
if ((*newS).processes==NULL){
	free(newS);
	//function error allocating
}else{
	(*newS).processes[0]=newProcess(name, p, C);
	(*newS).size=1;
}
scheduleCheck=1;
return newS;
}


int existInSchedule(struct schedule *sched, struct process *proc){
	if (sched == NULL || proc == NULL || (*sched).processes == NULL) { 
		//error
        return 0;
    }

    for (size_t i=0;i<(*sched).size;i++){//compare each pos of the array with each process
        if(((*sched).processes[i]).p==(*proc).p && ((*sched).processes[i]).C==(*proc).C && strcmp(((*sched).processes[i]).name,(*proc).name)==0){
                    return 1; //found same process
        }
    }
    return 0;   //not found any process equal
}


int addLast(struct schedule *sched, char *name, int p, int C){
	struct process *newarray = (struct process *)realloc((*sched).processes, sizeof(struct process)*((*sched).size+1));
	if (newarray==NULL){
	//error allocating function
		free()
		return 0;
	}else{
		(*sched).processes=newarray;
		(*sched).processes[(*sched).size]=newProcess(name, p, c);
		(*sched).size=(*sched).size+1;
	}
    return 1;
}



struct collection sort_collection(struct collection col, int (*compare)(const void *, const void *), int *error){
    *error = SUCCESS;
    if (col.number_occupied != 0){
        qsort(col.array, col.number_occupied, sizeof(struct item),compare);
    }
    
    else
        *error = ERR_NO_ITEMS;
return col;
}




//FUNCTIONS FOUND IN AULAGLOBAL to be implemented with qsort()




int compare_id(const void *pa, const void *pb){
    struct item *a = (struct item *) pa;
    struct item *b = (struct item *) pb;
    if(a->id == b->id){
        return 0;
    }
    if(a->id < b->id){
        return -1;
    }
    return 1;
}



int compare_name(const void *pa, const void *pb){
    struct item *a = (struct item *) pa;
    struct item *b = (struct item *) pb;

    return (strcmp(a->data.name, b->data.name));

}


int quitfromschedule(struct schedule *s, int pid){
    int i, j;
    struct process *deleted;

    if (s == NULL || pid <= 0)
        return 0    ; //set error for empty schedule

    for (i = 0; i < (*s).size; i++) {
        if ((*(*s).processes[i]).pid == pid) {
            deleted = (*s).processes[i];

            for (j = i; j <(*s).size - 1; j++)
                (*s).processes[j] = (*s).processes[j + 1];

            (*s).size=(*s).size-1;
            free(deleted);
            return 1;

        }
    }
    return NULL;
}


int deleteSchedule(struct schedule *s){
    if (s==NULL){
            //function error
            return 0;
    }
    for(int i = 0; i<(*s).size;i++){
        if ((*s).processes[i]!=NULL){
            free((*(*s).processes[i]).name);
            free((*s).processes[i]);
        }
    }
    free((*s).processes);
    if ((*s).processes==NULL){
        free(s);
    }
    scheduleCheck=0;
    return 1;
    
}

int printSchedule(struct schedule *s){ 
    if (s==NULL){
        return 0;
    }
    printf("POS\tPID\tP\tC\tSTATE\tOWNER\n");
	
    int i=0;
    for (i;i<(*s).size;i++){
        printf("%d\t%u\t%u\t%u\t%s\t%s\n", i+1, (*(*s).processes[i]).pid,(*(*s).processes[i]).p, (*(*s).processes[i]).C, (*(*s).processes[i]).state, (*(*s).processes[i]).user);;
    }

}



