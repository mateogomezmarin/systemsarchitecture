#ifndef COLLECTION_H
#define COLLECTION_H

#define INITIAL_CAPACITY 10

#include "item.h"
#include "util.h"



typedef struct schedule {
    Process **items; //It points to first element of an array of pointers of processes
    int size;
    int capacity;
    int next_pid; //next process identifier
} Schedule;

Schedule *collection_create_empty(Status *error); //creates new collection
void collection_delete(Schedule *collection); //delete all

int collection_add(Schedule *collection, const char *user, int p, int c, Status *error);//add processes to array
int collection_exists_same(const Schedule *collection, const char *user, int p, int c);//compare vars with al processes
void collection_remove_by_pid(Schedule *collection, int pid, Status *error);
Process *collection_find_by_pid(const Schedule *collection, int pid, int *position, Status *error); 

Process *collection_get_by_index(const Schedule *collection, int index,Status *error);

int collection_size(const Schedule *collection);
int collection_is_empty(const Schedule *collection);

void collection_clear(Schedule *collection); //does not reset the PID counter

void collection_sort(Schedule *collection, int (*compare)(const void *, const void *), Status *error);
//Part2 functions
void loadScheduleFromFile(Schedule *schedule, const char *filename, Status *error);
void saveScheduleToFile(const Schedule *schedule, const char *filename, Status *error);
#endif


