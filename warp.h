#ifndef __WARP_H
#define __WARP_H

void warp_driver(char** args,int num_args,char *current_dir,char *HOME,char *previous_dir);

void warp_function(const char *new_path, char *previous_dir, const char *HOME, char *current_dir);

#endif
