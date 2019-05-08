#include <stdio.h>
#include <stdlib.h>

#include "pet_globals.h"
#include "error_handle.h"

#define BUF_SIZE 700000

/*Hash function for assignin a table position to a word*/
int hash(char *word) {           //djb2

    unsigned long hash = 5381;
    int c;

    while ((c = *word++)) {

        //same as hash = hash * 33 + c, but faster(?)
        hash = ((hash << 5) + hash) + c;
    }

    return (int) (hash % T_SIZE);
}

/*Hash function for resolving collisions*/
long digest(char *word) {        //sdbm

    unsigned long hash = 0;
    int c;

    //same as hash = hash * 65599 + c, but faster(?)
    while ((c = *word++)) {

        hash = c + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}

/*Reads all the structures in [db] and records the line
    number for a given linked list in the h-table [table].
    Returns the number of structs loaded on success, -1 otherwise.*/
int init_table(node *table, FILE *db) {

    dogType *buffer = malloc(sizeof(dogType) * BUF_SIZE);

    if (!buffer) sys_error("Malloc failure in init_table\n");

    size_t elems_read = fread(buffer, sizeof(dogType), BUF_SIZE, db);
    int line_index = 0;

    while (elems_read) {

        dogType *last = buffer + elems_read;
        for (dogType *curr = buffer; curr < last; curr++, line_index++) {

            int h = hash(curr->name);
            long d = digest(curr->name);

            for (int i = 0; table[h].signature && table[h].signature != d; i++) {

                h += (i * i);
                if (T_SIZE <= h) {

                    free(buffer);
                    sys_error("Accessing out of h-table bounds\n");
                    return -2;
                }
            }

            if (!table[h].line) {

                table[h].line = line_index;
                table[h].signature = d;
            }
        }

        elems_read = fread(buffer, sizeof(dogType), BUF_SIZE, db);
    }

    free(buffer);

    int result = ferror(db);
    clearerr(db);

    return result != 0 ? -1 : line_index;
}

/*Returns the index in the table for some [name].
    If [name] isn't found, returns -1.*/
int get_index(node *table, char *name) {

    int h = hash(name);
    long d = digest(name);

    if (d == table[h].signature) return h;

    for (int i = 1; h < T_SIZE && table[h].line; i++) {

        h += i * i;
        if (d == table[h].signature) return h;
    }

    return -1;
}

/*Inserts the value [line] for the key [name] in [table].
    Returns the index in the table, or -1 if it was duplicated.*/
int insert_new_line(node *table, char *name, int line) {

    int h = hash(name);
    long d = digest(name);

    for (int i = 0; h < T_SIZE && table[h].line; i++) {

        h += i * i;
        if (d == table[h].signature) return -1;
    }

    table[h].line = line;
    table[h].signature = d;

    return h;
}

/*Returns the line value for the key [name] in [table],
    or -1 if [name] wasn't found*/
int get_line(node *table, char *name) {

    int index = get_index(table, name);

    return index < 0 ? -1 : table[index].line;
}

/*Updates the value in [table] for the key [name] with
    the value [newln]. Returns 1 on success, 0 on failure*/
int update_line(node *table, char *name, int newln) {

    int index = get_index(table, name);

    if (index < 0) return 0;

    table[index].line = newln;
    if (newln == -1) table[index].signature = -1;

    return 1;
}

/*Deletes value [name] from [table]. Returns 1 on success,
    0 on failure.*/
int delete_line(node *table, char *name) {

    int index = get_index(table, name);

    if (index < 0) return 0;

    table[index].line = -1;

    table[index].signature = -1;

    return 1;
}
