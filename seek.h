#ifndef __SEEK_H
#define __SEEK_H

void print_colored_seek(const char *name, mode_t mode) ;

void process_single_entry(const char *entry_path, mode_t entry_mode, int flags);

void seek_recursive(const char *search, const char *target_dir, int flags) ;

void seek_command(const char *search, const char *target_dir, int flags);

void seek_driver(int num_args,char** args);

#endif