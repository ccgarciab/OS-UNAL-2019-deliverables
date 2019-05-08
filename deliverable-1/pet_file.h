/*Struct for collecting all the information to update
    the h-table after a deletion in the file.*/
typedef struct delResult{

    int update_del;
    int newln_del;
    int update_repl;
    int newln_repl;
    char word_repl[33];
} delResult;

/*sets the database-global line counter to [n]*/
void set_total_lines(int n);

/*Searches the en of the in-disk linked list that starts
    at [firstln] in [db] and appends [pet] to it*/
void add_pet_from_line(FILE *db, dogType *pet, int firstln);

/*Removes the structure in the line# [line] in [db],
    and reports the modifications to do in the h-table.*/
void del_pet(FILE *db, int line, delResult *res);

/*Appends [pet] to the very end of [db] and returns the
    line# that it now occupies*/
int append_pet(FILE *db, dogType *pet);

/*Returns number of lines in the file*/
int get_total_lines();

/*Fill the information of a pet from the line # [line]
    in [db] into [pet]*/
void read_pet_at_line(FILE *db, dogType *pet, int line);
