#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "pet_globals.h"

#include "pet_input.h"
#include "pet_output.h"

#include "error_handle.h"
#include "sock_aux.h"

#define PORT 3535

struct sockaddr_in server;
size_t size;
int err;
char buffer[32];

int main() {

    int fd = socket(AF_INET, SOCK_STREAM,0);

    if (fd == -1) sys_error("socket error");

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    bzero(server.sin_zero, 8);

    size = sizeof(struct sockaddr_in);

    err = connect(fd, (struct sockaddr*)&server, size);
    if (err == -1) sys_error("connect error");

    char opt[2];
    //char name[33];
    int ans;
    opt[0] = '\1';
    opt[0] = '\0';
    dogType pet;
    int nstructs, index;

    do {

        printf("\nMenu:\n");
        printf("1. Insert a Register\n");
        printf("2. Visualize a Register\n");
        printf("3. Erase a Register\n");
        printf("4. Search a Register\n");
        printf("5. Quit\n\n");

        if (!get_bounded_str(opt, 1)) continue;

        printf("\n");

        switch(opt[0]){

            case '1':

                fill_pet_info(&pet);
                pet.next = -1;
                printf("\nThis is the register you are about to create:\n\n");
                print_pet(&pet);
                ans = confirmation("Do you want to create it?");

                if(ans){

                    send_full(fd, opt, 1);
                    send_full(fd, &pet, sizeof(dogType));
                }

                break;

            case '2':

                send_full(fd, opt, 1);
                recv_full(fd, &nstructs, sizeof(int));
                printf("Registers in database: %d\n", nstructs);
                if(!nstructs) break;
                do{

                    index = get_int("Insert the register number to visualize: ");

                }while(nstructs < index || index == 0);
                ans = confirmation("are you sure you want to search that index?");
                if(!ans){

                    index = -1;
                    send_full(fd, &index, sizeof(int));
                    break;
                }

                send_full(fd, &index, sizeof(int));
                recv_full(fd, &pet, sizeof(dogType));
                print_pet(&pet);

                recv_full(fd, &ans, sizeof(int));
                char *pmt;
                if(ans){

                    pmt = "Do you want to see the existing medical record?";
                }
                else{

                    pmt = "Do you want to create a medical record for this pet?";
                }
                ans = confirmation(pmt);
                send_full(fd, &ans, sizeof(int));
                
                if(!ans) break;
                
                char path[33];
                sprintf(path, "client/%i.temp", pet.doc_id);
                FILE *file = fopen(path, "w+");
                if (file == NULL) sys_error("fopen error client 2");
                
                recv_write_file(file, fd);
                fclose(file);
                open_medical_record(path);
                file = fopen(path, "r");
                send_file(file, fd);
                fclose(file);
                
                //if (cfileexists(path) && (remove(path) != 0))
                //    sys_error("tmp file removal error");

                break;
                
            case '3':
            
                send_full(fd, opt, 1);
                recv_full(fd, &nstructs, sizeof(int));
                printf("Registers in database: %d\n", nstructs);
                do{
                
                    index = get_int("Insert the register number to delete: ");
                
                }while(nstructs < index || index == 0);
                send_full(fd, &index, sizeof(int));
                recv_full(fd, &pet, sizeof(dogType));
                print_pet(&pet);
                ans = confirmation("are you sure you want to delete this pet?");
                send_full(fd, &ans, sizeof(int));
            
                break;
                
            case '4':;
            
                char name[33];
                do{
                
                    printf("Please enter the name of the pet to search: ");
                    
                }while(!get_bounded_str(name, 32));
                ans = confirmation("confirm search");
                if(!ans) break;
                word_to_upper(name);
                send_full(fd, opt, 1);
                send_full(fd, name, 33);
                while(1){
                
                   recv_full(fd, &pet, sizeof(dogType));
                   if(pet.sex == 'E') break;
                   print_pet(&pet);
                }
                
                break;
            
            default:

                send_full(fd, opt, 1);

            break;
        }

    } while (opt[0] != '5');

    shutdown(fd, SHUT_RDWR);
    close(fd);
}
