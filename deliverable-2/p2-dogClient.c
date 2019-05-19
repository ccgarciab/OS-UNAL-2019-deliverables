#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "pet_globals.h"

#include "error_handle.h"
#include "pet_input.h"
#include "pet_output.h"
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
            
                //TODO NEXT
            
                break;
            
            default:

                send_full(fd, opt, 1);

            break;
        }

    } while (opt[0] != '5');

    close(fd);
}
