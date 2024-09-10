#include "headers.h"

// void print_process_info(int pid,BG b) {
//     struct kinfo_proc info;
//     size_t info_size = sizeof(info);

//     int mib[] = {CTL_KERN, KERN_PROC, KERN_PROC_PID, pid};
//     if (sysctl(mib, sizeof(mib) / sizeof(mib[0]), &info, &info_size, NULL, 0) == 0) {
//         // Print PID
//         printf("PID: %d\n", pid);

//     char status = info.kp_proc.p_stat;
//         char status_str[5];
//         switch (status) {
//             case SIDL:
//                 strncpy(status_str, "I", sizeof(status_str));
//                 break;
//             case SRUN:
//                 strncpy(status_str, "R", sizeof(status_str));
//                 int flag=0;
//                 for(int i=0;i<PIDSCount;i++){
//                     if(pid==b->Pids[i]){
//                         flag=1;
//                         break;
//                     }
//                 }
//                 if(!flag)
//                     strncat(status_str, "+", sizeof(status_str) - strlen(status_str) - 1);
//                 break;
//             case SSLEEP:
//                 strncpy(status_str, "S", sizeof(status_str));
//                 break;
//             case SZOMB:
//                 strncpy(status_str, "Z", sizeof(status_str));
//                 break;
//             default:
//                 strncpy(status_str, "Unknown", sizeof(status_str));
//         }
//         status_str[sizeof(status_str) - 1] = '\0';
//         printf("Process Status: %s\n", status_str);

//         // Print Process Group
//         printf("Process Group: %d\n", info.kp_eproc.e_pgid);

//         // Print Virtual Memory (Using getrusage)
//         struct rusage usage;
//         getrusage(RUSAGE_SELF, &usage);
//         printf("Virtual Memory: %ld KB\n", usage.ru_maxrss);

//         // Print Executable Path from Home Directory
//         char pathbuf[PROC_PIDPATHINFO_MAXSIZE];
//         if (proc_pidpath(pid, pathbuf, sizeof(pathbuf)) > 0) {
//             // Extract the path relative to the home directory
//             char *home = getenv("HOME");
//             if (home != NULL && strncmp(pathbuf, home, strlen(home)) == 0) {
//                 printf("Executable Path: ~%s\n", pathbuf + strlen(home));
//             } else {
//                 printf("Executable Path: %s\n", pathbuf);
//             }
//         } else {
//             fprintf(stderr, "Error: Failed to retrieve executable path for PID %d\n", pid);
//         }
//     } else {
//         fprintf(stderr, "Error: Failed to retrieve process information for PID %d\n", pid);
//     }
// }


void print_process_info(pid_t pid) {
    char proc_path[100];
    snprintf(proc_path, sizeof(proc_path), "/proc/%d/status", pid);

    FILE *file = fopen(proc_path, "r");
    if (file == NULL) {
        perror("fopen");
        return;
    }

    char line[100];
    char status[10] = "";
    int pgrp = 0;
    char vm_size[20] = "";
    char exe_path[100] = "";

    // Set the default process status to "Z" (Zombie)
    strcpy(status, "Z");

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "State:\t%s", status) == 1) {
            // Extract process status
            // Add '+' for foreground processes
            if (status[0] == 'R' || status[0] == 'S') {
                int is_foreground = 0;
                // if (sscanf(line, "Tgid:\t%d", &pgrp) == 1) {
                //     if (pgrp == pid) {
                //         is_foreground = 1;
                //     }
                // }
                for(int i=0;i<PIDSCount;i++){
                    if(BgArray[i].Pids==pid){
                        is_foreground=1;
                        break;
                    }
                }
                if (!is_foreground) {
                    strcat(status, "+");
                }
            }
        } else if (sscanf(line, "VmSize:\t%s", vm_size) == 1) {
            // Extract virtual memory size
        }
    }

    fclose(file);

    // Obtain process group
    if (pgrp == 0) {
        snprintf(proc_path, sizeof(proc_path), "/proc/%d/stat", pid);

        file = fopen(proc_path, "r");
        if (file == NULL) {
            perror("fopen");
            return;
        }

        int dummy;
        if (fscanf(file, "%d %*s %*c %d", &dummy, &pgrp) != 2) {
            perror("fscanf");
        }

        fclose(file);
    }

    // Obtain executable path
    snprintf(proc_path, sizeof(proc_path), "/proc/%d/exe", pid);
    ssize_t len = readlink(proc_path, exe_path, sizeof(exe_path) - 1);
    if (len != -1) {
        exe_path[len] = '\0';
    } else {
        perror("readlink");
    }

    // Print the requested information
    printf("PID: %d\n", pid);
    printf("Process Status: %s\n", status);
    printf("Process Group: %d\n", pgrp);
    printf("Virtual Memory: %s\n", vm_size);
    printf("Executable Path: %s\n", exe_path);
}