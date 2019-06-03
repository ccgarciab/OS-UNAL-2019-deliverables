#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "pet_globals.h"

#include "pet_file.h"
#include "pet_hashtable.h"
#include "pet_input.h"
#include "pet_output.h"

#include "error_handle.h"
#include "sock_aux.h"

#define PORT 3535
#define BACKLOG 32

struct sockaddr_in server;
struct sockaddr_in clients[BACKLOG];
size_t size;
socklen_t sizeClient;
int fd_clients[BACKLOG];
pthread_t threads[BACKLOG];
int err;

node table[T_SIZE];
int curr_hist = 0;

int main () {

    int fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd == -1) sys_error("Socket error");

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;
    bzero(server.sin_zero, 8);
    size =  sizeof(struct sockaddr_in);

    err = bind(fd, (struct sockaddr*)&server, size);
    if (err == -1) sys_error("bind error");

    err = listen(fd, BACKLOG);
    if (err == -1) sys_error("listen error");

    sizeClient = 0;
    
    //Openning dataDogs.dat
    FILE *db = fopen(PATH, "rb+");
    if (!db) sys_error("can't open file\n");
    
    //hash table init
    int num_lines = init_table(table, db);
    set_total_lines(num_lines);

    for(int i = 0; i < BACKLOG; i++){
    
        fd_clients[i] = accept(fd, (struct sockaddr*)(clients + i), &sizeClient);
        
    }

    char got;
    char name[33];
    int ans;
    dogType pet;

    //Updating IDs sequence number by structs number inside dataDogs.dat
    curr_hist = ftell(db) / sizeof(dogType);

    do{
    
        do{
        
            err = recv(fd_clients[0], &got, 1, 0);
            if(err == -1) sys_error("receive error");

        }while(err == 0);
        
        printf("%c\n", got);

        switch(got){
        
            case '1':
                
                recv_full(fd_clients[0], &pet, sizeof(dogType));
                
                strcpy(name, pet.name);
                word_to_upper(name);
                pet.next = -1;
                
                int line = get_line(table, name);
                
                if (line < 0) {

                    pet.prev = -1;
                    line = append_pet(db, &pet);
                    insert_new_line(table, name, line);
                } else {

                    add_pet_from_line(db, &pet, line);
                }
                
                break;
                
            case '2':
                
                //TODO NEXT
                
                //enviar # structs
                //recibir # struct
                // enviar # struct
                
                break;
                
            case '3':
            
                // enviar # structs
                // recv # a operar
                // enviar struct
                // recv confirmacion
                // si confirma borra
            
                break;
                
            case '4':
            
                // recv nombre (32)
                // evniar structs
                
                break;
                
            default:
            
                break;
        }

    } while(got != '5');

    shutdown(fd_clients[0], SHUT_RDWR);
    shutdown(fd, SHUT_RDWR);
    close(fd_clients[0]);
    close(fd);

	return 0;
}
