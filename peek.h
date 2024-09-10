#ifndef __PEEK_H
#define __PEEK_H

void peek_driver(char **args,int num_args,char* current_dir,char *HOME,char *previous_dir);

void handle_special_symbols(char *path, const char *home, const char *prev_dir, const char *curr_dir);

void print_colored(const char *text, const char *color_code);

void print_permissions(mode_t mode);

int compare_strings(const void *a, const void *b);

void peek_directory(const char *dir_path, int show_hidden, int show_details);

#endif
