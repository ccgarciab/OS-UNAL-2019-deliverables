#include <sys/types.h>
#include <sys/socket.h>

#include "error_handle.h"

/*Sends thru the socket identified with [sockfd] [size] bytes 
    pointed by [buff], making sure to send all*/
void send_full(int sockfd, void *buff, int size){

    int sent = 0;
    
    while(sent < size){
    
        int res = send(sockfd, buff + sent, size - sent, 0);
        if(res == -1) sys_error("send error");
        sent += res;
    }
}

/*Receives thru the socket identified with [sockfd] exactly 
    [size] bytes pointed by [buff], making sure to get all*/
void recv_full(int sockfd, void *buff, int size) {

    int received = 0;

    while(received < size) {
    
        int res = recv(sockfd, buff + received, size - received, 0);
        if(res == -1) sys_error("receive error");
        received += res;
    }
}
