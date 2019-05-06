#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "pet_globals.h"
#include "pet_file.h"

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


void replace_pet_at_line(FILE *db, dogType *new_pet, int line){

    if(fseek(db, line * sizeof(dogType), SEEK_SET) < 0)
        sys_error("fseek_error\n");

    if(!fwrite(new_pet, sizeof(dogType), 1, db))
        sys_error("fwrite error\n");
}

/*Searches the end of the in-disk linked list that starts
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

void bridge_over(FILE *db, dogType *pet){

    dogType temp;

    if(pet->prev != -1){

        read_pet_at_line(db, &temp, pet->prev);
        temp.next = pet->next;
        overwrite_last_pet(db, &temp);
    }

    if(pet->next != -1){

        read_pet_at_line(db, &temp, pet->next);
        temp.prev = pet->prev;
        overwrite_last_pet(db, &temp);
    }
}

/*Removes the structure in the line# [line] in [db],
    and reports the modifications to do in the h-table.*/
void del_pet(FILE *db, int line, delResult *res){

    dogType to_del, temp, temp_2;
    read_pet_at_line(db, &to_del, line);

    if(line == line_counter){

        if(to_del.prev == -1){

            res->update_del = 1;
            res->newln_del = to_del.next;
        }

        bridge_over(db, &to_del);
    }
    else if(to_del.next == line_counter){

        if(to_del.prev != -1){

            read_pet_at_line(db, &temp, to_del.prev);
            temp.next = line;
            overwrite_last_pet(db, &temp);
        }

        read_pet_at_line(db, &temp, to_del.next);
        temp.prev = to_del.prev;
        replace_pet_at_line(db, &temp, line);
        if(temp.next != -1){

            read_pet_at_line(db, &temp, temp.next);
            temp.prev = line;
            overwrite_last_pet(db, &temp);
        }
    }
    else if(to_del.prev == line_counter){

        read_pet_at_line(db, &temp, to_del.prev);
        if(temp.prev != -1){

            read_pet_at_line(db, &temp_2, temp.prev);
            temp_2.next = line;
            overwrite_last_pet(db, &temp_2);
        }
        else{

            res->update_del = 1;
            res->newln_del = line;
        }

        if(to_del.next != -1){

            read_pet_at_line(db, &temp_2, to_del.next);
            temp_2.prev = line;
            overwrite_last_pet(db, &temp_2);
        }

        temp.next = to_del.next;
        replace_pet_at_line(db, &temp, line);
    }
    else{

        bridge_over(db, &to_del);
        read_pet_at_line(db, &temp, line_counter);

        if(temp.next != -1){

            read_pet_at_line(db, &temp_2, temp.next);
            temp_2.prev = line;
            overwrite_last_pet(db, &temp_2);
        }

        if(temp.prev != -1){

            read_pet_at_line(db, &temp_2, temp.prev);
            temp_2.next = line;
            overwrite_last_pet(db, &temp_2);
        }
        else{

            res->update_repl = 1;
            res->newln_repl = line;
            strcpy(res->word_repl, temp.name);
        }

        if(to_del.prev == -1){

            res->update_del = 1;
            res->newln_del = to_del.next;
        }

        replace_pet_at_line(db, &temp, line);
    }

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
