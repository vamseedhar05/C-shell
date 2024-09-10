#include "headers.h"

void peek_driver(char **args,int num_args,char* current_dir,char *HOME,char *previous_dir){
    char peek_path[MAX_SIZE];
    int show_hidden = 0;
    int show_details = 0;
    peek_path[0] = '\0';

    for (int j = 1; j < num_args; j++) {
        if (strcmp(args[j], "-a") == 0) {
            show_hidden = 1;
        } else if (strcmp(args[j], "-l") == 0) {
            show_details = 1;
        } else if(strcmp(args[j],"-al")==0 || strcmp(args[j],"-la")==0 ){
            show_hidden = 1;
            show_details = 1;
        }else {
            strcpy(peek_path, args[j]); // Assume it's the path
        }
    }
    // If no path is provided, default to the current directory
    getcwd(current_dir, MAX_SIZE);
    if (peek_path[0] == '\0') {
        strcpy(peek_path, current_dir);
    }

    // Handle special symbols in the path
    handle_special_symbols(peek_path, HOME, previous_dir, current_dir);

    // Display directory contents using peek_directory function
    peek_directory(peek_path, show_hidden, show_details);
}


void handle_special_symbols(char *path, const char *home, const char *prev_dir, const char *curr_dir) {
    if (strcmp(path, "~") == 0) {
        strcpy(path, home);
    } else if (strcmp(path, "-") == 0) {
        strcpy(path, prev_dir);
    } else if (strcmp(path, ".") == 0) {
        strcpy(path, curr_dir);
    } else if (strcmp(path, "..") == 0) {
        if (strcmp(path, "/") != 0) {
            char *last_slash = strrchr(path, '/');
            if (last_slash != NULL) {
                *last_slash = '\0';
            }
        }
    }
}

void print_colored(const char *text, const char *color_code) {
    printf("%s%s%s", color_code, text, COLOR_RESET);
}

void print_permissions(mode_t mode) {
    printf((S_ISDIR(mode)) ? "d" : "-");
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
}

int compare_strings(const void *a, const void *b) {
    const char **str1 = (const char **)a;
    const char **str2 = (const char **)b;
    return strcmp(*str1, *str2);
}

void peek_directory(const char *dir_path, int show_hidden, int show_details) {
    DIR *dir;
    struct dirent *entry;
    struct stat file_info;
    char **names = NULL;
    int num_names = 0;
    
    dir = opendir(dir_path);
    if (dir == NULL) {
        perror("opendir");
        return;
    }
    
    while ((entry = readdir(dir)) != NULL) {
        if (!show_hidden && entry->d_name[0] == '.') {
            continue;
        }
        
        char full_path[MAX_SIZE];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);
        
        if (lstat(full_path, &file_info) == -1) {
            perror("lstat");
            continue;
        }
        
        names = realloc(names, (num_names + 1) * sizeof(char *));
        names[num_names] = strdup(entry->d_name);
        num_names++;
    }

    qsort(names, num_names, sizeof(char *), compare_strings);
    for (int i = 0; i < num_names; i++) {
        char full_path[MAX_SIZE];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, names[i]);
        lstat(full_path, &file_info);

        if (show_details) {
            char perms[11];
            perms[0] = (S_ISDIR(file_info.st_mode)) ? 'd' : '-';
            perms[1] = (file_info.st_mode & S_IRUSR) ? 'r' : '-';
            perms[2] = (file_info.st_mode & S_IWUSR) ? 'w' : '-';
            perms[3] = (file_info.st_mode & S_IXUSR) ? 'x' : '-';
            perms[4] = (file_info.st_mode & S_IRGRP) ? 'r' : '-';
            perms[5] = (file_info.st_mode & S_IWGRP) ? 'w' : '-';
            perms[6] = (file_info.st_mode & S_IXGRP) ? 'x' : '-';
            perms[7] = (file_info.st_mode & S_IROTH) ? 'r' : '-';
            perms[8] = (file_info.st_mode & S_IWOTH) ? 'w' : '-';
            perms[9] = (file_info.st_mode & S_IXOTH) ? 'x' : '-';
            perms[10] = '\0';
            
            struct tm *time_info;
            time_info = localtime(&(file_info.st_mtime));
            char time_str[13];
            strftime(time_str, sizeof(time_str), "%b %d %H:%M", time_info);
            
            printf("%s %2lu %s %s %5lld %s ", 
                perms,
                (uintmax_t)file_info.st_nlink,
                getpwuid(file_info.st_uid)->pw_name,
                getgrgid(file_info.st_gid)->gr_name,
                (long long)file_info.st_size,
                time_str
            );
            if (S_ISDIR(file_info.st_mode)) {
                print_colored(names[i], COLOR_BLUE); // Blue for directories
            } else if (file_info.st_mode & S_IXUSR) {
                print_colored(names[i], COLOR_GREEN); // Green for executables
            } else {
                print_colored(names[i], COLOR_WHITE); // White for files
            }
            
            printf("\n");
        } else {
            // Display file/directory name with color coding
            if (S_ISDIR(file_info.st_mode)) {
                print_colored(names[i], COLOR_BLUE); // Blue for directories
            } else if (file_info.st_mode & S_IXUSR) {
                print_colored(names[i], COLOR_GREEN); // Green for executables
            } else {
                print_colored(names[i], COLOR_WHITE); // White for files
            }
            
            printf("\n");
        }
    }

    for (int i = 0; i < num_names; i++) {
        free(names[i]);
    }
    free(names);
    closedir(dir);
}