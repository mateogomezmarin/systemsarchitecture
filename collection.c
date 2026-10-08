#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <collection.h>



struct schedule *newShedule(struct process actualProcess){
	struct schedule *newS=(strcut schedule *)malloc(sizeof(struct schedule));
if (newS==NULL){
return null;
//function error allocating
}
(*newS	).processes=(struct processes *)malloc(sizeof(struct process)
if ((*newS).processes==NULL){
	free(newS);
	//function error allocating
}else{
	(*newS).processes[0]=actualProcess;
	(*newS).size=1;
}
return newS;
}

int scheduleCheck // Funtion that needs to be implemented

//Returns 0 if schedule already exists

//1 if need to create schedule already



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

void addLast(struct schedule *sched, struct process *proc){
	struct process *newarray = (struct process *)realloc((*sched).processes, sizeof(struct process)*((*sched).size+1));
	if (newarray==NULL){
		//error allocating function
		free()
		return
	}else{
		(*sched).processes=newarray;
		(*sched).procceses[(*sched).size];
		(*sched).size=(*sched).size+1;
	}
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


	
