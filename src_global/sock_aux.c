#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "pet_globals.h"
#include "error_handle.h"

#include "pet_file.h"

#define FBUFF 1024

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

/*Sends the contents of [file] thru [sockfd] socket*/
void send_file(FILE *file, int sockfd){

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    if(ftell < 0) sys_error("ftell in send_file");
    rewind(file);
    
    send_full(sockfd, &size, sizeof(long));
    
    if(size == 0) return;
    
    long blocks = size / FBUFF;
    
    char bindata[FBUFF];
    int rd = 0;
    
    for(int i = 0; i < blocks; i++){
    
        rd = fread(bindata, 1, FBUFF, file);
        if(rd < 0) sys_error("read error sending file");
        send_full(sockfd, &bindata, FBUFF);
    }
    
    int rest = (int)(size - (blocks * FBUFF));
    
    rd = fread(bindata, 1, rest, file);
    if(rd < 0) sys_error("read error sending file.");
    send_full(sockfd, &bindata, rest);
}

/*Gets bytes thru [sockfd] socket and writes them into [file]*/
void recv_write_file(FILE *file, int sockfd){

    long size;
    recv_full(sockfd, &size, sizeof(long));
    
    if(size == 0) return;
    
    long blocks = size / FBUFF;
    
    char bindata[FBUFF];
    int wr = 0;
    
    for(int i = 0; i < blocks; i++){
    
        wr = fwrite(bindata, 1, FBUFF, file);
        if(wr < 0) sys_error("write error receiving file");
        recv_full(sockfd, &bindata, FBUFF);
    }
    
    int rest = (int)(size - (blocks * FBUFF));
    
    recv_full(sockfd, &bindata, rest);
    wr = fwrite(bindata, 1, rest, file);
    if(wr < 0) sys_error("write error receiving file.");
}
