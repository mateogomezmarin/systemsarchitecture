#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <sys/types.h>

int main (int argc, char *argv[]) { //argc is the number of strings, argv is an array of strings
    if (argc != 2) {
        fprintf(stderr, "Program requires exactly 1 argument\n");
        return EXIT_FAILURE;
    }

    char *end;
    errno = 0;
    long n = strtol(argv[1], &end, 10);   

    if (n <= 0 || *end != '\0' || errno == ERANGE || n > INT_MAX) {
        fprintf(stderr, "Argument must be a positive integer\n");
        return EXIT_FAILURE;
    }

    char array[n + 1];
    for (int i = 0; i < n; i++) {
        array[i] = '+';                   
    }
    array[n] = '\0';

    int j = 0;

    while (1) {
        printf("Process %ld %s iter %d\n", n, array, j);
        j++;
    }
}
