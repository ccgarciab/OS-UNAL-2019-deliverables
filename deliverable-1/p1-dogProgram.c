#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pet_globals.h"

#include "pet_file.h"
#include "pet_hashtable.h"
#include "pet_input.h"
#include "pet_output.h"

#include "error_handle.h"

node table[T_SIZE];
int curr_hist = 0; // IDs Sequence

int main(int argc, char *argv[]) {


    //Openning dataDogs.dat
    FILE *db = fopen(PATH, "rb+");
    if (!db) sys_error("can't open file\n");

    //hash table init
    int num_lines = init_table(table, db);
    set_total_lines(num_lines);

    char opt[2];
    char name[33];
    opt[0] = 'A';
    dogType pet;

    //Updating IDs sequence number by structs number inside dataDogs.dat
    curr_hist = ftell(db) / sizeof(dogType);


    do {

        printf("\nMenu:\n");
        printf("1. Insert a Register\n");
        printf("2. Visualize a Register\n");
        printf("3. Erase a Register\n");
        printf("4. Search a Register\n");
        printf("5. Quit\n");

        if (!get_bounded_str(opt, 1)) continue;

        switch (opt[0]) {

            case '1':

                pet.doc_id = ++curr_hist;

                fill_pet_info(&pet);
                strcpy(name, pet.name);
                word_to_upper(name);
                int line = get_line(table, name);

                pet.next = -1;

                if (line < 0) {

                    pet.prev = -1;
                    line = append_pet(db, &pet);
                    insert_new_line(table, name, line);
                } else {

                    add_pet_from_line(db, &pet, line);
                }
                break;

            case '2':;

                fseek(db, 0, SEEK_END);

                int total_lines = get_total_lines();

                if (total_lines == 0) {
                    printf("\nThe Data Base is empty, first insert a register\n\n");
                    break;
                } else {
                    printf("Numbers of Pets inside dataDogs.dat: %d\n", total_lines);
                    int numPet;

                    while (1) {
                        if ((numPet = get_int("Please digit the number of register for data visualization: "))) {

                            if (numPet > total_lines || numPet == 0) {

                                printf("\nPlease insert a number inside the register number of the Data base\n\n");
                            } else {

                                dogType *datos = malloc(sizeof(dogType));
                                if(!datos) sys_error("malloc error\n");

                                fseek(db, (numPet - 1) * sizeof(dogType), SEEK_SET);
                                int readVeredict = fread(datos, sizeof(dogType), 1, db);

                                if (readVeredict == 0) {
                                    sys_error("Read Error\n");
                                    exit(-1);
                                }

                                print_pet(datos);

                                char path[33], command[38] = "gedit ";

                                sprintf(path, "%i.txt", datos->doc_id);
                                FILE *file = fopen(path, "r");

                                if (file == NULL) {
                                    printf("The Clinical History is not created, do you want create it? y/n\n");
                                    char q1[1];
                                    get_bounded_str(q1, 1);
                                    if (q1[0] == 'y') {
                                        file = fopen(path, "w+");
                                        if (file == NULL) {
                                            sys_error("Writting Error");
                                            exit(-1);
                                        }

                                        print_pet(datos);

                                        free(datos);
                                        fclose(file);
                                        strcat(command, path);
                                        system(command);
                                    } else {
                                        break;
                                    }
                                } else {
                                    printf("There is a Clinical History existent, do you want see it? y/n\n");
                                    char q1[1];
                                    get_bounded_str(q1, 1);
                                    if (q1[0] == 'y') {
                                        strcat(command, path);
                                        system(command);
                                    }
                                }

                                free(datos);
                                break;
                            }

                        } else {
                            sys_error("Error in Scanf");
                            break;
                        }
                    }
                }

                break;

            case '3':;

                int n = get_total_lines();
                printf("Number of pets in dataDogs.dat: %d\n", n);
                int input = get_int("number of register to delete: ");

                if (input <= 0 || n < input){

                    printf("\nInvalid Register Number\n\n");
                    break;
                }

                input--;
                dogType d;
                read_pet_at_line(db, &d, input);
                print_pet(&d);
                char yn[2];
                while (1) {
                    printf("\nDo you want to delete this pet?(y/n)\n");
                    if (!get_bounded_str(yn, 1)) continue;
                    if (yn[0] != 'y' && yn[0] != 'n' && yn[0] != 'Y' && yn[0] != 'N')
                        continue;
                    if (yn[0] == 'Y' || yn[0] == 'y' || yn[0] == 'N' || yn[0] == 'n') break;

                }
                if (yn[0] == 'Y' || yn[0] == 'y') {

                    delResult dr;
                    dr.update_del = 0;
                    dr.update_repl = 0;
                    del_pet(db, input, &dr);
                    if (dr.update_del) {

                        strcpy(name, d.name);
                        word_to_upper(name);
                        update_line(table, name, dr.newln_del);
                    }
                    if (dr.update_repl) {

                        strcpy(name, dr.word_repl);
                        word_to_upper(name);
                        update_line(table, dr.word_repl, dr.newln_repl);
                    }
                }
                break;

            case '4':;

                do {

                    printf("enter a pet name to look up: ");

                } while (!get_bounded_str(name, 32));
                word_to_upper(name);
                int result = get_line(table, name);
                if (result != -1) {

                    print_list(db, result);
                } else {

                    printf("pet name not found\n");
                }
                break;

            default:
                break;
        }

    } while (opt[0] != '5');

    int writeVeredict = fclose(db);

    if (writeVeredict == EOF) {
        sys_error("fclose error");
    }

    printf("\nGood Bye, Dude!\n\n");
}
