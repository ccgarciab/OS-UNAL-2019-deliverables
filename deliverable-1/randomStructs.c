#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

typedef struct dogType{

    char name[33];
    char type[33];
    int age;
    char breed[17];
    int size;
    float weight;
    char sex;
    int doc_id; //unique number for naming medical records
    int prev;
    int next;
} dogType;

int namesSize = 1000, breedsSize = 5, typesSize = 6, globalTmpSize = 33;

void inserting(char fileName[], char data[][globalTmpSize]){

    FILE *file;

    file = fopen(fileName, "r");

    if(file == NULL){

	perror("Reading File error");
        exit(-1);
    }
    for(int i = 0; !feof(file); i++) fscanf(file, " %s", data[i]);
    int closeVeredict = fclose(file);

    if(closeVeredict  == EOF){

	      perror("Closing File error");
        exit(-1);
    }
}

void randomStructs(int numberPets, char names[][33], char breeds[][17], char types[][33]){


    FILE *file;


    dogType *randpet;

    dogType *pets = (dogType *)malloc (sizeof(dogType)*numberPets);

    randpet = malloc(sizeof(dogType));
    if(randpet == NULL){
	     perror("malloc error");
        exit(-1);
    }

    file = fopen("dataDogs.dat", "wb+");
    if(file == NULL){
	       perror("Open file error");
        exit(-1);
    }

    srand48(time(NULL)); //CPU time Seed

    for(int i = 0; i < numberPets; i++){
      strcpy(randpet->name, names[(int)(drand48()*namesSize)]);
      strcpy(randpet->type, types[(int)(drand48()*typesSize)]);
      randpet->age = (int)(drand48()*15);
      strcpy(randpet->breed, breeds[(int)(drand48()*breedsSize)]);
      randpet->size = (int)(drand48()*180);
      randpet->weight = drand48()*80;
      randpet->sex = (drand48() > 0.5)?'H':'M';
      randpet->next = -1;
      randpet->prev = -1;
      randpet->doc_id = -1;

      pets[i] = *randpet;
    }

        int writeVeredict = fwrite(pets, sizeof(dogType), numberPets, file);

        if(writeVeredict < numberPets){

           perror("Writting file error");
          exit(-1);
        }


    free(randpet);
        free(pets);

    writeVeredict = fclose(file);
    if(writeVeredict == EOF){
	     perror("fclose error");
        exit(-1);
    }

}


int main() {
    dogType *myDogType = malloc(sizeof(dogType));
    char names[namesSize][sizeof(myDogType->name)], breeds[breedsSize][sizeof(myDogType->breed)],
            types[typesSize][sizeof(myDogType->name)];

    int numStructs = 10000000;

    globalTmpSize = sizeof(myDogType->name);
    inserting("names.txt", names);
    inserting("types.txt", types);

    globalTmpSize= sizeof(myDogType->breed);
    inserting("breeds.txt", breeds);

    randomStructs(numStructs, names, breeds, types);

	return 0;

}

