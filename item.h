#ifndef ITEM_H
#define ITEM_H

#include "util.h"

#include <stdio.h>

#define MAX_USER_LEN 255

/* Item is one process in the schedule.
   Every function that takes a Status *error must receive a non-NULL pointer.
   A returned value is only valid when *error == STATUS_OK. */

typedef enum states {
    READY,
    RUNNING,
    STOPPED,
    TERMINATED
} States; //defined states

//Typedef allows us to create an alias for a type, in this case Process

typedef struct process {
    int pid;       // > 0, unique, never reused, must check
    char *user;    // owner's name, owned by the Process (freed in item_delete)
    int p;         // priority, > 0 (must check)
    int C;         // CPU execution time, > 0 (must check)
    States state;
} Process; //We can declare a variable as: Process p1;

//Returns the new Process, or NULL on failure. The caller must free it with item_delete
//user is const because item_create makes its own copy
Process *item_create(int pid, const char *user, int p, int C, Status *error);

void item_delete(Process *item); //The caller should set its pointer to NULL afterwards

//Getters, with const so we don't change the Processes
int item_get_pid(const Process *item, Status *error);
const char *item_get_user(const Process *item, Status *error); //Owned by the item, the caller must not free it
int item_get_p(const Process *item, Status *error);
int item_get_C(const Process *item, Status *error);
States item_get_state(const Process *item, Status *error);

void item_set_state(Process *item, States state, Status *error);

const char *item_state_to_string(States state, Status *error); //Read only string literal, never free it

void item_print_header(void); //Prints only the column titles: POS PID P C STATE OWNER
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