#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#include "collection.h"
#include "item.h"
#include "util.h"


Schedule *collection_create_empty(Status *error){
    if(error == NULL){
        return NULL;
    }

    *error = STATUS_OK;

    Schedule *schedule = malloc(sizeof(Schedule));//malloc for the scheudle

    if (schedule == NULL) {
        *error = STATUS_ERROR;
        return NULL;
    }

    schedule->items = malloc(INITIAL_CAPACITY * sizeof(Process *)); //alocates with initial capacity
    

    if (schedule->items == NULL) {
        free(schedule);
        *error = STATUS_ERROR;
        return NULL;
    }

    schedule->size = 0;
    schedule->capacity = INITIAL_CAPACITY;
    schedule->next_pid = 1; //set basic variables

    return schedule;
}

void collection_delete(Schedule *collection){ //delete the whole schedule, including freeing the schedule
    if (collection == NULL) {
        return;
    }

    for (int i = 0; i < collection->size; i++) {
        item_delete(collection->items[i]); //free all processes and user names
    }

    free(collection->items); //free the array header
    free(collection); //free the schedule
}

int collection_add(Schedule *collection, const char *user, int p, int c, Status *error){ //add a process
    if (error == NULL) {
        return 0;
    }

    *error = STATUS_OK; //Asume OK

    if (collection == NULL || user == NULL) {
        *error = STATUS_INVALID;
        return 0;
    }

    if (collection->size >= collection->capacity) {
        int new_capacity = collection->capacity * 2; //bigger space for the array of pointers
        
        Process **new_items = realloc(collection->items, new_capacity * sizeof(Process *)); //reallocate memory for the array

        if (new_items == NULL) {
            *error = STATUS_ERROR; // memory allocation error
            return 0;
        }

        collection->items = new_items; //all the processes on the array of the schedule
        collection->capacity = new_capacity; //give the new value of its capacity
    }

    Process *item = item_create(collection->next_pid, user, p, c, error); //creates a new process to add it

    if (item == NULL) {
        return 0;
    }



    collection->items[collection->size] = item; //adds the new process
    collection->size++;
    collection->next_pid++; //increase values

    return 1;
}

int collection_exists_same(const Schedule *collection, const char *user, int p, int c){ //compare if a process exists with same vars
    if (collection == NULL || user == NULL) {
        return 0; //invalid inputs
    }

    for (int i = 0; i < collection->size; i++) {
        if (strcmp(collection->items[i]->user, user) == 0 && collection->items[i]->p == p && collection->items[i]->C == c) {
        return 1; //1=true, there is a process with same vars
        }
    }

    return 0; //not found, 0=false
}

void collection_remove_by_pid(Schedule *collection, int pid, Status *error){ 
    if (error == NULL) {
        return;
    }

    *error = STATUS_OK;

    if (collection == NULL) {
        *error = STATUS_INVALID; //invalid input (empty schedule)
        return;
    }

    int position = -1;

    for (int i = 0; i < collection->size; i++) {
        if (collection->items[i]->pid == pid) {
            position = i;
            break;
        }
    }

    if (position == -1) {
        *error = STATUS_INVALID; //not found so invalid input
        return;
    }

    item_delete(collection->items[position]); //calls the function for deleting with position

    for (int i = position; i < collection->size - 1; i++) {
        collection->items[i] = collection->items[i + 1]; //rearange the array
    }

    collection->size--; //reduces size
    collection->items[collection->size] = NULL; //last process duplicated, then it eliminates the last one
}

Process *collection_find_by_pid(const Schedule *collection, int pid, int *position, Status *status){ 


    if (status == NULL) {
        return NULL;
    }

    if (position != NULL) {
    *position = -1;
    }

    *status = STATUS_OK;

    if (collection == NULL) {
        *status = STATUS_INVALID;
        return NULL;
    }

    for (int i = 0; i < collection->size; i++) { //compare witha loop and keep the position where it has the same pid
        if (collection->items[i]->pid == pid) {
            if (position != NULL) {
                *position = i;
            }

            return collection->items[i]; //returns the process with that pid
        }
    }

    *status = STATUS_INVALID;

    if (position != NULL) {
        *position = -1;
    }

    return NULL; // not found that pid
}

Process *collection_get_by_index(const Schedule *collection, int index, Status *status){ //get process by its position
    if (status == NULL) { 
        return NULL;
    }

    *status = STATUS_OK;

    if (collection == NULL || index < 0 || index >= collection->size) { //invalid inputs
        *status = STATUS_INVALID;
        return NULL;
    }

    return collection->items[index]; 
}

int collection_size(const Schedule *collection){ //aux getter
    if (collection == NULL) {
        return 0;
    }

    return collection->size;
}
int collection_is_empty(const Schedule *collection){ //check if is empty or not
     if (collection == NULL) {
        return 1;//true, is empty
    }

    return collection->size == 0; //1 if empty, 0 if not 
}

void collection_clear(Schedule *collection){//delete all processes but not the schedule
    if (collection == NULL) {
        return;
    }

    for (int i = 0; i < collection->size; i++) {//delete all processes
        item_delete(collection->items[i]);
        collection->items[i] = NULL;
    }

    collection->size = 0;//set to 0 the size
}

void collection_sort(Schedule *collection,int (*compare)(const void *, const void *),Status *error){
    if (error == NULL) {
        return;
    }

    *error = STATUS_OK;

    if (collection == NULL || compare == NULL || collection->size == 0) {
        *error = STATUS_INVALID;
        return;
    }

    //sort with qsort
    qsort(collection->items, (size_t)collection->size,
          sizeof(Process *), compare);
}


void saveScheduleToFile(const Schedule *collection,const char *filename,Status *error){//Part 2 function schedule to a file

    if (error == NULL) {
        return;
    }

    *error = STATUS_OK;

    if (collection == NULL || filename == NULL || filename[0] == '\0') {//invalid inputs
        errno = EINVAL;
        *error = STATUS_ERROR;
        return;
    }

    FILE *file = fopen(filename, "wb");

    if (file == NULL) { // errno set by fopen
        *error = STATUS_ERROR;//error opening the file
        return;
    }

    int count = collection->size;
    int failed = 0;

    errno = 0;

    if (fwrite(&count, sizeof(count), 1, file) != 1) { //if error writing the size
        if (errno == 0) {
            errno = EIO;
        }

        *error = STATUS_ERROR;
        failed = 1;//error 
    }

    for (int i = 0; i < collection->size && !failed; i++) { //brakes the loop when there is an error on saveprocess
        saveProcessToFile(collection->items[i], file, error); //saves all the processes

        if (*error != STATUS_OK) {//when there is an error when writing the process
            failed = 1; //breaks the loop
        }
    }

    int saved_errno = errno; // fclose() could overwrite it

    if (fclose(file) != 0 && !failed) {//error closing
        *error = STATUS_ERROR; // errno comes from fclose()
        return;
    }

    if (failed) { 
        errno = saved_errno;
    }
}

void loadScheduleFromFile(Schedule *collection, const char *filename,Status *error){//Part 2 function, file to schedule

    if (error == NULL) {
        return;
    }

    *error = STATUS_OK;

    if (collection == NULL || filename == NULL || filename[0] == '\0') {
        errno = EINVAL;
        *error = STATUS_ERROR; //invalid inputs
        return;
    }

    FILE *file = fopen(filename, "rb"); //opnes the file for binary reading 

    if (file == NULL) { // errno set by fopen
        *error = STATUS_ERROR; //error when opening
        return;
    }

    int total = 0;

    errno = 0;

    if (fread(&total, sizeof(total), 1, file) != 1) {//check error when reading 
        if (ferror(file)) { //check if is an error when reading the file
            if (errno == 0) {
                errno = EIO; //other error
            }
        } else {
            errno = EBADMSG; // reached the end of the file too soon
        }

        int saved_errno = errno; //errno is kept as when closing errno may be lost
        fclose(file);
        errno = saved_errno;

        *error = STATUS_ERROR;
        return;
    }

    if (total < 0) { //cant be less than 0
        errno = EBADMSG;

        int saved_errno = errno;
        fclose(file);
        errno = saved_errno;//again errno may be lost

        *error = STATUS_ERROR;
        return;
    }

    Process **loaded = NULL; // Temporary array of loaded processes
    int count = 0;
    int failed = 0;

    for (int i = 0; i < total && !failed; i++) { //loop for writing all processes

        if (count == INT_MAX ||collection->next_pid > INT_MAX - count - 1) { //error of the inpts
            errno = EOVERFLOW;
            *error = STATUS_ERROR;
            failed = 1;
            break;
        }

        Process **bigger = realloc(loaded,(size_t)(count + 1) * sizeof(Process *)); //reallocate space for a bigger size arrays

        if (bigger == NULL) {
            errno = ENOMEM;
            *error = STATUS_ERROR; //error allocating 
            failed = 1;
            break;
        }

        loaded = bigger; //was correctly allocated so we keep it for giving it to the array of items when finishing with no errors

        Process *item = loadProcessFromFile(file,collection->next_pid + count,error); //

        if (item == NULL) {
            failed = 1;
            break;
        }

        loaded[count++] = item;//put in the list
    }

    if (!failed) { //if no fail
        errno = 0;

        if (fgetc(file) != EOF) {//there was a fail when 
            errno = EBADMSG; // Extra bytes after the last process written so the file was not read until the end
            *error = STATUS_ERROR;
            failed = 1; 
        } else if (ferror(file)) {//check if really end of file or error reading 
            if (errno == 0) {
                errno = EIO; //error reading 
            }

            *error = STATUS_ERROR;
            failed = 1;
        }
    }

    int saved_errno = errno; //once again keep the error

    if (fclose(file) != 0 && !failed) { //try to close the file
        if (errno == 0) {
            errno = EIO;
        }

        *error = STATUS_ERROR;
        failed = 1;
    }

    if (failed) { //if failed when writting
        saved_errno = errno;

        for (int i = 0; i < count; i++) {
            item_delete(loaded[i]); //free all processes of the temporary array
        }

        free(loaded);//free the temporary array

        errno = saved_errno;
        *error = STATUS_ERROR;
        return; // Schedule remains unchanged
    }

    if (count > collection->capacity) { //if the written capacity is bigger than the shcedules capacity 
        Process **bigger = realloc(collection->items,(size_t)count * sizeof(Process *)); //realloc with the new capacity that was written 

        if (bigger == NULL) {
            errno = ENOMEM; //error allocating 
            *error = STATUS_ERROR;

            saved_errno = errno;

            for (int i = 0; i < count; i++) {
                item_delete(loaded[i]); //free because of error the array
            }

            free(loaded); //free array

            errno = saved_errno;
            return;
        }

        collection->items = bigger; //sets the values to the schedule 
        collection->capacity = count; 
    }

    collection_clear(collection); // Does not reset next_pid but clears previus processes

    for (int i = 0; i < count; i++) {
        collection->items[i] = loaded[i]; //give values to all the array
    }

    collection->size = count;
    collection->next_pid += count; //increase values

    free(loaded); // Processes now belong to the schedule so we free the temporary array
}
