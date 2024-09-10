#include "headers.h"


void warp_driver(char** args,int num_args,char *current_dir,char *HOME,char *previous_dir){
     if(num_args == 1) {
        getcwd(current_dir,MAX_SIZE);
        chdir(HOME);
        strcpy(previous_dir,current_dir);
        strcpy(current_dir, HOME);
        printf("%s\n",HOME);
    }else{
        for (int j = 1; j < num_args; j++) {
            warp_function(args[j], previous_dir, HOME, current_dir);
        }
    }
}


void warp_function(const char *new_path, char *previous_dir, const char *HOME, char *current_dir) {
    getcwd(current_dir,MAX_SIZE);
    if(strcmp(new_path,"-")==0){
        chdir(previous_dir);
    }
    else if(new_path[0]=='~'){
        char temp[MAX_SIZE];
        strcpy(temp, HOME);
        strcat(temp, new_path + 1);
        printf("%s\n", temp);
        chdir(temp);
    }
    else{
        char temp[MAX_SIZE];
        strcpy(temp, new_path);
        if (chdir(temp) == -1)
        {
            perror("chdir");
            return;
        }
        getcwd(temp,MAX_SIZE);
        printf("%s\n",temp);
    }
        strcpy(previous_dir,current_dir);        
}
