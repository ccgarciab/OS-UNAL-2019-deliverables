#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "pet_globals.h"
#include "pet_file.h"
#include "pet_output.h"

#include "error_handle.h"

#define EDITOR "nano"


/*Prints the fields of the [pet] struct to stdin.*/
void print_pet(dogType *pet) {

    printf(" name: %s\n \
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

    printf("\
clinic history id: %d\n \
previous record: %d\n \
next record: %d\n \
**************************************************\n",
           pet->doc_id, pet->prev, pet->next);
}

/*Opens a medical record identified with [doc_id]
    in a text editor.*/
void open_medical_record(int doc_id) {

    pid_t pid = fork();

    if (pid < 0) sys_error("fork failure\n");

    if (pid) return;

    char arg[30];
    sprintf(arg, "./records/%d", doc_id);

    if (execl(EDITOR, arg, NULL))
        sys_error("exec error\n");
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
