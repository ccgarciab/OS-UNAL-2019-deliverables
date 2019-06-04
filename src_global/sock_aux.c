#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "pet_globals.h"
#include "error_handle.h"

#include "pet_file.h"

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

/*Sends the structs belonging to the linked list that starts
    in [db]'s [line] line and sends them through [sockfd]. 
    Then sends a last struct with field sex == 'E'.*/
void send_pet_list(FILE *db, int sockfd, int line){

    dogType pet;
    
    read_pet_at_line(db, &pet, line);
    send_full(sockfd, &pet, sizeof(dogType));
    
    while (pet.next != -1) {

        read_pet_at_line(db, &pet, pet.next);
        send_full(sockfd, &pet, sizeof(dogType));
    }
    
    pet.sex = 'E';
    send_full(sockfd, &pet, sizeof(dogType));
}
