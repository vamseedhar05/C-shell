#include "headers.h"

void save_command(char* cmd,char *dup_home) {

    char *current_commands[16];
    int i;
    char temp[MAX_SIZE];
    strcpy(temp,dup_home);
    strcat(temp,"/history.txt");
    FILE *fp = fopen(temp, "r");
    if (fp == NULL) {
        printf("HII\n");
        fprintf(stderr, "Error opening file for reading.\n");
        return;
    }

    char *line = (char *)malloc(sizeof(char) * MAX_SIZE);
    i = 0;
    while(fgets(line, MAX_SIZE, fp) != NULL){
        line[strlen(line) - 1] = '\0';
        current_commands[i] = strdup(line);
        i++;
    }
    fclose(fp);

    if (strcmp(line, cmd) != 0 )
    {
        current_commands[i] = (char *)malloc(1000 * sizeof(char));
        strcpy(current_commands[i], cmd);
        i++;
    }

    int j=0;

    if(i==16)
        j=1;

    fp = fopen( temp, "w");

    while (j < i)
    {
        fprintf(fp, "%s\n", current_commands[j]);
        j++;
    }

    free(line);
    fclose(fp);
}


void print_history(char* dup_home){
    char temp[MAX_SIZE];
    strcpy(temp,dup_home);
    strcat(temp,"/history.txt");
    FILE* fp = fopen(temp, "r");
    if (fp == NULL) {
        perror("Error opening history file");
        return;
    }

    char line[MAX_SIZE];
    while (fgets(line, sizeof(line), fp) != NULL) {
        printf("%s",line);
    }
    fclose(fp);
}


char* execute_arg(char* arg,char* dup_home) {
    char temp[MAX_SIZE];
    strcpy(temp,dup_home);
    strcat(temp,"/history.txt");
    int index_from_last = atoi(arg);
    FILE* fp = fopen( temp, "r");
    if (fp == NULL) {
        perror("Error opening history file");
        return NULL;
    }

    char lines[16][MAX_SIZE]; // Assuming a maximum of 15 commands + 1 empty line
    int line_count = 0;
    while (fgets(lines[line_count], sizeof(lines[line_count]), fp) != NULL) {
        line_count++;
    }

    fclose(fp);

    int index = line_count - index_from_last;
    if (index <= 0 || index > line_count) {
        printf("Invalid index\n");
        return NULL;
    }
     char* executed_cmd = strdup(lines[index]);
     executed_cmd[strlen(executed_cmd)-1]='\0';
     return executed_cmd;
}


void clearFile(char* dup_home) {
    char temp[MAX_SIZE];
    strcpy(temp,dup_home);
    strcat(temp,"/history.txt");
    FILE* fp = fopen(temp , "w");
    if (fp == NULL) {
        perror("Error opening history file");
        return;
    }
    fclose(fp);
}