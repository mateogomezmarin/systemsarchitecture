#ifndef INPUT_USER
#define INPUT_USER

/*We will now create a wrapper function for getline() that takes the raw input
and correctly cleans up the text */

/*Create 3 functions, one that gets the string and cleans it which I will use for the normal string 
and then using that same one one that parses string to int and another that translates string to Y/N*/
enum input_status {
    INPUT_OK = 0, //Correct usage
    INPUT_INVALID, //user introduced something wrong, we use message and ask again
    INPUT_EOF, //user presssed CTRL + D, exit or go back one menu
    INPUT_ERROR //program failed, abort and clean up
};

//enum before the function
char *user_input(enum input_status *err); //returns the cleaned string. We need it to remove \n from the input 
int int_input(enum input_status *err); //returns the parsed integer
int YN_input(enum input_status *err); //It returns Y/N as an integer

#endif