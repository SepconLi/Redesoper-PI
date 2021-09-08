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
      
    int n, len;
    

     char file[20];
     char name[200];
     char password[200];
     char info[1024];
     
     // Identificacion del usuario 
    // printf("Ingrese el nombre del archivo \n");
    // scanf("%s", file);
    printf("Ingrese su usuario \n");
    scanf("%s", name);
    printf("Ingrese la contrasena \n");
    scanf("%s", password);
    
    strcat(name,"/");
    strcat(password,"/");
    strcat(strcpy(info,name), password);
    printf("%s\n", info);

      
    sendto(sockfd, (const char *)info, strlen(info), 
        MSG_CONFIRM, (const struct sockaddr *) &servaddr, 
            sizeof(servaddr));
    printf("Hello message sent.\n");
          
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
                MSG_WAITALL, (struct sockaddr *) &servaddr,
                &len);
    buffer[n] = '\0';
    printf("Server : %s\n", buffer);
  
    close(sockfd);
    return 0;
}