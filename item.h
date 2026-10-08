#ifndef ITEM_H
#define ITEM_H

enum states { READY, RUNNING, STOPPED, TERMINATED }; //defined states

typedef struct process {
    unsigned int pid;     // > 0, unique, never reused 
    char        *user;    //  owner's name 
    unsigned int p;       // priority, > 0
    unsigned int C;       // CPU execution time, > 0
    enum states  state;
} Process;


#endif




/*
// We declare them static so that they can only be accessed from menu file
static void print_main_menu(void);          // just the printf of the options

static void menu_add_process(void);         // option 1
static void menu_delete_process(void);      // option 2
static void menu_process_info(void);        // option 3
static void menu_show_schedule(void);       // option 4
static void menu_clear_schedule(void);      // option 5
static int  menu_sort_schedule(void);       // option 6 (submenu)
static void menu_help(void);  


*/



