#ifndef INPUT_USR_H
#define INPUT_USR_H

#include "util.h"

/* Three functions:
   - input_usr_line reads a line and removes \n.
   - input_usr_int reuses it and parses the string into an int.
   - input_usr_YN reuses it and translates the string into Y/N.

   Each function reports its status through *error.
   The return value is only valid when *error == STATUS_OK. 
*/

char *input_usr_line(Status *error); // returns the cleaned string, caller must free() it. NULL on EOF/error
int input_usr_int(Status *error);    // returns the parsed integer
int input_usr_YN(Status *error);     // returns 1 for Y/y, 0 for N/n

#endif