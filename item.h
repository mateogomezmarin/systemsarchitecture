#ifndef MENU_H
#define MENU_H

typestruct process struct {
    unsigned int pid;
    char *name;
    unsigned int p;
    unsigned int C;  //Execution time
    enum states state;
} 

enum states{READY,RUNNING,STOP,TERMINATING};

