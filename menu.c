#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "menu.h"
#include "input_usr.h"

void menu_print_main(void){

    printf("Program emulating a process scheduler.\n\n");
    printf("Select one of the following options:\n\n");
    printf("1. Add a new process to the schedule\n");
    printf("2. Delete a specific process from the schedule\n");
    printf("3. Information about a process\n");
    printf("4. Show entire schedule\n");
    printf("5. Delete current schedule\n");
    printf("6. Sort schedule by a criterion\n");
    printf("7. Load from file\n");
    printf("8. Save to file\n");
    printf("9. Help\n");
    printf("0. Exit\n\n");
    printf("If you enter CTRL+D:\n");
    printf("- In this menu, the program will terminate gracefully.\n");
    printf("- In a submenu, the program will return to this menu.\n");
}


int menu_ask_option(Status *error){
    if (error == NULL) {
        return -1;
    }

    *error = STATUS_OK;

    Status input_error = STATUS_OK;
    int option = input_usr_int(&input_error);

    if (input_error != STATUS_OK) {
        *error = input_error;
        return -1;
    }

    if (option < 0 || option > 9) {
        *error = STATUS_INVALID;
        return -1;
    }

    return option;
}


void menu_help(void){

    printf("\nHelp - Menu options\n\n");
    printf("1. Add a new process to the schedule.\n");
    printf("   Creates a process with the specified owner and parameters.\n\n");
    printf("2. Delete a specific process from the schedule.\n");
    printf("   Removes a process identified by its PID.\n\n");
    printf("3. Information about a process.\n");
    printf("   Displays information about a selected process.\n\n");
    printf("4. Show entire schedule.\n");
    printf("   Displays all processes currently in the schedule.\n\n");
    printf("5. Delete current schedule.\n");
    printf("   Removes all processes from the schedule.\n\n");
    printf("6. Sort the schedule by a criterion.\n");
    printf("   Sorts processes according to a selected criterion.\n\n");
    printf("7. Load from file.\n");
    printf("   Loads a schedule from a binary file.\n\n");
    printf("8. Save to file.\n");
    printf("   Saves the current schedule to a binary file.\n\n");
    printf("9. Help.\n");
    printf("   Displays this help information.\n\n");
    printf("0. Exit.\n");
    printf("   Terminates the program.\n\n");
}

char *menu_ask_owner(Status *error){

    if (error == NULL) {
        return NULL;
    }
    printf("Enter process owner: ");
    return input_usr_line(error);
}


int menu_ask_positive_int(const char *prompt, Status *error){

printf("%s", prompt);

int number = input_usr_int(error);

if (*error != STATUS_OK) {
    return 0;
}

if (number <= 0) {
    *error = STATUS_INVALID;
    return 0;
}

return number;

}


int menu_ask_yes_no(const char *prompt, Status *error){

    printf("%s", prompt);
    return input_usr_YN(error);
}

char *menu_ask_filename(Status *error){
    printf("Enter filename: ");
    return input_usr_line(error);
}

int menu_ask_sort_criterion(Status *error){

printf("Select sorting criterion:\n");
printf("1. PID\n");
printf("2. Priority (p)\n");
printf("3. C\n");
printf("4. Owner\n");
printf("Enter option: ");

int criterion = input_usr_int(error);

if (*error != STATUS_OK) {
    return 0;
}

if (criterion < 1 || criterion > 4) {
    *error = STATUS_INVALID;
    return 0;
}

return criterion;

}

void menu_show_process(const Process *process, int position)
{
    Status error;
    item_print(process, position, &error);
}

void menu_show_all(const Schedule *schedule)
{
    if (schedule == NULL || collection_is_empty(schedule)) {
        printf("The schedule is empty.\n");
        return;
    }

    item_print_header();

    for (int i = 0; i < collection_size(schedule); i++) {
        Status error;
        Process *process = collection_get_by_index(schedule, i, &error);

        if (error == STATUS_OK) {
            menu_show_process(process, i + 1);
        }
    }
}

void menu_add(Schedule *schedule, Status *error)
{
if (schedule == NULL || error == NULL) {
return;
}

char *owner = menu_ask_owner(error);
if (*error != STATUS_OK) {
    free(owner);
    return;
}

int p = menu_ask_positive_int("Enter priority (p): ", error);
if (*error != STATUS_OK) {
    free(owner);
    return;
}

int C = menu_ask_positive_int("Enter C: ", error);
if (*error != STATUS_OK) {
    free(owner);
    return;
}

collection_add(schedule, owner, p, C, error);
free(owner);

}

void menu_delete(Schedule *schedule, Status *error){

    if (schedule == NULL || error == NULL) {
        return;
    }

    int pid = menu_ask_positive_int("Enter process PID: ", error);

    if (*error != STATUS_OK) {
        return;
    }

    collection_remove_by_pid(schedule, pid, error);
}

void menu_info(Schedule *schedule, Status *error){

    if (schedule == NULL || error == NULL) {
        return;
    }

    int pid = menu_ask_positive_int("Enter process PID: ", error);

    if (*error != STATUS_OK) {
        return;
    }

    Process *process = collection_find_by_pid(schedule, pid, NULL, error);

    if (*error != STATUS_OK) {
        return;
    }

    item_print_header();
    item_print(process, 1, error);
}


void menu_delete_all(Schedule *schedule, Status *error){

    if (schedule == NULL || error == NULL) {
        return;
    }

    if (collection_is_empty(schedule)) {
        *error = STATUS_INVALID;
        return;
    }

    int answer = menu_ask_yes_no(
        "Are you sure you want to delete the entire schedule? (y/n): ",
        error
    );

    if (*error != STATUS_OK) {
        return;
    }

    if (answer == 1) {
        collection_clear(schedule);
    }
}



void menu_sort(Schedule *schedule, Status *error){

    if (schedule == NULL || error == NULL) {
        return;
    }

    if (collection_is_empty(schedule)) {
        *error = STATUS_INVALID;
        return;
    }

    int criterion = menu_ask_sort_criterion(error);

    if (*error != STATUS_OK) {
        return;
    }

    switch (criterion) {
        case SORT_BY_PID:
            collection_sort(schedule, item_cmp_pid, error);
            break;

        case SORT_BY_PRIORITY:
            collection_sort(schedule, item_cmp_priority, error);
            break;

        case SORT_BY_C:
            collection_sort(schedule, item_cmp_c, error);
            break;

        case SORT_BY_USER:
            collection_sort(schedule, item_cmp_user, error);
            break;

        default:
            *error = STATUS_INVALID;
            break;
    }
}


void menu_load(Schedule *schedule, Status *error)
{
    // CHANGED: Also check that schedule is not NULL.
    if (schedule == NULL || error == NULL) {
        return;
    }

    *error = STATUS_OK;
    int saved_errno = 0;

    printf("Loading schedule from file.\n");
    printf("Enter filename: ");

    char *filename = input_usr_line(error);

    if (filename == NULL) {
        if (*error == STATUS_EOF) {
            printf("\n");
        }
        return; //STATUS_EOF (CTRL + D) or STATUS_ERROR (errno set by getline)
    }

    //1. Check that the file can be read: if fopen fails, errno says why (ENOENT, EACCES...)
    FILE *test = fopen(filename, "rb");

    if (test == NULL) {
        saved_errno = errno; //free() could overwrite the cause of the failure
        free(filename);
        errno = saved_errno;
        *error = STATUS_INVALID;
        return;
    }

    fclose(test); //It was only a check

    //2. If the schedule is not empty, ask before erasing it
    int was_empty = collection_is_empty(schedule);

    if (!was_empty) {
        printf("Current schedule is not empty.\n");

        printf("Proceed? Continuing will erase it (Y/N): ");
        int proceed = input_usr_YN(error);

        // CHANGED: Removed the retry loop and fprintf(stderr, ...)
        // so program.c can handle invalid input and print the error message.

        if (*error == STATUS_EOF) {
            printf("\n");
        }

        if (*error != STATUS_OK) { //CTRL + D or input failure
            saved_errno = errno; //free() could overwrite the cause of the failure
            free(filename);      // CHANGED: Fixed indentation.
            errno = saved_errno;
            return;
        }

        if (!proceed) { //The user answered N: back to the main menu
            free(filename);
            return;
        }
    }

    //3. Load. The old content is only removed if the whole file is valid
    loadScheduleFromFile(schedule, filename, error);

    if (*error != STATUS_OK) {
        saved_errno = errno; //free() could overwrite the cause of the failure
        free(filename);
        errno = saved_errno;

        if (errno == ENOMEM) { //The program cannot continue
            *error = STATUS_ERROR;
        } else {
            *error = STATUS_INVALID; //Problem with the file (EBADMSG if it is not a valid schedule)
        }
        return;
    }

    if (!was_empty) {
        printf("Current schedule cleared\n");
    }

    printf("Schedule loaded from %s\n", filename);

    free(filename);
}   

void menu_save(const Schedule *schedule, Status *error)
{
    // CHANGED: Also check that schedule is not NULL.
    if (schedule == NULL || error == NULL) {
        return;
    }

    *error = STATUS_OK;
    int saved_errno = 0;

    printf("Saving schedule to file.\n");
    printf("Enter filename: ");

    char *filename = input_usr_line(error);

    if (filename == NULL) {
        if (*error == STATUS_EOF) {
            printf("\n");
        }
        return; //STATUS_EOF (CTRL + D) or STATUS_ERROR (errno set by getline)
    }

    //1. Does the file already exist? If fopen fails with ENOENT it does not
    int exists = 1;
    FILE *test = fopen(filename, "rb");

    if (test == NULL && errno == ENOENT) {
        exists = 0; //It will be created when saving
    }

    if (test != NULL) {
        fclose(test); //It was only a check
    }

    // CHANGED: If opening failed for a reason other than ENOENT,
    // report an error instead of treating the file as writable.
    if (test == NULL && exists) {
        saved_errno = errno;
        free(filename);
        errno = saved_errno;
        *error = STATUS_INVALID;
        return;
    }

    if (exists) {
        //2. It exists: check that it can be written. "ab" does not erase anything
        test = fopen(filename, "ab");

        if (test == NULL) { //errno: EACCES, EROFS, EISDIR...
            saved_errno = errno; //free() could overwrite the cause of the failure
            free(filename);      // CHANGED: Fixed indentation.
            errno = saved_errno;
            *error = STATUS_INVALID;
            return;
        }

        fclose(test);

        //3. It exists and can be written: ask before overwriting
        printf("File exists. Overwrite? (Y/N): ");
        int overwrite = input_usr_YN(error);

        // CHANGED: Removed the retry loop and fprintf(stderr, ...)
        // so program.c can handle invalid input and print the error message.

        if (*error == STATUS_EOF) {
            printf("\n");
        }

        if (*error != STATUS_OK) { //CTRL + D or input failure
            saved_errno = errno; //free() could overwrite the cause of the failure
            free(filename);      // CHANGED: Fixed indentation.
            errno = saved_errno;
            return;
        }

        if (!overwrite) { //The user answered N: back to the main menu
            free(filename);
            return;
        }
    }

    //4. Write the data
    saveScheduleToFile(schedule, filename, error);

    if (*error != STATUS_OK) {
        saved_errno = errno; //free() could overwrite the cause of the failure
        free(filename);
        errno = saved_errno;

        if (errno == ENOMEM) { //The program cannot continue
            *error = STATUS_ERROR;
        } else {
            *error = STATUS_INVALID; //Problem writing the file
        }
        return;
    }

    printf("Schedule saved to file %s\n", filename);

    free(filename);
}