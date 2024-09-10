#include "headers.h"

pid_t FgPid = -100;
char *FgName;
double Time_tt = 0;

// BG Initialise()
// {
//     BG b = (BG)malloc(sizeof(struct BgProcesses));
//     PIDSCount = 0;
//     return b;
// }

void SystemCommands(char **args, int num_args, bool run_in_background)
{
    pid_t pid = fork();
    if (run_in_background)
    {
        setpgid(0, 0);
    }
    if (pid < 0)
    {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        if (strcmp(args[0], "sleep") == 0 && num_args == 2)
        {
            unsigned int sleep_time = atoi(args[1]);
            sleep(sleep_time);
            exit(EXIT_SUCCESS);
        }
        char *DupArgs[MAX_SIZE];
        int pk;
        for (pk = 0; pk < num_args; pk++)
        {
            if (strcmp(args[pk], ">") == 0 || strcmp(args[pk], ">>") == 0 || strcmp(args[pk], "<") == 0)
                continue;
            DupArgs[pk] = (char *)malloc(strlen(args[pk]) + 1);
            strcpy(DupArgs[pk], args[pk]);
        }
        DupArgs[pk] = NULL;
        CheckForRedirction(num_args, args);
        // setpgid(0, 0);
        execvp(DupArgs[0], DupArgs);
        fprintf(stderr, "ERROR: '%s' is not a valid command\n", args[0]); // Print error message
        exit(EXIT_FAILURE);
    }
    else
    {
        if (run_in_background)
        {
            BgArray[PIDSCount].Pids=pid;
            BgArray[PIDSCount++].Name=strdup(args[0]);
            // b->Pids[PIDSCount] = pid;
            // b->Name[PIDSCount++] = strdup(args[0]);
            printf("%d\n", pid);
        }
        else
        {
            int status1;
            Time_tt = 0;
            time_t start = time(NULL);
            FgPid = pid;
            FgName = strdup(args[0]);
            waitpid(pid, &status1, 0);

            FgPid = -100;
            time_t end = time(NULL);
            Time_tt = ((double)(end - start));
        }
    }
}

void pipes(char **args, int num_args, bool run_in_background)
{
    int Input[MAX_SIZE];
    int Output[MAX_SIZE];
    int NumPipes = 0;
    int pipe_fd[2];

    // Save the original stdin and stdout
    int original_stdin = dup(STDIN_FILENO);
    int original_stdout = dup(STDOUT_FILENO);

    for (int i = 0; i < num_args; i++)
    {
        if (strcmp(args[i], "|") == 0)
        {
            NumPipes += 1;
        }
    }
    NumPipes++;

    char **current_args = args; // Keep a separate pointer to the current position in args

    for (int i = 0; i < NumPipes; ++i)
    {
        if (pipe(pipe_fd) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        if (i < NumPipes - 1)
        {
            dup2(pipe_fd[1], STDOUT_FILENO);
            close(pipe_fd[1]);
        }
        else
        {
            dup2(original_stdout, STDOUT_FILENO);
            close(original_stdout);
        }

        int tempArgs = 0;
        while (current_args[tempArgs] != NULL && strcmp(current_args[tempArgs], "|") != 0)
        {
            ++tempArgs;
        }
        SystemCommands(current_args, tempArgs, run_in_background);

        // Move current_args to the next command
        current_args += tempArgs + 1;

        tempArgs = 0;

        if (i < NumPipes - 1)
        {
            dup2(pipe_fd[0], STDIN_FILENO);
            close(pipe_fd[0]);
        }
        else
        {
            dup2(original_stdin, STDIN_FILENO);
            close(original_stdin);
        }
    }
}

void BgProcessesStatus()
{
    // printf("%d\n",PIDSCount);

    for (int i = 0; i < PIDSCount; i++)
    {
        int status2;
        pid_t result = waitpid(BgArray[i].Pids, &status2, WNOHANG);
        // printf("%d\n",result);
        if (result)
        {
            if (WIFEXITED(status2) > 0)
            {
                if (WEXITSTATUS(status2) == 0){
                    printf("%s exited normally (%d)\n", BgArray[i].Name, BgArray[i].Pids);
                }
                else
                {
                    printf("%s exited abnormally (%d)\n", BgArray[i].Name, BgArray[i].Pids);
                }
            }
            PIDSCount--;
            BgArray[i]=BgArray[PIDSCount];
            i--;
        }
    }
}
void selectionSort(int n)
{
    if (n <= 0) return;  // Boundary check
    
    int i, j, min_idx;
    
    // Iterate through the array
    for (i = 0; i < n - 1; i++)
    {
        min_idx = i;
        for (j = i + 1; j < n; j++)
        {
            if (BgArray[j].Pids < BgArray[min_idx].Pids)
            {
                min_idx = j;
            }
        }

        if (i != min_idx)
        {   
            struct BgProcesses temp=BgArray[min_idx];
            BgArray[min_idx]=BgArray[i];
            BgArray[i]=temp;
        }
    }
}

int compare (const void* a,const void* b)
{
    BgProcesses* x = (BgProcesses*)a;
    BgProcesses* y = (BgProcesses*)b;
    if(x->Pids < y->Pids)
    {
        return -1;
    }
    else if(x->Pids == y->Pids)
    {
        return 0;
    }
    else
    {
        return 1;
    }


    

}
void PrintActivities()
{
    // selectionSort(PIDSCount);
    qsort(BgArray,PIDSCount,sizeof(struct BgProcesses),compare);
    // printf("Count : %d\n",PIDSCount);
    for (int i = 0; i < PIDSCount; i++)
    {
        char proc_path[100];
        snprintf(proc_path, sizeof(proc_path), "/proc/%d/stat", BgArray[i].Pids);

        FILE *file = fopen(proc_path, "r");
        if (file == NULL)
        {
            perror(proc_path);  // Print the specific file that failed to open
            return;
        }
        char status3;
        fscanf(file, "%*d %*s %c", &status3);
        fclose(file);
        
        if (status3 != 'T')
        {
            printf("%d : %s - Running\n", BgArray[i].Pids, BgArray[i].Name);
        }
        else
        {
            printf("%d : %s - Stopped\n", BgArray[i].Pids, BgArray[i].Name);
        }
    }
}
