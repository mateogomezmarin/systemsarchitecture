#define _GNU_SOURCE // Needed for strdup() under -std=c11
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "util.h"
#include "item.h"


Process *item_create(int pid, const char *user, int p, int C, Status *error) { //returns a pointer of a process

    if (pid <= 0 || p <= 0 || C <= 0 ||
        user == NULL || user[0] == '\0') {
        *error = STATUS_INVALID; //invalid inputs
        return NULL;
    }

    Process *item = malloc(sizeof(Process)); //Malloc

    if (item == NULL) {
        *error = STATUS_ERROR;//error allocating
        return NULL;
    }

    item->user = strdup(user); //already mallocs de name in the process

    if (item->user == NULL) {
        free(item); //error allocating so free the process
        *error = STATUS_ERROR;
        return NULL;
    }

    item->pid = pid;
    item->p = p;
    item->C = C;
    item->state = READY;

    *error = STATUS_OK;
    return item; 
}

void item_delete(Process *item) //frees everything of the process
{
    if (item == NULL) {
        return;
    }

    free(item->user); //free the name
    free(item);//free the pointer of the process
}

int item_get_pid(const Process *item, Status *error) { //aux getters
    if (item == NULL) {
        *error = STATUS_INVALID;
        return -1;
    }
    *error = STATUS_OK;
    return item->pid;
}

const char *item_get_user(const Process *item, Status *error) {//gets the name of the user
    if (item == NULL) {
        *error = STATUS_INVALID;
        return NULL;
    }

    *error = STATUS_OK;
    return item->user;
}

int item_get_p(const Process *item, Status *error) {
    if (item == NULL) {
        *error = STATUS_INVALID;
        return -1;
    }

    *error = STATUS_OK;
    return item->p;
}

int item_get_C(const Process *item, Status *error) {
    if (item == NULL) {
        *error = STATUS_INVALID;
        return -1;
    }

    *error = STATUS_OK;
    return item->C;
}

States item_get_state(const Process *item, Status *error) {
    if (item == NULL) {
        *error = STATUS_INVALID;
        return (States)-1;
    }

    *error = STATUS_OK;
    return item->state;
}

void item_set_state(Process *item, States state, Status *error) {
    if (item == NULL || (state != READY && state != RUNNING &&
                         state != STOPPED && state != TERMINATED)) {
        *error = STATUS_INVALID;
        return;
    }

    item->state = state;
    *error = STATUS_OK;
}

const char *item_state_to_string(States state, Status *error) {
    switch (state) {
        case READY:
            *error = STATUS_OK;
            return "READY";

        case RUNNING:
            *error = STATUS_OK;
            return "RUNNING";

        case STOPPED:
            *error = STATUS_OK;
            return "STOPPED";

        case TERMINATED:
            *error = STATUS_OK;
            return "TERMINATED";

        default:
            *error = STATUS_INVALID;
            return NULL;
    }
}

void item_print_header(void) { //prints the head
    printf("%5s %5s %5s %5s %-10s %-20s\n", "POS", "PID", "P", "C", "STATE", "OWNER");
}



int item_print(const Process *item, int position, Status *error) { //print and search by its position
    if (item == NULL || position <= 0) {
        *error = STATUS_INVALID;
        return -1;
    }

    Status state_error;
    const char *state = item_state_to_string(item->state, &state_error);

    if (state_error != STATUS_OK) {
        *error = state_error;
        return -1;
    }

    printf("%5d %5d %5d %5d %-10s %-20s\n",position,item->pid,item->p,item->C,state,item->user);

    *error = STATUS_OK;
    return 0;
}

int item_same_data(const Process *item, const Process *item2, Status *error) {//compares
    if (item == NULL || item2 == NULL) {//error of the inputs
        *error = STATUS_INVALID;
        return 0;
    }

    *error = STATUS_OK;

    //Both pointers are already checked, so the fields can be read directly
    if (item->pid != item2->pid ||item->p != item2->p ||item->C != item2->C ||item->state != item2->state ||strcmp(item->user, item2->user) != 0) {
        return 0;
    }

    return 1; //1 if both processes have identical data, 0 if does not
}

//functions for qsort of comparisons
int item_cmp_pid(const void *pa, const void *pb){
    const Process *a = *(const Process * const *)pa; //desreference from the double pointer
    const Process *b = *(const Process * const *)pb;

    if (a->pid < b->pid) return -1;
    if (a->pid > b->pid) return 1;
    return 0;
}

int item_cmp_priority(const void *pa, const void *pb){
    const Process *a = *(const Process * const *)pa;
    const Process *b = *(const Process * const *)pb;

    if (a->p < b->p) return -1;
    if (a->p > b->p) return 1;
    return item_cmp_pid(pa, pb);  //if they are the same, also by pid
}

int item_cmp_c(const void *pa, const void *pb){
    const Process *a = *(const Process * const *)pa;
    const Process *b = *(const Process * const *)pb;

    if (a->C < b->C) return -1;
    if (a->C > b->C) return 1;
    return item_cmp_pid(pa, pb);
}

int item_cmp_user(const void *pa, const void *pb){
    const Process *a = *(const Process * const *)pa;
    const Process *b = *(const Process * const *)pb;

    int result = strcmp(a->user, b->user);
    if (result != 0) return result;
    return item_cmp_pid(pa, pb);
}


//part 2 functions
static int item_write_all(FILE *file, const void *data, size_t size){ //writes information and returns an error depending on what happens
    errno = 0;

    if (fwrite(data, 1, size, file) != size) { //if error writing 
        if (errno == 0) {//error
            errno = EIO;
        }
        return 0;//could not write
    }

    return 1;//could write
}

static int item_read_all(FILE *file, void *data, size_t size){ //same but reading 
    errno = 0;

    if (fread(data, 1, size, file) == size) { //could read
        return 1;
    }

    if (ferror(file)) { //error reading 
        if (errno == 0) {
            errno = EIO;//check if error reading
        }
    } else {
        errno = EBADMSG; // end of file soon
    }

    return 0;
}

void saveProcessToFile(const Process *proc, FILE *file, Status *error){ //process for saving a process
    if (error == NULL) {
        return;
    }

    *error = STATUS_OK;

    if (proc == NULL || file == NULL || proc->user == NULL) { //invalid inputs
        errno = EINVAL;
        *error = STATUS_ERROR;
        return;
    }

    size_t length = strlen(proc->user); 

    if (length == 0 || length > MAX_USER_LEN) {
        errno = EINVAL;
        *error = STATUS_ERROR; //check lenght is correct
        return;
    }

    int p = proc->p;
    int c = proc->C;
    int state = proc->state;
    int user_length = (int)length;
    //check if error when writing with the previus function
    if (!item_write_all(file, &p, sizeof(p)) ||!item_write_all(file, &c, sizeof(c)) ||!item_write_all(file, &state, sizeof(state)) ||!item_write_all(file, &user_length, sizeof(user_length)) ||!item_write_all(file, proc->user, length)) {
        *error = STATUS_ERROR; // errno set by item_write_all
    }
}

Process *loadProcessFromFile(FILE *file, int pid, Status *error){ //function for loading info
    if (error == NULL) {
        return NULL;
    }

    *error = STATUS_OK;

    if (file == NULL || pid <= 0) { //input errors
        errno = EINVAL;
        *error = STATUS_ERROR;
        return NULL;
    }

    int p, c, state;
    int user_length;
    //check if errors when writing
    if (!item_read_all(file, &p, sizeof(p)) ||!item_read_all(file, &c, sizeof(c)) ||!item_read_all(file, &state, sizeof(state)) ||!item_read_all(file, &user_length, sizeof(user_length))) {
        *error = STATUS_ERROR; // errno set by item_read_all
        return NULL;
    }

    if (p <= 0 || c <= 0 ||state < READY || state > TERMINATED ||user_length <= 0 || user_length > MAX_USER_LEN) {
        errno = EBADMSG;
        *error = STATUS_ERROR;//errors when vars dont follow the rules
        return NULL;
    }

    char *user = malloc((size_t)user_length + 1);//malloc of the name

    if (user == NULL) {
        errno = ENOMEM;//error allocating
        *error = STATUS_ERROR;
        return NULL;
    }

    if (!item_read_all(file, user, (size_t)user_length)) {
        int saved_errno = errno; // free() could change errno
        free(user);//we free the user
        errno = saved_errno; //error reading the name
        *error = STATUS_ERROR;
        return NULL;
    }

    user[user_length] = '\0'; //add an empty space at the end

    if (strlen(user) != (size_t)user_length) {//error in the lenght
        free(user);//free
        errno = EBADMSG;
        *error = STATUS_ERROR;
        return NULL;
    }

    Status status = STATUS_OK;
    Process *item = item_create(pid, user, p, c, &status);

    free(user); // item_create() keeps its own copy

    if (item == NULL) {
        if (status == STATUS_ERROR) { //error allocating 
            errno = ENOMEM;
        } else {
            errno = EBADMSG;
        }

        *error = STATUS_ERROR;
        return NULL;
    }

    item->state = (States)state; // Already checked

    return item;
}
