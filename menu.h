#ifndef MENU_H
#define MENU_H

#include "collection.h"
#include "item.h"
#include "util.h"

// Main menu 
void menu_print_main(void);
int menu_ask_option(Status *error);
void menu_help(void);

// Input helpers 
char *menu_ask_owner(Status *error);
int menu_ask_positive_int(const char *prompt, Status *error);
int menu_ask_yes_no(const char *prompt, Status *error);
char *menu_ask_filename(Status *error);
int menu_ask_sort_criterion(Status *error);

// Display helpers 
void menu_show_process(const Process *process, int position);
void menu_show_all(const Schedule *schedule);

// Option handlers 
void menu_add(Schedule *schedule, Status *error);
void menu_delete(Schedule *schedule, Status *error);
void menu_info(Schedule *schedule, Status *error);
void menu_delete_all(Schedule *schedule, Status *error);
void menu_sort(Schedule *schedule, Status *error);
void menu_load(Schedule *schedule, Status *error);
void menu_save(const Schedule *schedule, Status *error);

#endif // MENU_H 
