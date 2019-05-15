/*Prints all dogType structs in the same linked list at [db],
	starting with the one at [line].*/
void print_list(FILE *db, int line);

/*Prints the fields of the [pet] struct to stdin.*/
void print_pet(dogType *pet);

/*Prints the fields of the [pet] struct to stdin, including
 info related to the implementation of on-disk linked lists.*/
void print_pet_debug(dogType *pet);

/*Opens a medical record identified with [doc_id]
    in a text editor.*/
void open_medical_record(int doc_id);