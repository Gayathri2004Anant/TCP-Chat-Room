#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define PORT 2004

int main(){
    printf("=========== CHATROOM ===========\n");

    printf("Name: ");
    char name[100];
    scanf("%s", name);

    int sockfd;
    struct sockaddr_in serv_addr;

    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
        printf("Connection failed\n");
        exit(1);
    }

    send(sockfd, name, strlen(name)+1, 0);
    char buf[100];

    fd_set rfds;

    while(1){
        FD_ZERO(&rfds);
        FD_SET(0, &rfds);
        FD_SET(sockfd, &rfds);

        select(sockfd+1, &rfds, NULL, NULL, NULL);

        if(FD_ISSET(0, &rfds)){
            memset(buf, '\0', sizeof(buf));
            // read(0, buf, sizeof(buf));
            fgets(buf, sizeof(buf), stdin);
            buf[strlen(buf) - 1] = '\0';
            if(strlen(buf)){
                // printf("sending..%s\n", buf);
                send(sockfd, buf, strlen(buf)+1, 0);
                if(strncmp(buf, "exit", 4) == 0){
                    break;
                }
            }
        }
        else if(FD_ISSET(sockfd, &rfds)){
            recv(sockfd, buf, sizeof(buf), 0);
            printf("%s\n", buf);
        }
    }
    printf("Exiting the chat...\n");
    close(sockfd);
    return 0;
}