#include <stdio.h>
#include <stdlib.h> //EXIT_SUCCESS and EXIT_FAILURE are integer constants (int) defined by the C standard library in <stdlib.h>.

#include "menu.h"
#include "collection.h"
#include "util.h"

int main(void)
{
    Status error = STATUS_OK;

    Schedule *schedule = collection_create_empty(&error);

    if (schedule == NULL || error != STATUS_OK) {
        fprintf(stderr, "Error: could not create the schedule.\n");
        return EXIT_FAILURE;
    }

    int running = 1;
    int option;

    while (running) {
        error = STATUS_OK;

        menu_print_main();
        option = menu_ask_option(&error);

        /* Ctrl+D in the main menu terminates the program. */
        if (error == STATUS_EOF) {
            break;
        }

        if (error == STATUS_INVALID) {
            fprintf(stderr, "Error: invalid option.\n");
            continue;
        }

        if (error != STATUS_OK) {
            fprintf(stderr, "Error: could not read the option.\n");
            break;
        }

        switch (option) {
            case 1:
                menu_add(schedule, &error);
                break;

            case 2:
                menu_delete(schedule, &error);
                break;

            case 3:
                menu_info(schedule, &error);
                break;

            case 4:
                menu_show_all(schedule);
                break;

            case 5:
                menu_delete_all(schedule, &error);
                break;

            case 6:
                menu_sort(schedule, &error);
                break;

            case 7:
                menu_load(schedule, &error);
                break;

            case 8:
                menu_save(schedule, &error);
                break;

            case 9:
                menu_help();
                break;

            case 0:
                running = 0;
                break;

            default:
                fprintf(stderr, "Error: invalid option.\n");
                break;
        }

        /* Handle errors reported by the menu functions. */
        if (error == STATUS_EOF) {
            /* Ctrl+D in a submenu returns to the main menu. */
            error = STATUS_OK;
        } else if (error == STATUS_INVALID) {
            fprintf(stderr, "Error: invalid input or operation.\n");
            error = STATUS_OK;
        } else if (error == STATUS_ERROR) {
            fprintf(stderr, "Error: an unexpected failure occurred.\n");
            running = 0;
        }
    }

    collection_delete(schedule);

    printf("\nProgram terminated.\n");

    return EXIT_SUCCESS;
}