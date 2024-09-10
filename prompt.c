#include "headers.h"

void prompt(char *HOME) {

    uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid);
    if (pw == NULL) {
        perror("getpwuid");
        exit(EXIT_FAILURE);
    }
    char *username = pw->pw_name;

    char hostname[MAX_SIZE];
    if (gethostname(hostname, sizeof(hostname)) == -1) {
        perror("gethostname");
        exit(EXIT_FAILURE);
    }

    char cwd[MAX_SIZE];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd");
        exit(EXIT_FAILURE);
    }
    
    int home_len=strlen(HOME);
    int curr_len=strlen(cwd);
    if(strcmp(cwd,HOME)==0){
        strcpy(cwd,"~");
        printf("<%s@%s:%s", username, hostname, cwd);
    }

    else if(curr_len > home_len ) {
        char temp[MAX_SIZE];
        temp[0] = '~';
        int j = 0;
        while (j < (curr_len - home_len)) {
            temp[j + 1] = cwd[home_len + j];
            j++;
        }
        temp[j+1] = '\0'; 
        printf("<%s@%s:%s", username, hostname, temp);
    }
    else{
        printf("<%s@%s:%s", username, hostname, cwd);
    }
    if(Time_tt>2){
        printf(" %s : %0.1fs > ",FgName,Time_tt);
        Time_tt=0;
    }
    else{
        printf("> ");
    }
}
