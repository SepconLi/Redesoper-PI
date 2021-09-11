#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define PACKAGE_LENGTH 20
#define PACKAGE_MESSAGE_LENGTH 16
#define PACKAGE_NUMBER_LENGTH 4

void create_message( char* buffer, char* message, int current_package, int begin);
void send_file_to_server(char* filename);
int calculate_package_number(int message_len);
void package_number_to_string(char* package, int current_package);
char* read_client_file(char* filename);
void decode_message(char* buffer);
int package_number_to_integer(char* buffer);

// VARIABLE GLOBAL. MR. JEISSON GET DOWN!
char* server_string;

int main() {
    server_string = (char *) calloc(sizeof(char), 486);
    send_file_to_server("archivo3.txt");
    free(server_string);
    return 0;
}

void send_file_to_server(char* filename) {
    printf("Filename: [%s]\n\n", filename);
    char* message = read_client_file(filename);
    const int message_len = strlen(message);
    int package_number = calculate_package_number(message_len);
    int begin = 0;
    int current_package = 0;
    char buffer[PACKAGE_LENGTH];
    printf("Package total: %d\n", package_number); // To do: Revisar la informacion que se va a mandar en el primer mensaje
    while (current_package < package_number) {
        create_message(buffer, message, current_package, begin);
        ++current_package;
        begin += PACKAGE_MESSAGE_LENGTH;
        if(current_package == package_number) {
            buffer[PACKAGE_NUMBER_LENGTH+(message_len%PACKAGE_MESSAGE_LENGTH)] = '\0'; // 16, 16, 14 = 46 % 16 = 14
            printf("Package: \"%s\"\n", buffer);
            decode_message(buffer);
            printf("%9d chars sent\n",message_len);
            printf("\nFinal message sent\n\n");
        } else {
            printf("Package: \"%s\"\n", buffer);
            decode_message(buffer);
        }
    }
    free(message);
    printf("%s\n", server_string);
}

char* read_client_file(char* filename) {
    FILE *f = fopen(filename, "r");
    fseek(f, 0, SEEK_END);
    int numTotal = ftell(f);
    rewind(f);
    char* message = (char *) calloc(sizeof(char), numTotal);
    assert(message);
    int length = fread(message, sizeof(char), numTotal, f);
    printf("File has \"%s\": %d chars\n", filename, length);
    fclose(f);
    return message;
}

int calculate_package_number(int message_len) {
    int result = message_len/PACKAGE_MESSAGE_LENGTH;
    if (message_len%PACKAGE_MESSAGE_LENGTH != 0) {
        ++result;
    }
    return result;
}

void create_message(char* buffer, char* message, int current_package, int begin) {
    //printf("Message: %s\n", message);
    // pasar el numero de paquete a string
    char package[PACKAGE_NUMBER_LENGTH];
    package_number_to_string(package, current_package);
    for(int i = 0; i < PACKAGE_LENGTH; ++i) {
        // Puede que este haciendo un segmentation fault con el char, pero funciona por el '\0'
        (i < PACKAGE_NUMBER_LENGTH)? (buffer[i] = package[i]): (buffer[i] = message[begin+i-PACKAGE_NUMBER_LENGTH]);
    }
    buffer[PACKAGE_LENGTH] = '\0';
}

void package_number_to_string(char* package, int current_package) {
    // Numero de ejemplo 362
    int digits = 0;
    int position = PACKAGE_NUMBER_LENGTH-1;
    while (current_package != 0) {
        package[position] = (current_package % 10) + '0'; // 2, 6, 3
        current_package = current_package / 10;
        --position;
        ++digits;
    }
    int filler = PACKAGE_NUMBER_LENGTH - digits; // (362) 4 - 3 = 1
    for (int i = 0; i < filler; ++i) { // 362 = 0362
        package[i] = '0';
    }
    package[PACKAGE_NUMBER_LENGTH] = '\0';
}

void decode_message(char* buffer){
    int package_number = package_number_to_integer(buffer);
    // todo: ver si coincide con el ultimo paquete enviado
    char buffer_message[PACKAGE_MESSAGE_LENGTH];
    for(int i = 0; i <= PACKAGE_MESSAGE_LENGTH; ++i) {
        buffer_message[i] = buffer[i+PACKAGE_NUMBER_LENGTH];
    }
    int begin = package_number * PACKAGE_MESSAGE_LENGTH;
    int end = begin + PACKAGE_MESSAGE_LENGTH;
    int counter = 0;
    for(int i = begin; i < end; ++i) {
        server_string[i] = buffer_message[counter];
        ++counter;
    }
    // agregar el mensaje al final de una variable global
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
