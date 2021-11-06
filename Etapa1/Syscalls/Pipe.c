#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include "Read.h"
#include "Write.h"

int main(int argc, char* argv[]) {
    int fd[2];
    char* buf;
    if (argc != 2) {
        printf("ERROR: Usage: %s <string>\n",argv[0]);
        return 1;
    } else {
        // Se supone que lee en fd[0] y lo escribe en fd[1]
        int length = strlen(argv[1]);
        void* direction = read(fd[0],argv[1],length); // Agarra la direccion en memoria donde lee
        write(fileno(stdout),argv[1],length); // y escribe
    }
    return 0;
}
