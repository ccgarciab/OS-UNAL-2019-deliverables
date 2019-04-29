#include <stdio.h>
#include <stdlib.h>

#include "pet_globals.h"

int hash(char *word){           //djb2

   unsigned long hash = 5381;
   int c;

   while((c = *word++)){

        //same as hash = hash * 33 + c, but faster(?)
        hash = ((hash  << 5) + hash) + c;
   }

   return (int)(hash % T_SIZE);
}

int init_table(int **table, FILE *db){

    int *table_hits = calloc(T_SIZE, sizeof(int));

    dogType *pet = malloc(sizeof(dogType));

    while(fread(pet, sizeof(dogType), 1, db)){

         int h = hash(pet->name);
         table_hits[h]++;
    }

    if(!feof(db)) return -1;

    clearerr(db);

    for(int i = 0; i < T_SIZE; i++){

         if(table_hits[i]){

             table[i] = calloc(table_hits[i], sizeof(int));
         }
    }

    rewind(db);

    for(int curr = 0; fread(pet, sizeof(dogType), 1, db); curr++){

        int h = hash(pet->name);
        int *pos = table[h];
        for(;*pos; pos++);
        *pos = curr;
    }

    if(!feof(db)) return -1;

    clearerr(db);

    return 0;
}

int insert_val_at_pos(int **table, int val, int pos){

    int *ptr = table[pos];

    if(!ptr){

        ptr = calloc(2, sizeof(int));

        if(!ptr) return -1;

        ptr[0] = val;
        table[pos] = ptr;
        return 0;
    }

    int len = 0;
    for(; ptr[len]; len++);

    int *new_ptr = realloc(ptr, len + 2);

    if(!new_ptr) return -1;

    table[pos] = new_ptr;
    table[pos][len] = val;
    table[pos][len + 1] = 0;

    return 0;
}
