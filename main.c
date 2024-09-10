#include "headers.h"

void tokenize_command(char *command, char **args, int *num_args)
{
    char *token;
    int index = 0;

    token = strtok(command, " \t");
    while (token != NULL)
    {
        args[index++] = token;
        token = strtok(NULL, " \t");
    }
    args[index] = NULL;
    *num_args = index;
}

void tokenize_input(char *input, char **commands, char **bg_commands, int *num_bg, int *num_commands)
{
    char token[MAX_SIZE];
    int index_1 = 0;
    int index_2 = 0;
    int j = 0;
    for (int i = 0; i < strlen(input); i++, j++)
    {
        token[j] = input[i];
        if (token[j] == ';')
        {
            token[j] = '\0';
            commands[index_1++] = strdup(token);
            j = -1;
        }
        else if (token[j] == '&')
        {
            token[j] = '\0';
            bg_commands[index_2++] = strdup(token);
            j = -1;
        }
    }
    if (j != 0)
    {
        token[j] = '\0';
        commands[index_1++] = strdup(token);
    }
    *num_commands = index_1;
    *num_bg = index_2;
}

struct BgProcesses BgArray[MAX_SIZE];

int PIDSCount=0;

int main()
{
    signal(SIGINT, CTRL_C);
    signal(SIGSTOP, CTRL_Z);
    char HOME[MAX_SIZE];
    getcwd(HOME, MAX_SIZE);
    char *dup_home;
    dup_home = strdup(HOME);
    char previous_dir[MAX_SIZE];
    strcpy(previous_dir, HOME);
    char current_dir[MAX_SIZE];
    strcpy(current_dir, HOME);
    char final_inp[MAX_SIZE];
    int need_to_execute = 0;

    while (1)
    {
        prompt(HOME);
        char input[MAX_SIZE];
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            CTRL_D();
        }
        input[strlen(input) - 1] = '\0';
        BgProcessesStatus();
        char *commands[MAX_SIZE];
        char *bg_commands[MAX_SIZE];
        int num_bg = 0;
        int num_commands = 0;
        tokenize_input(input, commands, bg_commands, &num_bg, &num_commands);
        char *cmd;
        char *args[MAX_SIZE];
        int num_args = 0;
        bool run_in_background = false;
        for (int i = 0; i < num_bg; i++)
        {
            cmd = strdup(bg_commands[i]);
            tokenize_command(cmd, args, &num_args);
            run_in_background = true;
            pipes(args, num_args, 1);
            if (i == 0)
            {
                strcpy(final_inp, bg_commands[i]);
            }
            else
            {
                strcat(final_inp, "&");
                strcat(final_inp, bg_commands[i]);
            }
        }
        int flag = 0;
        for (int i = 0; i < num_commands; i++)
        {
            cmd = strdup(commands[i]);
            char *doop = strdup(cmd);

            tokenize_command(cmd, args, &num_args);

            if (strcmp(args[0], "exit") == 0)
            {
                char *exit_1 = "exit";
                save_command(exit_1, dup_home);
                exit(EXIT_SUCCESS);
            }
            else if (strcmp(args[0], "pastevents") == 0)
            {
                if (num_args == 1)
                {
                    print_history(dup_home);
                    flag = 1;
                }
                else
                {
                    if (strcmp(args[1], "purge") == 0)
                    {
                        flag = 1;
                        clearFile(dup_home);
                    }
                    else if (strcmp(args[1], "execute") == 0)
                    {
                        char *executed_arg = execute_arg(args[2], dup_home);
                        if (executed_arg != NULL)
                        {
                            doop = strdup(executed_arg);
                            commands[i] = strdup(executed_arg);
                            i--;
                            continue;
                        }
                        else
                        {
                            flag = 1;
                        }
                    }
                }
            }
            else if (strcmp(args[0], "warp") == 0)
            {
                warp_driver(args, num_args, current_dir, HOME, previous_dir);
            }
            else if (strcmp(args[0], "peek") == 0)
            {
                peek_driver(args, num_args, current_dir, HOME, previous_dir);
            }
            else if (strcmp(args[0], "seek") == 0)
            {
                seek_driver(num_args, args);
            }
            else if (strcmp(args[0], "proclore") == 0)
            {
                int pid;

                if (num_args == 1)
                {
                    pid = getpid();
                }
                else if (num_args == 2)
                {
                    pid = atoi(args[1]);
                }
                else
                {
                    fprintf(stderr, "Usage: %s [PID]\n", args[0]);
                    return 1;
                }

                print_process_info(pid);
            }
            else if (strcmp(args[0], "iMan") == 0)
            {
                if (num_args != 2)
                {
                    fprintf(stderr, "Usage: %s <command_name>\n", args[0]);
                    continue;
                }
                else
                {
                    char *command_name = strdup(args[1]);
                    fetch_man_page(command_name);
                    free(command_name);
                }
            }
            else if (strcmp(args[0], "activities") == 0)
            {
                PrintActivities();
            }
            else if (strcmp(args[0], "ping") == 0)
            {
                PingSignal(num_args, args);
            }
            else if (strcmp(args[0], "neonate") == 0)
            {
                if (num_args != 3)
                {
                    printf("Invalid Comman Usage\n");
                    continue;
                }
                int time_arg = atoi(args[2]);
                printf("Press 'x' to exit neonate mode\n");

                setbuf(stdout, NULL);

                enableRawMode();
                Time_tt = 0;
                time_t start = time(NULL);
                FgName = strdup(args[0]);
                print_most_recent_pid(time_arg);
                time_t end = time(NULL);
                Time_tt = ((double)(end - start));

                disableRawMode();
            }
            else if (strcmp(args[0], "bg") == 0)
            {
                if (num_args != 2)
                {
                    printf("Invalid arguments for bg\n");
                }
                pid_t pid = atoi(args[1]);
                BgPidEXEC(pid);
            }
            else if (strcmp(args[0], "fg") == 0)
            {
                if (num_args != 2)
                {
                    printf("Invalid arguments for fg\n");
                }
                pid_t pid = atoi(args[1]);
                FgPidEXEC(pid);
            }
            else
            {
                pipes(args, num_args, 0);
            }
            if (i == 0)
            {
                if (run_in_background)
                {
                    strcat(final_inp, "&");
                    strcat(final_inp, doop);
                }
                else
                {
                    strcpy(final_inp, doop);
                }
            }
            else
            {
                strcat(final_inp, ";");
                strcat(final_inp, doop);
            }
        }
        if (!flag)
            save_command(final_inp, dup_home);
    }
}
