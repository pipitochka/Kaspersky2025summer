#ifndef OPTIONS_H
#define OPTIONS_H
#include <regex.h>

//struct to understand type of expression which we will avoid
struct options {
    char *name;
    regex_t regex;
    int type;
};

#endif //OPTIONS_H
