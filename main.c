#include <stdio.h>
#include <string.h>
#include <regex.h>
#include "src/directory_utils.h"
#include <unistd.h>
#include "src/options.h"

struct options to_avoid;


void print_help() {
    printf("run ./mangen [DIR_PATH] [OPTIONS] \n");
    printf("if DIR_PATH empty program will work with current directory \n");
    printf("OPTIONS: \n");
    printf("\t-h print this help \n");
    printf("\t-v show version info \n");
}

void print_version() {
    printf("version 1.0\n");
    printf("author: Akulov Artem\n");
}

//work with comand line
int main(int argc, char *argv[])
{
    char *path = NULL;
    char current_path[1024];
    to_avoid.type = 0;
    if (argc > 1 && argv[1][0] != '-') {
        path = argv[1];
    } else {
        if (getcwd(current_path, sizeof(current_path)) != NULL) {
            path = current_path;
        } else {
            printf("Error: failed to get current directory\n");
            return 1;
        }
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            print_help();
            return 0;
        }
        else if (strcmp(argv[i], "-v") == 0) {
            print_version();
            return 0;
        }
        else if (strcmp(argv[i], "-e") == 0 && i + 1 < argc) {
            char *param = argv[i + 1];
            if (param[0] == '/' || strchr(param, '*') || strchr(param, '.')) {
                to_avoid.type = 1;
                if (regcomp(&to_avoid.regex, param, REG_EXTENDED) != 0) {
                    printf("Invalid regex: %s\n", param);
                    return 1;
                }
            }
            else {
                to_avoid.type = 2;
                to_avoid.name = param;
            }
        }
    }
    
    manifest_directory(path);
    
    return 0;
}
