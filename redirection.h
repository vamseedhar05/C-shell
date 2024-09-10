#ifndef __REDIRECTION_H
#define __REDIRECTION_H

void CheckForRedirction(int num_args, char** args);
void Redirect(char* InputFile, char* OutputFile, bool Input, bool Output, bool Append);

#endif