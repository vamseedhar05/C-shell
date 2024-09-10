#ifndef HEADERS_H_
#define HEADERS_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include <stdbool.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <limits.h>
#include <grp.h>
#include <fcntl.h>
#include <fnmatch.h>
#include <time.h>
#include <netdb.h>
// #include <libproc.h>
#include <arpa/inet.h>
// #include <sys/sysctl.h>
#include <sys/param.h>
#include <stdint.h>
#include <signal.h>
#include <termios.h>

#define MAX_SIZE 4096
#define COLOR_RESET "\x1B[0m"
#define COLOR_BLUE "\x1B[34m"
#define COLOR_GREEN "\x1B[32m"
#define COLOR_WHITE "\x1B[37m"


struct BgProcesses{
    char* Name;
    pid_t Pids;
};


extern int PIDSCount;

#include "prompt.h"
#include "warp.h"
#include "peek.h"
#include "pastevents.h"
#include "seek.h"
#include "proclore.h"
#include "redirection.h"
#include "pipes.h"
#include "iman.h"
#include "ping.h"
#include "neonate.h"
#include "FGBG.h"

typedef struct BgProcesses BgProcesses;

extern double Time_tt;
extern pid_t FgPid;
extern BgProcesses BgArray[MAX_SIZE];
extern char * FgName;

#endif