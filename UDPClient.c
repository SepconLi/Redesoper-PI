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

int run_udp_client();

// Driver code
int main() {
    return run_udp_client();
}

int run_udp_client() {
    int sockfd = 0;
    char buffer[MAXLINE];
    char *hello = "Hello from client";
    struct sockaddr_in servaddr;
  
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
        return 1;   // return error
    }
    printf("Client : %s\n", buffer);

    send_file_to_server(sockfd, &servaddr);
  
    close(sockfd);
    printf("\nClient end\n");
    return 0;
}

int send_file_to_server(int sockfd, struct sockaddr_in* servaddr) {
    const int* total_packages = calloc(1, sizeof(int) );
    *total_packages = 5; //calculate_package_mumber(file);
    sendto(sockfd, (const int*)total_packages, sizeof(int), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
    int n = 0;
    int len = 0;
    // n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
    // Preguntar si hay que confirmar si llego el mensaje
    buffer[n] = '\0';
    char message[MAXLINE] = "aaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbccccccccccccccccccccddddddddddddddddddddeeeeeeeeeeeeeeeeeeee\0";
    int current_package = 0;
    int begin = 0; // 20
    int end = 19; // 39
    char buffer[20];
    while (current_package != total_packages) {
        create_message(buffer, message, current_package, begin, end, 20);
        sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
        n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
        buffer[n] = '\0';
        if (n != current_package) {
            // en caso de que se salte 1 paquete
            current_package = n;
            begin -= 20;
            end -= 20;
        } else {
            // en caso de que el paquete que haya llegado sea el que es
            ++current_package;
            begin += 20;
            end += 20;
        }
    }
    free(total_packages);
} 

void create_message( char* buffer, char* message, int current_package, int begin, int end, int length) {
    // paquete actual 4b? - message 16b?
    // pasar el numero de paquete a string
    char* string_current = itoa(current_package);
    // extraer el fragmento de mensaje actual para ponerlo en el buffer

    // concatener el numero de paquete con el mensaje
    // meter el resultado en el buffer
}

/*
    Tenemos que hacer una funcion que calcule el numero de paquetes en los que se puede
    mandar un archivo
*/

/*
    Preguntar si hay que confirmar si llego el mensaje para todo
    Preguntar sobre la constitucion del mensaje y cuantos bytes se pueden mandar
    Preguntar sobre los syscall (fork sobre todo)
    Preguntar cuantos paquetes se tienen que mandar de golpe, si 1 a 1 o de cuanto
*/
