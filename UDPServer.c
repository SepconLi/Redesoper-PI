// Server side implementation of UDP client-server model
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

int check_autentication();
  
// Driver code
int main() {
    int sockfd;
    char buffer[MAXLINE];
    char *hello = "Hello from server";
    struct sockaddr_in servaddr, cliaddr;
      
    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
      
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
      
    // Filling server information
    servaddr.sin_family    = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
      
    // Bind the socket with the server address
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, 
            sizeof(servaddr)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
      
    int len, n;
  
    len = sizeof(cliaddr);  //len is value/resuslt
  
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
                MSG_WAITALL, ( struct sockaddr *) &cliaddr,
                &len);
    buffer[n] = '\0';
      
    check_autentication(buffer);

    sendto(sockfd, (const char *)hello, strlen(hello), 
        MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
            len);
    printf("Hello message sent.\n");

    return 0;
}

int check_autentication(char* buffer){
    int separador[2];
    FILE *data_base;
    int j = 0;
    for(int i = 0; i < MAXLINE; ++i){
       if(buffer[i] == '/'){
         separador[j] = i;
         ++j;
       }
    }
    char name[20];
    char password[20];
    printf("Nombre: "); 
    for(int i = 0; i < separador[0]; ++i){
       name[i]=buffer[i];
       printf("%c",buffer[i]); 
    }
    printf("\n");
    printf("Password: ");
    for(int i = separador[0]+1; i < separador[1]; ++i){
       password[i]=buffer[i];
       printf("%c",buffer[i]); 
    }
    printf("\n");

     // Consulta si el archivo se abrio correctamente
    if((data_base = fopen("data_base.txt", "r")) == NULL){
        printf("ERROR: File couldnt be open");
        exit(1);
    }

    // Mientras que hayan datos en el archivo
    char file_username[40];
    char file_password[40];
    while(!feof(data_base)){
        fgets(file_username, 40, data_base);
        fgets(file_password, 40, data_base);
        // Si el usuario y la contrasena son iguales 
        if(strstr(file_username, name) != NULL && strstr(file_password, password) != NULL){
                printf("Autentificacion correcta, puede acceder al sistema \n");
                break;
        }
    }
    
    if(strstr(file_username, name) == NULL || strstr(file_password, password) == NULL){
        printf("Autentificacion incorrecta, no puede acceder al sistema \n");
    }
    fclose(data_base);
}
