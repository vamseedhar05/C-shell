#include "headers.h"

void BgPidEXEC(pid_t pid){
    int flag=0;
    for(int i=0;i<PIDSCount;i++){
        if(pid==BgArray[i].Pids){
            flag=1;
            break;
        }
    }
    if(!flag){
        printf("No such process found\n");
        return;
    }
    char proc_path[100];
    snprintf(proc_path, sizeof(proc_path), "/proc/%d/stat", pid);

    FILE *file = fopen(proc_path, "r");
    if (file == NULL) {
        perror("fopen");
        return;
    }
    char status;
    fscanf(file,"%*d %*s %c",&status);
    fclose(file);
    if(status=='T'){
        kill(pid,SIGCONT);
    }
}


void FgPidEXEC(pid_t pid){
    int flag=0;
    int idx=0;
    for(int i=0;i<PIDSCount;i++){
        if(pid==BgArray[i].Pids){
            flag=1;
            idx=i;
            break;
        }
    }
    if(!flag){
        printf("No such process found\n");
        return;
    }
    BgPidEXEC(pid);
    FgPid=pid;
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU,SIG_IGN);
    tcsetpgrp(STDIN_FILENO,pid);
    int status2;
    waitpid(pid,&status2,WUNTRACED);
    FgPid=-100;
    tcsetpgrp(STDIN_FILENO,getpgid(0));
    signal(SIGTTIN,SIG_DFL);
    signal(SIGTTOU,SIG_DFL);
    for(int i=idx;i<(PIDSCount)-1;i++){
        BgArray[i]=BgArray[i+1];
    }
    PIDSCount--;
}