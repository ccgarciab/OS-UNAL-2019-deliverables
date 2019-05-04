#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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

long digest(char *word){        //sdbm

    unsigned long hash = 0;
    int c;

    //same as hash = hash * 65599 + c, but faster(?)
    while((c = *word++)){

        hash = c + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}

int init_table(node *table, FILE *db){

    dogType *buffer = malloc(sizeof(dogType) * BUF_SIZE);

    size_t elems_read = fread(buffer, sizeof(dogType), BUF_SIZE, db);
    int line_index = 1;

    while(elems_read){

        dogType *last = buffer + elems_read;
        for(dogType *curr = buffer; curr < last; curr++, line_index++){

            int h = hash(curr->name);
            long d = digest(curr->name);

            for(int i = 0; table[h].signature && table[h].signature != d; i++){

                h += (i * i);
                if(T_SIZE <= h){

                    printf("Droping %s \nCan't init table\n", curr->name);
                    free(buffer);
                    return -2;
                }
            }

            if(!table[h].line){

                table[h].line = line_index;
                table[h].signature = d;
            }
        }

        elems_read = fread(buffer, sizeof(dogType), BUF_SIZE, db);
    }

    free(buffer);

    int result = ferror(db);
    clearerr(db);

    return result;
}

int get_index(node *table, char *name){

    int h = hash(name);
    long d = digest(name);

    if(d == table[h].signature) return h;

    for(int i = 1; h < T_SIZE && table[h].line; i++){

        h += i * i;
        if(d == table[h].signature) return h;
    }

    return -1;
}

int get_line(node *table, char *name){

    int index = get_index(table, name);

    return index < 0 ? -1 : table[index].line;
}

int update_line(node *table, char *name, int newln){

    int index = get_index(table, name);

    if(index < 0) return 0;

    table[index].line = newln;

    return 1;
}


int delete_line(node *table, char *name){

    int index = get_index(table, name);

    if(index < 0) return 0;

    table[index].line = -1;

    table[index].signature = -1;

    return 1;
}
