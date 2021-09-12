// Server side implementation of UDP client-server model

#include <stdbool.h>
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
#define PACKAGE_LENGTH 20
#define PACKAGE_MESSAGE_LENGTH 16
#define PACKAGE_NUMBER_LENGTH 4

int run_udp_server();
int check_autentication();
void decode_message(char* buffer);
int package_number_to_integer(char* buffer);
int receive_file_from_client(int sockfd, struct sockaddr_in* cliaddr);

// VARIABLE GLOBAL. MR. JEISSON GET DOWN!
char* server_string;

// Driver code
int main() {
    server_string = (char *) calloc(sizeof(char), 486);
    int error_status = run_udp_server();
    free(server_string);
    return error_status;
}

int run_udp_server() {
    int sockfd = 0;
    struct sockaddr_in servaddr;
    struct sockaddr_in cliaddr;
    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        return 4;
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

    printf("Server start\n\n");
   

    int len = sizeof(cliaddr);  //len is value/resuslt
    
   /* int auten_code = 69;
    char buffer[MAXLINE];
    printf("\nWaiting for the username and password\n");
    int n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
    buffer[n] = '\0';
    auten_code = check_autentication(buffer);
    
    printf("Auten code: %d\n", auten_code);
    if (auten_code != 0) {
        char* auten_error = "auten_error";
        printf("Sending: [%s]\n", auten_error);
        sendto(sockfd, (const char *)auten_error, strlen(auten_error), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
        return 1;
    }
    char* auten_sucess = "auten_sucess";
    sendto(sockfd, (const char *)auten_sucess, strlen(auten_sucess), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);*/

 
    int error = receive_file_from_client(sockfd, &cliaddr);
    printf("\nServer end\n");
    return error;
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
    for(int i = 0; i < separador[0]; ++i){
       name[i]=buffer[i];
    }
    name[separador[0]] = '\0';
    int auxCounter = 0;
    for(int i = separador[0]+1; i < separador[1]; ++i){
       password[auxCounter]=buffer[i];
       ++auxCounter;
    }
    password[auxCounter+1] = '\0';
    printf("Name: %s\n", name); 
    printf("Password: %s\n", password);

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
                printf("Autentication completed, access granted\n");
                fclose(data_base);
                return 0;
        }
    }
    printf("Autentication failed, access denied\n");
    fclose(data_base);
    return 1;
}

int receive_file_from_client(int sockfd, struct sockaddr_in* cliaddr){
    //Recibe un mensaje con el numero de paquetes y nombre del archivo 
    socklen_t len = sizeof(cliaddr);  //len is value/resuslt
    printf("Hola");
    printf("\nWaiting for the setup message\n");
    printf("Hola2");
    char buffer[PACKAGE_LENGTH];
    int n = recvfrom(sockfd, (char *)buffer, PACKAGE_LENGTH, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
    buffer[n] = '\0';
   
    printf("Message received: \"%s\"\n", buffer);
    //TODO: Confirma que le llego el mensaje inicial y sino lo pide de vuelta
    //Crea y abre un archivo con el nombre en el mensaje
    //Ciclo que reciba x cantidad de mensajes/paquetes
    for (int i = 0; i < PACKAGE_LENGTH; ++i ) {
        //Recibe el mensaje en una var
        int n = recvfrom(sockfd, (char *)buffer, PACKAGE_LENGTH, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
        buffer[n] = '\0';
        decode_message(buffer);
        //Si el numero de paquete es igual al anterior + 1
            //Copia el mensaje al final dl archivo
        //Sino 
            //Vuelve a pedir el mensaje desde el numero que no se recibio
    }
    //Imprimir contenidos y cerrar archivo 
    printf("%s\n",server_string);
    return 0;
}

void decode_message(char* buffer){
    int package_number = package_number_to_integer(buffer);
    char buffer_message[PACKAGE_MESSAGE_LENGTH];
    for(int i = 0; i <= PACKAGE_MESSAGE_LENGTH; ++i) {
        buffer_message[i] = buffer[i+PACKAGE_NUMBER_LENGTH];
    }
    int begin = package_number * PACKAGE_MESSAGE_LENGTH;
    int end = begin + PACKAGE_MESSAGE_LENGTH;
    int counter = 0;
    for(int i = begin; i < end; ++i) {
        // Todo: Escribir en un archivo
        server_string[i] = buffer_message[counter];
        ++counter;
    }
}

int package_number_to_integer(char* buffer) {
    // Numero de ejemplo 0362
    int package_number = 0;
    int multiply = 1;   // 1, 10, 100, 1000
    for (int i = PACKAGE_NUMBER_LENGTH-1; i >= 0; --i) {
        package_number += (buffer[i] - '0') * multiply; // 0 + 2, 2 + 60, 62 + 300, 362 + 0 = 362
        multiply *= 10;
    }
    // printf("\t{Package_number: %d}\n", package_number);
    return package_number;
}
