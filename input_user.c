#define _GNU_SOURCE     // Needed for getline(), GNU/linux functions
#include <stdio.h>
#include <stdlib.h> //needed for free()
#include <errno.h>
#include "input_user.h"


char *user_input(enum input_status *err){ // Use of enum in the error to have various options

    char *line = NULL; //pointer to the first char of user_input array
    size_t size = 0; //needed for getline()
    
    errno = 0;  
    //ssize_t Integer with sign 64 bits    
    ssize_t len = getline(&line,&size,stdin); //getline() allocates the memory to line address and returns number of characters read in ssize_T
    if (len == -1){
        free(line); //even if it failed getline() might have allocated memory  
        if (errno == 0) { 
            *err = INPUT_EOF; //Ctrl + D
        } else {
            *err = INPUT_ERROR; //Error in the stystem 
        }
        return NULL;
    }

    if(len > 0 && line[len -1] == '\n'){
        line[len - 1] = '\0';
    }

    *err = INPUT_OK;
    return line; //we need to return always even if the user doesn't return enter
}


int int_input(enum input_status *err){ //We use strtol family functions to parse strings into integers
    char *unparsed_text = user_input(err);
    if (unparsed_text == NULL){      
        return 0;  //Caller must later check err == INPUT_OK
    }

    //need to create the variables for strtol
    char *end; //points to the first character that was not part of the number.
    int decimal_base = 10;
    errno = 0;  // placed BEFORE the strtol call
    long number = strtol(unparsed_text,&end,decimal_base);

    if (end == unparsed_text) { // nothing converted ("abc", "")
        *err = INPUT_INVALID;
    } else if (*end != '\0') { // garbage after the number ("12abc", "3.5")
        *err = INPUT_INVALID;
    } else if (number < INT_MIN || number > INT_MAX) { // huge number, doesn't fit an int
        *err = INPUT_INVALID;
    } else {
        *err = INPUT_OK;
    } 

    free(unparsed_text); //no longer needed so we free it to prevent memory leaks

    if (*err != INPUT_OK) {
        return 0;         // placeholder, the caller must ignore it
    }
    return (int)number;
}

int YN_input(enum input_status *err){
    char *text = user_input(err);

    if(text == NULL){
        return 0;
    }

    int answer = 0; //must check err when calling
    *err = INPUT_INVALID; //We assume invalid input

    if(text[0] != '\0' && text[1] == '\0'){
        if(text[0] == 'y' || text[0] == 'Y'){
            *err = INPUT_OK;
            answer = 1;
        }
        else if (text[0] == 'n' || text[0] == 'N'){
            *err = INPUT_OK;
            answer = 0;
        }
    }

    free(text);
    return answer;
}




