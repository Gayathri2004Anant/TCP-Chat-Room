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

    int sockfd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen = sizeof(cli_addr);

    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    int clifds[10];
    memset(clifds, 0, sizeof(clifds));

    if(bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        printf("Bind error\n");
        exit(1);
    }

    if(listen(sockfd, 10) < 0){
        printf("Listen error\n");
        exit(1);
    }

    fd_set rfds;
    int maxfd = 0;
    char names[10][100];

    printf("========== CHATROOM SERVER ==========\n");

    while(1){
        FD_ZERO(&rfds);
        FD_SET(sockfd, &rfds);

        if(maxfd < sockfd) maxfd = sockfd;

        for(int i = 0; i < 10; ++i){
            if(clifds[i] != 0){
                FD_SET(clifds[i], &rfds);
                if (clifds[i] > maxfd) maxfd = clifds[i];
            }
        }

        select(maxfd + 1, &rfds, NULL, NULL, NULL);

        if(FD_ISSET(sockfd, &rfds)){
            // printf("sockfd\n");
            int newfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
            char buf[100];
            recv(newfd, buf, sizeof(buf), 0);
            char msg[500];
            sprintf(msg, "%s joined the chat...", buf);
            printf("%s\n", msg);
            for(int i = 0; i < 10; ++i){
                if(clifds[i] == 0){
                    clifds[i] = newfd;
                    sprintf(names[i], "%s", buf);
                    break;
                }
                else{
                    // if(strlen(msg) == 0) continue;
                    send(clifds[i], msg, strlen(msg)+1, 0);
                }
            }
        }
        for(int i = 0; i < 10; ++i){
            char buf[100];
            char msg[500];
            if(clifds[i] && FD_ISSET(clifds[i], &rfds)){
                recv(clifds[i], buf, sizeof(buf), 0);
                if(strncmp(buf, "exit", 4) == 0){
                    sprintf(msg, "%s left the chat.", names[i]);
                    fflush(stdout);
                    close(clifds[i]);
                    clifds[i] = 0;
                    names[i][0] = '\0';
                }
                else sprintf(msg, "%s > %s", names[i], buf);
                printf("%s\n", msg);
                for(int j = 0; j < 10; ++j){
                    if (clifds[j] != 0 && j != i) {
                        send(clifds[j], msg, strlen(msg) + 1, 0);
                    }
                }
            }
        }
    }

    close(sockfd);

    return 0;
}