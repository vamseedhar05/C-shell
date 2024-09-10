#include "headers.h"

void CTRL_C(int signal_1)
{
    signal(SIGINT, CTRL_C);
    if (FgPid != -100)
    {
        kill(FgPid, SIGINT);
    }
}

void CTRL_D()
{
    for (int i = 0; i < PIDSCount; i++)
    {
        kill(BgArray[i].Pids, SIGINT);
    }
    printf("\n");
    exit(EXIT_SUCCESS);
}

void PingSignal(int num_args, char **args)
{
    if (num_args != 3)
    {
        printf("Usage: %s <PID> <SIGNAL>\n", args[0]);
    }
    pid_t pid = atoi(args[1]);
    int signal = atoi(args[2]) % 32;
        if(kill(pid,0)==-1){
        printf("No such Processes Exist\n");
        return;
    }
    if (pid <= 0)
    {
        printf("Invalid pid\n");
        return;
    }
    if(signal==20){
        kill(pid,SIGSTOP);
    }
    else{
        kill(pid, signal);
    }
    printf("Sent signal %d to process with pid %d\n", signal, pid);
}


void CTRL_Z(int signal_1)
{
    // Register the signal handler again to ensure it's persistent
    signal(SIGTSTP, CTRL_Z);

    if (FgPid != -100)
    {
        printf("Ctrl+Z: Pushing the foreground process to the background...\n");

        // Send SIGSTOP to the foreground process to stop it
        if (kill(FgPid, SIGTSTP) == 0)
        {
            // Add the process to the background array
            BgArray[PIDSCount].Name = strdup(FgName);
            BgArray[PIDSCount++].Pids = FgPid;

            // Change the state from "Running" to "Stopped"
            printf("Process with PID %d (%s) stopped and pushed to the background.\n", FgPid, FgName);

            // Reset the foreground process variables
            FgPid = -100;
            free(FgName);
            FgName = NULL;
        }
        else
        {
            perror("Ctrl+Z: Failed to stop the foreground process");
        }
    }
    else
    {
        printf("Ctrl+Z: No foreground process is running.\n");
    }
}