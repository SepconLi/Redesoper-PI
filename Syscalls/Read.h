#ifndef READ_H
#define READ_H
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stddef.h>

void* read(int fd, const char *buf, int bytes) {
    void* direction;
    fd = open(buf,bytes);
    if(fd == -1) {
       return NULL;
    } else {
        direction = mmap(NULL,bytes,PROT_READ,MAP_SHARED,fd,0);
    }
    return direction;
}
#endif // READ_H
