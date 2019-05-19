/*Sends thru the socket identified with [sockfd] [size] bytes 
    pointed by [buff], making sure to send all*/
void send_full(int sockfd, void *buff, int size);

/*Receives thru the socket identified with [sockfd] exactly 
    [size] bytes pointed by [buff], making sure to get all*/
void recv_full(int sockfd, void *buff, int size);
