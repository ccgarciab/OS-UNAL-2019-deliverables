/*Prints all dogType structs in the same linked list at [db],
	starting with the one at [line].*/
void print_list(FILE *db, int line);

/*Prints the fields of the [pet] struct to stdin.*/
void print_pet(dogType *pet);

/*Prints the fields of the [pet] struct to stdin, including
 info related to the implementation of on-disk linked lists.*/
void print_pet_debug(dogType *pet);

/*Opens a medical record placed at [path] in a text editor.*/
void open_medical_record(char *path);

/*Puts the basic info of a [pet] in a medical record [file]*/
void fill_new_mr(FILE *file, dogType *pet);

/*Outputs a log of activity to [file] specifying among others 
    the command executed specified by [opt], the argument [arg] 
    of said command and the ip of the client, specified by [ipstr]*/
void output_log(FILE *file, char *ipstr, char opt, char *arg);
