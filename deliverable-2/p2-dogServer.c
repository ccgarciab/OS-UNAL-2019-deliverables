#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include "pet_globals.h"

#include "pet_file.h"
#include "pet_hashtable.h"
#include "pet_input.h"
#include "pet_output.h"

#include "error_handle.h"

#define PORT 3535
#define BACKLOG 32

struct sockaddr_in server, client_I;
size_t size;
socklen_t sizeClient;
int err, fd_I;

node table[T_SIZE];
int curr_hist = 0;

int main () {

    int fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd == -1){
        perror("error in socket");	
        exit(-1);	
    }

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

    fd_I = accept(fd, (struct sockaddr*)&client_I, &sizeClient);

    //Openning dataDogs.dat
    FILE *db = fopen(PATH, "rb+");
    if (!db) sys_error("can't open file\n");

    //hash table init
    int num_lines = init_table(table, db);
    set_total_lines(num_lines);

    char got;
    char name[33];
    int ans;
    dogType pet;

    //Updating IDs sequence number by structs number inside dataDogs.dat
    curr_hist = ftell(db) / sizeof(dogType);

    do{
    
        do{
        
            err = recv(fd_I, &got, 1, 0);
            if(err == -1) sys_error("receive error");

        }while(err == 0);
        
        printf("%c\n", got);

        switch(got){
        
            case '1':
                
                for(int received = 0; received < sizeof(dogType);){
                
                    int res = recv(fd_I, (void *)(&pet) + received, sizeof(dogType) - received, 0);
                    if(res == -1) sys_error("receive error, got 1");
                    received += res;
                }
                
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
                
                break;
                
            default:
            
                break;
        }

    } while(got != '5');

    shutdown(fd_I, SHUT_RDWR);
    shutdown(fd, SHUT_RDWR);
    close(fd_I);
    close(fd);

	return 0;
}
