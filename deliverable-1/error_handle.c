#include <stdio.h>
#include <stdlib.h>

void sys_error(char *msg){

    perror(msg);
    exit(EXIT_FAILURE);
}
