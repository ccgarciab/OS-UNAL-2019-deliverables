#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "pet_globals.h"
#include "error_handle.h"

int namesSize = 1000, breedsSize = 5, typesSize = 6, globalTmpSize = 33;

/*
 * Reads the FileName and put the data (words
 * separated by breaklines) inside the data array
 */
void inserting(char fileName[], char data[][globalTmpSize]) {

    FILE *file;
    file = fopen(fileName, "r");

    if (file == NULL) sys_error("Reading File error");

    for (int i = 0; !feof(file); i++) fscanf(file, " %s", data[i]);

    int closeVeredict = fclose(file);

    if (closeVeredict == EOF) sys_error("Closing File error");

}

/*
 * Put a "numberPets" structs inside dataDogs.dat File using random
 * data generated using the "names", "breeds" and "types" data arrays.
 */
void randomStructs(int numberPets, char names[][33], char breeds[][17], char types[][33]) {


    FILE *file;
    dogType *randpet;

    dogType *pets = (dogType *) malloc(sizeof(dogType) * numberPets);
    randpet = malloc(sizeof(dogType));

    if (randpet == NULL) sys_error("malloc error");

    file = fopen("dataDogs.dat", "wb+");

    if (file == NULL) sys_error("Open file error");

    srand48(time(NULL)); //CPU time Seed

    for (int i = 0; i < numberPets; i++) {
        strcpy(randpet->name, names[(int) (drand48() * namesSize)]);
        strcpy(randpet->type, types[(int) (drand48() * typesSize)]);
        randpet->age = (int) (drand48() * 15);
        strcpy(randpet->breed, breeds[(int) (drand48() * breedsSize)]);
        randpet->size = (int) (drand48() * 180);
        randpet->weight = drand48() * 80;
        randpet->sex = (drand48() > 0.5) ? 'H' : 'M';
        randpet->next = -1;
        randpet->prev = -1;
        randpet->doc_id = i + 1;

        pets[i] = *randpet;
    }

    int writeVeredict = fwrite(pets, sizeof(dogType), numberPets, file);

    if (writeVeredict < numberPets) sys_error("Writting file error");

    free(randpet);
    free(pets);

    writeVeredict = fclose(file);

    if (writeVeredict == EOF) sys_error("fclose error");

}


int main() {

    dogType *myDogType = malloc(sizeof(dogType));
    char names[namesSize][sizeof(myDogType->name)], breeds[breedsSize][sizeof(myDogType->breed)],
            types[typesSize][sizeof(myDogType->name)];

    int numStructs = 10000000;

    globalTmpSize = sizeof(myDogType->name);
    inserting("names.txt", names);
    inserting("types.txt", types);

    globalTmpSize = sizeof(myDogType->breed);
    inserting("breeds.txt", breeds);

    randomStructs(numStructs, names, breeds, types);
    return 0;
}

