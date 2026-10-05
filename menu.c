#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(void)
{
    char *linea = NULL;
    size_t tamaño = 0;
    ssize_t bytes_leidos;

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

        bytes_leidos = getline(&linea, &tamaño, stdin);

        if (bytes_leidos == -1)
        {
            // Ctrl+D -> EOF
            printf("\nGoodbye!\n");
            free(linea);
            return 0;
        }

        int numero = strtol(linea, NULL, 10);

        switch (numero)
        {
            case 0:
                free(linea);
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