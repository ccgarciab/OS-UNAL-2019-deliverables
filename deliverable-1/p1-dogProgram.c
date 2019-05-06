#include <stdio.h>
#include <stdlib.h>

#include "pet_globals.h"

#include "pet_file.h"
#include "pet_hashtable.h"
#include "pet_input.h"
#include "pet_output.h"

#include "error_handle.h"

node table[T_SIZE];
int curr_hist = 0;

int main(int argc, char* argv[]){

    FILE *db = fopen(PATH, "rb+");

    if(!db) sys_error("can't open file\n");

    int num_lines = init_table(table, db);
    set_total_lines(num_lines);

    char opt[2];
    opt[0] = 'A';
    dogType pet;

    do{
//        system("clear");
        printf("Menu:\n");
        printf("1. Insert a Register\n");
        printf("2. Visualize a Register\n");
        printf("3. Erase a Register\n");
        printf("4. Search a Register\n");
        printf("5. Quit\n");

        if(!get_bounded_str(opt, 1)) continue;

        switch(opt[0]){

            case '1':

                fill_pet_info(&pet);
                int line = get_line(table, pet.name);
                pet.doc_id = curr_hist++;
                pet.next = -1;

                if(line < 0){

                    pet.prev = -1;
                    line = append_pet(db, &pet);
                    insert_new_line(table, pet.name, line);
                }
                else{

                    add_pet_from_line(db, &pet, line);
                }
                break;

	   case '2':
           fseek(db, 0, SEEK_END);

           int total_lines = get_total_lines();

           if(total_lines==0){
                printf("\nThe Data Base is empty, first insert a register\n\n");
                break;
           }else{
               printf("Numbers of Pets inside dataDogs.dat: %d\n", total_lines);
               int numPet;

               while(1) {
                   if((numPet = get_int("Please digit the number of register for data visualization: "))){
                       if (numPet>total_lines || numPet== 0 ){
                           printf("\n");
                           printf("Please insert a number inside the register number of the Data base\n");
                           printf("\n");
                       }else{
                           dogType *datos = malloc(sizeof(dogType));

                           fseek(db, (numPet - 1) * sizeof(dogType), SEEK_SET);
                           int readVeredict = fread(datos, sizeof(dogType), 1, db);

                           if (readVeredict == 0 ){
                               perror("Read Error\n");
                               exit(-1);
                           }

                           printf("-------------------------------------------------\n");
                           printf("Name: %s\n", datos->name);
                           printf("Type:   %s\n", datos->type);
                           printf("Age:   %i\n", datos->age);
                           printf("Breed:   %s\n", datos->breed);
                           printf("Size: %i\n", datos->size);
                           printf("Weight:   %f\n", datos->weight);
                           printf("Sex: %c\n", datos->sex);
                           printf("Next: %d\n", datos->next);
                           printf("-------------------------------------------------\n");

                           free(datos);
                           break;
                       }

                   }else{
                       perror("Error in Scanf");
                       exit(-1);
                       break;
                   }
               }
           }

	       break;

            case '3':

                ;
                int n = get_total_lines();
                printf("Number of pets in dataDogs.dat: %d\n", n);
                int input = get_int("number of register to delete: ");
                if(input <= 0 || n < input) break;
                input--;
                dogType d;
                read_pet_at_line(db, &d, input);
                print_pet(&d);
                char yn[2];
                while(1){
                    printf("\nDo you want to delete this pet?(y/n)\n");
                    if(!get_bounded_str(yn, 1)) continue;
                    if(yn[0] != 'y' && yn[0] != 'n' && yn[0] != 'Y' && yn[0] != 'N')
                        continue;
                }
                if(yn[0] == 'Y' || yn[0] == 'y'){

                    delResult dr;
                    del_pet(db, input, &dr);
                    if(dr.update_del){

                        update_line(table, d.name, dr.newln_del);
                    }
                    if(dr.update_repl){

                        update_line(table, dr.word_repl, dr.newln_repl);
                    }
                }
                break;

            case '4':

                ;
                char name[33];
                do{

                    printf("enter a pet name to look up: ");

                }while(!get_bounded_str(name, 32));
                int result = get_line(table, name);
                if(result != -1){

                    print_list(db, result);
                }
                else{

                    printf("pet name not found\n");
                }
                break;

            default:
                break;
        }

    }while(opt[0] != '5');

    int writeVeredict = fclose(db);

    if(writeVeredict == EOF){
        perror("fclose error");
        exit(-1);
    }
    printf("\nGood Bye, Dude!\n\n");
}
