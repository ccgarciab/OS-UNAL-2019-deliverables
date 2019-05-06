#include "stdio.h"
#include "unistd.h"

#include "pet_globals.h"
#include "error_handle.h"

int line_counter = 0;

/*Returns number of lines in the file*/
int get_total_lines(){

    return line_counter;
}

void set_total_lines(int n){

    line_counter = n;
}

/*Fill the information of a pet from the line # [line]
    in [db] into [pet]*/
void read_pet_at_line(FILE *db, dogType *pet, int line){

    if(fseek(db, line * sizeof(dogType), SEEK_SET) < 0)
        sys_error("fseek error\n");

    if(!fread(pet, sizeof(dogType), 1, db))
        sys_error("fread error\n");
}

/*Overwrites the structure immediately before the file
    cursor in [db] with [new_pet]*/
void overwrite_last_pet(FILE *db, dogType *new_pet){

    if(fseek(db, -1 * sizeof(dogType), SEEK_CUR) < 0)
        sys_error("fseek error\n");

    if(!fwrite(new_pet, sizeof(dogType), 1, db))
        sys_error("fwrite error\n");
}


/*Searches the en of the in-disk linked list that starts
    at [firstln] in [db] and appends [pet] to it*/
void add_pet_from_line(FILE *db, dogType *pet, int firstln){

    if(fseek(db, firstln * sizeof(dogType), SEEK_SET) < 0)
        sys_error("fseek error at add_pet\n");

    dogType temp;

    if(!fread(&temp, sizeof(dogType), 1, db))
        sys_error("fread error at add_pet\n");

    int currln = firstln;

    for(int nextln = temp.next; 0 <= nextln; nextln = temp.next){

        read_pet_at_line(db, &temp, nextln);
        currln = nextln;
    }

    temp.next = ++line_counter;

    overwrite_last_pet(db, &temp);

    pet->prev = currln;
    pet->next = -1;

    if(fseek(db, 0, SEEK_END) < 0)
        sys_error("fseek error at add_pet (appending)\n");

    if(!fwrite(pet, sizeof(dogType), 1, db))
        sys_error("fwrite error at add_pet (appending)\n");
}

/*Removes the structure in the line# [line] in [db],
    closing the gap in the adequate linked list*/
void del_pet(FILE *db, int line){

    dogType temp;
    read_pet_at_line(db, &temp, line);

    int prev_line = temp.prev;
    int next_line = temp.next;

    if(0 <= prev_line){

        read_pet_at_line(db, &temp, prev_line);
        temp.next = next_line;
        overwrite_last_pet(db, &temp);
    }

    if(0 <= next_line){

        read_pet_at_line(db, &temp, next_line);
        temp.prev = prev_line;
        overwrite_last_pet(db, &temp);
    }

    if(fseek(db, -1 * sizeof(dogType), SEEK_END) < 0)
        sys_error("fseek error\n");

    dogType repl;

    if(!fread(&repl, sizeof(dogType), 1, db))
        sys_error("fread error\n");

    prev_line = repl.prev;

    if(0 <= prev_line){

        read_pet_at_line(db, &temp, prev_line);
        temp.next = line;
        overwrite_last_pet(db, &temp);
    }

    if(fseek(db, (line + 1) * sizeof(dogType), SEEK_SET) < 0)
        sys_error("fseek error\n");

    overwrite_last_pet(db, &repl);

    line_counter--;

    if(truncate(PATH, line_counter * sizeof(dogType)) < 0)
        sys_error("truncate error\n");
}

/*Appends [pet] to the very end of [db] and returns the
    line# that it now occupies*/
int append_pet(FILE *db, dogType *pet){

    if(fseek(db, 0, SEEK_END) < 0)
        sys_error("fseek error at append_pet\n");

    if(!fwrite(pet, sizeof(dogType), 1, db))
        sys_error("fread error\n");

    return ++line_counter;
}
