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

    FILE *db = fopen(PATH, "wr+");

    if(!db) sys_error("can't open file\n");

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
           printf("Numbers of Pets inside dataDogs.dat: %d\n", total_lines);
           printf("Please digit the number of register for data visualization: ");
           int numPet;

           if (scanf("%d", &numPet)== EOF) {
               perror("Error in Scanf");
               exit(-1);
           }

           if (numPet>total_lines){
               printf("Please insert a number inside the register number inside of the Data base\n");
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
           }
	       break;


            default:
                break;
        }

    }while(opt[0] != '5');

    fclose(db);
}
