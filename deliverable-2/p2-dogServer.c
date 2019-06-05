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
FILE *db;
int num_lines;


void *client_function(int *fd_client){

    char got;
    char name[33];
    int ans;
    dogType pet;

    //Updating IDs sequence number by structs number inside dataDogs.dat
    curr_hist = ftell(db) / sizeof(dogType);
    
    int line;

    do{
    
        do{
        
            err = recv(*fd_client, &got, 1, 0);
            if(err == -1) sys_error("receive error");

        }while(err == 0);
        
        printf("%c\n", got);

        switch(got){
        
            case '1':
                
                recv_full(*fd_client, &pet, sizeof(dogType));
                
                pet.doc_id = ++curr_hist;
                
                strcpy(name, pet.name);
                word_to_upper(name);
                pet.next = -1;
                
                line = get_line(table, name);
                
                if (line < 0) {

                    pet.prev = -1;
                    line = append_pet(db, &pet);
                    insert_new_line(table, name, line);
                } else {

                    add_pet_from_line(db, &pet, line);
                }
                
                break;
                
            case '2':
                
                line = get_total_lines();
                send_full(*fd_client, &line, sizeof(int));
                recv_full(*fd_client, &line, sizeof(int));
                if(line == -1) break;
                read_pet_at_line(db, &pet, line - 1);
                send_full(*fd_client, &pet, sizeof(dogType));
                
                char path[33];
                sprintf(path, "server/%i.txt", pet.doc_id);
                FILE *file = fopen(path, "r");
                int fexists = file != NULL;
                send_full(*fd_client, &fexists, sizeof(int));
                recv_full(*fd_client, &ans, sizeof(int));
                
                if(!ans) break;
                
                if(!fexists){
                
                    file = fopen(path, "w+");
                    if (file == NULL) sys_error("fopen error server 2");
                    
                    fill_new_mr(file, &pet);
                    fclose(file);
                    file = fopen(path, "r");
                    if (file == NULL) sys_error("fopen error server 2");
                }
                
                send_file(file, *fd_client);
                fclose(file);
                file = fopen(path, "w");
                if (file == NULL) sys_error("fopen error server 2");
                recv_write_file(file, *fd_client);
                fclose(file);
                
                break;
                
            case '3':
            
                line = get_total_lines();
                send_full(*fd_client, &line, sizeof(int));
                recv_full(*fd_client, &line, sizeof(int));
                
                line--;
                
                read_pet_at_line(db, &pet, line);
                send_full(*fd_client, &pet, sizeof(dogType));
                recv_full(*fd_client, &ans, sizeof(int));
                if (ans) {

                    delResult dr;
                    dr.update_del = 0;
                    dr.update_repl = 0;
                    del_pet(db, line, &dr);
                    if (dr.update_del) {

                        strcpy(name, pet.name);
                        word_to_upper(name);
                        update_line(table, name, dr.newln_del);
                    }
                    if (dr.update_repl) {

                        strcpy(name, dr.word_repl);
                        word_to_upper(name);
                        update_line(table, dr.word_repl, dr.newln_repl);
                    }
                    char fileName[33];
                    sprintf(fileName, "server/%d.txt", pet.doc_id);
                    int exist = cfileexists(fileName);
                    if (exist) {
                        if (remove(fileName) != 0) sys_error("Unable to delete the clinical history\n\n");
                    }
                }

                break;

            case '4':;

                char name[33];
                recv_full(*fd_client, name, 33);
                line = get_line(table, name);
                if(line != -1){

                    send_pet_list(db, *fd_client, line);
                }
                else{

                    pet.sex = 'E';
                    send_full(*fd_client, &pet, sizeof(dogType));
                }

                break;

            default:

                break;
        }

    } while(got != '5');

}

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


    //Openning dataDogs.dat
    db = fopen(PATH, "rb+");
    if (!db) sys_error("can't open file\n");

    //hash table init
    num_lines = init_table(table, db);
    set_total_lines(num_lines);

    for (int i = 0; i < BACKLOG; i++) fd_clients[i] = -1;
    sizeClient = 0;
   

    for(int i = 0; i < BACKLOG; i++){
    
        fd_clients[i] = accept(fd, (struct sockaddr*)(clients + i), &sizeClient);

        if(fd_clients[i] == -1) sys_error("Connection Error");

	pthread_create(&threads[i], NULL, (void *)client_function, (void *)&fd_clients[i]);
 
        
        
    }
    
    //fd_clients[0] = accept(fd, (struct sockaddr*)(clients), &sizeClient);
    

	for (int i = 0; i < BACKLOG; i++) {
	    int ok = pthread_join(threads[i], NULL);
	    if (ok != 0) sys_error("Error al hacer join al hilo\n");
	}

    //shutdown(fd_clients[0], SHUT_RDWR);
    shutdown(fd, SHUT_RDWR);
    //close(fd_clients[0]);
    close(fd);

	return 0;
}
