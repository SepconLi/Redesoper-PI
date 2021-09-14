#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[]){
    int fd = open(argv[1],50);
    if(fd == -1) {
       printf("ERROR, FILE NOT FOUND.\n");
       return 1;
    }
    return 0;
}