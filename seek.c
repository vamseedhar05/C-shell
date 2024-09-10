#include "headers.h"

void print_entry(const char *entry_path, mode_t mode) {
    if (S_ISDIR(mode)) {
        printf("\033[34m%s\033[0m\n", entry_path); // Blue for directories
    } else {
        printf("\033[32m%s\033[0m\n", entry_path); // Green for files
    }
}

void process_single_entry(const char *entry_path, mode_t entry_mode, int flags) {
    if ((flags & 4) != 0) {  // -e flag
        if (S_ISDIR(entry_mode)) {
            if (access(entry_path, X_OK) == 0) {
                chdir(entry_path);
            } else {
                printf("Missing permissions for task!\n");
            }
        } else { // It's a file
            if (access(entry_path, R_OK) == 0) {
                FILE *file = fopen(entry_path, "r");
                if (file) {
                    char line[MAX_SIZE];
                    while (fgets(line, MAX_SIZE, file)) {
                        printf("%s", line);
                    }
                    fclose(file);
                }
            } else {
                printf("Missing permissions for task!\n");
            }
        }
    } else {
        if (S_ISDIR(entry_mode)) {
            printf("Found a directory: %s\n", entry_path);
        } else {
            printf("Found a file: %s\n", entry_path);
        }
    }
}

void search_in_directory(const char *search, const char *directory, int flags) {
    DIR *dir = opendir(directory);
    if (dir == NULL) {
        perror("Failed to open directory");
        return;
    }

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || entry->d_name[0]=='.' ) {
            continue;
        }

        char entry_path[PATH_MAX];
        snprintf(entry_path, PATH_MAX, "%s/%s", directory, entry->d_name);

        struct stat entry_stat;
        if (stat(entry_path, &entry_stat) != 0) {
            continue;
        }

        if (S_ISDIR(entry_stat.st_mode)) {
            search_in_directory(search, entry_path, flags); // Recursively search directories
        }

        if (flags & 1 && !S_ISDIR(entry_stat.st_mode)) {
            continue;  // Skip if -d flag is enabled and it's not a directory
        } else if (flags & 2 && S_ISDIR(entry_stat.st_mode)) {
            continue;  // Skip if -f flag is enabled and it's a directory
        }

        if (strncmp(entry->d_name, search, strlen(search)) == 0) {
            if (flags & 4) {
                print_entry(entry_path, entry_stat.st_mode);
                process_single_entry(entry_path, entry_stat.st_mode, flags);
            } else {
                print_entry(entry_path, entry_stat.st_mode);
            }
        }
    }

    closedir(dir);
}

void seek_command(const char *search, const char *target_dir, int flags) {
    search_in_directory(search, target_dir, flags);
}

void seek_driver(int num_args, char **args) {
    int flags = 0;
    const char *search = NULL;
    const char *target_dir = ".";
    int args_index = 1;
    int doop=0;
    for (int i = 1; i < num_args; i++) {
        if (args[i][0] == '-') {
            for (int j = 1; args[i][j] != '\0'; j++) {
                if (args[i][j] == 'd') {
                    flags = flags | 1;
                    doop=1;
                } else if (args[i][j] == 'f') {
                    flags = flags | 2;
                    if(doop=1){
                        printf("Invalid flags!\n");
                        return;
                    }
                } else if (args[i][j] == 'e') {
                    flags = flags | 4;
                } else {
                    fprintf(stderr, "Invalid flags!\n");
                    return;
                }
            }
        } else if (search == NULL) {
            search = args[i];
        } else {
            target_dir = args[i];
        }
    }

    seek_command(search, target_dir, flags);
}
