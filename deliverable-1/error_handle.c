#include <stdio.h>
#include <stdlib.h>


/*Function for abreviating printing a message, last errno,
    and exiting with failure code*/
void sys_error(char *msg){

    perror(msg);
    exit(EXIT_FAILURE);
}
