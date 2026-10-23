#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <collection.h>

int scheduleCheck=0; //constant that says if an schedule exists or not, 0 if not, 1 if yes


struct schedule *newShedule(char *name, int p, int C, int *error){
	*error=SUCCESS;
    struct schedule *newS=(struct schedule *)malloc(sizeof(struct schedule));
    if (newS==NULL){
        *error=ERROR_ALLOCATING;
        return null;
    }
    (*newS).processes=(struct processes *)malloc(sizeof(struct process))
    if ((*newS).processes==NULL){
	    free(newS);
	    *error=ERROR_ALLOCATING;
    }else{
	    (*newS).processes[0]=newProcess(name, p, C, error);
	    (*newS).size=1;
    }
    scheduleCheck=1;
    return newS;
}


int existInSchedule(struct schedule *sched, struct process *proc, int *error) {
    *error = SUCCESS;
	if (sched == NULL || proc == NULL || (*sched).processes == NULL) { 
		*error = ERR_INVALID_INPUT;
        return 0;
    }

    for (size_t i=0;i<(*sched).size;i++){//compare each pos of the array with each process
        if(((*sched).processes[i]).p==(*proc).p && ((*sched).processes[i]).C==(*proc).C && strcmp(((*sched).processes[i]).name,(*proc).name)==0){
            return 1; //found same process
        }
    }
    return 0;   //not found any process equal
}


int addLast(struct schedule *sched, char *name, int p, int C, int *error) {
    *error = SUCCESS;
    if (sched == NULL || name == NULL || p <= 0 || C <= 0) {
        *error = ERR_INVALID_INPUT;
        return 0;
    }
	struct process *newarray = (struct process *)realloc((*sched).processes, sizeof(struct process)*((*sched).size+1));
	if (newarray==NULL){
		*error = ERR_ALLOCATING;
		return 0;
	}else{
		(*sched).processes=newarray;
		(*sched).processes[(*sched).size]=newProcess(name, p, c, error);
		(*sched).size=(*sched).size+1;
	}
    return 1;
}



struct process *sort_schedule(struct schedule *s, int (*compare)(const void *, const void *), int *error){
    *error = SUCCESS;
    if (s == NULL || s->processes == NULL || s->size == 0) {
        *error = ERR_NO_ITEMS;
        return s;
    }
    if (s->size != 0){
        qsort(s->processes, s->size, sizeof(struct process),compare);
    }else{
        *error = ERR_NO_ITEMS;
    }
return s;
}

int compare_pid(const void *a, const void *b) {
    const struct process *p1 = (const struct process *)a;
    const struct process *p2 = (const struct process *)b;
    return compareIntegers(p1->pid, p2->pid);
}

int compare_p(const void *a, const void *b) {
    const struct process *p1 = (const struct process *)a;
    const struct process *p2 = (const struct process *)b;
    return compareIntegers(p1->p, p2->p);
}

int compare_C(const void *a, const void *b) {
    const struct process *p1 = (const struct process *)a;
    const struct process *p2 = (const struct process *)b;
    return compareIntegers(p1->C, p2->C);



int compare_name(const void *a, const void *b) {
    const struct process *p1 = (const struct process *)a;
    const struct process *p2 = (const struct process *)b;

    if (p1->name == NULL) return -1;
    if (p2->name == NULL) return 1;

    return strcmp(p1->name, p2->name);
}


int quitfromschedule(struct schedule *s, int pid, int *error){
    int i, j;

    if (s == NULL || pid <= 0) {
        *error = ERR_NO_ITEMS;
        return 0;
    }

    for (i = 0; i < (*s).size; i++) {
        if ((*(*s).processes[i]).pid == pid) {
            deleted = (*s).processes[i];
            deleteProcess(&(*s).processes[i]);

            for (j = i; j <(*s).size - 1; j++)
                (*s).processes[j] = (*s).processes[j + 1];

            (*s).size=(*s).size-1;
            return 1;       //You dont have to free the memory of the process, because you free only if it is in a double pointer array.

        }
    }
    return 0;
}


int deleteSchedule(struct schedule *s, int *error){
    *error = SUCCESS;
    if (s==NULL){
        *error = ERR_NO_ITEMS;
        return 0;
    }
    for(int i = 0; i<(*s).size;i++){
        if ((*s).processes[i]!=NULL){
            deleteProcess(&(*s).processes[i]);
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
    printheader();
    int i=0;
    for (i;i<(*s).size;i++){
        printf("%d\t%u\t%u\t%u\t%s\t%s\n", i+1, (*(*s).processes[i]).pid,(*(*s).processes[i]).p, (*(*s).processes[i]).C, (*(*s).processes[i]).state, (*(*s).processes[i]).user);
    }
}




