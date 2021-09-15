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

#define PORT 8080
#define MAXLINE 1024
#define PACKAGE_LENGTH 20
#define PACKAGE_MESSAGE_LENGTH 16
#define PACKAGE_NUMBER_LENGTH 4

int run_udp_server();
int check_autentication();
void decode_message(char *buffer, char *output_filename);
int package_number_to_integer(char *buffer);
int receive_file_from_client(int sockfd, struct sockaddr_in *cliaddr);
void package_number_to_string(char *package, int current_package);
void write_in_file(char *output_filename, char *text);

// VARIABLE GLOBAL. MR. JEISSON GET DOWN!
//char* server_string;

// Driver code
int main()
{
    //server_string = (char *) calloc(sizeof(char), 486);
    int error_status = run_udp_server();
    //free(server_string);
    return error_status;
}

int run_udp_server()
{
    int sockfd = 0;
    struct sockaddr_in servaddr;
    struct sockaddr_in cliaddr;
    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed");
        return 4;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Filling server information
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr,
             sizeof(servaddr)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Server start\n\n");

    socklen_t len = sizeof(cliaddr); //len is value/resuslt
    /*
    int auten_code = 69;
    char buffer[MAXLINE];
    printf("\nWaiting for the username and password\n");
    int n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
    buffer[n] = '\0';
    auten_code = check_autentication(buffer);

    printf("Auten code: %d\n", auten_code);
    if (auten_code != 0)
    {
        char *auten_error = "auten_error";
        printf("Sending: [%s]\n", auten_error);
        sendto(sockfd, (const char *)auten_error, strlen(auten_error), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
        return 1;
    }
    char *auten_sucess = "auten_sucess";
    sendto(sockfd, (const char *)auten_sucess, strlen(auten_sucess), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
    */
    char output_filename[20];
    printf("Write the filename: ");
    scanf("%s", output_filename);
    printf("\nWaiting for the setup message\n");
    char package_message[PACKAGE_LENGTH];
    int n2 = recvfrom(sockfd, (char *)package_message, PACKAGE_LENGTH, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
    package_message[n2] = '\0';
    printf("Message received: \"%s\"\n", package_message);
    int package_number = package_number_to_integer(package_message);
    char confirmation[8] = "Received";
    sendto(sockfd, (const char *)confirmation, strlen(confirmation), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
    char send_next[4] = "0000";
    int next_message = 0;
    int last_received = 0;
    for (int i = 0; i < package_number; ++i)
    {
        int n3 = recvfrom(sockfd, (char *)package_message, PACKAGE_LENGTH, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
        package_message[n3] = '\0';
        last_received = package_number_to_integer(package_message);
        if (last_received == next_message) {
            decode_message(package_message, output_filename);
            ++next_message;
            package_number_to_string(send_next, next_message);
        } else {
            --i;
        }
        printf("next message %d\n", next_message);
        sendto(sockfd, (const char *)send_next, PACKAGE_NUMBER_LENGTH, MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
    }
    //Imprimir contenidos y cerrar archivo
    //int error = receive_file_from_client(sockfd, &cliaddr);
    printf("\nServer end\n");
    return 0;
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

int check_autentication(char *buffer)
{
    int separador[2];
    FILE *data_base;
    int j = 0;
    for (int i = 0; i < MAXLINE; ++i)
    {
        if (buffer[i] == '/')
        {
            separador[j] = i;
            ++j;
        }
    }
    char name[20];
    char password[20];
    for (int i = 0; i < separador[0]; ++i)
    {
        name[i] = buffer[i];
    }
    name[separador[0]] = '\0';
    int auxCounter = 0;
    for (int i = separador[0] + 1; i < separador[1]; ++i)
    {
        password[auxCounter] = buffer[i];
        ++auxCounter;
    }
    password[auxCounter] = '\0';
    printf("Name: %s\n", name);
    printf("Password: %s\n", password);

    // Consulta si el archivo se abrio correctamente
    if ((data_base = fopen("data_base.txt", "r")) == NULL)
    {
        printf("ERROR: File couldnt be open");
        exit(1);
    }

    // Mientras que hayan datos en el archivo
    char file_username[40];
    char file_password[40];
    while (!feof(data_base))
    {
        fgets(file_username, 40, data_base);
        fgets(file_password, 40, data_base);
        // Si el usuario y la contrasena son iguales
        if (strstr(file_username, name) != NULL && strstr(file_password, password) != NULL)
        {
            printf("Autentication completed, access granted\n");
            fclose(data_base);
            return 0;
        }
    }
    printf("Autentication failed, access denied\n");
    fclose(data_base);
    return 1;
}

void decode_message(char *buffer, char *output_filename)
{
    char buffer_message[PACKAGE_MESSAGE_LENGTH];
    for (int i = 0; i <= PACKAGE_MESSAGE_LENGTH; ++i)
    {
        buffer_message[i] = buffer[i + PACKAGE_NUMBER_LENGTH];
    }
    buffer_message[PACKAGE_MESSAGE_LENGTH] = '\0';
    write_in_file(output_filename, buffer_message);
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

void write_in_file(char *output_filename, char *text)
{
    FILE *f;
    f = fopen(output_filename, "a");
    fprintf(f, "%s", text);
    fclose(f);
}