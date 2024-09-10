#include "headers.h"

void CheckForRedirction(int num_args, char** args){
    bool Input=false;
    bool Output=false;
    bool Append=false;
    char InputFile[MAX_SIZE];
    char OutputFile[MAX_SIZE];
    for(int pi=0;pi<num_args;pi++){
        if(strcmp(args[pi],">")==0){
            Output=true;
            strcpy(OutputFile,args[pi+1]);
        }
        else if(strcmp(args[pi],">>")==0){
            Append=true;
            strcpy(OutputFile,args[pi+1]);
        }
        else if(strcmp(args[pi],"<")==0){
            Input=true;
            strcpy(InputFile,args[pi+1]);
        }
    }

    if(Input || Output || Append){
        Redirect(InputFile,OutputFile,Input,Output,Append);
    }
}

void Redirect(char* InputFile, char* OutputFile, bool Input, bool Output, bool Append){
    if(Input){
        char* dupInp=strdup(InputFile);
        dupInp[strlen(InputFile)-1]='\0';
        if (access(dupInp,F_OK) == 0) {
            perror("access");
            exit(1);
        }
        int File_Desc1 = open(InputFile, O_RDONLY);
        if (File_Desc1 == -1) {
            perror("Error opening input file");
            exit(EXIT_FAILURE);
        }

        // Redirect standard input (0) to File_Desc1
        if (dup2(File_Desc1, 0) == -1) {
            perror("Error redirecting standard input");
            exit(EXIT_FAILURE);
        }

        close(File_Desc1);
    }

    if(Output){
        int File_Desc2 = open(OutputFile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (File_Desc2 == -1) {
            perror("Error opening output file");
            exit(EXIT_FAILURE);
        }

        // Redirect standard output (1) to File_Desc2
        if (dup2(File_Desc2, 1) == -1) {
            perror("Error redirecting standard output");
            exit(EXIT_FAILURE);
        }

        close(File_Desc2);
    }

    if(Append){
        int File_Desc3 = open(OutputFile, O_CREAT | O_WRONLY | O_APPEND, 0644);
        if (File_Desc3 == -1) {
            perror("Error opening output file for append");
            exit(EXIT_FAILURE);
        }

        // Redirect standard output (1) to File_Desc3
        if (dup2(File_Desc3, 1) == -1) {
            perror("Error redirecting standard output for append");
            exit(EXIT_FAILURE);
        }

        close(File_Desc3);
    }
}
