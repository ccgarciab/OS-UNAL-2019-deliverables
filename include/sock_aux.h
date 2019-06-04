/*Sends thru the socket identified with [sockfd] [size] bytes 
    pointed by [buff], making sure to send all*/
void send_full(int sockfd, void *buff, int size);

/*Receives thru the socket identified with [sockfd] exactly 
    [size] bytes pointed by [buff], making sure to get all*/
void recv_full(int sockfd, void *buff, int size);

/*Sends the structs belonging to the linked list that starts
    in [db]'s [line] line and sends them through [sockfd]. 
    Then sends a last struct with field sex == 'E'.*/
void send_pet_list(FILE *db, int sockfd, int line);

/*Sends the contents of [file] thru [sockfd] socket*/
void send_file(FILE *file, int sockfd);

/*Gets bytes thru [sockfd] socket and writes them into [file]*/
void recv_write_file(FILE *file, int sockfd);
