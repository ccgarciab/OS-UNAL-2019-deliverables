#include <stdio.h>
#include <stdlib.h>

#include "pet_globals.h"
#include "pet_hashtable.h"
#include "pet_input.h"

int *table[T_SIZE];
int current = 0;

int main(int argc, char* argv[]){

    dogType *pet = malloc(sizeof(dogType));

    fill_pet_info(pet);

    printf("pet name: %s\n", pet->name);
    printf("pet age: %i\n", pet->age);
}
