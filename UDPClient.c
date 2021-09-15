// Client side implementation of UDP client-server model
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080
#define MAXLINE 1024
#define PACKAGE_LENGTH 20
#define PACKAGE_MESSAGE_LENGTH 16
#define PACKAGE_NUMBER_LENGTH 4

int send_file_to_server(int sockfd, struct sockaddr_in *servaddr, char *filename);
int run_udp_client();
int calculate_package_number(int message_len);
void package_number_to_string(char *package, int current_package);
char *read_client_file(char *filename);
void create_message(char *buffer, char *message, int current_package, int begin);
void create_setup_message(int package_number, char *filename, char *setup_message);
int package_number_to_integer(char *buffer);

// Driver code
int main()
{
    return run_udp_client();
}

int run_udp_client()
{
    int sockfd = 0;
    struct sockaddr_in servaddr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    printf("Client start\n\n");

    socklen_t len;
    // Identificacion del usuario
    // printf("Ingrese el nombre del archivo \n");
    // scanf("%s", file);
    /*
    printf("Ingrese su usuario \n");
    scanf("%s", name);
    printf("\nIngrese la contrasena \n");
    scanf("%s", password);
    strcat(name, "/");
    strcat(password, "/");
    strcat(strcpy(info, name), password);
    // printf("%s\n", info);
    sendto(sockfd, (const char *)info, strlen(info), MSG_CONFIRM, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    printf("\nUser and password sent. Waiting for response\n");
    char buffer[MAXLINE];
    int n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&servaddr, &len);
    buffer[n] = '\0';
    printf("Auten message: %s\n", buffer);
    if (strstr(buffer, "auten_sucess") == NULL)
    {
        return 1; // return error
    }
    printf("Client : %s\n", buffer);
    */
    //TODO: Pedir filename por terminal

    char *filename = "archivo3.txt";
    printf("Filename: [%s]\n\n", filename);
    char *message = read_client_file(filename);
    const int message_len = strlen(message);
    int package_number = calculate_package_number(message_len);
    char setup_message[PACKAGE_LENGTH];
    create_setup_message(package_number, filename, setup_message);
    char confirmation_message[8] = "12345678";
    bool confirmation = false;
    while (!confirmation) {
        sendto(sockfd, (const char *)setup_message, PACKAGE_LENGTH, MSG_CONFIRM, (const struct sockaddr *)&servaddr, sizeof(servaddr));
        int n = recvfrom(sockfd, (char *)confirmation_message, strlen(confirmation_message), MSG_WAITALL, (struct sockaddr *)&servaddr, &len);
        confirmation_message[n] = '\0';
        if (strstr(confirmation_message, "Received") != NULL) {
            confirmation = true;
        }
    }
    //TODO: hacer receive
    int begin = 0;
    int current_package = 0;
    char buffer2[PACKAGE_LENGTH];
    printf("Package total: %d\n", package_number); // To do: Revisar la informacion que se va a mandar en el primer mensaje
    char last_received[PACKAGE_NUMBER_LENGTH];
    while (current_package < package_number)    // 0003 mensaje de confirmacion traeria el numero del ultimo recibido
    {
        create_message(buffer2, message, current_package, begin);
        if (current_package == package_number){
            buffer2[PACKAGE_NUMBER_LENGTH + (message_len % PACKAGE_MESSAGE_LENGTH)] = '\0'; // 16, 16, 14 = 46 % 16 = 14
        }
        printf("Package: \"%s\"\n", buffer2);
        sendto(sockfd, (const char *)buffer2, PACKAGE_LENGTH, MSG_CONFIRM, (const struct sockaddr *)&servaddr, sizeof(servaddr));
        int n = recvfrom(sockfd, (char *)last_received, PACKAGE_NUMBER_LENGTH, MSG_WAITALL, (struct sockaddr *)&servaddr, &len);
        last_received[n] = '\0';
        current_package = package_number_to_integer(last_received); // si recibio el mensaje 2, entonces mandara el 3 siguiente
        begin = current_package * PACKAGE_MESSAGE_LENGTH;   // 0 * 16 = 0, 1 * 16 = 16, 2 * 16 = 32, etc
    }
    printf("%9d chars sent\n", message_len);
    printf("\nFinal message sent\n\n");
    free(message);
    //send_file_to_server(sockfd, &servaddr, "archivo3.txt");

    close(sockfd);
    printf("\nClient end\n");
    return 0;
}

int package_number_to_integer(char *buffer)
{
    // Numero de ejemplo 0362
    int package_number = 0;
    int multiply = 1; // 1, 10, 100, 1000
    for (int i = PACKAGE_NUMBER_LENGTH - 1; i >= 0; --i)
    {
        package_number += (buffer[i] - '0') * multiply; // 0 + 2, 2 + 60, 62 + 300, 362 + 0 = 362
        multiply *= 10;
    }
    // printf("\t{Package_number: %d}\n", package_number);
    return package_number;
}

char *read_client_file(char *filename)
{
    FILE *f;
    // Consulta si el archivo se abrio correctamente
    if ((f = fopen(filename, "r")) == NULL)
    {
        printf("ERROR: File couldnt be open");
        exit(1);
    }
    fseek(f, 0, SEEK_END);
    int numTotal = ftell(f);
    rewind(f);
    char *message = (char *)calloc(sizeof(char), numTotal);
    assert(message);
    int length = fread(message, sizeof(char), numTotal, f);
    printf("File has \"%s\": %d chars\n", filename, length);
    fclose(f);
    return message;
}

int calculate_package_number(int message_len)
{
    int result = message_len / PACKAGE_MESSAGE_LENGTH;
    if (message_len % PACKAGE_MESSAGE_LENGTH != 0)
    {
        ++result;
    }
    return result;
}

void create_message(char *buffer, char *message, int current_package, int begin)
{
    //printf("Message: %s\n", message);
    // pasar el numero de paquete a string
    char package[PACKAGE_NUMBER_LENGTH];
    package_number_to_string(package, current_package);
    for (int i = 0; i < PACKAGE_LENGTH; ++i)
    {
        // Puede que este haciendo un segmentation fault con el char, pero funciona por el '\0'
        (i < PACKAGE_NUMBER_LENGTH) ? (buffer[i] = package[i]) : (buffer[i] = message[begin + i - PACKAGE_NUMBER_LENGTH]);
    }
    buffer[PACKAGE_LENGTH] = '\0';
}

void package_number_to_string(char *package, int current_package)
{
    // Numero de ejemplo 362
    int digits = 0;
    int position = PACKAGE_NUMBER_LENGTH - 1;
    while (current_package != 0)
    {
        package[position] = (current_package % 10) + '0'; // 2, 6, 3
        current_package = current_package / 10;
        --position;
        ++digits;
    }
    int filler = PACKAGE_NUMBER_LENGTH - digits; // (362) 4 - 3 = 1
    for (int i = 0; i < filler; ++i)
    { // 362 = 0362
        package[i] = '0';
    }
    package[PACKAGE_NUMBER_LENGTH] = '\0';
}

void create_setup_message(int package_number, char *filename, char *setup_message)
{
    char number_string[4];
    package_number_to_string(number_string, package_number);

    for (int i = 0; i < PACKAGE_LENGTH; ++i)
    {
        //Revisar
        (i < PACKAGE_NUMBER_LENGTH) ? (setup_message[i] = number_string[i]) : (setup_message[i] = filename[i - PACKAGE_NUMBER_LENGTH]);
    }
    setup_message[PACKAGE_LENGTH] = '\0';
}
