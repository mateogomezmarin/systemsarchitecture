#ifndef UTIL_H
#define UTIL_H

typedef enum{
    STATUS_OK = 0, //Correct usage
    STATUS_INVALID, //user introduced something wrong, we use message and ask again
    STATUS_EOF, //user presssed CTRL + D, exit or go back one menu
    STATUS_ERROR //program failed, abort and clean up
}Status;

enum { 
    SORT_BY_PID = 1,
    SORT_BY_PRIORITY, 
    SORT_BY_C, 
    SORT_BY_USER 
};

#endif



