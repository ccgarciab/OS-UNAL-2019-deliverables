#include <stdio.h>
#include <stdlib.h>

#include "pet_globals.h"
#include "pet_input.h"

int *table[T_SIZE];
int current = 0;

int hash(char *word){           //djb2

   unsigned long hash = 5381;
   int c;

   while((c = *word++)){

        //same as hash = hash * 33 + c, but faster(?)
        hash = ((hash  << 5) + hash) + c;
   }

   return (int)(hash % T_SIZE);
}

int main(int argc, char* argv[]){

    dogType *pet = malloc(sizeof(dogType));

    fill_pet_info(pet);

    printf("pet name: %s\n", pet->name);
    printf("pet age: %i\n", pet->age);
}
