#include <stdio.h>

#include "pet_globals.h"
#include "pet_output.h"

void print_pet(dogType *pet){

    printf(" name: %s\n \
type: %s\n \
breed: %s\n \
age: %d years\n \
size: %d cm\n \
weight: %f kg\n \
sex: %c\n \
*________________________________________________*\n",
       pet->name, pet->type, pet->breed, pet->age, pet->size,
       pet-> weight, pet->sex);
}

void print_pet_debug(dogType *pet){

    print_pet(pet);

    printf("\
clinic history id: %d\n \
previous record: %d\n \
next record: %d\n \
**************************************************\n", 
    pet->doc_id, pet->prev, pet->next);
}
