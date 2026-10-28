#ifndef COLLECTION_H
#define COLLECTION_H

#define INITIAL_CAPACITY 10

#include "item.h"
#include "util.h"



typedef struct schedule {
    Process **items; //It points to the first element of an array whose elements are Process * pointers.
    int size;
    int capacity;
    int next_pid; //next process identifier
} Schedule;

Schedule *collection_create_empty(Status *error);
void collection_delete(Schedule *collection);

int collection_add(Schedule *collection, const char *user, int p, int c, Status *error);//it will call item_create
int collection_exists_same(const Schedule *collection, const char *user, int p, int c);
void collection_remove_by_pid(Schedule *collection, int pid, Status *error);
Process *collection_find_by_pid(const Schedule *collection, int pid, int *position, Status *error); //int *position is a secondary output parameter:

Process *collection_get_by_index(const Schedule *collection, int index,Status *error);

int collection_size(const Schedule *collection);
int collection_is_empty(const Schedule *collection);

void collection_clear(Schedule *collection); //does not reset the PID counter

void collection_sort(Schedule *collection, int (*compare)(const void *, const void *), Status *error);

void loadScheduleFromFile(Schedule *schedule, const char *filename, Status *error);
void saveScheduleToFile(const Schedule *schedule, const char *filename, Status *error);
#endif


//Graph made by Claude AI depicting the memory display
/*
collection_t (one block)
┌────────────────┐
│ items    ──────┼──────┐
│ size = 3       │      │
│ capacity = 4   │      ▼
│ next_pid = 6   │   array of pointers (one block, 4 slots)
└────────────────┘   ┌───────┬───────┬───────┬───────┐
                     │ [0]   │ [1]   │ [2]   │ [3]   │
                     │  ●    │  ●    │  ●    │ NULL  │
                     └──┼────┴──┼────┴──┼────┴───────┘
                        │       │       │
                        ▼       ▼       ▼
                   Process   Process   Process      (three separate blocks)
                   pid = 2   pid = 5   pid = 12
                   p, C      p, C      p, C
                   state     state     state
                   user ●    user ●    user ●
                        │       │       │
                        ▼       ▼       ▼
                   "Annie    "Maria    "Grace       (three separate text blocks)
                    Easley"   Mitchell" Hopper"
*/
