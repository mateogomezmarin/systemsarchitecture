#include <stdio.h>
#include <errno.h> //Error handling library
#include <stdlib.h>

int main(){

    printf("Program emulating a process scheduler.\n"
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
       "If you enter CTRL+D:\n"
       "- In this menu, the program will terminate gracefully.\n"
       "- In a submenu, the program will return to this menu.\n"
       "\n"
       "Please, enter an option (help: 7): ");

    char *linea = NULL;
    size_t tamaño = 0;
    ssize_t bytes_leidos; //Variable to store how many characters have been read by get line
    //ssize_t is = unsigned int char *cadena;
   
    bytes_leidos = getline(&linea, &tamaño, stdin); //bytes_leidos 

    if (bytes_leidos == -1) //When using Ctrl+D
    {   
        if(errno == 0){
        // Ctrl+D / End Of File
        free(linea); // We free the memory before exiting
        return 0;
        }
        if(errno != 0){
            puts("Error.");
        }
    }

    // strtol necesita un puntero 
    char *endptr;
    int numero = strtol(linea, &endptr, 10); 

    int i = 0;

    while(i == 0){
        switch (numero) {  // Implemented following w3schools C basic tutorial

            case 0:
                i = 1;
                free(linea);  // Liberamos la memoria antes de salir
                exit(0);

            case 1:
                printf("You are in menu 1\n");
                break;

            case 2:
                // code block
                break;

            case 3:
                // code block
                break;

            case 4:
                // code block
                break;

            case 5:
                // code block
                break;

            case 6:
                // code block
                break;

            case 7:
                // code block
                break;

            case 8:
                // code block
                break;

            default:
                printf("Enter a valid number 1-7\n");
        }

        // Por ahora salimos del while después de procesar una opción.
        i = 1;
    }

    free(linea);

    return 0;
}