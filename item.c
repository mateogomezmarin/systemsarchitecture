#define _GNU_SOURCE // Needed for strdup() under -std=c11
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    printf("POS\tPID\tP\tC\tSTATE\tOWNER\n");
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

int item_cmp_pid(const Process *item, const Process *item2, Status *error) {
    if (item == NULL || item2 == NULL) {
        *error = STATUS_INVALID;
        return 0;
    }

    Status e1, e2;
    int a = item_get_pid(item, &e1);
    int b = item_get_pid(item2, &e2);

    if (e1 != STATUS_OK || e2 != STATUS_OK) {
        *error = STATUS_INVALID;
        return 0;
    }

    *error = STATUS_OK;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

int item_cmp_priority(const Process *item, const Process *item2, Status *error) {
    if (item == NULL || item2 == NULL) {
        *error = STATUS_INVALID;
        return 0;
    }

    Status e1, e2;
    int a = item_get_p(item, &e1);
    int b = item_get_p(item2, &e2);

    if (e1 != STATUS_OK || e2 != STATUS_OK) {
        *error = STATUS_INVALID;
        return 0;
    }

    *error = STATUS_OK;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

int item_cmp_c(const Process *item, const Process *item2, Status *error) {
    if (item == NULL || item2 == NULL) {
        *error = STATUS_INVALID;
        return 0;
    }

    Status e1, e2;
    int a = item_get_C(item, &e1);
    int b = item_get_C(item2, &e2);

    if (e1 != STATUS_OK || e2 != STATUS_OK) {
        *error = STATUS_INVALID;
        return 0;
    }

    *error = STATUS_OK;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

int item_cmp_user(const Process *item, const Process *item2, Status *error) {
    if (item == NULL || item2 == NULL) {
        *error = STATUS_INVALID;
        return 0;
    }

    Status e1, e2;
    const char *a = item_get_user(item, &e1);
    const char *b = item_get_user(item2, &e2);

    if (e1 != STATUS_OK || e2 != STATUS_OK) {
        *error = STATUS_INVALID;
        return 0;
    }

    *error = STATUS_OK;
    return strcmp(a, b);
}
