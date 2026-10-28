#define _GNU_SOURCE // Needed for getline(), GNU/linux functions
#include <stdio.h>
#include <stdlib.h> //needed for free()
#include <errno.h>
#include "util.h" // file containing enum for errors
#include "input_usr.h"
#include <limits.h>

char *input_usr_line(Status *error){ // Use of enum to return the input status

    char *line = NULL; //pointer to the first char of user_input array
    size_t size = 0; //needed for getline()
    errno = 0;

    //ssize_t Integer with sign 64 bits
    ssize_t len = getline(&line, &size, stdin); //getline() allocates the memory to line address and returns number of characters read in ssize_T
    if (len == -1){
    free(line);
    if (errno == 0) {
        clearerr(stdin);     // reset the EOF flag so the next read works
        *error = STATUS_EOF; //Ctrl + D
    } else {
        *error = STATUS_ERROR; //Error in the system
    }
    return NULL;
}

    

    if(len > 0 && line[len - 1] == '\n'){ //Used for removing \n
        line[len - 1] = '\0';
    }

    *error = STATUS_OK; 
    return line; //we need to return always even if the user doesn't return enter
}

int input_usr_int(Status *error){ //We use strtol family functions to parse strings into integers
    char *unparsed_text = input_usr_line(error); 
    if (unparsed_text == NULL){
        return 0;
    }

    //need to create the variables for strtol
    char *end; //points to the first character that was not part of the number.
    int decimal_base = 10;
    errno = 0;  // placed BEFORE the strtol call
    long number = strtol(unparsed_text,&end,decimal_base);

    if (end == unparsed_text) { // nothing converted 
        *error = STATUS_INVALID; 
    } else if (*end != '\0') { // garbage after the number 
        *error = STATUS_INVALID; 
    } else if (errno == ERANGE || number < INT_MIN || number > INT_MAX) { //Checks whether a conversion function reported a range error.
        *error = STATUS_INVALID; 
    } else {
        *error = STATUS_OK;
    }

    free(unparsed_text); //no longer needed so we free it to prevent memory leaks

    if (*error != STATUS_OK) { 
        return 0;         
    }
    return (int) number;
}

int input_usr_YN(Status *error){ //Y/N input function.
    char *text = input_usr_line(error); 

    if(text == NULL){
        return 0;
    }

    int answer = 0; //must check err when calling
    *error = STATUS_INVALID; 

    if(text[0] != '\0' && text[1] == '\0'){
        if(text[0] == 'y' || text[0] == 'Y'){
            *error = STATUS_OK; 
            answer = 1;
        }
        else if (text[0] == 'n' || text[0] == 'N'){
            *error = STATUS_OK; 
            answer = 0;
        }
    }

    free(text);
    return answer;  //Must check before calling
}