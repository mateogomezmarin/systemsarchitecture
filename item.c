#define _GNU_SOURCE // Needed for strdup() under -std=c11
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "util.h"
#include "item.h"


Process *item_create(int pid, const char *user, int p, int C, Status *error) {

    if (pid <= 0 || p <= 0 || C <= 0 ||
        user == NULL || user[0] == '\0') {
        *error = STATUS_INVALID;
        return NULL;
    }

    Process *item = malloc(sizeof(Process)); //If item == NULL, you already know it failed,
    //malloc fails for only one reason: out of memory

    if (item == NULL) {
        *error = STATUS_ERROR;
        return NULL;
    }

    item->user = strdup(user); //Same as malloc

    if (item->user == NULL) {
        free(item);
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

void item_delete(Process *item)
{
    if (item == NULL) {
        return;
    }

    free(item->user);
    free(item);
}

int item_get_pid(const Process *item, Status *error) {
    if (item == NULL) {
        *error = STATUS_INVALID;
        return -1;
    }
    *error = STATUS_OK;
    return item->pid;
}

const char *item_get_user(const Process *item, Status *error) {
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

void item_print_header(void) {
    printf("%5s %5s %5s %5s %-10s %-20s\n", "POS", "PID", "P", "C", "STATE", "OWNER");
}



int item_print(const Process *item, int position, Status *error) {
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

    printf("%5d %5d %5d %5d %-10s %-20s\n", //- means its left aligned
           position,
           item->pid,
           item->p,
           item->C,
           state,
           item->user);

    *error = STATUS_OK;
    return 0;
}

int item_same_data(const Process *item, const Process *item2, Status *error) {
    if (item == NULL || item2 == NULL) {
        *error = STATUS_INVALID;
        return 0;
    }

    *error = STATUS_OK;

    //Both pointers are already checked, so the fields can be read directly
    if (item->pid != item2->pid ||
        item->p != item2->p ||
        item->C != item2->C ||
        item->state != item2->state ||
        strcmp(item->user, item2->user) != 0) {
        return 0;
    }

    return 1; //1 if both processes have identical data, 0 if their data differs
}

/* qsort passes pointers to the array elements. Each element is a Process *,
   so each argument is really a Process ** and needs a double cast. */

int item_cmp_pid(const void *pa, const void *pb)
{
    const Process *a = *(const Process * const *)pa;
    const Process *b = *(const Process * const *)pb;

    if (a->pid < b->pid) return -1;
    if (a->pid > b->pid) return 1;
    return 0;
}

int item_cmp_priority(const void *pa, const void *pb)
{
    const Process *a = *(const Process * const *)pa;
    const Process *b = *(const Process * const *)pb;

    if (a->p < b->p) return -1;
    if (a->p > b->p) return 1;
    return item_cmp_pid(pa, pb);   /* tie-break: keeps the order deterministic */
}

int item_cmp_c(const void *pa, const void *pb)
{
    const Process *a = *(const Process * const *)pa;
    const Process *b = *(const Process * const *)pb;

    if (a->C < b->C) return -1;
    if (a->C > b->C) return 1;
    return item_cmp_pid(pa, pb);
}

int item_cmp_user(const void *pa, const void *pb)
{
    const Process *a = *(const Process * const *)pa;
    const Process *b = *(const Process * const *)pb;

    int result = strcmp(a->user, b->user);
    if (result != 0) return result;
    return item_cmp_pid(pa, pb);
}

static int item_write_all(FILE *file, const void *data, size_t size)
{
    errno = 0;

    if (fwrite(data, 1, size, file) != size) {
        if (errno == 0) {
            errno = EIO;
        }
        return 0;
    }

    return 1;
}

static int item_read_all(FILE *file, void *data, size_t size)
{
    errno = 0;

    if (fread(data, 1, size, file) == size) {
        return 1;
    }

    if (ferror(file)) {
        if (errno == 0) {
            errno = EIO;
        }
    } else {
        errno = EBADMSG; // Premature end of file
    }

    return 0;
}

void saveProcessToFile(const Process *proc, FILE *file, Status *error)
{
    if (error == NULL) {
        return;
    }

    *error = STATUS_OK;

    if (proc == NULL || file == NULL || proc->user == NULL) {
        errno = EINVAL;
        *error = STATUS_ERROR;
        return;
    }

    size_t length = strlen(proc->user);

    if (length == 0 || length > MAX_USER_LEN) {
        errno = EINVAL;
        *error = STATUS_ERROR;
        return;
    }

    int p = proc->p;
    int c = proc->C;
    int state = proc->state;
    int user_length = (int)length;

    if (!item_write_all(file, &p, sizeof(p)) ||
        !item_write_all(file, &c, sizeof(c)) ||
        !item_write_all(file, &state, sizeof(state)) ||
        !item_write_all(file, &user_length, sizeof(user_length)) ||
        !item_write_all(file, proc->user, length)) {
        *error = STATUS_ERROR; // errno set by item_write_all
    }
}

Process *loadProcessFromFile(FILE *file, int pid, Status *error)
{
    if (error == NULL) {
        return NULL;
    }

    *error = STATUS_OK;

    if (file == NULL || pid <= 0) {
        errno = EINVAL;
        *error = STATUS_ERROR;
        return NULL;
    }

    int p, c, state;
    int user_length;

    if (!item_read_all(file, &p, sizeof(p)) ||
        !item_read_all(file, &c, sizeof(c)) ||
        !item_read_all(file, &state, sizeof(state)) ||
        !item_read_all(file, &user_length, sizeof(user_length))) {
        *error = STATUS_ERROR; // errno set by item_read_all
        return NULL;
    }

    if (p <= 0 || c <= 0 ||
        state < READY || state > TERMINATED ||
        user_length <= 0 || user_length > MAX_USER_LEN) {
        errno = EBADMSG;
        *error = STATUS_ERROR;
        return NULL;
    }

    char *user = malloc((size_t)user_length + 1);

    if (user == NULL) {
        errno = ENOMEM;
        *error = STATUS_ERROR;
        return NULL;
    }

    if (!item_read_all(file, user, (size_t)user_length)) {
        int saved_errno = errno; // free() could change it
        free(user);
        errno = saved_errno;
        *error = STATUS_ERROR;
        return NULL;
    }

    user[user_length] = '\0';

    if (strlen(user) != (size_t)user_length) {
        free(user);
        errno = EBADMSG;
        *error = STATUS_ERROR;
        return NULL;
    }

    Status status = STATUS_OK;
    Process *item = item_create(pid, user, p, c, &status);

    free(user); // item_create() keeps its own copy

    if (item == NULL) {
        if (status == STATUS_ERROR) {
            errno = ENOMEM;
        } else {
            errno = EBADMSG;
        }

        *error = STATUS_ERROR;
        return NULL;
    }

    item->state = (States)state; // Already validated above

    return item;
}