#include <stdio.h>
#include <stdlib.h>

#include "pet_globals.h"

#define BUF_SIZE 700000

int hash(char *word){           //djb2

   unsigned long hash = 5381;
   int c;

   while((c = *word++)){

        //same as hash = hash * 33 + c, but faster(?)
        hash = ((hash  << 5) + hash) + c;
   }

   return (int)(hash % T_SIZE);
}

long digest(char *word){

    unsigned long hash = 0;
    int c;

    while((c = *word++)){

        hash = c + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}

int init_table(node *table, FILE *db){

    dogType *buffer = malloc(sizeof(dogType) * BUF_SIZE);

    if(!fread(buffer, sizeof(dogType), 1, db)){

        clearerr(db);
        return -1;
    }

    size_t elems_read = fread(buffer, sizeof(dogType), BUF_SIZE, db);
    int line_index = 1;

    while(elems_read){

        dogType *last = buffer + elems_read;
        for(dogType *curr = buffer; curr < last; curr++, line_index++){

            int h = hash(curr->name);
            long d = digest(curr->name);

            if(!table[h].line){

                table[h].line = line_index;
                table[h].signature = d;
            }
            else if(d != table[h].signature){

                for(int i = 0; table[h].line; i++){

                    h += (i * i);
                }
            }
        }

        elems_read = fread(buffer, sizeof(dogType), BUF_SIZE, db);
    }

    free(buffer);

    int result = ferror(db);
    clearerr(db);

    return result;
}
