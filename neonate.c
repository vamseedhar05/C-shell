#include "headers.h"

volatile int stop = 0;

void die(const char *s) {
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

void print_most_recent_pid(int time_arg) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
    while (true) {
        FILE *loadavg_file = fopen("/proc/loadavg", "r");
        if (loadavg_file == NULL) {
            perror("fopen");
            return;
        }

        char buffer[256];
        if (fgets(buffer, sizeof(buffer), loadavg_file) != NULL) {
            float Recent_pid = 0;
            sscanf(buffer, "%*s %*s %*s %*s %f", &Recent_pid);
            if (Recent_pid > 0)
            {
                if (pid != (int)Recent_pid)
                {
                    pid = (int)Recent_pid;
                    printf("%d\n", pid);
                }
            }
        }

        fclose(loadavg_file);

        sleep(time_arg); // Sleep for the specified time_arg in seconds
    }
    }
    else
    { 
        char X;
        if (read(STDIN_FILENO, &X, 1) == 1 && X == 'x')
        {
            kill(pid, SIGKILL);
        }

    }
}
