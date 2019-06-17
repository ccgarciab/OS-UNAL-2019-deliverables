#include <arpa/inet.h>
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
#define LOGPATH "serverDogs.log"

/*Structure to pass as argument to threads*/
typedef struct threadarg{

    struct in_addr* ip;
    int fd;

} threadarg;

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
FILE *logfile;
int num_lines;


void *client_function(void *argp){


    threadarg *arg = (threadarg *)argp;
    char got;
    char name[33];
    char logarg[33];
    int ans;
    int fd_client = arg->fd;
    dogType pet;
    
    char str_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, arg->ip, str_ip, INET_ADDRSTRLEN);
    
    int line;

    do{
    
        do{
        
            err = recv(fd_client, &got, 1, 0);
            if(err == -1) sys_error("receive error");

        }while(err == 0);

        switch(got){
        
            case '1':
                
                recv_full(fd_client, &pet, sizeof(dogType));
                
                pet.doc_id = ++curr_hist;
                
                strcpy(name, pet.name);
                strcpy(logarg, name);
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
                send_full(fd_client, &line, sizeof(int));
                recv_full(fd_client, &line, sizeof(int));
                if(line == -1) break;
                read_pet_at_line(db, &pet, line - 1);
                send_full(fd_client, &pet, sizeof(dogType));
                
                sprintf(logarg, "%d", line);
                
                char path[33];
                sprintf(path, "server/%i.txt", pet.doc_id);
                FILE *file = fopen(path, "r");
                int fexists = file != NULL;
                send_full(fd_client, &fexists, sizeof(int));
                recv_full(fd_client, &ans, sizeof(int));
                
                if(!ans) break;
                
                if(!fexists){
                
                    file = fopen(path, "w+");
                    if (file == NULL) sys_error("fopen error server 2");
                    
                    fill_new_mr(file, &pet);
                    fclose(file);
                    file = fopen(path, "r");
                    if (file == NULL) sys_error("fopen error server 2");
                }
                
                send_file(file, fd_client);
                fclose(file);
                file = fopen(path, "w");
                if (file == NULL) sys_error("fopen error server 2");
                recv_write_file(file, fd_client);
                fclose(file);
                
                break;
                
            case '3':
            
                line = get_total_lines();
                send_full(fd_client, &line, sizeof(int));
                recv_full(fd_client, &line, sizeof(int));
                
                sprintf(logarg, "%d", line);
                
                line--;
                
                read_pet_at_line(db, &pet, line);
                send_full(fd_client, &pet, sizeof(dogType));
                recv_full(fd_client, &ans, sizeof(int));
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
                recv_full(fd_client, name, 33);
                
                strcpy(logarg, name);
                
                line = get_line(table, name);
                if(line != -1){

                    send_pet_list(db, fd_client, line);
                }
                else{

                    pet.sex = 'E';
                    send_full(fd_client, &pet, sizeof(dogType));
                }

                break;

            default:

                break;
        }
        
        output_log(logfile, str_ip, got, logarg);
        fclose(logfile);
        logfile = fopen(LOGPATH, "a+");
        if(!logfile) sys_error("cant open logfile again");

    } while(got != '5');

    pthread_exit(NULL);
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
    
    //Updating IDs sequence number by structs number inside dataDogs.dat
    curr_hist = num_lines;
    
    //Opening log file
    logfile = fopen(LOGPATH, "a+");
    if (!logfile) sys_error("can't open logfile");

    for (int i = 0; i < BACKLOG; i++) fd_clients[i] = -1;
    sizeClient = 0;

    pthread_attr_t thread_params;

    err = pthread_attr_init(&thread_params);
    if(err) sys_error("Threads attribute init");

    err = pthread_attr_setstacksize(&thread_params, 1024 * 16);
    if(err) sys_error("Setting pthread stack size parameter");

    err = pthread_attr_setdetachstate(&thread_params, PTHREAD_CREATE_DETACHED);
    if(err) sys_error("Making threads detached");

    for(int i = 0; i < BACKLOG; i++){
    
        fd_clients[i] = accept(fd, (struct sockaddr*)(clients + i), &sizeClient);
        if(fd_clients[i] == -1) sys_error("Connection Error");
        
        struct in_addr ip = clients[i].sin_addr;
        threadarg arg;
        arg.ip = &ip;
        arg.fd = fd_clients[i];
        pthread_create(&threads[i], &thread_params, (void *)client_function, (void *)&arg);
    }

    //shutdown(fd_clients[0], SHUT_RDWR);
    shutdown(fd, SHUT_RDWR);
    //close(fd_clients[0]);
    close(fd);

	return 0;
}
