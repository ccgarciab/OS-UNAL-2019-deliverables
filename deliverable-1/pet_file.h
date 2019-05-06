void set_total_lines(int n);

/*Searches the en of the in-disk linked list that starts
    at [firstln] in [db] and appends [pet] to it*/
void add_pet_from_line(FILE *db, dogType *pet, int firstln);

/*Removes the structure in the line# [line] in [db],
    closing the gap in the adequate linked list*/
void del_pet(FILE *db, int line);

/*Appends [pet] to the very end of [db] and returns the
    line# that it now occupies*/
int append_pet(FILE *db, dogType *pet);

/*Returns number of lines in the file*/
int get_total_lines();

/*Fill the information of a pet from the line # [line]
    in [db] into [pet]*/
void read_pet_at_line(FILE *db, dogType pet, int line);
