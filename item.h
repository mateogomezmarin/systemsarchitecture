#ifndef ITEM_H
#define ITEM_H

#include "util.h"

#include <stdio.h>

#define MAX_USER_LEN 255

typedef enum states {
    READY,
    RUNNING,
    STOPPED,
    TERMINATED
} States; //defined states


typedef struct process {
    int pid;       
    char *user;    
    int p;         
    int C;         
    States state;
} Process; 


Process *item_create(int pid, const char *user, int p, int C, Status *error);

void item_delete(Process *item); 

//Getters, with const so we don't change the Processes
int item_get_pid(const Process *item, Status *error);
const char *item_get_user(const Process *item, Status *error); 
int item_get_p(const Process *item, Status *error);
int item_get_C(const Process *item, Status *error);
States item_get_state(const Process *item, Status *error);

void item_set_state(Process *item, States state, Status *error);

const char *item_state_to_string(States state, Status *error); 

void item_print_header(void); 
int item_print(const Process *item, int position, Status *error); //Prints a row for one process, 0 on success, -1 on failure

int item_same_data(const Process *item, const Process *item2, Status *error); //1 if identical data, 0 if different or invalid

//Return -1, 0 or 1 like strcmp
int item_cmp_pid(const void *pa, const void *pb);
int item_cmp_priority(const void *pa, const void *pb);
int item_cmp_c(const void *pa, const void *pb);
int item_cmp_user(const void *pa, const void *pb);

void saveProcessToFile(const Process *proc, FILE *file, Status *error);

Process *loadProcessFromFile(FILE *file, int pid, Status *error);

#endif
