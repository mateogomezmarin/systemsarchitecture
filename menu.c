#include <stdio.h>
#include "input_user.h"
#include "menu.h"

int menu(void)
{
    while (1)
    {
        printf(
            "\nProgram emulating a process scheduler.\n"
            "Select one of the following options:\n"
            "1. Add a new process to the schedule\n"
            "2. Delete a specific process from the schedule\n"
            "3. Information about a process\n"
            "4. Show the entire schedule\n"
            "5. Delete the current schedule\n"
            "6. Sort the schedule by a criterion\n"
            "7. Help\n"
            "0. Exit\n"
            "\n"
            "Please, enter an option: "
        );

        enum input_status err;
        int option = int_input(&err);

        switch (err)
        {
            case INPUT_EOF:
                printf("\nGoodbye!\n");
                return 0;

            case INPUT_ERROR:
                perror("input");
                return -1;

            case INPUT_INVALID:
                printf("Enter a valid number 0-7\n");
                continue;       // go back and show the menu again

            case INPUT_OK:
                break;          // continue to the switch below
        }

        switch (option)
        {
            case 0:
                printf("Goodbye!\n");
                return 0;

            case 1:
                printf("You are in menu 1\n");
                break;

            case 2:
                printf("You are in menu 2\n");
                break;

            case 3:
                printf("You are in menu 3\n");
                break;

            case 4:
                printf("You are in menu 4\n");
                break;

            case 5:
                printf("You are in menu 5\n");
                break;

            case 6:
                printf("You are in menu 6\n");
                break;

            case 7:
                printf("You are in menu 7\n");
                break;

            default:
                printf("Enter a valid number 0-7\n");
        }
    }
}