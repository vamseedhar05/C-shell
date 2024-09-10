#ifndef __PASTEVENTS_H
#define __PASTEVENTS_H

void save_command(char* cmd,char *dup_home);

void print_history(char *dup_home);

char* execute_arg(char* arg,char *dup_home);

void clearFile(char *dup_home);

#endif
