#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#include "collection.h"
#include "item.h"
#include "util.h"


Schedule *collection_create_empty(Status *error){ //Function for creating a collection
    if(error == NULL){
        return NULL;
    }

    *error = STATUS_OK;

    Schedule *schedule = malloc(sizeof(Schedule)); //allocate information for schedule

    if (schedule == NULL) { 
        *error = STATUS_ERROR; //error allocating 
        return NULL;
    }

    schedule->items = malloc(INITIAL_CAPACITY * sizeof(Process *)); 
    //allocates room for INITIAL_CAPACITY addresses, where sizeof(Process *) is the size of one address

    if (schedule->items == NULL) {
        free(schedule); //free the memory because of the error allocating the array
        *error = STATUS_ERROR;
        return NULL;
    }
    //give values to the other variables of the shcedule
    schedule->size = 0;
    schedule->capacity = INITIAL_CAPACITY;
    schedule->next_pid = 1;

    return schedule; 
}

void collection_delete(Schedule *collection){
    if (collection == NULL) {
        return;
    }

    for (int i = 0; i < collection->size; i++) {
        item_delete(collection->items[i]); //frees the information of each process of the array
    }

    free(collection->items); //frees the header of the array 
    free(collection); //free the whole schedule
}

int collection_add(Schedule *collection, const char *user, int p, int c, Status *error){
    if (error == NULL) {
        return 0;
    }

    *error = STATUS_OK; //Asume OK

    if (collection == NULL || user == NULL) {
        *error = STATUS_INVALID; //empty inputs
        return 0; //not able to add process
    }

    if (collection->size >= collection->capacity) {
        int new_capacity = collection->capacity * 2;
        // Process **new_items: this is a pointer to an array of pointers to procesess.

        Process **new_items = realloc(collection->items,new_capacity * sizeof(Process *)//reallocate the array of pointers with new capacity when capacity is small
        );

        if (new_items == NULL) {
            *error = STATUS_ERROR; // Memory allocation error
            return 0;
        }

        collection->items = new_items; 
        collection->capacity = new_capacity; //no error, then give the values to the schedule
    }

    Process *item = item_create(collection->next_pid, user, p, c, error); //creates the process

    if (item == NULL) {
        return 0;
    }

    collection->items[collection->size] = item; //enters the process on the array
    collection->size++;
    collection->next_pid++; 

    return 1; //return 1 for succeed introducing the process
}

int collection_exists_same(const Schedule *collection, const char *user, int p, int c){ //function for comparing if a process exists with same characteristics
    if (collection == NULL || user == NULL) {
        return 0; //invalid inputs
    }

    for (int i = 0; i < collection->size; i++) {
        if (strcmp(collection->items[i]->user, user) == 0 && collection->items[i]->p == p && collection->items[i]->C == c) {
        return 1; //return 1 if there is a process with same name, p, and C
        }
    }

    return 0;
}

void collection_remove_by_pid(Schedule *collection, int pid, Status *error){ //function to delete a process searching with its pid
    if (error == NULL) {
        return;
    }

    *error = STATUS_OK;

    if (collection == NULL) {
        *error = STATUS_INVALID; //invalid input
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
        *error = STATUS_INVALID; //no process found with such pid
        return;
    }

    item_delete(collection->items[position]); //calls function that deletes with the position

    for (int i = position; i < collection->size - 1; i++) {
        collection->items[i] = collection->items[i + 1]; //rearanges the array
    }

    collection->size--; //reduce size
    collection->items[collection->size] = NULL; //last process is duplicated, so make the last one null
}

Process *collection_find_by_pid(const Schedule *collection, int pid, int *position, Status *status){ //int *position is a secondary output parameter that helps us 


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

    for (int i = 0; i < collection->size; i++) {
        if (collection->items[i]->pid == pid) {
            if (position != NULL) {
                *position = i;
            }

            return collection->items[i];
        }
    }

    *status = STATUS_INVALID;

    if (position != NULL) {
        *position = -1;
    }

    return NULL;
}

Process *collection_get_by_index(const Schedule *collection, int index, Status *status){ //get a process by its position
    if (status == NULL) {
        return NULL;
    }

    *status = STATUS_OK;

    if (collection == NULL || index < 0 || index >= collection->size) {
        *status = STATUS_INVALID; //invalid inputs
        return NULL;
    }

    return collection->items[index]; 
}

int collection_size(const Schedule *collection){ //auxiliary getter just inc ase
    if (collection == NULL) {
        return 0;
    }

    return collection->size;
}
int collection_is_empty(const Schedule *collection){//check if schedule is empty or not
     if (collection == NULL) {
        return 1; //true =1
    }

    return collection->size == 0; //1 if is empty, 0 if not
}

void collection_clear(Schedule *collection) //schedue still exists but with no processes
{
    if (collection == NULL) {
        return;
    }

    for (int i = 0; i < collection->size; i++) {
        item_delete(collection->items[i]); //clears all items on the collection
        collection->items[i] = NULL; //sets to null the first process as it does not delete
    }

    collection->size = 0;
}

void collection_sort(Schedule *collection,int (*compare)(const Process *, const Process *, Status *),Status *error){ //soritng processes
    if (error == NULL) {
        return;
    }

    *error = STATUS_OK;

    if (collection == NULL || compare == NULL) {
        *error = STATUS_INVALID;
        return;
    }

    if (collection->size == 0) {
        *error = STATUS_INVALID;
        return;
    }

    for (int i = 1; i < collection->size; i++) {
        Process *current = collection->items[i];
        int j = i - 1;

        while (j >= 0) {
            Status compare_status = STATUS_OK;

            int result = compare(collection->items[j],current,&compare_status);

            if (compare_status != STATUS_OK) {
                *error = compare_status; //error when comparing
                return;
            }

            if (result <= 0) { //it is well sorted
                break;
            }

            collection->items[j + 1] = collection->items[j]; 
            j--;
        }

        collection->items[j + 1] = current;
    }
}


void saveScheduleToFile(const Schedule *collection,
                        const char *filename,
                        Status *error){

    if (error == NULL) {
        return;
    }

    *error = STATUS_OK;

    if (collection == NULL || filename == NULL || filename[0] == '\0') {
        errno = EINVAL;
        *error = STATUS_ERROR;
        return;
    }

    FILE *file = fopen(filename, "wb");

    if (file == NULL) { // errno set by fopen
        *error = STATUS_ERROR;
        return;
    }

    int count = collection->size;
    int failed = 0;

    errno = 0;

    if (fwrite(&count, sizeof(count), 1, file) != 1) {
        if (errno == 0) {
            errno = EIO;
        }

        *error = STATUS_ERROR;
        failed = 1;
    }

    for (int i = 0; i < collection->size && !failed; i++) {
        saveProcessToFile(collection->items[i], file, error);

        if (*error != STATUS_OK) {
            failed = 1;
        }
    }

    int saved_errno = errno; // fclose() could overwrite it

    if (fclose(file) != 0 && !failed) {
        *error = STATUS_ERROR; // errno comes from fclose()
        return;
    }

    if (failed) {
        errno = saved_errno;
    }
}

void loadScheduleFromFile(Schedule *collection, const char *filename,Status *error){
    
    if (error == NULL) {
        return;
    }

    *error = STATUS_OK;

    if (collection == NULL || filename == NULL || filename[0] == '\0') {
        errno = EINVAL;
        *error = STATUS_ERROR;
        return;
    }

    FILE *file = fopen(filename, "rb");

    if (file == NULL) { // errno set by fopen
        *error = STATUS_ERROR;
        return;
    }

    int total = 0;

    errno = 0;

    if (fread(&total, sizeof(total), 1, file) != 1) {
        if (ferror(file)) {
            if (errno == 0) {
                errno = EIO;
            }
        } else {
            errno = EBADMSG; // File is too short
        }

        int saved_errno = errno;
        fclose(file);
        errno = saved_errno;

        *error = STATUS_ERROR;
        return;
    }

    if (total < 0) {
        errno = EBADMSG;

        int saved_errno = errno;
        fclose(file);
        errno = saved_errno;

        *error = STATUS_ERROR;
        return;
    }

    Process **loaded = NULL; // Temporary array of loaded processes
    int count = 0;
    int failed = 0;

    for (int i = 0; i < total && !failed; i++) {

        if (count == INT_MAX ||
            collection->next_pid > INT_MAX - count - 1) {
            errno = EOVERFLOW;
            *error = STATUS_ERROR;
            failed = 1;
            break;
        }

        Process **bigger = realloc(
            loaded,
            (size_t)(count + 1) * sizeof(Process *)
        );

        if (bigger == NULL) {
            errno = ENOMEM;
            *error = STATUS_ERROR;
            failed = 1;
            break;
        }

        loaded = bigger;

        Process *item = loadProcessFromFile(
            file,
            collection->next_pid + count,
            error
        );

        if (item == NULL) {
            failed = 1;
            break;
        }

        loaded[count++] = item;
    }

    if (!failed) {
        errno = 0;

        if (fgetc(file) != EOF) {
            errno = EBADMSG; // Extra bytes after the last process
            *error = STATUS_ERROR;
            failed = 1;
        } else if (ferror(file)) {
            if (errno == 0) {
                errno = EIO;
            }

            *error = STATUS_ERROR;
            failed = 1;
        }
    }

    int saved_errno = errno;

    if (fclose(file) != 0 && !failed) {
        if (errno == 0) {
            errno = EIO;
        }

        *error = STATUS_ERROR;
        failed = 1;
    }

    if (failed) {
        saved_errno = errno;

        for (int i = 0; i < count; i++) {
            item_delete(loaded[i]);
        }

        free(loaded);

        errno = saved_errno;
        *error = STATUS_ERROR;
        return; // Schedule remains unchanged
    }

    if (count > collection->capacity) {
        Process **bigger = realloc(
            collection->items,
            (size_t)count * sizeof(Process *)
        );

        if (bigger == NULL) {
            errno = ENOMEM;
            *error = STATUS_ERROR;

            saved_errno = errno;

            for (int i = 0; i < count; i++) {
                item_delete(loaded[i]);
            }

            free(loaded);

            errno = saved_errno;
            return;
        }

        collection->items = bigger;
        collection->capacity = count;
    }

    collection_clear(collection); // Does not reset next_pid

    for (int i = 0; i < count; i++) {
        collection->items[i] = loaded[i];
    }

    collection->size = count;
    collection->next_pid += count;

    free(loaded); // Processes now belong to the schedule
}
