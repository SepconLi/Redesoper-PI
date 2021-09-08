// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
  
#define PORT     8080
#define MAXLINE 1024
  
// Driver code
int main() {
    int sockfd;
    char buffer[MAXLINE];
    char *hello = "Hello from client";
    struct sockaddr_in     servaddr;
  
    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
  
    memset(&servaddr, 0, sizeof(servaddr));
      
    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    printf("Client start\n\n");
      
    int n, len;

     char file[20];
     char name[200];
     char password[200];
     char info[1024];
     
    // Identificacion del usuario 
    // printf("Ingrese el nombre del archivo \n");
    // scanf("%s", file);
    int auten_code = 69;
    while (auten_code) {
        printf("Ingrese su usuario \n");
        scanf("%s", name);
        printf("\nIngrese la contrasena \n");
        scanf("%s", password);
        strcat(name,"/");
        strcat(password,"/");
        strcat(strcpy(info,name), password);
        // printf("%s\n", info);
        sendto(sockfd, (const char *)info, strlen(info), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
        printf("\nUser and password sent. Waiting for response\n");

        n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
        buffer[n] = '\0';
        printf("Auten message: %s\n", buffer);
        if (strstr(buffer, "auten_sucess") != NULL) {
            auten_code = 0;
        }
    }
    printf("Client : %s\n", buffer);
  
    close(sockfd);
    printf("\nClient end\n");
    return 0;
}