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
#include <errno.h>
#include <unistd.h>

#define PORT 8080
#define MAXLINE 1024
#define PACKAGE_LENGTH 20
#define PACKAGE_MESSAGE_LENGTH 16
#define PACKAGE_NUMBER_LENGTH 4

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
