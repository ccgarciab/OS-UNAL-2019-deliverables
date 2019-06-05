#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "pet_globals.h"

#include "pet_input.h"
#include "pet_file.h"
#include "pet_output.h"

#include "error_handle.h"

#define EDITOR "nano "


/*Prints the fields of the [pet] struct to stdin.*/
void print_pet(dogType *pet) {

    printf(
            "*________________________________________________*\n \
name: %s\n \
type: %s\n \
breed: %s\n \
age: %d years\n \
size: %d cm\n \
weight: %f kg\n \
sex: %c\n \
*________________________________________________*\n",
            pet->name, pet->type, pet->breed, pet->age, pet->size,
            pet->weight, pet->sex);
}

/*Prints the fields of the [pet] struct to stdin, including
    info related to the implementation of on-disk linked lists.*/
void print_pet_debug(dogType *pet) {

    print_pet(pet);

    printf(
            "*************************************************\n \
clinic history id: %d\n \
previous record: %d\n \
next record: %d\n \
**************************************************\n",
            pet->doc_id, pet->prev, pet->next);
}

/*Opens a medical record placed at [path] in a text editor.*/
void open_medical_record(char *path) {

    pid_t pid = fork();

    if (pid < 0) sys_error("fork failure\n");

    if (pid){
        
        printf("Processing\n");
        wait(NULL);
        return;
    }

    char command[39] = EDITOR;
    strcat(command, path);

    printf("executing: %s\n", command);    
    
    system(command);
    
    printf("Confirm with any key after saving your changes: ");
    char anyk[2]; 
    get_bounded_str(anyk, 1);
}

/*Prints all dogType structs in the same linked list at [db],
    starting with the one at [line].*/
void print_list(FILE *db, int line) {

    dogType pet;

    read_pet_at_line(db, &pet, line);

    while (pet.next != -1) {

        print_pet(&pet);
        read_pet_at_line(db, &pet, pet.next);
    }

    print_pet(&pet);
}

/*Puts the basic info of a [pet] in a medical record [file]*/
void fill_new_mr(FILE *file, dogType *pet){

    fprintf(file, "Name: %s\n", pet->name);
    fprintf(file, "Type:   %s\n", pet->type);
    fprintf(file, "Age:   %i\n", pet->age);
    fprintf(file, "Breed:   %s\n", pet->breed);
    fprintf(file, "Size: %i\n", pet->size);
    fprintf(file, "Weight:   %f\n", pet->weight);
    fprintf(file, "Sex: %c\n", pet->sex);
}


/*Outputs a log of activity to [file] specifying among others 
    the command executed specified by [opt], the argument [arg] 
    of said command and the ip of the client, specified by [ipstr]*/
void output_log(FILE *file, char *ipstr, char opt, char *arg){

    time_t t = time(NULL);
    struct tm ltm = *localtime(&t);
    char log[100];

    sprintf(log, "%d|%02d|%02dT%02d:%02d:%02d Cliente ", 
        ltm.tm_year + 1900, ltm.tm_mon + 1, ltm.tm_mday,
        ltm.tm_hour, ltm.tm_min, ltm.tm_sec);

    strcat(log, ipstr);

    switch(opt){
    
        case '1':

            strcat(log, " insercion ");
            break;
            
        case '2':
 
            strcat(log, " lectura ");
            break;
        
        case '3':

            strcat(log, " borrado ");
            break;

        case '4':

            strcat(log, " busqueda ");
            break;

        default:

            return;
    }
    
    strcat(log, arg);
    
    fprintf(file, "%s\n", log);
}
