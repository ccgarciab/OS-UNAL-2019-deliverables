/*Transforms the alphabetic chars in a string to uppercase,
    in place.*/
void word_to_upper(char *word);

/*Uses stdin to get all info for a pet record.
    Handles invalid input by 'asking' again*/
void fill_pet_info(dogType *pet);

/*Read a line up to [bound] chars.
    If the line is too long, truncates it and returns 0*/
int get_bounded_str(char *buffer, unsigned int bound);

/*Get a non-negative int from stdin*/
int get_int(char *msg);

/*Easily ask for a yes/no question in stdin*/
int confirmation(char *msg);
