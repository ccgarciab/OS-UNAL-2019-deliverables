/*Reads all the structures in [db] and records the line
    number for a given linked list in the h-table [table].
    Returns the number of structs loaded on success, -1 otherwise.*/
int init_table(node *table, FILE *db);

/*Returns the line value for the key [name] in [table],
    or -1 if [name] wasn't found*/
int get_line(node *table, char *name);

/*Updates the value in [table] for the key [name] with
    the value [newln]. Returns 1 on success, 0 on failure*/
int update_line(node *table, char *name, int newln);

/*Deletes value [name] from [table]. Returns 1 on success,
    0 on failure.*/
int delete_line(node *table, char *name);

/*Inserts the value [line] for the key [name] in [table].
    Returns the index in the table, or -1 if it was duplicated.*/
int insert_new_line(node *table, char *name, int line);
